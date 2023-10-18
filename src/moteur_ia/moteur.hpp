#pragma once
#include "../Fish.hpp"
#include "Espece.hpp"
#include "Analyseur.hpp"

class Moteur{
private:
    sf::Vector2u sizeEnv;
    unsigned int framesDepuisDerniereMajEspece = 0;
    float nbInteractions100Frames = 0;

    //types vec float pour pouvoir les utiliser dans les fonctions d'analyse et création de graphes
    std::vector <float> cyclesNourriture; // un élément représente 1000 frames. Utilisé pour l'analyse finale.
    std::vector <float> population;       // un élément représente 1000 frames
    std::vector <float> interactions;



protected:
    std::vector<Fish>& ecureuils;
    std::vector<sf::Vector2f>& noisettes;
    std::vector<Espece> especes;


    void ajouterNoisette();
    void ajouterEcureuil();

public:  
    Moteur(std::vector<Fish>& ecur, std::vector<sf::Vector2f>& nois) :
        ecureuils(ecur), noisettes(nois) {}
    void init(sf::Vector2u szEnv);
    void update(QuadTree& quadTree, sf::Time& dt);
    void uneInteraction(Fish& e1, Fish& e2);
    void majLesEspeces();
    void reproduire(Fish& e1, Fish& e2);

    void genererFichiersAnalyseGlobale();
};
