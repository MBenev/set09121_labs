#include <SFML/Graphics.hpp>
#include <iostream>
#include "ship.h"

using namespace sf;
using namespace std;

//Main.cpp
Texture spritesheet;
Sprite invader;
const int gameWidth = 800;
const int gameHeight = 600;

vector<Ship*> ships;

void Load() {
    if (!spritesheet.loadFromFile("res/img/invaders_sheet.png")) {
        cerr << "Failed to load spritesheet!" << endl;
    }
    invader.setTexture(spritesheet);
    invader.setTextureRect(IntRect(0, 0, 32, 32));

	Invader* inv = new Invader(IntRect(0, 0, 32, 32), { 100,100 });
	ships.push_back(inv);
}

void Update(RenderWindow& window) {
	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) {
			window.close();
			return;
		}
	}

	// Quit Via ESC Key
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}
}

void Render(RenderWindow& window) {
	for (auto& s : ships)
	{
		window.draw(*s);
	}
}

int main() {
	RenderWindow window(VideoMode(gameWidth, gameHeight), "Space Invaders");
	Load();
	while (window.isOpen()) {
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}
	return 0;
}