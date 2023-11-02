#include "boids/Commons.hpp"
#include "AI_Engine/Engine.hpp"


#include <filesystem>



extern bool allow_colored_print;

//	create one big vertexArray with every body-parts of fishes and draw 
//	them all at once to minimize the number of draw calls
void drawAllCircles(sf::RenderWindow& window, std::vector<Fish*>& fishesVector, std::vector<Entity*>& foodVector, sf::Texture& object_texture) {
	int nbOfCircles = (fishesVector.size() * fishesVector[0]->bodyLen << 2) + (foodVector.size() << 2);

	int nbOfCirclesDrawn = 0;

	sf::VertexArray va;
	va.setPrimitiveType(sf::Quads);

	va.resize(nbOfCircles);

	//push the verticies of the fishes
	for (int i = 0; i < fishesVector.size(); i++) {
		fishesVector[i]->draw(va, nbOfCirclesDrawn << 2);
		nbOfCirclesDrawn += fishesVector[i]->bodyLen;
	}

	
	//same for the food
	const float texture_size = 1024.0f;
	float rad = 3;
	unsigned int idx = nbOfCirclesDrawn << 2;
	sf::Color algaeColor (151, 198, 50);
	//sf::Color algaeColor(101, 138, 24);

	for (int i = 0; i < foodVector.size(); i++) {
		va[idx + 0].position.x = foodVector[i]->position.x - rad;
		va[idx + 0].position.y = foodVector[i]->position.y - rad;

		va[idx + 1].position.x = foodVector[i]->position.x + rad;
		va[idx + 1].position.y = foodVector[i]->position.y - rad;

		va[idx + 2].position.x = foodVector[i]->position.x + rad;
		va[idx + 2].position.y = foodVector[i]->position.y + rad;

		va[idx + 3].position.x = foodVector[i]->position.x - rad;
		va[idx + 3].position.y = foodVector[i]->position.y + rad;

		va[idx + 0].texCoords = { 0.0f        , 0.0f };
		va[idx + 1].texCoords = { texture_size, 0.0f };
		va[idx + 2].texCoords = { texture_size, texture_size };
		va[idx + 3].texCoords = { 0.0f        , texture_size };

		va[idx + 0].color = algaeColor;
		va[idx + 1].color = algaeColor;
		va[idx + 2].color = algaeColor;
		va[idx + 3].color = algaeColor;

		nbOfCirclesDrawn += 1;
		idx += 4;
	}

	sf::RenderStates states;
	states.texture = &object_texture;
	window.draw(va, states);

}




int main() {
	bool pause = true;
	const bool display = true;

	std::srand(std::time(nullptr));

	int windowWidth = 1920;
	int windowHeight = 1080;

	allow_colored_print = true;

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



	//vector with the position of the foods
	std::vector<Entity*> foodVector;

	//vector with the fishes positions
	std::vector<Fish*> fishesVector;

	//creation of the ai's engine
	Engine Engine(fishesVector, foodVector);
	Engine.init(sf::Vector2u(windowWidth, windowHeight));


	//creation of the timer to calculate dt beween each update and the FPS
	sf::Clock deltaClock;
	sf::Time dt = deltaClock.restart();

	//creation of the window
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Name");
	const sf::Vector2i viewCenter = sf::Vector2i(-9, -38);
	window.setPosition(viewCenter);
	window.clear(sf::Color(0, 0, 0));

	sf::Event event;
	rectByCenter screenRect;

	screenRect.center = sf::Vector2f(windowWidth / 2, windowHeight / 2);
	screenRect.radius = sf::Vector2f(windowWidth / 2, windowHeight / 2);
	
	//transparent black layer if we want to fill the screen with it later
	/*
	sf::RectangleShape black;
	black.setSize(sf::Vector2f(windowWidth, windowHeight));
	black.setFillColor(sf::Color(0, 0, 0, 10));
	black.setPosition(sf::Vector2f(0, 0));
	*/





	unsigned long long int nbOfFramesDisplayed = 0;
	bool was_space_key_pressed_last_frame = false;

	while (window.isOpen()) {
		//define quadtree
		QuadTree* boidsQuad = new QuadTree(screenRect);
		QuadTree* foodQuad = new QuadTree(screenRect);

		//events
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		//pause or unpause the simulation
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
			if (!was_space_key_pressed_last_frame) pause = !pause;
			was_space_key_pressed_last_frame = true;
		}
		else {
			was_space_key_pressed_last_frame = false;
		}


		//update window
		//window.clear(sf::Color(15, 80, 60));
		if (display) window.draw(backgroundRocks);
		//if (nbOfFramesDisplayed%5==0)window.draw(black);


		//fill the quadtree
		for (int b = 0; b < fishesVector.size(); b++) {
			boidsQuad->insert(fishesVector[b]);
		}
		//fill the quadtree
		for (int f = 0; f < foodVector.size(); f++) {
			foodQuad->insert(foodVector[f]);
		}

		//update position of boids if the simulation isn't paused
		if (!pause) {
			long dtInMicroS = std::fmin(dt.asMicroseconds(), 30000);
			//update boids position
			Engine.update((*boidsQuad), (*foodQuad), dtInMicroS);
		}

		//display the boids all at once
		if (display) drawAllCircles(window, fishesVector, foodVector, circleText);

		//draw the layer on top of the boids
		//window.draw(water);
		
		//FPS
		//std::cout << 1000/(dt.asMilliseconds()+1) << "\n";
		dt = deltaClock.restart();
	


		//update display
		window.display();
		//delete the used quadtree (we will create a new one next frame)
		boidsQuad->del();
		foodQuad->del();
		delete boidsQuad;
		delete foodQuad;
		
		nbOfFramesDisplayed++;
	}
	
	return 0;
	
};