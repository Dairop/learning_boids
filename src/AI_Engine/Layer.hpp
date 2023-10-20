#include "Synapse.hpp"

//rRandom float between -3 and 3
float rand3();


struct Layer {
    int size;
    int sizePreviousLayer;

    std::vector <Neuron> neurons;
    std::vector <Synapse> synapses; // Synapses from the previous layer to the current one

    Layer(int layerSize, int prevLayerSize): size(layerSize), sizePreviousLayer(prevLayerSize){
        synapses.clear();
        neurons.clear();

        for (unsigned int i = 0; i < layerSize; i++){
            neurons.push_back(Neuron());
            neurons[i].bias = rand3()/3.0f;

            // Every neuron of the current layer is linked to every neuron from the previous layer
            for (unsigned int j = 0; j < sizePreviousLayer; j++){
                Synapse sy;
                sy.idInputNeuron = j;
                sy.idOutputNeuron = i;
                sy.weight = rand3();

                synapses.push_back(sy);
            }
        }
    };

    void initNeuronsValues();
    void update(const Layer* pre);
};