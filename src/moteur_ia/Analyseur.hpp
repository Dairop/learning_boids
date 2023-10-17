#pragma once
#include "ReseauNeurones.hpp"
#include "creationGraphes.hpp"


void analyser(ReseauNeurones& r);

// crée un graphe avec le comportement favori du réseau en fonction de p
/*
 p = 0 -> age 1
 p = 1 -> age 2
 p = 2 -> nourriture 1
 p = 3 -> nourriture 2
 p = 4 -> distance génétique
*/
void comportementsScores(ReseauNeurones& r, int parametre);

void comportementFavori(ReseauNeurones& r, int parametre);
