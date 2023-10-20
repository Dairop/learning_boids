#pragma once
#ifndef iosteam
#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Commons.hpp"
#endif



void Boid::draw(sf::VertexArray& va, const int start) {
	const float texture_size = 1024.0f; //size of the circle.png texture in the img folder
	unsigned int idx; //start position in the VertexArray
	int rad; //radius of the Boid

	idx = start;
	rad = this->size;

	//define the rectangle in which the texture will be displayed
	va[idx + 0].position.x = position.x - rad;
	va[idx + 0].position.y = position.y - rad;

	va[idx + 1].position.x = position.x + rad;
	va[idx + 1].position.y = position.y - rad;

	va[idx + 2].position.x = position.x + rad;
	va[idx + 2].position.y = position.y + rad;

	va[idx + 3].position.x = position.x - rad;
	va[idx + 3].position.y = position.y + rad;
	
	//define the position of the texture's corners
	va[idx + 0].texCoords = { 0.0f        , 0.0f };
	va[idx + 1].texCoords = { texture_size, 0.0f };
	va[idx + 2].texCoords = { texture_size, texture_size };
	va[idx + 3].texCoords = { 0.0f        , texture_size };

	//define the color of each texture's corner
	const sf::Color color = sf::Color::Red; //color of all the Boids (can be changed if we want different colors for each Boid)
	va[idx + 0].color = color;
	va[idx + 1].color = color;
	va[idx + 2].color = color;
	va[idx + 3].color = color;
}


sf::Vector2f Boid::alignment(const QuadTree& quad) {
	std::vector<Entity*> nearbyPoints;

	//define the range of the query to the QuadTree to find the neighbors of the Boid
	rectByCenter range;
	range.center = position;
	range.radius = sf::Vector2f(this->size * 7, this->size * 7);
	quad.queryRangeCircle(range, nearbyPoints);

	sf::Vector2f steering = sf::Vector2f(0, 0);

	//if there are any neighbor
	if (nearbyPoints.size() > 0) {
		for (int p = 0; p < nearbyPoints.size(); p++) {
			Boid* boidPtr = static_cast<Boid*>(nearbyPoints[p]); // convert the Entity back to a Boid, 
			//should return an error if something else than a Boid's Entity is in the QuadTree

			steering.x += boidPtr->vel.x;
			steering.y += boidPtr->vel.y;
		}
		steering.x /= nearbyPoints.size();
		steering.y /= nearbyPoints.size();

		//normalize
		steering = normalize(steering);

		steering.x *= speed;
		steering.y *= speed;

		steering.x -= vel.x;
		steering.y -= vel.y;

	}
	return steering;

}


sf::Vector2f Boid::cohesion(const QuadTree& quad) {
	std::vector<Entity*> nearbyPoints;

	//define the range of the query to the QuadTree to find the neighbors of the Boid
	rectByCenter range;
	range.center = position;
	range.radius = sf::Vector2f(this->size * 6, this->size * 6);
	quad.queryRangeCircle(range, nearbyPoints);

	//center of gravity of the neighbors. The Boid will want to turn toward it
	sf::Vector2f avgPoint = sf::Vector2f(0, 0);
	sf::Vector2f steering = sf::Vector2f(0, 0);

	if (nearbyPoints.size() > 0) {
		//sum the position of the neighbors
		for (int p = 0; p < nearbyPoints.size(); p++) {
			avgPoint.x += nearbyPoints[p]->position.x;
			avgPoint.y += nearbyPoints[p]->position.y;
		}

		//make the sum into an average of the neighbors' position
		avgPoint.x /= nearbyPoints.size();
		avgPoint.y /= nearbyPoints.size();

		steering = avgPoint;

		steering.x -= position.x;
		steering.y -= position.y;

		//normalize
		steering = normalize(steering);

		steering.x *= speed;
		steering.y *= speed;

		
		steering.x -= vel.x;
		steering.y -= vel.y;

	}
	return steering;

}

sf::Vector2f Boid::separation(const QuadTree& quad) {
	std::vector<Entity*> nearbyPoints;

	//define the range of the query to the QuadTree to find the neighbors of the Boid
	rectByCenter range;
	range.center = position;
	range.radius = sf::Vector2f(this->size * 2.0, this->size * 2.0);
	quad.queryRangeCircle(range, nearbyPoints);

	//distance between the Boid and his neighbor
	sf::Vector2f dist;
	//steering force, will be an average of where the Boid is pushed by his neighbors at the end
	sf::Vector2f steering = sf::Vector2f(0, 0);

	if (nearbyPoints.size() > 0) {
		for (int p = 0; p < nearbyPoints.size(); p++) {
			dist.x = position.x - nearbyPoints[p]->position.x;
			dist.y = position.y - nearbyPoints[p]->position.y;

			//Squared dist
			float d = dist.x * dist.x + dist.y * dist.y;
			//if very close, then make the repulsive force even stronger
			if (d < 1) {
				dist.x *= 1 / (d + 0.0001);
				dist.y *= 1 / (d + 0.0001);
			}

			steering.x += dist.x;
			steering.y += dist.y;
		}
		
		steering.x /= nearbyPoints.size();
		steering.y /= nearbyPoints.size();

		//normalize
		steering = normalize(steering);

		steering.x *= speed;
		steering.y *= speed;

		steering.x -= vel.x;
		steering.y -= vel.y;
	}
	return steering;

}



void Boid::update(const sf::Vector2u screenSize, QuadTree& boidsQuad, const long dt_){
	long dt = dt_ / 1000;

	//init
	acceleration.x = 0;
	acceleration.y = 0;

	//bounce on walls
	if (position.x < 0) {
		acceleration.x = speed*1.5;
	}
	else if (position.x > screenSize.x) {
		acceleration.x = -speed*1.5;
	}
	if (position.y < 0) {
		acceleration.y = speed*1.5;
	}
	else if (position.y > screenSize.y) {
		acceleration.y = -speed*1.5;
	}


	//acceleration for every rule
	sf::Vector2f a;

	a = alignment(boidsQuad);
	acceleration.x += a.x * 0.5;
	acceleration.y += a.y * 0.5;

	a = cohesion(boidsQuad);
	acceleration.x += a.x * 0.05;
	acceleration.y += a.y * 0.05;

	a = separation(boidsQuad);
	acceleration.x += a.x * 3;
	acceleration.y += a.y * 3;

	//random acceleration, not needed but is a must
	acceleration.x += cos(std::fmod((float) dt + this->acceleration.x * 127.0f, 360.0f)) * speed * 0.1f;
	acceleration.y += sin(std::fmod((float) dt - this->acceleration.y * 571.0f, 360.0f)) * speed * 0.1f;
	/*
	int long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	//std::cout << ms%360 << " ";
	acceleration.x += cos(ms/100 % 360) * speed * 0.1;
	acceleration.y += sin(ms/100 % 360) * speed * 0.1;
	*/

	//add acceleration to the current velocity
	vel.x += acceleration.x / 150;
	vel.y += acceleration.y / 150;
	//normalize the velocity
	vel = normalize(vel);

	//add velocity (speed orientation) * speed * delta time to the position
	position.x += vel.x * speed * dt / 1000 * SIMULATION_SPEED;
	position.y += vel.y * speed * dt / 1000 * SIMULATION_SPEED;
}
