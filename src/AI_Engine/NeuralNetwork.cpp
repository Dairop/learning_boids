#include "NeuralNetwork.hpp"



void NeuralNetwork::update(){
    unsigned int nbLayers = Layers.size();

    for (unsigned int i = 1; i < nbLayers; i++){
        Layers[i].update(&Layers[i-1]);
    }

    this->finalValues.clear();
    for (Neuron& n: Layers[nbLayers-1].neurons){
        this->finalValues.push_back(n.value);
    }

}




void NeuralNetwork::setInputValues(std::vector<float>& vals){
    if (Layers.size() == 0) return;
    if (vals.size() != Layers[0].neurons.size()) return;

    for (unsigned int i = 0; i < vals.size(); i++){
        Layers[0].neurons[i].value = vals[i];
    }
}



void NeuralNetwork::getLayers(std::vector<Layer>& vecLayers) const {
    vecLayers = Layers;
}



void NeuralNetwork::copy_NN(const NeuralNetwork& r2){
    this->Layers = r2.Layers;
}



//renvoie une valeur aléatoire entre -3 et 3 proba% du temps, sinon renvoie la valeur val
float randProba(float val, float proba /*de 0.0 à 1.0*/){
    if (rand()%10000 < proba*10000){
        return ((float (rand()%1000))-500)/166.7f;
    }
    return val;
}


void NeuralNetwork::randomlyMutate(const float MUTATION_PROB){
    for (Layer& c: this->Layers){
        for (Neuron& n: c.neurons){
            n.bias = randProba(n.bias, MUTATION_PROB);
            if (n.bias > 1.0f || n.bias < -1.0f) n.bias = ((float) (rand()%10000))/5000.0f-1.0f;
        }

        for (Synapse& s: c.synapses){
            s.weight = randProba(s.weight, MUTATION_PROB);
        }
    }
}





float NeuralNetwork::geneticDistanceWith(const NeuralNetwork& NN2) const{
    float s = 0;

    float v1, v2;

    for (unsigned int i = 0; i < this->Layers.size(); i++){
        for (unsigned int j = 0; j < this->Layers[i].neurons.size(); j++){
            v1 = this->Layers[i].neurons[j].bias;
            v2 = NN2.Layers[i].neurons[j].bias;

            s += fabs(v2 - v1);
        }

        for (unsigned int k = 0; k < this->Layers[i].synapses.size(); k++){
            v1 = this->Layers[i].synapses[k].weight;
            v2 = NN2.Layers[i].synapses[k].weight;

            s += fabs(v2 - v1);
        }
    }

    
    return s;
}
