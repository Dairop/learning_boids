#include "Neurone.hpp"





void Neurone::execFonctionActivation(){
    switch (this->fonctionActivation)
    {
    case 1:
        //tangente hyperbolique
        this->valeur = std::tanh(this->valeur);
        break;

    case 2:
        //ReLU (Unité de Rectification Linéaire)
        this->valeur = std::fmax(0, this->valeur);
        break;
    
    case 3:
        //fonction Gaussienne - 0.5 (pour avoir des positifs et négatifs)
        this->valeur = std::exp(-this->valeur*this->valeur) - 0.5;
        break;
            
    case 4:
        //fonction sigmoïde
        this->valeur = 2 / (1 + std::exp(-this->valeur)) - 1;
        break;

    case 5:
        //Leaky ReLU
        this->valeur = (this->valeur > 0) ? this->valeur : 0.01 * this->valeur;
        break;

    default:
        // linéaire
        break;
    }
}