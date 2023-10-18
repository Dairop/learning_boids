#include "moteur.hpp"

const float SPEED = 5.0f; // penser à changer l'apport de nourriture après modification de la vitesse

//min et max des cycles de nourriture
unsigned int nbMinNoisettesParFrame;
unsigned int nbMaxNoisettesParFrame;

unsigned long int tempsDepuisDebut = 0;


void Moteur::init(sf::Vector2u szEnv){
    this->sizeEnv = szEnv;

    nbMinNoisettesParFrame = 2;
    nbMaxNoisettesParFrame = 4;

    especes.clear();


    //creer 100 Ecureuils
    for (unsigned int i = 0; i < 100; i++){
        ajouterEcureuil();
    }

    //ajouter des noisettes s'il y en a moins de 1000
    for (unsigned int i = 0; noisettes.size() < 1000; i++){
        ajouterNoisette();
    }
}




void Moteur::ajouterNoisette(){
    float _x, _y;

    int tx = (sizeEnv.x) - 10.0f;
    int ty = (sizeEnv.x) - 10.0f;

    _x = rand() % (tx)+5.0f;
    _y = rand() % (ty)+5.0f;

    noisettes.push_back(Entity(sf::Vector2f(_x, _y)));
}

void Moteur::ajouterEcureuil(){
    float _x, _y;

    int tx = (sizeEnv.x) - 10.0f;
    int ty = (sizeEnv.x) - 10.0f;

    _x = rand() % (tx)+5.0f;
    _y = rand() % (ty)+5.0f;

    Fish e(sf::Vector2f(_x, _y));
    ecureuils.push_back(e);
}









void Moteur::update(QuadTree& boidsQuad, QuadTree& foodQuad, sf::Time& dt){
    tempsDepuisDebut++;


    //crée des cycles d'abondance de nourriture et de manques
    //https://www.desmos.com/calculator/ezsdetdjqi 
    float a_cycles = nbMinNoisettesParFrame;
    float b_cycles = nbMaxNoisettesParFrame;
    int nbNoisettesParFrameCycles = a_cycles + (b_cycles-a_cycles) * (std::sin( ((float) tempsDepuisDebut)/10000 )*0.5+0.5) ;


    if (tempsDepuisDebut%1000 == 0){
        std::cout << "\nsaving data" << std::endl;

        //données pour l'analyse
        this->cyclesNourriture.push_back(nbNoisettesParFrameCycles);
        this->population.push_back(ecureuils.size());
        this->interactions.push_back(nbInteractions100Frames);
        nbInteractions100Frames = 0;


        //générer les graphes d'analyse
        if (tempsDepuisDebut%1000 == 0){
            std::cout << "\ncreating graphs" << std::endl;

            this->genererFichiersAnalyseGlobale();
        }
    }



    //ajouter n noisettes par frame
    for (unsigned int i = 0; i < nbNoisettesParFrameCycles; i++){
        ajouterNoisette();
    }


    if (this->framesDepuisDerniereMajEspece >= 100){
        std::cout << "\nupdating species" << std::endl;
        majLesEspeces();
        this->framesDepuisDerniereMajEspece = 0;
    } else {
        this->framesDepuisDerniereMajEspece++;
    }


    float timeElapsed = dt.asMicroseconds();
    //maj de tous les ecureuils
    for (Fish& e: ecureuils) {
        e.updateFish(sizeEnv, boidsQuad, foodQuad, dt);
        e.updateBody();

        e.foodReserves -= timeElapsed/200000.0f;
        e.age += timeElapsed / 500000.0f;
        e.timerInteraction -= timeElapsed/10000.0f;
        if (e.foodReserves < 0 || e.age > 100) e.isAlive = false;
    }

    //supprimer ceux qui ne sont plus en vie
    for (unsigned int i = 0; i < ecureuils.size(); ){
        if (!ecureuils[i].isAlive){
            ecureuils.erase(ecureuils.begin()+i);
        } else {
            i++;
        }
    }

    for (unsigned int i = 0; i < noisettes.size(); ) {
        if (!noisettes[i].isAlive) {
            noisettes.erase(noisettes.begin() + i);
        }
        else {
            i++;
        }
    }

    //ajouter des ecureuils aléatoires s'ils disparaissent
    if (ecureuils.size() < 100){
        //parfois la simulation reste bloquée et aucun agent n'arrive à prendre le dessus, dans ce cas on recommence la simulation
        if (tempsDepuisDebut > 1000) {
            this->init(sizeEnv);
            return;
        }

        for (unsigned int i = 0; i < 85; i++){
            ajouterEcureuil();
        }

        for (unsigned int i = 0; noisettes.size() < 750; i++){
            ajouterNoisette();
        }
    }


    //trouver les interactions
    for (unsigned int a = 0; a < ecureuils.size(); a++){
        rectByCenter r;
        r.center = ecureuils[a].position;
        r.radius = sf::Vector2f(ecureuils[a].size*2, ecureuils[a].size*2);

        std::vector<Entity*> neighbors;
        foodQuad.queryRangeCircle(r, neighbors);

        for (unsigned int b; b < neighbors.size(); b++) {
            if (ecureuils[a].timerInteraction <= 0.0f && ecureuils[b].timerInteraction <= 0.0f) {
                uneInteraction(ecureuils[a], ecureuils[b]);

                ecureuils[a].timerInteraction = ecureuils[a].maxTimerInteraction;
                ecureuils[b].timerInteraction = ecureuils[b].maxTimerInteraction;
            }
        }
    }
}





