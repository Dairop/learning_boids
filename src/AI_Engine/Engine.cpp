#include "Engine.hpp"

const float SPEED = 5.0f; // penser à changer l'apport de nourriture après modification de la vitesse
 
//min et max des cycles de nourriture
unsigned int minQttOfFoodPerFrame;
unsigned int maxQttOfFoodPerFrame;

unsigned long long int timeSinceStart = 0;


void Engine::init(sf::Vector2u szEnv){
    this->sizeEnv = szEnv;

    timeSinceStart = 0;
    framesSinceSpeciesUpdate = 0;
    framesSinceStart = 0;
    nbInteractions100Frames = 0;

    minQttOfFoodPerFrame = 3;
    maxQttOfFoodPerFrame = 5;

    species.clear();

    //create 100 fishes
    for (unsigned int i = 0; i < 100; i++){
        addFish();
    }

    //add food if < 1000
    for (unsigned int i = 0; food.size() < 1000; i++){
        addFood();
    }
}




void Engine::addFood(){
    float _x, _y;

    int tx = (sizeEnv.x) - 10.0f;
    int ty = (sizeEnv.x) - 10.0f;

    _x = rand() % (tx)+5.0f;
    _y = rand() % (ty)+5.0f;

    food.push_back(Entity(sf::Vector2f(_x, _y)));
}

void Engine::addFish(){
    float _x, _y;

    int tx = (sizeEnv.x) - 10.0f;
    int ty = (sizeEnv.x) - 10.0f;

    _x = rand() % (tx)+5.0f;
    _y = rand() % (ty)+5.0f;

    fishes.push_back(sf::Vector2f(_x, _y));
}









void Engine::update(QuadTree& boidsQuad, QuadTree& foodQuad, long dt){
    timeSinceStart += dt/1000;
    framesSinceStart++;

    //creates cycles of food abundance and shortages
    //https://www.desmos.com/calculator/ezsdetdjqi 
    float low_cycles = minQttOfFoodPerFrame;
    float high_cycles = maxQttOfFoodPerFrame;
    int foodPerFrameQtt = low_cycles + (high_cycles - low_cycles) * (std::sin( ((float)timeSinceStart)/10000)*0.5+0.5) ;


    if (framesSinceStart % 100 == 0) {
        //data for future analysis
        this->foodCyclesData.push_back(foodPerFrameQtt);
        this->populationData.push_back(fishes.size());
        this->interactionsData.push_back(nbInteractions100Frames);
        nbInteractions100Frames = 0;


        //generate analysis graphs
        if (framesSinceStart % 1000 == 0){
            this->generateGlobalAnalysisFiles();
        }
    }



    //add n food
    for (unsigned int i = 0; i < foodPerFrameQtt; i++){
        addFood();
    }


    if (this->framesSinceSpeciesUpdate >= 100){
        std::cout << "\nupdating species" << std::endl;
        updateSpecies();
        this->framesSinceSpeciesUpdate = 0;
    } else {
        this->framesSinceSpeciesUpdate++;
    }


    float timeElapsed = dt;
    //update every fish
    for (Fish& f: fishes) {
        f.updateFish(sizeEnv, boidsQuad, foodQuad, dt);
        f.updateBody();

        f.foodReserves -= timeElapsed/300000.0f;
        f.age += timeElapsed / 800000.0f;
        f.timerInteraction -= timeElapsed/1000.0f;
        if (f.foodReserves < 0 || f.age > 100) f.isAlive = false;
    }

    // delete the ones that are dead
    for (unsigned int i = 0; i < fishes.size(); ){
        if (!fishes[i].isAlive){
            fishes.erase(fishes.begin()+i);
        } else {
            i++;
        }
    }

    for (unsigned int i = 0; i < food.size(); ) {
        if (!food[i].isAlive) {
            food.erase(food.begin() + i); //dead for food means it has been eaten
        }
        else {
            i++;
        }
    }

    //add random fish if they are all dying
    if (fishes.size() < 50){
        // Sometimes the simulation get stuck because no fish is getting significantly better after a long time, 
        // in this case we start again the whole simulation
        if (timeSinceStart > 10000) {
            this->init(sizeEnv);
            return;
        }

        for (unsigned int i = 0; fishes.size() < 85; i++){
            addFish();
        }

        for (unsigned int i = 0; food.size() < 1000; i++){
            addFood();
        }
    }


    //find and perform interactions
    for (unsigned int a = 0; a < fishes.size(); a++){
        rectByCenter r;
        r.center = fishes[a].position;
        r.radius = sf::Vector2f(fishes[a].size*2.7, fishes[a].size*2.7);

        std::vector<Entity*> neighbors;
        boidsQuad.queryRangeCircle(r, neighbors);

        for (unsigned int b = 0; b < neighbors.size(); b++) {
            if (fishes[a].timerInteraction <= 0.0f && fishes[b].timerInteraction <= 0.0f && fishes[b].isAlive) {
                interact(fishes[a], fishes[b]);

                fishes[a].timerInteraction = fishes[a].maxTimerInteraction;
                fishes[b].timerInteraction = fishes[b].maxTimerInteraction;

                nbInteractions100Frames++;
            }
        }
    }
}





void Engine::updateSpecies(){
    updateSpeciesVector(species, fishes);
    updateEntitiesSpecies(species, fishes);
}




