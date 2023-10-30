#pragma once
#include "Commons.hpp"
#include "../AI_Engine/NeuralNetwork.hpp"
#include "Entity.hpp"

class QuadTree;

class Boid : public Entity {
public:
	sf::Vector2f vel; //velocity vector, used as the speed direction. This vector is normalized
	sf::Vector2f acceleration; //acceleration vector
	float size = 7; //radius of the Boid
	float speed;

	//constructor
	Boid(sf::Vector2f pos) : Entity(pos) {
		vel = sf::Vector2f(rand() % 5 - 2, rand() % 5 - 2);
		acceleration = sf::Vector2f(rand() % 10 - 5, rand() % 10 - 5);
		speed = size * 15;
	}

	//set a circle at the Boid' position in va, which will be drawn later
	virtual void draw(sf::VertexArray& va, const int start);

	//update the position of the Boid with the date passed in parameters (dt is in microseconds)
	void update(const sf::Vector2u screenSize, QuadTree& boidsQuad, const long dt);

	//the three rules of boids: cohesion, alignment and separation. They make the movement feel natural, like birds or fishes.
	sf::Vector2f cohesion(const QuadTree& quad);
	sf::Vector2f alignment(const QuadTree& quad);
	sf::Vector2f separation(const QuadTree& quad);

};

