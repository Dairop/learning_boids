#include "../Utils.hpp"

struct Neurone {
    float valeur;
    float biais;
    short int fonctionActivation = 4; /* indice de la fonction d'activation parmi une liste donnée
                                        1: tangente hyperbolique   
                                        2: ReLU (Unité de Rectification Linéaire)     
                                        3: fonction Gaussienne
                                        4: fonction sigmoïde
                                        5: fonction Leaky ReLU
                                        6: fonction linéaire
                                  */
    void execFonctionActivation();
};