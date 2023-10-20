#include "../Utils.hpp"

struct Neuron {
    float value;
    float bias;
    short int activationFunction = 4; /* indice de la fonction d'activation parmi une liste donnée
                                        1: tangente hyperbolique   
                                        2: ReLU (Unité de Rectification Linéaire)     
                                        3: fonction Gaussienne
                                        4: fonction sigmoïde
                                        5: fonction Leaky ReLU
                                        6: fonction linéaire
                                  */
    void execActivationFonction();
};