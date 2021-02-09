#include <SFML/Graphics.hpp>
#include <iostream>
#include "ship.h"

using namespace sf;
using namespace std;

//Main.cpp
sf::Texture spritesheet;
sf::Sprite invader;
const int gameWidth = 800;
const int gameHeight = 600;

vector<Ship*> ships;

void Load() {
    if (!spritesheet.loadFromFile("res/img/invaders_sheet.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }
    invader.setTexture(spritesheet);
    invader.setTextureRect(sf::IntRect(0, 0, 32, 32));
}

void Render(RenderWindow& window) {
    window.draw(invader);
}

int main() {
	RenderWindow window(VideoMode(gameWidth, gameHeight), "Space Invaders");
	Load();
	while (window.isOpen()) {
		window.clear();
		Render(window);
		window.display();
	}
	return 0;
}