#include "Fish.hpp"


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

		const sf::Color color = colors[i + 1];
		va[idx + 0].color = color;
		va[idx + 1].color = color;
		va[idx + 2].color = color;
		va[idx + 3].color = color;
	}

}