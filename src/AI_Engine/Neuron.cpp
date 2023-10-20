#include "Neuron.hpp"





void Neuron::execActivationFonction(){
    switch (this->activationFunction)
    {
    case 1:
        //hyperbolic tangent
        this->value = std::tanh(this->value);
        break;

    case 2:
        //ReLU (Linear Rectification Unit)
        this->value = std::fmax(0, this->value);
        break;
    
    case 3:
        //Gaussian function - 0.5 (to have positives and negatives)
        this->value = std::exp(-this->value *this->value) - 0.5;
        break;
            
    case 4:
        //sigmoid function
        this->value = 2 / (1 + std::exp(-this->value)) - 1;
        break;

    case 5:
        //Leaky ReLU
        this->value = (this->value > 0) ? this->value : 0.01 * this->value;
        break;

    default:
        // linear
        break;
    }
}