#include "Fish.hpp"


void Fish::init() {
	body.clear();

	body.push_back(position);
	for (int i = 1; i < bodyLen + 1; i++) {
		body.push_back(sf::Vector2f(position.x - i * nodeLen, position.y));
	}

	color = sf::Color::Cyan;

	foodReserves = 30.0f;
	age = 0;


	maxTimerInteraction = 5.0f; //can interact 19 to 20 times before dying of old age
	timerInteraction = std::fmod(rand(), maxTimerInteraction*100.0f)/100.0f;
}


void Fish::draw(sf::VertexArray& va, const int start) {
	const float texture_size = 1024.0f; //size of the circle.png texture in the img folder
	unsigned int idx; //start position in the VertexArray
	int rad; //radius of the Boid


	for (unsigned int i = 0; i < bodyLen; i++) {
		idx = start + (i << 2); // shift to not overwrite
		rad = this->size / ((float)i / 4 + 1); // smaller radius to make a tail to the fish

		//define the rectangle in which the texture will be displayed
		va[idx + 0].position.x = body[i].x - rad;
		va[idx + 0].position.y = body[i].y - rad;

		va[idx + 1].position.x = body[i].x + rad;
		va[idx + 1].position.y = body[i].y - rad;

		va[idx + 2].position.x = body[i].x + rad;
		va[idx + 2].position.y = body[i].y + rad;

		va[idx + 3].position.x = body[i].x - rad;
		va[idx + 3].position.y = body[i].y + rad;

		//define the position of the texture's corners
		va[idx + 0].texCoords = { 0.0f        , 0.0f };
		va[idx + 1].texCoords = { texture_size, 0.0f };
		va[idx + 2].texCoords = { texture_size, texture_size };
		va[idx + 3].texCoords = { 0.0f        , texture_size };

		//define the color of each texture's corner
		va[idx + 0].color = this->color;
		va[idx + 1].color = this->color;
		va[idx + 2].color = this->color;
		va[idx + 3].color = this->color;
	}

}





void Fish::updateBody() {
	// make every body part (after the head) follow it previous one
	body[0] = position;
	sf::Vector2f prev;

	for (int i = 1; i < bodyLen; i++) {
		//position of body part actually processed
		sf::Vector2f act = body[i];

		//position of the previous body part
		prev = body[i - 1];

		//distance between it and the previous one
		float actDist = dist(prev, act);

		// calculate delta x and delta y
		float dx = (act.x - prev.x) / actDist;
		float dy = (act.y - prev.y) / actDist;

		//follow
		body[i] = add(prev, mult(sf::Vector2f(dx, dy), nodeLen));

		//apply
		prev = body[i];
	}
}




void Fish::updateFish(sf::Vector2u screenSize, QuadTree& boidsQuad, QuadTree& foodQuad, long dt) {
	//update position using Boids' logic
	update(screenSize, boidsQuad, dt);

	//eat the food around
	rectByCenter r;
	r.center = this->position;
	r.radius = sf::Vector2f(this->size, this->size);

	std::vector<Entity*> food;
	foodQuad.queryRangeRect(r, food);

	this->foodReserves += food.size() * 20;

	//set the food particles to dead (they have been consummed)
	for (unsigned int i = 0; i < food.size(); i++) {
		food[i]->isAlive = false;
	}
}