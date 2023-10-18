#pragma once

#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <random>
#include <chrono>

#include <SFML/Graphics.hpp>

extern float tailleEnvironnement;


struct rectByCenter {
    sf::Vector2f center;
    sf::Vector2f radius;
};

sf::Vector2f add(const sf::Vector2f a, const sf::Vector2f b);
sf::Vector3f add(const sf::Vector3f a, const sf::Vector3f b);

sf::Vector2f sub(const sf::Vector2f a, const sf::Vector2f b);
sf::Vector3f sub(const sf::Vector3f a, const sf::Vector3f b);

sf::Vector2f mult(const sf::Vector2f a, const sf::Vector2f b);
sf::Vector3f mult(const sf::Vector3f a, const sf::Vector3f b);


sf::Vector2f mult(const sf::Vector2f a, const float le);
sf::Vector3f mult(const sf::Vector3f a, const float le);

sf::Vector2f normalize(sf::Vector2f vect);


float dist(const sf::Vector2f p1, const sf::Vector2f p2);
float dist2(const sf::Vector2f p1, const sf::Vector2f p2);



std::string v2toString(sf::Vector2f v);
std::string v3ToString(sf::Vector3f v);
std::string colorToString(sf::Color c);


sf::Color HSVtoRGB(float H, float S, float V); // h:0-360.0, s:0.0-1.0, v:0.0-1.0
sf::Color RGBtoHSV(float R, float G, float B); // (0-255, 0-255, 0-255) -> (0-360.0, 0.0-1.0, 0.0-1.0)


void coloredStdCout(std::string str, sf::Color color); // cout dans 7 couleurs possibles, on selectionne la plus proche de celle passee en parametre. Blanc par défaut


float randomNormalDistrib(float precision); //renvoie un nombre aléatoire entre 0 et 1 avec une distribution normale comme probabilité
//Basé sur le théorème de la limite centrale, precision est le nombre d'itérations de l'algorithme



std::string rgbToHex(int r, int g, int b);


//calcul de l amoyenne mobile d'une liste de valeurs, permet d'avoir des 
//graphes moins aléatoires (attention, on risque de perdre aussi des données en augmentant n)
std::vector<float> moyenneMobile(const std::vector<float>& vals, unsigned int n);
