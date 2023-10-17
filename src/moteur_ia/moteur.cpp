#include "moteur.hpp"

const float SPEED = 5.0f; // penser à changer l'apport de nourriture après modification de la vitesse

//min et max des cycles de nourriture
unsigned int nbMinNoisettesParFrame;
unsigned int nbMaxNoisettesParFrame;

unsigned long int tempsDepuisDebut = 0;


void Moteur::init(sf::Vector2u szEnv){
    this->sizeEnv = szEnv;

    nbMinNoisettesParFrame = 3;
    nbMaxNoisettesParFrame = 3;

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

    _x = rand()%(tx) - tx;
    _y = rand()%(ty) - ty;

    noisettes.push_back(sf::Vector2f(_x, _y));
}

void Moteur::ajouterEcureuil(){
    float _x, _y;

    int tx = (sizeEnv.x) - 10.0f;
    int ty = (sizeEnv.x) - 10.0f;

    _x = rand() % (tx)-tx;
    _y = rand() % (ty)-ty;

    Fish e(sf::Vector2f(_x, _y));
    ecureuils.push_back(e);
}









void Moteur::update(){
    tempsDepuisDebut++;

    //crée des cycles d'abondance de nourriture et de manques
    //https://www.desmos.com/calculator/ezsdetdjqi 
    float a_cycles = nbMinNoisettesParFrame;
    float b_cycles = nbMaxNoisettesParFrame;
    int nbNoisettesParFrameCycles = a_cycles + (b_cycles-a_cycles) * (std::sin( ((float) tempsDepuisDebut)/10000 )*0.5+0.5) ;


    if (tempsDepuisDebut%1000 == 0){
        //données pour l'analyse
        this->cyclesNourriture.push_back(nbNoisettesParFrameCycles);
        this->population.push_back(ecureuils.size());
        this->interactions.push_back(nbInteractions100Frames);
        nbInteractions100Frames = 0;

        //générer les graphes d'analyse
        if (tempsDepuisDebut%100000 == 0){
            this->genererFichiersAnalyseGlobale();
        }
    }



    //ajouter n noisettes par frame
    for (unsigned int i = 0; i < nbNoisettesParFrameCycles; i++){
        ajouterNoisette();
    }


    if (this->framesDepuisDerniereMajEspece >= 100){
        majLesEspeces();
        this->framesDepuisDerniereMajEspece = 0;

    } else {
        this->framesDepuisDerniereMajEspece++;
    }



    //maj de tous les ecureuils
    for (Fish& e: ecureuils) {
        //e.update(noisettes);
    }

    //supprimer ceux qui ne sont plus en vie
    for (unsigned int i = 0; i < ecureuils.size(); ){
        if (!ecureuils[i].isAlive){
            ecureuils.erase(ecureuils.begin()+i);
        } else {
            i++;
        }
    }

    //ajouter des ecureuils aléatoires s'ils disparaissent
    if (ecureuils.size() < 15){
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


    //trouver les interactions et collisions
    /*
    for (unsigned int a = 0; a < ecureuils.size(); a++){
        for (unsigned int b = a+1; b < ecureuils.size(); b++){
            float d2 = dist2(ecureuils[a].position, ecureuils[b].position);
            if (d2 < (ecureuils[a].taille+ecureuils[b].taille)*(ecureuils[a].taille+ecureuils[b].taille)){
                //les ecureuils a et b se touchent
                collision2cercles(ecureuils[a].position, ecureuils[b].position, ecureuils[a].orientation, ecureuils[b].orientation, 
                ecureuils[a].taille, ecureuils[b].taille, d2);

                //interaction ?
                if (ecureuils[a].timerInteraction <= 0.0f && ecureuils[b].timerInteraction <= 0.0f){
                    uneInteraction(ecureuils[a], ecureuils[b]);
                    
                    ecureuils[a].timerInteraction = ecureuils[a].maxTimerInteraction;
                    ecureuils[b].timerInteraction = ecureuils[b].maxTimerInteraction;
                }
            }
        }
    }
    */
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
    sf::Vector3f hsvC; // couleur de l'écureuil parent
    
    if (rand()%2){ 
        e.NN.copier(e1.NN);
        hsvC = RGBtoHSV(e1.color.x, e1.color.y, e1.color.z);
    } else {
        e.NN.copier(e2.NN);
        hsvC = RGBtoHSV(e2.color.x, e2.color.y, e2.color.z);
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
    enregisterEnSvg("Cycles de nourriture", this->cyclesNourriture, 1000.0f, "Temps", "Qtt de nourriture", tempsDepuisDebut, sf::Vector3f(50, 200, 20), false);
    enregisterEnSvg("Population globale", this->population, 1000.0f, "Temps", "Population", tempsDepuisDebut, sf::Vector3f(0, 00, 255), false);
    enregisterEnSvg("Nb d'interactions", this->interactions, 1000.0f, "Temps", "Interactions", tempsDepuisDebut, sf::Vector3f(0, 00, 255), false);
}
