#pragma once
#include "Couche.hpp"



class ReseauNeurones {
    std::vector <Couche> couches;
    std::vector <float> valeursArrivee;

public:
    ReseauNeurones(){
        //les tailles de chaque couche sont définies manuellement
        //soit les paramètres (x1, y1) (x2, y2), ..., (xn, yn) avec n l'indice de la couche,
        // yn = x(n-1)
        //(x1, y1) = (5, 0) et x(n_max) = 4

        
        //reseaux simples, apprentissage rapide mais peu-complexe
        /*
        couches.push_back(Couche(5, 0));
        couches.push_back(Couche(6, 5));
        couches.push_back(Couche(4, 6));
        */
        
        

        //apprentissage plus long mais en théorie meilleur
        
        couches.push_back(Couche(5, 0));
        couches.push_back(Couche(9, 5));        
        couches.push_back(Couche(7, 9));
        couches.push_back(Couche(9, 7));
        couches.push_back(Couche(6, 9));
        couches.push_back(Couche(4, 6));
        
    }

    void copier(const ReseauNeurones& r2);
    void modifierAleatoirement(const float PROBABILITE_MUTATION); //0.10 modifie aléatoirement 10% des poids et biais dans le réseau

    void update();
    void setValeursDepart(std::vector<float>& vals);
    
    void getCouches(std::vector <Couche>& vecCouches) const;

    std::vector<float> getValeursArrivee() const { return this->valeursArrivee; }
    float getValeurArrivee(int i) const { return this->valeursArrivee[i]; }

    //calcule la différence entre 2 réseaux. Plus la valeur est petite, plus ils sont proches "génétiquement"
    float distanceDe(const ReseauNeurones& RN2) const;
};