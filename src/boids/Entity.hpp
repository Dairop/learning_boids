#pragma once
#include "commons.hpp"

struct Entity {
	sf::Vector2f position;
	bool isAlive = true;

	Entity();
	Entity(sf::Vector2f p) : position(p) {};
};