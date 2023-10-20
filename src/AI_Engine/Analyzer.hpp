#pragma once
#include "NeuralNetwork.hpp"
#include "GraphsCreation.hpp"


void analize(NeuralNetwork& nn); // full analysis if "do_complete_analysis" = true

void behaviorsScores(NeuralNetwork& r, int parameter); // behaviors scores evolution depending on the value for the parameter p

void favoriteBehavior(NeuralNetwork& r, int parameter); // evolution of the % of time each behavior have the biggest score depending on the value for the parameter p
