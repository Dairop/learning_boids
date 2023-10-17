#include "Couche.hpp"


float rand3(){
    return (rand()%1000-500)/166.67f;
}


void Couche::initValeursNeurones(){
    for(Neurone& n: neurones){
        n.valeur = 0;
    }
}



void Couche::update(const Couche* pre){
    initValeursNeurones();

    if (pre == nullptr) return;

    for (Synapse& sy: synapses){
        float vs = pre->neurones[sy.idNeuroneDepart].valeur;
        vs *=  sy.poids;
        this->neurones[sy.idNeuroneArrivee].valeur += vs;
    }


    //fonctionActivation pour tous les neurones de la couche
    for(Neurone& n: neurones){
        n.valeur += n.biais;
        n.execFonctionActivation();
    }


}