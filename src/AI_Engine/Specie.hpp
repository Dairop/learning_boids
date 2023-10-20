#include "NeuralNetwork.hpp"
#include "../Fish.hpp"
#include "Analyzer.hpp"


#ifndef Specie_def
#define Specie_def

struct Specie {
public:
    unsigned int _id;
    NeuralNetwork _NN;
    unsigned int _age = 0;
    sf::Color _color;

    Specie(unsigned int id, NeuralNetwork n): _id(id), _NN(n) {
        _color = HSVtoRGB(_id%360, 1.0f, 1.0f);
    };
    std::string toString() const;

    bool operator<(const struct Specie& other) const 
    {
        return _age < other._age || (_age == other._age && _id < other._id);
    }
};



extern std::vector<Specie> topSpecies; // top of species with the best longevity (alive and dead ones)



void updateSpeciesVector(std::vector<Specie>& Species, const std::vector<Fish>& ecureuils);
void updateEntitiesSpecies(const std::vector<Specie>& Species, std::vector<Fish>& ecureuils);
void displaySpecies(const std::vector<Specie>& Species);


#endif