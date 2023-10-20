#include "Engine.hpp"

//min et max des cycles de nourriture
unsigned int minQttOfFoodPerFrame;
unsigned int maxQttOfFoodPerFrame;

unsigned long long timeSinceStart = 0;


void Engine::init(sf::Vector2u szEnv){
    this->sizeEnv = szEnv;

    timeSinceStart = 0; //time since start in ms
    framesSinceSpeciesUpdate = 0;
    framesSinceStart = 0;
    nbInteractions100Frames = 0;

    minQttOfFoodPerFrame = 5;
    maxQttOfFoodPerFrame = 8;

    species.clear();

    //create 500 fishes
    for (unsigned int i = 0; i < 500; i++){
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

    food.push_back(new Entity(sf::Vector2f(_x, _y)));
}

void Engine::addFish(){
    float _x, _y;

    int tx = (sizeEnv.x) - 10.0f;
    int ty = (sizeEnv.x) - 10.0f;

    _x = rand() % (tx)+5.0f;
    _y = rand() % (ty)+5.0f;


    Fish* a = new Fish(sf::Vector2f(_x, _y));
    fishes.push_back(a);
}









void Engine::update(QuadTree& boidsQuad, QuadTree& foodQuad, long dt){

    /*                  ,,            ,,             ,,    ,,                             ,,
    `7MMF'              db    mm      db           `7MM    db                     mm      db
      MM                      MM                     MM                           MM
      MM  `7MMpMMMb.  `7MM  mmMMmm  `7MM   ,6"Yb.    MM  `7MM  M"""MMV  ,6"Yb.  mmMMmm  `7MM   ,pW"Wq.  `7MMpMMMb.
      MM    MM    MM    MM    MM      MM  8)   MM    MM    MM  '  AMV  8)   MM    MM      MM  6W'   `Wb   MM    MM
      MM    MM    MM    MM    MM      MM   ,pm9MM    MM    MM    AMV    ,pm9MM    MM      MM  8M     M8   MM    MM
      MM    MM    MM    MM    MM      MM  8M   MM    MM    MM   AMV  , 8M   MM    MM      MM  YA.   ,A9   MM    MM
    .JMML..JMML  JMML..JMML.  `Mbmo .JMML.`Moo9^Yo..JMML..JMML.AMMmmmM `Moo9^Yo.  `Mbmo .JMML. `Ybmd9'  .JMML  JMML.
    */

    timeSinceStart += dt/1000;
    framesSinceStart++;

    //creates cycles of food abundance and shortages
    //https://www.desmos.com/calculator/ezsdetdjqi 
    float low_cycles = minQttOfFoodPerFrame;
    float high_cycles = maxQttOfFoodPerFrame;
    int foodPerFrameQtt = low_cycles + 
                (high_cycles - low_cycles) * (
                    std::sin( ((float)timeSinceStart*SIMULATION_SPEED)/10000)   * 0.5 + 0.5
                ) ;


    /*                                   ,,
          db                           `7MM
         ;MM:                            MM
        ,V^MM.    `7MMpMMMb.   ,6"Yb.    MM  `7M'   `MF'M"""MMV  .gP"Ya
       ,M  `MM      MM    MM  8)   MM    MM    VA   ,V  '  AMV  ,M'   Yb
       AbmmmqMA     MM    MM   ,pm9MM    MM     VA ,V     AMV   8M""""""
      A'     VML    MM    MM  8M   MM    MM      VVV     AMV  , YM.    ,
    .AMA.   .AMMA..JMML  JMML.`Moo9^Yo..JMML.    ,V     AMMmmmM  `Mbmmd'
                                                ,V
                                             OOb"
    */

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

    if (this->framesSinceSpeciesUpdate >= 100) {
        std::cout << "\nupdating species" << std::endl;
        updateSpecies();
        this->framesSinceSpeciesUpdate = 0;
    }
    else {
        this->framesSinceSpeciesUpdate++;
    }



    /*                                                                                               
                                                                            ,,                                
    `7MMF'              mm                                          mm      db                                
      MM                MM                                          MM                                        
      MM  `7MMpMMMb.  mmMMmm   .gP"Ya  `7Mb,od8  ,6"Yb.   ,p6"bo  mmMMmm  `7MM   ,pW"Wq.  `7MMpMMMb.  ,pP"Ybd 
      MM    MM    MM    MM    ,M'   Yb   MM' "' 8)   MM  6M'  OO    MM      MM  6W'   `Wb   MM    MM  8I   `" 
      MM    MM    MM    MM    8M""""""   MM      ,pm9MM  8M         MM      MM  8M     M8   MM    MM  `YMMMa. 
      MM    MM    MM    MM    YM.    ,   MM     8M   MM  YM.    ,   MM      MM  YA.   ,A9   MM    MM  L.   I8 
    .JMML..JMML  JMML.  `Mbmo  `Mbmmd' .JMML.   `Moo9^Yo. YMbmd'    `Mbmo .JMML. `Ybmd9'  .JMML  JMML.M9mmmP'
    */

    //find and perform interactions
    for (unsigned int a = 0; a < fishes.size(); a++) {
        //skip if dead
        if (!fishes[a]->isAlive) continue;

        Fish* fishA = fishes[a];

        rectByCenter r;
        r.center = fishA->position;
        r.radius = sf::Vector2f(fishA->size * 2.7, fishA->size * 2.7);

        std::vector<Entity*> neighbors;
        boidsQuad.queryRangeCircle(r, neighbors);


        for (unsigned int b = 0; b < neighbors.size(); b++) {
            if (fishA != neighbors[b] &&
                fishA->timerInteraction <= 0.0f &&
                neighbors[b]->isAlive) {

                if (Fish* fishB = static_cast<Fish*>(neighbors[b])) { // convert the Entity back to a Fish, 
                    //should return an error if something else than a Fish is in the QuadTree

                    if (!fishB->isAlive || fishB->timerInteraction > 0) continue;

                    interact(fishA, fishB);

                    fishA->timerInteraction = fishA->maxTimerInteraction;
                    fishB->timerInteraction = fishB->maxTimerInteraction;

                    nbInteractions100Frames++;
                }
                else {
                    //std::cout << "wrong types in the quadtree;
                }
            }
        }
    }




    /*                                       
                           ,,                          
      .g8""8q.     mm    `7MM                          
    .dP'    `YM.   MM      MM                          
    dM'      `MM mmMMmm    MMpMMMb.   .gP"Ya  `7Mb,od8 
    MM        MM   MM      MM    MM  ,M'   Yb   MM' "' 
    MM.      ,MP   MM      MM    MM  8M""""""   MM     
    `Mb.    ,dP'   MM      MM    MM  YM.    ,   MM     
      `"bmmd"'     `Mbmo .JMML  JMML. `Mbmmd' .JMML.   
    */

    //add n food
    for (unsigned int i = 0; i < foodPerFrameQtt; i++){
        addFood();
    }


    float timeElapsed = static_cast<float>(dt);

    //update every fish ressources
    for (Fish* f: fishes) {
        if (!f->isAlive) continue;

        f->updateFish(sizeEnv, boidsQuad, foodQuad, dt);
        f->updateBody();

        f->foodReserves -= timeElapsed/100000.0f * SIMULATION_SPEED;
        f->age += timeElapsed / 800000.0f * SIMULATION_SPEED;
        f->timerInteraction -= timeElapsed / 800000.0f * SIMULATION_SPEED;
        if (f->foodReserves < 0 || f->age > 100) f->isAlive = false;
    }


    //delete the dead fish and food if they are dead (or consummed)
    // 
    //      / \     The QuadTrees aren't updated when we delete the Objects in fishes and vood
    //     / ! \    Don't use the quadtrees after this part of the function or you may have
    //    /__°__\   unexpected behaviors (the pointers still exist but not the objects being pointed)
    //
    //

    for (unsigned int i = 0; i < fishes.size(); ) {
        if (fishes[i]->isAlive) {
            i++;
        } else {
            delete fishes[i];
            fishes.erase(fishes.begin() + i);
        }
    }

    for (unsigned int i = 0; i < food.size(); ) {
        if (food[i]->isAlive) {
            i++;
        }
        else {
            delete food[i];
            food.erase(food.begin() + i);
        }
    }    

    preventDyingSimulations();
}





void Engine::updateSpecies(){
    updateSpeciesVector(species, fishes);
    updateEntitiesSpecies(species, fishes);
}






void Engine::preventDyingSimulations() {
    //add random fish if they are all dying
    if (fishes.size() < 40) {
        // Sometimes the simulation get stuck because no fish is getting significantly better after a
        // long time (they are still all dying). in this case we start again the whole simulation
        if (timeSinceStart > 10000) {
            this->init(sizeEnv);
            return;
        }

        for (unsigned int i = 0; fishes.size() < 100; i++) {
            addFish();
        }
    }
}






void inputNNetwork(std::vector<float>& v1, Fish* e1, Fish* e2){
    v1.clear();
    // We send values between -1 and 1 to make the work easier for the neural network
    v1.push_back((e1->age-50)/50.0f);
    v1.push_back((e2->age-50)/50.0f);

    v1.push_back((e1->foodReserves -25)/25.0f);
    v1.push_back((e2->foodReserves -25)/25.0f);

    v1.push_back(e1->NN.geneticDistanceWith(e2->NN)/200.0f-1.0f); // can have a value > 1, but never < -1

    //  /!\ If modification are done on the equations, the changes 
    //      must be done in Analyser.cpp too or the analysis will be erroneous
}







void Engine::reproduce(Fish* e1, Fish* e2){
    // Each fish looses 15 food after reproduction (the new fish is born with 30)
    e1->foodReserves -= 15.0f;
    e2->foodReserves -= 15.0f;

    if (e1->foodReserves < -10 || e2->foodReserves < -10) return;

    Fish* e = new Fish(mult(add(e1->position, e2->position), 0.5f));

    sf::Color parentColor; //color of the parent fish
    
    if (rand()%2){ 
        e->NN.copy_NN(e1->NN);
        parentColor = e1->color;
    } else {
        e->NN.copy_NN(e2->NN);
        parentColor = e2->color;
    }

    // Mutation rate
    float mut = rand()%100;

    // Change randomly "mut"*100% of the neural network in order to apply mutations 
    if (mut == 0){       e->NN.randomlyMutate(0.5f); }
    else if (mut < 5){   e->NN.randomlyMutate(0.01f); }
    else if (mut < 10){   e->NN.randomlyMutate(0.002f); }
    else if (mut < 50){   e->NN.randomlyMutate(0.0005f); }
    else {               e->NN.randomlyMutate(0.0f); }

    // Copy the parent color, can change later when calculating the specie of each fish if 
    // we find that it doesn't belong to it's parents' specie
    e->color = parentColor;
    fishes.push_back(e);
}








bool fight(const Fish* f1, const Fish* f2){
    // true if f1 wins, false else

    // Equation on https://www.desmos.com/calculator/84yec9ya6v
    float score1 = (f1->age>30) ? (f1->age/30*100) : (100-((f1->age-30)/70*100));
    float score2 = (f2->age>30) ? (f2->age/30*100) : (100-((f2->age-30)/70*100));

    // Randomness to the result but statistically the higher the score the higher the chance of winning the fight
    return ((score1 * (rand()%100)) > score2*50);
}








void Engine::interact(Fish* f1, Fish* f2){
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
    f1->NN.setInputValues(v1);

    inputNNetwork(v1, f2, f1);
    f2->NN.setInputValues(v1);

    f1->NN.update();
    f2->NN.update();
    
    // Score of each type of interaction
    float sShare = f1->NN.getfinalValue(0) + f2->NN.getfinalValue(0);
    float sReproduction = f1->NN.getfinalValue(1) + f2->NN.getfinalValue(1);
    float sFight = f1->NN.getfinalValue(2) + f2->NN.getfinalValue(2);
    float sIgnore = f1->NN.getfinalValue(3) + f2->NN.getfinalValue(3);


   
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
        foodTotal = f1->foodReserves + f2->foodReserves;
        f1->foodReserves = foodTotal /2;
        f2->foodReserves = foodTotal /2;
        break;

    case 2:
        /* reproduce */
        reproduce(f1, f2);

    case 3:
        /* fight */
        if (fight(f1, f2)){
            f2->foodReserves -= 20;
            f1->foodReserves += 20;
        } else {
            f1->foodReserves -= 20;
            f2->foodReserves += 20;
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
