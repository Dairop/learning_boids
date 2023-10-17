#include "Espece.hpp"

std::vector<Espece> topEspeces;
const unsigned int NB_ESPECES_TOP = 15; // nombre d'espèces sauvegardées dans le classement



const int DIVERSITE_GENETIQUE_UNE_ESPECE = 75.0f;

unsigned int tempsDepuisAnalyse = 0;
const unsigned int analyserTousLes = 10000;

unsigned int prevAnalyse = 0; // afin de ne pas analyser 2x le même réseau de neurones 


void majTop(std::vector<Espece>& especes) {
    if (especes.size() == 0) return;

    //le vecteur espèces est normalement trié du plus agé au plus jeune
    for (unsigned int i = 0; ((i < NB_ESPECES_TOP) && (i < especes.size())); i++){
        topEspeces.push_back(especes[i]);
    }

    std::sort(topEspeces.begin(), topEspeces.end());

    //supprimer les doublons
    for (unsigned int i = 0; i < topEspeces.size(); i++){
        for (int j = i+1; j < topEspeces.size(); j++){
            if (topEspeces[i]._id == topEspeces[j]._id){
                topEspeces[i]._age = topEspeces[j]._age;
                topEspeces.erase(topEspeces.begin()+j);
                j--;
            }
        }
    }

    while (topEspeces.size()>NB_ESPECES_TOP){
        topEspeces.erase(topEspeces.begin(), topEspeces.begin()+1);
    }


    Espece& top1 = topEspeces[topEspeces.size()-1];
    if (tempsDepuisAnalyse >= analyserTousLes && prevAnalyse != top1._id){
        prevAnalyse = top1._id;
        tempsDepuisAnalyse = 0;

        analyser(top1._reseauBase);	//analyse de la meilleure espèce depuis le début de la simulation
        //analyser(especes[especes.size()-1]._reseauBase); // on peut aussi analyser la meilleure espèce à être toujours en vie
    }
}



void majVecteurEspeces(std::vector<Espece>& especes, const std::vector<Fish>& ecureuils){
    for (Espece& esp: especes){
        esp._age++;
    }

    tempsDepuisAnalyse++;

    if (tempsDepuisAnalyse%10 == 0){
        majTop(especes);
    }

    //si aucune entité "n'utilise" une espèce, alors on l'oublie
    if (tempsDepuisAnalyse%10 == 0){
            for (unsigned int i = 0; i < especes.size();){
            bool utilise = false;
            for (unsigned int j = 0; j < ecureuils.size(); j++){
                utilise = utilise || (especes[i]._id == ecureuils[j].idEspece);
            }

            if (utilise) {
                i++;
            } else {
                especes.erase(especes.begin()+i);
            }
        }

        std::sort(topEspeces.begin(), topEspeces.end());
        afficherEspeces(topEspeces);
    }


    //crée une Espece pour chaque individu n'ayant pas déjà une espece de distance max DIVERSITE_GENETIQUE_...
    for (const Fish& ec: ecureuils){
        bool aUneEspece = false;

        for (Espece& esp: especes){
            if (ec.NN.distanceDe(esp._reseauBase) < DIVERSITE_GENETIQUE_UNE_ESPECE){
                aUneEspece = true;
                break;
            }
        }

        if (!aUneEspece){
            especes.push_back(Espece(rand(), ec.NN));
        }
    }
}




void majEspeceDesIndividus(const std::vector<Espece>& especes, std::vector<Fish>& ecureuils){
    //soit un vecteur especes correct, attribuer à chaque écureuil son espèce favorite
    for (Fish& ec: ecureuils){
        float dist_min = 1.7976931348623e+308;

        for (const Espece& esp: especes){
            float dist_test = ec.NN.distanceDe(esp._reseauBase);
            if (dist_test < dist_min){
                dist_min = dist_test;
                ec.idEspece =  esp._id;
                ec.color = esp._color;
            }
        }
    }
}








std::string Espece::toString() const {
    return "Age: "+std::to_string(_age)+"  id: "+std::to_string(_id) +"  Couleur: "+ v3ToString(_color) +"\n"; 
}

void afficherEspeces(const std::vector<Espece>& especes) {
    std::cout << "\n\n\n\n\n";

    for (unsigned int i = 0; i < especes.size();i++){
        coloredStdCout(especes[i].toString(), especes[i]._color);
    }
}