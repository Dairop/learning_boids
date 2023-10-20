#pragma once
#include "../Utils.hpp"


const std::string color1 = "blue";
const std::string color2 = "purple";
const std::string color3 = "red";
const std::string color4 = "green";


// Save the "data" vector as an svg graph with name "title" 
void saveAsSvg(std::string title, std::vector<float>& data, float graphWidth, std::string axisX, std::string axisY, float maxValAxisX, sf::Vector3f color, bool fill);

// Save a graph representing the % of the total sum each data vector represent at any point in the X axis (usually time)
void saveAsSvgWith4VectorsOneGraph(std::string title, std::vector<float>& data1, std::vector<float>& data2, std::vector<float>& data3, std::vector<float>& data4, std::string axisX, std::string axisY, std::string legend, float maxValAxisX);

// Save a graph representing the influence of two parameters on another variable. Looks like a heat map.
void createGraphWithFrequencyTwoParameters(std::string title, std::string axisX, std::string axisY, std::vector<sf::Vector3f>&data, unsigned int divisionsInX, unsigned int divisionsInY, sf::Vector3f color, std::string legend);


