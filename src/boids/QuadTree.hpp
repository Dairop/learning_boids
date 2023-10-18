#pragma once

#include "commons.hpp"

#include "Entity.hpp"
struct Entity;

class QuadTree
{
public:
	// Constante arbitraire indiquant combien d'éléments peuvent être stockés dans ce nœud de quadtree
	const int QT_NODE_CAPACITY = 20;

	// Zone de délimitation alignée sur l'axe (représentée par sa demi-dimension et son centre)
	// représentant les limites de ce quadtree
	rectByCenter boundary;

	// Points de ce nœud de quadtree
	std::vector <Entity*> points;


	// Méthodes
	QuadTree(rectByCenter bd) { boundary = bd; northWest = nullptr; northEast = nullptr; southEast = nullptr; southWest = nullptr; };
	bool insert(Entity* p);
	void subdivide(); // créer quatre enfants permettant de diviser ce quadrant en quatre quadrants d'égales dimensions
	void queryRangeRect(rectByCenter range, std::vector<Entity*> & pointsInRange);
	void queryRangeCircle(rectByCenter range, std::vector<Entity*>& pointsInRange);
	void display(sf::RenderWindow&);
	void getAllParticles(std::vector<Entity*>& particles);
	void del();  // delete the QuadTree from the actual pos. to the leafs

	// Enfants
	QuadTree* northWest;
	QuadTree* northEast;
	QuadTree* southWest;
	QuadTree* southEast;
};

