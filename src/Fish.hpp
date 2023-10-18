#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>

#include "boids/Boid.hpp"

class Fish: public Boid {
public:
	std::vector<sf::Color> colors;
	std::vector<sf::Vector2f> body;
	unsigned int bodyLen = 8;
	float fishLenght = size * 5;
	float nodeLen = fishLenght / bodyLen;


	unsigned int idEspece = 0;
	ReseauNeurones NN;
	sf::Color color;

	float age;
	float foodReserves;



	void updateBody();
	void draw(sf::VertexArray& va, int start) override;
	void init();
	void updateFish(sf::Vector2u screenSize, QuadTree& boidsQuad, QuadTree& foodQuad, sf::Time& dt);

	Fish(sf::Vector2f pos) : Boid(pos) {
		isAlive = true;
		init();
	}
};

