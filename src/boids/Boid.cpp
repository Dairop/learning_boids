#pragma once
#ifndef iosteam
#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>

#include "commons.hpp"
#endif



void Boid::draw(sf::VertexArray& va, int start) {
	const float texture_size = 1024.0f;
	unsigned int idx;
	int rad;

	idx = start;
	rad = this->size;

	va[idx + 0].position.x = position.x - rad;
	va[idx + 0].position.y = position.y - rad;

	va[idx + 1].position.x = position.x + rad;
	va[idx + 1].position.y = position.y - rad;

	va[idx + 2].position.x = position.x + rad;
	va[idx + 2].position.y = position.y + rad;

	va[idx + 3].position.x = position.x - rad;
	va[idx + 3].position.y = position.y + rad;

	va[idx + 0].texCoords = { 0.0f        , 0.0f };
	va[idx + 1].texCoords = { texture_size, 0.0f };
	va[idx + 2].texCoords = { texture_size, texture_size };
	va[idx + 3].texCoords = { 0.0f        , texture_size };

	const sf::Color color = sf::Color::Red;
	va[idx + 0].color = color;
	va[idx + 1].color = color;
	va[idx + 2].color = color;
	va[idx + 3].color = color;
}

sf::Vector2f Boid::alignment(QuadTree& quad) {
	std::vector<Entity*> nearbyPoints;
	rectByCenter range;
	range.center = position;
	range.radius = sf::Vector2f(this->size * 7, this->size * 7);
	quad.queryRangeCircle(range, nearbyPoints);

	sf::Vector2f steering = sf::Vector2f(0, 0);

	if (nearbyPoints.size() > 0) {
		for (int p = 0; p < nearbyPoints.size(); p++) {
			Boid* boidPtr = static_cast<Boid*>(nearbyPoints[p]); // convert the Entity back to a point, 
			//should return an error if something else than a Boid's Entity is in the QuadTree

			steering.x += boidPtr ->vel.x;
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


sf::Vector2f Boid::cohesion(QuadTree& quad) {
	std::vector<Entity*> nearbyPoints;
	rectByCenter range;
	range.center = position;
	range.radius = sf::Vector2f(this->size * 6, this->size * 6);
	quad.queryRangeCircle(range, nearbyPoints);

	sf::Vector2f avgPoint;
	sf::Vector2f steering = sf::Vector2f(0, 0);
	avgPoint.x = 0;
	avgPoint.y = 0;
	if (nearbyPoints.size() > 0) {
		for (int p = 0; p < nearbyPoints.size(); p++) {
			avgPoint.x += nearbyPoints[p]->position.x;
			avgPoint.y += nearbyPoints[p]->position.y;
		}
		avgPoint.x /= nearbyPoints.size();
		avgPoint.y /= nearbyPoints.size();

		//normalize
		steering = avgPoint;

		steering.x -= position.x;
		steering.y -= position.y;

		steering = normalize(steering);

		steering.x *= speed;
		steering.y *= speed;

		steering.x -= vel.x;
		steering.y -= vel.y;

	}
	return steering;

}

sf::Vector2f Boid::separation(QuadTree& quad) {
	std::vector<Entity*> nearbyPoints;
	rectByCenter range;
	range.center = position;
	range.radius = sf::Vector2f(this->size * 2.0, this->size * 2.0);
	quad.queryRangeCircle(range, nearbyPoints);

	sf::Vector2f diff;
	sf::Vector2f steering = sf::Vector2f(0, 0);

	if (nearbyPoints.size() > 0) {
		for (int p = 0; p < nearbyPoints.size(); p++) {
			diff.x = position.x - nearbyPoints[p]->position.x;
			diff.y = position.y - nearbyPoints[p]->position.y;

			//Squared dist
			float d = (nearbyPoints[p]->position.x - position.x) * (nearbyPoints[p]->position.x - position.x) +
				(nearbyPoints[p]->position.y - position.y) * (nearbyPoints[p]->position.y - position.y);
			if (d < 1) {
				diff.x *= 1 / (d + 0.0001);
				diff.y *= 1 / (d + 0.0001);
			}

			steering.x += diff.x;
			steering.y += diff.y;
		}
		steering.x /= nearbyPoints.size();
		steering.y /= nearbyPoints.size();

		steering = normalize(steering);
		steering.x *= speed;
		steering.y *= speed;

		steering.x -= vel.x;
		steering.y -= vel.y;
	}
	return steering;

}



void Boid::update(sf::Vector2u screenSize, QuadTree& boidsQuad, sf::Time& dt){
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
	//acceleration = normalize(acceleration);

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

	//random acceleration to make 
	int long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	//std::cout << ms%360 << " ";
	acceleration.x += cos(ms/100 % 360) * speed * 0.1;
	acceleration.y += sin(ms/100 % 360) * speed * 0.1;


	vel.x += acceleration.x / 150;
	vel.y += acceleration.y / 150;
	vel = normalize(vel);

	position.x += vel.x * speed * dt.asMilliseconds() / 1000;
	position.y += vel.y * speed * dt.asMilliseconds() / 1000;
}
