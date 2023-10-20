#include "Layer.hpp"


float rand3(){
    return (rand()%1000-500)/166.67f;
}


void Layer::initNeuronsValues(){
    for(Neuron& n: neurons){
        n.value = 0;
    }
}



void Layer::update(const Layer* pre){
    initNeuronsValues();

    if (pre == nullptr) return;

    for (Synapse& sy: synapses){
        float vs = pre->neurons[sy.idInputNeuron].value;
        vs *=  sy.weight;
        this->neurons[sy.idOutputNeuron].value += vs;
    }


    //fonctionActivation pour tous les neurons de la Layer
    for(Neuron& n: neurons){
        n.value += n.bias;
        n.execActivationFonction();
    }


}