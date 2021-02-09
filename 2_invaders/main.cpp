#include <SFML/Graphics.hpp>
#include <iostream>
#include "ship.h"
#include "game.h"

using namespace sf;
using namespace std;

//Main.cpp
Texture spritesheet;
Sprite invader;

vector<Ship*> ships;

void Load() {
    if (!spritesheet.loadFromFile("res/img/invaders_sheet.png")) {
        cerr << "Failed to load spritesheet!" << endl;
    }
    invader.setTexture(spritesheet);
    invader.setTextureRect(IntRect(0, 0, 32, 32));

	//rows
	for (int i = 1; i <= invaders_rows; i++)
	{
		//columns
		for (int j = 1; j <= invaders_columns; j++)
		{
			// Left, Top, Width, Height
			Invader* inv = new Invader(IntRect(i*32, 0, 32, 32), { float(j*40),float(i*40) });
			ships.push_back(inv);
		}		
	}
}

void Update(RenderWindow& window) {
	// Reset clock, recalculate deltatime
	static Clock clock;
	float dt = clock.restart().asSeconds();
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

	for (auto& s : ships)
	{
		s->Update(dt);
	}
}

void Render(RenderWindow& window) {
	for (const auto s : ships)
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