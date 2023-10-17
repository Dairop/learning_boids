#include "Synapse.hpp"

//float aléatoire entre -3 et 3
float rand3();


struct Couche {
    int taille;
    int tailleCouchePre;

    std::vector <Neurone> neurones;
    std::vector <Synapse> synapses; //synapses allant de la couche précédente à la couche actuelle

    Couche(int tailleCouche, int tailleCouchePrecedente): taille(tailleCouche), tailleCouchePre(tailleCouchePrecedente){
        synapses.clear();
        neurones.clear();

        for (unsigned int i = 0; i < tailleCouche; i++){
            neurones.push_back(Neurone());
            neurones[i].biais = rand3()/3.0f;

            //chaque neurone d'une couche est lié à tous les neurones de la couche précédente
            for (unsigned int j = 0; j < tailleCouchePrecedente; j++){
                Synapse sy;
                sy.idNeuroneDepart = j;
                sy.idNeuroneArrivee = i;
                sy.poids = rand3();

                synapses.push_back(sy);
            }
        }
    };

    void initValeursNeurones();
    void update(const Couche* pre);
};