void Moteur::majLesEspeces(){
    majVecteurEspeces(especes, ecureuils);
    majEspeceDesIndividus(especes, ecureuils);
}




void entreesReseau(std::vector<float>& v1, Fish& e1, Fish& e2){
    v1.clear();
    //on renvoie les valeurs entre -1 et 1 pour que le réseau les analyse plus facilement
    v1.push_back((e1.age-50)/50.0f);
    v1.push_back((e2.age-50)/50.0f);

    v1.push_back((e1.foodReserves -25)/25.0f);
    v1.push_back((e2.foodReserves -25)/25.0f);

    v1.push_back(e1.NN.distanceDe(e2.NN)/200.0f-1.0f); // peut valoir plus de 1, min de -1

    //  /!\ si modification des calculs, il faudra aussi les modifier dans analyseur.cpp ou les analyses seront faussées
}






void Moteur::reproduire(Fish& e1, Fish& e2){
    //on utilise des pointeurs car on ne peut pas faire de redéfinition dans un switch case 
    Fish e = Fish(mult(add(e1.position, e2.position), 0.5f));
    sf::Color hsvC; // couleur de l'écureuil parent
    
    if (rand()%2){ 
        e.NN.copier(e1.NN);
        hsvC = RGBtoHSV(e1.color.r, e1.color.g, e1.color.b);
    } else {
        e.NN.copier(e2.NN);
        hsvC = RGBtoHSV(e2.color.r, e2.color.g, e2.color.b);
    }

    //taux de mutation
    
    float mut = rand()%100;
    if (mut == 0){       e.NN.modifierAleatoirement(0.5f); }
    else if (mut < 5){   e.NN.modifierAleatoirement(0.01f); }
    else if (mut < 10){   e.NN.modifierAleatoirement(0.002f); }
    else if (mut < 50){   e.NN.modifierAleatoirement(0.0005f); }
    else {               e.NN.modifierAleatoirement(0.0f); }


    // on modifie la valeur HSV de l'écureuil afin qu'il n'ait pas exactement la même que son parent
    /* utile si les espèces sont desativées
    float newHue = fmod(hsvC.x+((float) (rand()%100-49.5)/10.0f)+360.0f, 360.0f);
    e.couleur = HSVtoRGB(newHue, hsvC.y, hsvC.z);
    */


    ecureuils.push_back(e);

    //chaque ecureuil perd 15 de nourriture
    e1.foodReserves -= 15.0f;
    e2.foodReserves -= 15.0f;
}



bool combat(const Fish& e1, const Fish& e2){
    //true si e1 gagne, false sinon

    //https://www.desmos.com/calculator/84yec9ya6v
    float score1 = (e1.age>30) ? (e1.age/30*100) : (100-((e1.age-30)/70*100));
    float score2 = (e2.age>30) ? (e2.age/30*100) : (100-((e2.age-30)/70*100));


    return ((score1 * (rand()%100)) > score2*50);
}






void Moteur::uneInteraction(Fish& e1, Fish& e2){
    //trouver l'interaction optimale pour les deux
    /*
    eg:
                    e1          e2      somme
    partage         0.2         0.3     0.5
    reproduction    0.4         -0.6    -0.2
    se battre       -0.7        0.8     0.1
    ignorer         0.9         0.2     1.1

    On réalisera l'interaction "ignorer" car les écureuils ont décidés de s'ignorer (plus grande somme)

    */

    std::vector<float> v1;
    entreesReseau(v1, e1, e2);
    e1.NN.setValeursDepart(v1);

    entreesReseau(v1, e2, e1);
    e2.NN.setValeursDepart(v1);

    e1.NN.update();
    e2.NN.update();
    
    float sPartage = e1.NN.getValeurArrivee(0) + e2.NN.getValeurArrivee(0);
    float sReproduction = e1.NN.getValeurArrivee(1) + e2.NN.getValeurArrivee(1);
    float sBattre = e1.NN.getValeurArrivee(2) + e2.NN.getValeurArrivee(2);
    float sIgnorer = e1.NN.getValeurArrivee(3) + e2.NN.getValeurArrivee(3);


   
    float totalNourriture;

    float max = sPartage;
    unsigned int cas = 1;
    if (sReproduction > max) { max = sReproduction; cas = 2;}
    if (sBattre > max) { max = sBattre; cas = 3;}
    if (sIgnorer > max) { max = sIgnorer; cas = 0;}

    switch (cas)
    {
    case 1:
        /* share */
        totalNourriture = e1.foodReserves + e2.foodReserves;
        e1.foodReserves = totalNourriture/2;
        e2.foodReserves = totalNourriture/2;
        break;

    case 2:
        /* reproduce */
        reproduire(e1, e2);

    case 3:
        /* fight */
        if (combat(e1, e2)){
            e2.foodReserves -= 20;
            e1.foodReserves += 20;
        } else {
            e1.foodReserves -= 20;
            e2.foodReserves += 20;
        }


        break;
    
    default:
        /*ignore*/
        break;
    }
}











void Moteur::genererFichiersAnalyseGlobale(){
    enregisterEnSvg("Cycles_de_nourriture", this->cyclesNourriture, 1000.0f, "Temps", "Qtt de nourriture", tempsDepuisDebut, sf::Vector3f(50, 200, 20), false);
    enregisterEnSvg("Population_globale", this->population, 1000.0f, "Temps", "Population", tempsDepuisDebut, sf::Vector3f(0, 00, 255), false);
    enregisterEnSvg("Nb_d_interactions", this->interactions, 1000.0f, "Temps", "Interactions", tempsDepuisDebut, sf::Vector3f(0, 00, 255), false);
}
