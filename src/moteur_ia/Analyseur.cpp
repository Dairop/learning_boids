#include "Analyseur.hpp"


bool activer_analyse_complete = true;



//Etudie les comportements avec des valeurs d'entrée aléatoires
void comportementLePlusCourrant(ReseauNeurones& r){
    std::vector<float> depart(5);

    float nbPartage = 0, nbRepro = 0, nbBattre = 0, nbIgnore = 0;

    for (unsigned int i = 0; i < 10000; i++){
        depart.clear();
        for (unsigned int j = 0; j < 5; j++){
            depart.push_back(rand3()/3.0f);
        }

        r.setValeursDepart(depart);
        
        r.update();


        float sPartage = r.getValeurArrivee(0);
        float sReproduction = r.getValeurArrivee(1);
        float sBattre = r.getValeurArrivee(2);
        float sIgnorer = r.getValeurArrivee(3);

        float max = sPartage;
        short unsigned int cas = 1;
        if (sReproduction > max) { max = sReproduction; cas = 2;}
        if (sBattre > max) { max = sBattre; cas = 3;}
        if (sIgnorer > max) { max = sIgnorer; cas = 0;}

        switch (cas)
        {
        case 1:
            nbPartage++;
            break;

        case 2:
            nbRepro++;
            break;

        case 3:
            nbBattre++;
            break;

        default:
            nbIgnore++;
            break;
        }
    }


    std::cout << "ProbaPartage: " << nbPartage / 100.0f << "%  ProbaRepro: " << nbRepro / 100.0f 
              << "%  ProbaCombat: " << nbBattre / 100.0f << "%  ProbaIgnore: " << nbIgnore / 100.0f << "%\n";
    
}










void comportementsScores(ReseauNeurones& r, int parametre){
    std::vector<float> vecPartage;
    std::vector<float> vecRepro;
    std::vector<float> vecCombat;
    std::vector<float> vecIgnore;

    std::vector<float> depart(5); //valeurs que l'on va rentrer dans chaque réseau

    std::string axeX;

    float valTestee, valMax;

    if (parametre == 4) { // on teste la distance génétique
        valTestee = 0.0f;
        valMax = 400.0f;
        axeX = "distance génétique";
    } else if (parametre == 0){ //par rapport à l'age de l'individu étudié
        valTestee = 0.0f;
        valMax = 100.0f;
        axeX = "age";
    } else if (parametre == 1){ //par rapport à l'age de l'autre individu étudié
        valTestee = 0.0f;
        valMax = 100.0f;
        axeX = "age autre individu";
    } else if (parametre == 2){ //par rapport aux reserves de nourriture de l'individu étudié
        valTestee = 0.0f;
        valMax = 100.0f;
        axeX = "reserves nourriture";
    } else if (parametre == 3){ //par rapport aux reserves de nourriture de l'autre individu étudié
        valTestee = 0.0f;
        valMax = 100.0f;
        axeX = "reserves nourriture autre individu";
    } 

    if (valMax == 0) return;

    float precision = 1000; //nombre de barres sur le graphe final (précision)
    float nombreIterations = 1000; // nombre d'itérations pour chaque barre, permet de mitiger l'effet de l'aléatoire grâce à la moyenne

    for (float barre = 0; barre < precision; barre++){
        float totalPartage = 0;
        float totalRepro = 0;
        float totalCombat = 0;
        float totalIgnore = 0;

        //on repete 100x le calcul avec des valeurs différentes pour réduire le caractère aléatoire
        for (unsigned int iter = 0; iter < nombreIterations; iter++){
            valTestee = barre / precision * valMax;

            depart.clear();

            if (parametre == 4){ //distance génétique
                for (unsigned int j = 0; j < 4; j++){
                    depart.push_back(rand3()/3.0f);
                }
                depart.push_back(valTestee/200.0f-1.0f); // changer le calcul s'il est modifié dans moteur.cpp

            } else if (parametre == 0){
                depart.push_back((valTestee-50)/50.0f); // changer le calcul s'il est modifié dans moteur.cpp
                for (unsigned int j = 0; j < 4; j++){
                    depart.push_back(rand3()/3.0f);
                }
            } else if (parametre == 1){
                depart.push_back(rand3()/3.0f);
                depart.push_back((valTestee-50)/50.0f); // changer le calcul s'il est modifié dans moteur.cpp
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);
            } else if (parametre == 2){
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);
                depart.push_back((valTestee-50)/50.0f); // changer le calcul s'il est modifié dans moteur.cpp
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);
            } else if (parametre == 3){
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);
                depart.push_back((valTestee-50)/50.0f); // changer le calcul s'il est modifié dans moteur.cpp
                depart.push_back(rand3()/3.0f);
            }

            r.setValeursDepart(depart);

            r.update();

            totalPartage += r.getValeurArrivee(0)+1;
            totalRepro += r.getValeurArrivee(1)+1;
            totalCombat += r.getValeurArrivee(2)+1;
            totalIgnore += r.getValeurArrivee(3)+1;        }

        
        float total;
        if (true) { //if true, on représentera chaque score dans le graphe en % du total et pas en valeur absolue
            total = totalPartage + totalRepro + totalCombat + totalIgnore;
        } else {
            total = 1;
        }

        vecPartage.push_back(totalPartage/total);
        vecRepro.push_back(totalRepro/total);
        vecCombat.push_back(totalCombat/total);
        vecIgnore.push_back(totalIgnore/total);
    } 



    if (false){ // diviser par le nombre d'itérations
        for (unsigned int j = 0; j < nombreIterations; j++){
            vecPartage[j] /= nombreIterations;
            vecRepro[j] /= nombreIterations;
            vecCombat[j] /= nombreIterations;
            vecIgnore[j] /= nombreIterations;
        }
    }


    
    vecPartage = moyenneMobile(vecPartage, 10);
    vecCombat = moyenneMobile(vecCombat, 10);
    vecIgnore = moyenneMobile(vecIgnore, 10);
    vecRepro = moyenneMobile(vecRepro, 10);
    

    enregistrerEnSvg4Vecteurs1Graphe("score de chaque choix en fonction de la "+axeX, vecPartage, vecRepro, vecCombat, vecIgnore, axeX, "frequence", "Bleu: Partage | Violet: Reproduction | Rouge: Combat | Vert: Ignorer", valMax);
}













