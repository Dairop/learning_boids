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


	maxTimerInteraction = 40.0f;
	timerInteraction = std::fmod(rand(), maxTimerInteraction);
	//std::cout << std::to_string(colors[0].r) + " " + std::to_string(colors[0].g) + " " + std::to_string(colors[0].b) << " ;";
}


void Fish::draw(sf::VertexArray& va, int start) {

	const float texture_size = 1024.0f;
	unsigned int idx;
	int rad;


	for (unsigned int i = 0; i < bodyLen; i++) {
		idx = start + (i << 2);
		rad = this->size / ((float)i / 4 + 1);

		va[idx + 0].position.x = body[i].x - rad;
		va[idx + 0].position.y = body[i].y - rad;

		va[idx + 1].position.x = body[i].x + rad;
		va[idx + 1].position.y = body[i].y - rad;

		va[idx + 2].position.x = body[i].x + rad;
		va[idx + 2].position.y = body[i].y + rad;

		va[idx + 3].position.x = body[i].x - rad;
		va[idx + 3].position.y = body[i].y + rad;

		va[idx + 0].texCoords = { 0.0f        , 0.0f };
		va[idx + 1].texCoords = { texture_size, 0.0f };
		va[idx + 2].texCoords = { texture_size, texture_size };
		va[idx + 3].texCoords = { 0.0f        , texture_size };

		va[idx + 0].color = this->color;
		va[idx + 1].color = this->color;
		va[idx + 2].color = this->color;
		va[idx + 3].color = this->color;
	}

}





void Fish::updateBody() {
	//lenght of each node in pixel
	sf::Vector2f prev; prev.x = position.x; prev.y = position.y;
	for (int i = 0; i < bodyLen; i++) {
		sf::Vector2f act = body[i];
		float actDist = dist(prev, act);
		//std::cout << ' ' << actDist;
		float dx = (act.x - prev.x) / actDist;
		float dy = (act.y - prev.y) / actDist;
		//std::cout <<"  dx:" << dx << " dy: " << dy;
		body[i] = add(prev, mult(sf::Vector2f(dx, dy), nodeLen));

		prev = body[i];
	}
}




void Fish::updateFish(sf::Vector2u screenSize, QuadTree& boidsQuad, QuadTree& foodQuad, sf::Time& dt) {
	update(screenSize, boidsQuad, dt);

	rectByCenter r;
	r.center = this->position;
	r.radius = sf::Vector2f(this->size, this->size);

	std::vector<Entity*> food;
	foodQuad.queryRangeCircle(r, food);

	this->foodReserves += food.size() * 20;
	for (unsigned int i = 0; i < food.size(); i++) {
		food[i]->isAlive = false;
	}
}