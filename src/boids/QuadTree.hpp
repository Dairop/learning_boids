#pragma once

#include "Commons.hpp"

#include "Entity.hpp"
struct Entity;

class QuadTree
{
public:
	// Arbitrary constant indicating how many elements can be stored in this quadtree node
	const int QT_NODE_CAPACITY = 20;

	// Axis-aligned bounding box (represented by its half-dimension and center)
	// representing the boundaries of this quadtree node
	rectByCenter boundary;

	// Points contained in this quadtree node
	std::vector<Entity*> points;



	// Constructor initializing the boundary of the quadtree node
	QuadTree(const rectByCenter bd) { boundary = bd; northWest = nullptr; northEast = nullptr; southEast = nullptr; southWest = nullptr; };

	// Inserts an entity into the quadtree
	bool insert(Entity* p);

	// Subdivides the quadrant into four equal-sized quadrants
	void subdivide();

	// Retrieves entities within a rectangular range and appends them to 'pointsInRange'
	void queryRangeRect(const rectByCenter range, std::vector<Entity*>& pointsInRange) const;

	// Retrieves entities within a circular range and appends them to 'pointsInRange'
	void queryRangeCircle(const rectByCenter range, std::vector<Entity*>& pointsInRange) const;

	// Displays the quadtree nodes on a SFML window
	void display(sf::RenderWindow&);

	// Retrieves all particles in the quadtree
	void getAllParticles(std::vector<Entity*>& particles) const;

	// Deletes the quadtree node recursively from the current position to the leaf nodes
	void del();

	// Children of the quadtree node
	QuadTree* northWest;
	QuadTree* northEast;
	QuadTree* southWest;
	QuadTree* southEast;
};

