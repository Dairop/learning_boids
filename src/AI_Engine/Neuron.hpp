#include "../Utils.hpp"

struct Neuron {
    float value = 0;
    float bias = 0;
    short int activationFunction = 4; /* activation function index
                                        1: hyperbolic tangent
                                        2: ReLU (Rectification Linear Unit)     
                                        3: Gaussian function
                                        4: Sigmoid function
                                        5: Leaky ReLU function
                                        6: Linear function
                                  */
    void execActivationFonction();
};