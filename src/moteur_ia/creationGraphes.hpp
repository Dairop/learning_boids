#pragma once
#include "../Utils.hpp"


const std::string color1 = "blue";
const std::string color2 = "purple";
const std::string color3 = "red";
const std::string color4 = "green";


void enregisterEnSvg(std::string titre, std::vector<float>& data, float largeurGraphe, std::string axeX, std::string axeY, float maxValAxeX, sf::Vector3f couleur, bool remplir);

void enregistrerEnSvg4Vecteurs1Graphe(std::string titre, std::vector<float>& d1, std::vector<float>& d2, std::vector<float>& d3, std::vector<float>& d4, std::string axeX, std::string axeY, std::string legende, float maxValAxeX);

void creerGrapheFrequence2Parametres(std::string titre, std::string axeX, std::string axeY, std::vector<sf::Vector3f>& data, unsigned int nbDivisionsEnX, unsigned int nbDivisionsEnY, sf::Vector3f color, std::string legend);


