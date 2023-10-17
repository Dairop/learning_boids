#include "commons.hpp"


bool pointInFrame(sf::Vector2f v, sf::IntRect rect, float radius) {
	if (v.x - radius < rect.left || v.x + radius > rect.left + rect.width) return false;
	return v.y - radius >= rect.top && v.y + radius <= rect.top + rect.height;
}