void comportementFavori(ReseauNeurones& r, int parametre){
    std::vector<float> vecPartage;
    std::vector<float> vecRepro;
    std::vector<float> vecCombat;
    std::vector<float> vecIgnore;

    std::vector<float> depart(5); //valeurs que l'on va rentrer dans chaque réseau

    std::string axeX;

    float valTestee, valMax;

    if (parametre == 4) { // on teste la distance génétique
        valTestee = 0.0f;
        valMax = 400.0f;
        axeX = "distance génétique";
    } else if (parametre == 0){ //par rapport à l'age de l'individu étudié
        valTestee = 0.0f;
        valMax = 100.0f;
        axeX = "age";
    } else if (parametre == 1){ //par rapport à l'age de l'autre individu étudié
        valTestee = 0.0f;
        valMax = 100.0f;
        axeX = "age autre individu";
    } else if (parametre == 2){ //par rapport aux reserves de nourriture de l'individu étudié
        valTestee = 0.0f;
        valMax = 100.0f;
        axeX = "reserves nourriture";
    } else if (parametre == 3){ //par rapport aux reserves de nourriture de l'autre individu étudié
        valTestee = 0.0f;
        valMax = 100.0f;
        axeX = "reserves nourriture autre individu";
    } 

    if (valMax == 0) return;

    float precision = 400; //nombre de barres sur le graphe final (précision)
    float nombreIterations = 500; // nombre d'itérations pour chaque barre, permet de mitiger l'effet de l'aléatoire grâce à la moyenne

    for (float barre = 0; barre < precision; barre++){
        float totalPartage = 0;
        float totalRepro = 0;
        float totalCombat = 0;
        float totalIgnore = 0;

        //on repete 100x le calcul avec des valeurs différentes pour réduire le caractère aléatoire
        for (unsigned int iter = 0; iter < nombreIterations; iter++){
            valTestee = barre / precision * valMax;

            depart.clear();

            if (parametre == 0){
                depart.push_back((valTestee-50)/50.0f); // changer le calcul s'il est modifié dans moteur.cpp
                for (unsigned int j = 0; j < 4; j++){
                    depart.push_back(rand3()/3.0f);
                }

            } else if (parametre == 1){
                depart.push_back(rand3()/3.0f);
                depart.push_back((valTestee-50)/50.0f); // changer le calcul s'il est modifié dans moteur.cpp
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);

            } else if (parametre == 2){
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);
                depart.push_back((valTestee-50)/50.0f); // changer le calcul s'il est modifié dans moteur.cpp
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);

            } else if (parametre == 3){
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);
                depart.push_back((valTestee-50)/50.0f); // changer le calcul s'il est modifié dans moteur.cpp
                depart.push_back(rand3()/3.0f);

            } else if (parametre == 4){ //distance génétique
                for (unsigned int j = 0; j < 4; j++){
                    depart.push_back(rand3()/3.0f);
                }
                depart.push_back(valTestee/200.0f-1.0f); // changer le calcul s'il est modifié dans moteur.cpp

            }

            r.setValeursDepart(depart);

            r.update();

            float max = r.getValeurArrivee(0); //score du choix actuel
            unsigned int cas = 1; // choix du réseau
            if (r.getValeurArrivee(1) > max) { max = r.getValeurArrivee(1); cas = 2;}
            if (r.getValeurArrivee(2) > max) { max = r.getValeurArrivee(2); cas = 3;}
            if (r.getValeurArrivee(3) > max) { max = r.getValeurArrivee(3); cas = 0;}

            switch (cas){ //ajouter 1 au nb total de x que ce choix a été réalisé
                case 1:
                    totalPartage++;
                    break;

                case 2:
                    totalRepro++;
                    break;

                case 3:
                    totalCombat++;
                    break;

                default:
                    totalIgnore++;
                    break;
            }
        }

        float total = totalPartage+totalRepro+totalCombat+totalIgnore;

        vecPartage.push_back(totalPartage/total);
        vecRepro.push_back(totalRepro/total);
        vecCombat.push_back(totalCombat/total);
        vecIgnore.push_back(totalIgnore/total);

    } 

    
    vecPartage = moyenneMobile(vecPartage, precision/400.0f*10);
    vecCombat = moyenneMobile(vecCombat, precision/400.0f*10);
    vecIgnore = moyenneMobile(vecIgnore, precision/400.0f*10);
    vecRepro = moyenneMobile(vecRepro, precision/400.0f*10);
    

    enregistrerEnSvg4Vecteurs1Graphe("Choix en fonction de "+axeX, vecPartage, vecRepro, vecCombat, vecIgnore, axeX, "frequence", "Bleu: Partage | Violet: Reproduction | Rouge: Combat | Vert: Ignorer", valMax);

}







