#include "Neuron.hpp"

struct Synapse {
    unsigned int idInputNeuron;
    unsigned int idOutputNeuron;
    float weight;
};