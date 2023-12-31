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


	unsigned int speciesId = 0;
	NeuralNetwork NN;
	sf::Color color;

	float age;
	float foodReserves;

	//one entity have to wait some time after interacting with another one in order for them to 
	// get away from one another and endure the result of the previous one
	float timerInteraction;
	float maxTimerInteraction;



	void updateBody();
	void draw(sf::VertexArray& va, const int start) override;
	void init();
	void updateFish(sf::Vector2u screenSize, QuadTree& boidsQuad, QuadTree& foodQuad, long dt);

	Fish(sf::Vector2f pos) : Boid(pos) {
		isAlive = true;
		init();
	}
};