void testVariablesLiees(ReseauNeurones& r, short int v1, short int v2, short int sortie){
    /*
        vn = 0 -> age 1
        vn = 1 -> age 2
        vn = 2 -> nourriture 1
        vn = 3 -> nourriture 2
        vn = 4 -> distance génétique

        sorties
        0 -> Partage
        1 -> Reproduction
        2 -> Combat
        3 -> Ignorer
    */

    std::string nomX, nomY, nomSortie;
    std::string listeNoms[] = {"age1", "age2", "nourriture1", "nourriture2", "distance genetique"};
    nomX = listeNoms[v1];
    nomY = listeNoms[v2];

    std::string lstNomsSortie[] = {"partage", "reproduction", "combat", "ignorer"};
    nomSortie = lstNomsSortie[sortie];




    std::vector<sf::Vector3f> data;
    std::vector<float> depart(5); //valeurs que l'on va rentrer dans chaque réseau

    unsigned int nbDivisions = 100;     // nombre de case par ligne/colonne
    const unsigned int PRECISION = 500; // nombre d'itérations par case, permet de réaliser une moyenne

    for (float x = 0; x < nbDivisions+1; x++){
        for (float y = 0; y < nbDivisions+1; y++){
            float vTotal = 0;

            for (unsigned int it = 0; it < PRECISION; it++) {
                depart.clear();

                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);
                depart.push_back(rand3()/3.0f);

                depart[v1] = x/50.0f - 1.0f;
                depart[v2] = y/50.0f - 1.0f;

                r.setValeursDepart(depart);
                r.update();

                float valArr = r.getValeurArrivee(sortie)/2+0.5f;

                vTotal += valArr;
            }

            vTotal /= PRECISION;
            
            if (v1 == 4 && v2 == 4) { data.push_back(sf::Vector3f(x*4.0f, y*4.0f, vTotal)); }
            else if (v1 == 4){ data.push_back(sf::Vector3f(x*4.0f, y, vTotal)); }
            else if (v2 == 4) {data.push_back(sf::Vector3f(x, y*4.0f, vTotal)); }
            else { data.push_back(sf::Vector3f(x, y, vTotal)); }
            
        }
    }


    sf::Vector3f couleurGraphe = sf::Vector3f(255, 0, 0);
    if (nomSortie == "partage") {
        couleurGraphe = sf::Vector3f(0, 0, 255);
    } else if (nomSortie == "reproduction"){
        couleurGraphe = sf::Vector3f(255, 0, 125);
    } else if (nomSortie == "ignorer"){
        couleurGraphe = sf::Vector3f(0, 255, 0);
    }




    creerGrapheFrequence2Parametres("lien entre deux variables: "+ nomX + " " + nomY + " -> " + nomSortie, nomX, nomY, data, nbDivisions, nbDivisions, couleurGraphe, "lien entre deux variables: "+nomX+" et "+nomY+" sur le comportement: " + nomSortie);
}








void analyser(ReseauNeurones& r){
    if (!activer_analyse_complete) return;


    //comportementLePlusCourrant(r);  //statistiques dans la console (peu utile)
    //comportementsScores(r, 4);    //score d'un type d'interaction par rapport à une variable

    //graphes choix du type préféré d'interaction
    comportementFavori(r, 0);
    comportementFavori(r, 1);
    comportementFavori(r, 2);
    comportementFavori(r, 3);
    comportementFavori(r, 4);

    //heatmaps
    for (short int var1 = 0; var1 < 5; var1++){
        for (short int var2 = var1 +1 ; var2 < 5; var2++){
            for (short int var3 = 0; var3 < 4; var3++){    
                if (var1 != var2) testVariablesLiees(r, var1, var2, var3);
            }
        }   
    }

}