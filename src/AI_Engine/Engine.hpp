#pragma once
#include "../Fish.hpp"
#include "Specie.hpp"
#include "Analyzer.hpp"

class Engine{
private:
    sf::Vector2u sizeEnv;
    unsigned int framesSinceSpeciesUpdate = 0;
    unsigned long long int framesSinceStart = 0;
    float nbInteractions100Frames = 0;

    //types vec float pour pouvoir les utiliser dans les fonctions d'analyse et création de graphes
    std::vector <float> foodCyclesData; // un élément représente 1000 frames. Utilisé pour l'analyse finale.
    std::vector <float> populationData;       // un élément représente 1000 frames
    std::vector <float> interactionsData;



protected:
    std::vector<Fish>& fishes;
    std::vector<Entity>& food;
    std::vector<Specie> species;


    void addFood();
    void addFish();

public:  
    Engine(std::vector<Fish>& fishVect, std::vector<Entity>& foodVect) :
        fishes(fishVect), food(foodVect) {}

    void init(sf::Vector2u szEnv);

    void update(QuadTree& quadTree, QuadTree& foodQuad, long dt);

    //interaction between two fishes
    void interact(Fish& e1, Fish& e2);
    //analyze the different species, create new ones if needed to categorize the fishes
    void updateSpecies();
    //reproduction between two fishes
    void reproduce(Fish& e1, Fish& e2);

    //generate 3 graphs about 3 data over time: Population, number of interactions, food cycles
    void generateGlobalAnalysisFiles();
};
