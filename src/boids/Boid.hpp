#pragma once
#include "commons.hpp"
#include "../moteur_ia/ReseauNeurones.hpp"

class QuadTree;

class Boid {
public:
	sf::Vector2f position;
	sf::Vector2f vel;
	sf::Vector2f acceleration;
	std::vector<sf::Color> colors;
	std::vector<sf::Vector2f> body;
	float size = 7;
	unsigned int bodyLen = 8;
	float fishLenght = size * 5;
	float nodeLen = fishLenght / bodyLen;
	float speed;
	float turningTo;


	Boid(sf::Vector2f pos) {
		position = pos;

		vel = sf::Vector2f(rand() % 5 - 2, rand() % 5 - 2);
		sf::Vector2f((rand() % 100 - 50), (rand() % 100 - 50));
		acceleration = sf::Vector2f(rand() % 10 - 5, rand() % 10 - 5);
		//sf::Color(rand() % 255, rand() % 50 + 50, rand() % 255);
		speed = size * 4;

		body.clear();
		int r = rand() % 70 + 185;
		int g = 1 + rand() % 50;
		colors.push_back(sf::Color(r, g, rand() % g));
		if (rand() % 7 == 0) {
			colors[0].g = 255 - rand() % 25 - 15; 
			colors[0].b = colors[0].g - rand() % 35;
		}

		for (int i = 1; i < bodyLen + 1; i++) {
			body.push_back(sf::Vector2f(position.x - i*nodeLen, position.y));
			colors.push_back(colors[0]);
			if (rand() % 5 == 0) {
				colors[i].g = 255 - rand() % 25 - 15;
				colors[i].b = colors[i].g - rand() % 35;
			}
			
		}
	}

	virtual void draw(sf::VertexArray& va, int start);
	void updateBody();
	void update(sf::Vector2u screenSize, QuadTree& quad, sf::Time& dt);

	sf::Vector2f cohesion(QuadTree& quad);
	sf::Vector2f alignment(QuadTree& quad);
	sf::Vector2f separation(QuadTree& quad);

};

