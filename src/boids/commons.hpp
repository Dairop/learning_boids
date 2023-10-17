#ifndef COMMONS_HPP
#define COMMONS_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <chrono>
#include "../Utils.hpp"

bool pointInFrame(sf::Vector2f v, sf::IntRect r, float radius = 0);

#include "Boid.hpp"
#include "QuadTree.hpp"

#endif

