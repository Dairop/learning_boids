#include "boids/commons.hpp"
#include "moteur_ia/moteur.hpp"


//	create one big vertexArray with every body-parts of fishes and draw 
//	them all at once to minimize the number of draw calls
void drawAllCircles(sf::RenderWindow& window, std::vector<Fish>* boidsV, sf::Texture& object_texture) {
	int nbOfCircles = boidsV->size() * boidsV->at(0).bodyLen << 2;

	int nbOfCirclesDrawn = 0;

	sf::VertexArray va;
	va.setPrimitiveType(sf::Quads);

	va.resize(nbOfCircles);

	for (int i = 0; i < boidsV->size(); i++) {
		boidsV->at(i).draw(va, nbOfCirclesDrawn);
		nbOfCirclesDrawn += boidsV->at(i).bodyLen<<2;
	}


	sf::RenderStates states;
	states.texture = &object_texture;
	window.draw(va, states);

}




int main() {
	bool pause = true;

	std::srand(std::time(nullptr));

	sf::Image tp;
	int windowWidth = 1920;
	int windowHeight = 1080;

	int numberOfBoids = 1000;

	sf::Texture backgroundRocksTexture;
	backgroundRocksTexture.loadFromFile("img/rocks.png");
	sf::Sprite backgroundRocks(backgroundRocksTexture);
	backgroundRocks.setPosition(sf::Vector2f(0, 0));
	
	sf::Texture waterTexture;
	waterTexture.loadFromFile("img/water.png");
	sf::Sprite water(waterTexture);
	water.setPosition(sf::Vector2f(0, 0));

	sf::Texture circleText;
	circleText.loadFromFile("img/circle.png");


	std::vector<Fish> *boidsV = new std::vector<Fish>;
	for (int i = 0; i < numberOfBoids; i++) {
		Fish mt(sf::Vector2f((i*23)%windowWidth, ((i + 31) * 63)%windowHeight)); //new one each time, so they get a new orientation & pos
		boidsV->push_back(mt);
	}

	sf::Clock deltaClock;
	sf::Time dt = deltaClock.restart();

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Name");
	const sf::Vector2i viewCenter = sf::Vector2i(-9, -38);
	window.setPosition(viewCenter);
	window.clear(sf::Color(0, 0, 0));
	
	//fill screen with a transparent black layer
	
	sf::RectangleShape black;
	black.setSize(sf::Vector2f(windowWidth, windowHeight));
	black.setFillColor(sf::Color(0, 0, 0, 10));
	black.setPosition(sf::Vector2f(0, 0));
	

	sf::Event event;
	rectByCenter screenRect;

	screenRect.center = sf::Vector2f(windowWidth / 2, windowHeight / 2);
	screenRect.radius = sf::Vector2f(windowWidth / 2, windowHeight / 2);


	std::vector<sf::Vector2f> food;

	Moteur moteur(*(boidsV), food);
	moteur.init(sf::Vector2u(windowWidth, windowHeight));

	unsigned long long int nbOfFramesPassed = 0;
	while (window.isOpen()) {
		//define quadtree
		QuadTree *bigQuad = new QuadTree(screenRect);

		//events
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
			pause = false;// !pause;
		}


		//update window
		//window.clear(sf::Color(15, 80, 60));
		//window.draw(backgroundRocks);
		if (nbOfFramesPassed%5==0)window.draw(black);


		//fill the quadtree
		for (int b = 0; b < boidsV->size(); b++) {
			bigQuad->insert(&boidsV->at(b));
		}

		//update position of boids if the simulation isn't paused
		sf::Vector2u windowSize = sf::Vector2u(windowWidth, windowHeight);
		for (int b = 0; b < numberOfBoids; b++) {
			if (!pause) {
				//update boids position
				boidsV->at(b).update(windowSize, *bigQuad, dt);

				//
			}
		}

		//display the boids all at once
		drawAllCircles(window, boidsV, circleText);

		//draw the layer on top of the boids
		//window.draw(water);
		
		//FPS
		std::cout << 1000/(dt.asMilliseconds()+1) << "\n";
		dt = deltaClock.restart();
	


		//update display
		window.display();
		//delete the used quadtree (we will create a new one next frame)
		bigQuad->del();
		
		//delete bigQuad;
		nbOfFramesPassed++;
	}
	
	return 0;
	
};