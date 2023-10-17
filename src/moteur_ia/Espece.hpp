#include "ReseauNeurones.hpp"
#include "../Fish.hpp"
#include "Analyseur.hpp"


#ifndef espece_def
#define espece_def

struct Espece {
public:
    unsigned int _id;
    ReseauNeurones _reseauBase;
    unsigned int _age = 0;
    sf::Vector3f _color;

    Espece(unsigned int id, ReseauNeurones reseauBase): _id(id), _reseauBase(reseauBase) {
        _color = HSVtoRGB(_id%360, 1.0f, 1.0f);
    };
    std::string toString() const;

    bool operator<(const struct Espece& other) const 
    {
        return _age < other._age || (_age == other._age && _id < other._id);
    }
};



extern std::vector<Espece> topEspeces; // top des espèces avec la meilleure longevité



void majVecteurEspeces(std::vector<Espece>& especes, const std::vector<Fish>& ecureuils);
void majEspeceDesIndividus(const std::vector<Espece>& especes, std::vector<Fish>& ecureuils);
void afficherEspeces(const std::vector<Espece>& especes);


#endif