#pragma once
#include "Commons.hpp"

struct Entity {
	sf::Vector2f position;
	bool isAlive = true;

	int _idE = rand() % 1000000;

	Entity();
	Entity(sf::Vector2f p) : position(p) {};
};