void inputNNetwork(std::vector<float>& v1, Fish& e1, Fish& e2){
    v1.clear();
    // We send values between -1 and 1 to make the work easier for the neural network
    v1.push_back((e1.age-50)/50.0f);
    v1.push_back((e2.age-50)/50.0f);

    v1.push_back((e1.foodReserves -25)/25.0f);
    v1.push_back((e2.foodReserves -25)/25.0f);

    v1.push_back(e1.NN.geneticDistanceWith(e2.NN)/200.0f-1.0f); // can have a value > 1, but never < -1

    //  /!\ If modification are done on the equations, the changes 
    //      must be done in Analyser.cpp too or the analysis will be erroneous
}






void Engine::reproduce(Fish& e1, Fish& e2){
    Fish e = Fish(mult(add(e1.position, e2.position), 0.5f));
    sf::Color parentColor; //color of the parent fish
    
    if (rand()%2){ 
        e.NN.copy_NN(e1.NN);
        parentColor = e1.color;
    } else {
        e.NN.copy_NN(e2.NN);
        parentColor = e2.color;
    }

    // Mutation rate
    float mut = rand()%100;

    // Change randomly "mut"*100% of the neural network in order to apply mutations 
    if (mut == 0){       e.NN.randomlyMutate(0.5f); }
    else if (mut < 5){   e.NN.randomlyMutate(0.01f); }
    else if (mut < 10){   e.NN.randomlyMutate(0.002f); }
    else if (mut < 50){   e.NN.randomlyMutate(0.0005f); }
    else {               e.NN.randomlyMutate(0.0f); }

    // Copy the parent color, can change later when calculating the specie of each fish if 
    // we find that it doesn't belong to it's parents' specie
    e.color = parentColor;
    fishes.push_back(e);

    // Each fish looses 15 food after reproduction (the new fish is born with around 30 food so it's like a transfer)
    e1.foodReserves -= 15.0f;
    e2.foodReserves -= 15.0f;
}



bool fight(const Fish& f1, const Fish& f2){
    // true if f1 wins, false else

    // Equation on https://www.desmos.com/calculator/84yec9ya6v
    float score1 = (f1.age>30) ? (f1.age/30*100) : (100-((f1.age-30)/70*100));
    float score2 = (f2.age>30) ? (f2.age/30*100) : (100-((f2.age-30)/70*100));

    // Randomness to the result but statistically the higher the score the higher the chance of winning the fight
    return ((score1 * (rand()%100)) > score2*50);
}






void Engine::interact(Fish& f1, Fish& f2){
    // Find optimal interaction for both of them
    /*
    eg:
                    f1          f2      sum
    Sharing         0.2         0.3     0.5
    Reproduction    0.4        -0.6    -0.2
    Fight          -0.7         0.8     0.1
    Ignore          0.9         0.2     1.1

    We will make the interaction of type "ignore" because each fish decided to ignore each-other (highest sum)
    */

    std::vector<float> v1;
    inputNNetwork(v1, f1, f2);
    f1.NN.setInputValues(v1);

    inputNNetwork(v1, f2, f1);
    f2.NN.setInputValues(v1);

    f1.NN.update();
    f2.NN.update();
    
    // Score of each type of interaction
    float sShare = f1.NN.getfinalValue(0) + f2.NN.getfinalValue(0);
    float sReproduction = f1.NN.getfinalValue(1) + f2.NN.getfinalValue(1);
    float sFight = f1.NN.getfinalValue(2) + f2.NN.getfinalValue(2);
    float sIgnore = f1.NN.getfinalValue(3) + f2.NN.getfinalValue(3);


   
    float foodTotal;

    float max = sShare;
    unsigned int cas = 1;
    if (sReproduction > max) { max = sReproduction; cas = 2;}
    if (sFight > max) { max = sFight; cas = 3;}
    if (sIgnore > max) { max = sIgnore; cas = 0;}

    switch (cas)
    {
    case 1:
        /* share */
        foodTotal = f1.foodReserves + f2.foodReserves;
        f1.foodReserves = foodTotal /2;
        f2.foodReserves = foodTotal /2;
        break;

    case 2:
        /* reproduce */
        reproduce(f1, f2);

    case 3:
        /* fight */
        if (fight(f1, f2)){
            f2.foodReserves -= 20;
            f1.foodReserves += 20;
        } else {
            f1.foodReserves -= 20;
            f2.foodReserves += 20;
        }


        break;
    
    default:
        /*ignore*/
        break;
    }
}










void Engine::generateGlobalAnalysisFiles(){
    //can't use timeSinceStart as an input because it value can cause undefined behavior in the function if too big
    float timeAsFloat;
    if (timeSinceStart/1000 <= std::numeric_limits<float>::max()) {
        timeAsFloat = static_cast<float>(timeSinceStart/1000)-1;
    }
    else {
        timeAsFloat = std::numeric_limits<float>::max();
    }

    saveAsSvg("Food_cycles", this->foodCyclesData, 1000.0f, "Time", "Qtt of food", timeAsFloat, sf::Vector3f(50, 200, 20), false);
    saveAsSvg("Population_over_time", this->populationData, 1000.0f, "Time", "Population", timeAsFloat, sf::Vector3f(0, 00, 255), false);
    saveAsSvg("Interactions_over_time", this->interactionsData, 1000.0f, "Time", "Interactions", timeAsFloat, sf::Vector3f(0, 00, 255), false);
}
