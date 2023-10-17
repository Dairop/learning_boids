#pragma once

#include "commons.hpp"


class Boid;

class QuadTree
{
public:
	// Constante arbitraire indiquant combien d'�l�ments peuvent �tre stock�s dans ce n�ud de quadtree
	const int QT_NODE_CAPACITY = 20;

	// Zone de d�limitation align�e sur l'axe (repr�sent�e par sa demi-dimension et son centre)
	// repr�sentant les limites de ce quadtree
	rectByCenter boundary;

	// Points de ce n�ud de quadtree
	std::vector <Boid*> points;


	// M�thodes
	QuadTree(rectByCenter bd) { boundary = bd; northWest = nullptr; northEast = nullptr; southEast = nullptr; southWest = nullptr; };
	bool insert(Boid* p);
	void subdivide(); // cr�er quatre enfants permettant de diviser ce quadrant en quatre quadrants d'�gales dimensions
	void queryRangeRect(rectByCenter range, std::vector<Boid*> & pointsInRange);
	void queryRangeCircle(rectByCenter range, std::vector<Boid*>& pointsInRange);
	void display(sf::RenderWindow&);
	void getAllParticles(std::vector<Boid*>& particles);
	void del();  // delete the QuadTree from the actual pos. to the leafs

	// Enfants
	QuadTree* northWest;
	QuadTree* northEast;
	QuadTree* southWest;
	QuadTree* southEast;
};
