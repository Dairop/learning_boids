#pragma once
#include "commons.hpp"
#include "../moteur_ia/ReseauNeurones.hpp"
#include "Entity.hpp"

class QuadTree;

class Boid : public Entity {
public:
	sf::Vector2f vel;
	sf::Vector2f acceleration;
	float size = 7;
	float speed;
	float turningTo;


	Boid(sf::Vector2f pos) : Entity(pos) {
		vel = sf::Vector2f(rand() % 5 - 2, rand() % 5 - 2);
		sf::Vector2f((rand() % 100 - 50), (rand() % 100 - 50));
		acceleration = sf::Vector2f(rand() % 10 - 5, rand() % 10 - 5);
		//sf::Color(rand() % 255, rand() % 50 + 50, rand() % 255);
		speed = size * 4;
	}

	virtual void draw(sf::VertexArray& va, int start);
	void update(sf::Vector2u screenSize, QuadTree& boidsQuad, sf::Time& dt);

	sf::Vector2f cohesion(QuadTree& quad);
	sf::Vector2f alignment(QuadTree& quad);
	sf::Vector2f separation(QuadTree& quad);

};

