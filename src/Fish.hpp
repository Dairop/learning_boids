#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>

#include "boids/Boid.hpp"

class Fish: public Boid {
public:
	sf::Vector2f position; //position of the head
	std::vector<Boid*> spinePoints;


	unsigned int idEspece;
	ReseauNeurones NN;
	sf::Vector3f color;

	float age;
	float foodReserves;

	bool isAlive;


	void updateSpine(int iterationsLeft);
	void draw(sf::VertexArray& va, int start) override;

	Fish(sf::Vector2f pos) : Boid(pos) {
		isAlive = true;
	}
};

