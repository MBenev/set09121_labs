#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

const Keyboard::Key controls[4] = {
	Keyboard::A,   // Player1 UP
	Keyboard::Z,   // Player1 Down
	Keyboard::Up,  // Player2 UP
	Keyboard::Down // Player2 Down
};
const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;

bool ai = true;
Font font;
Text text;
int scoreLeft = 0;
int scoreRight = 0;


CircleShape ball;
RectangleShape paddles[2];
Vector2f ballVelocity;
bool server = false;

void Load() {
	// Set size and origin of paddles
	for (auto& p : paddles) {
		p.setSize(paddleSize - Vector2f(3, 3));
		p.setOrigin(paddleSize / 2.f);
	}
	// Set size and origin of ball
	ball.setRadius(ballRadius - 3);
	ball.setOrigin(ballRadius / 2, ballRadius / 2);

	// Load font-face from res dir
	font.loadFromFile("res/fonts/BigShouldersDisplay-Regular.ttf");
	// Set text element to use font
	text.setFont(font);
	// set the character size to 24 pixels
	text.setCharacterSize(40);
}

void Reset() {
	// reset paddle position
	paddles[0].setPosition(10 + paddleSize.x / 2, gameHeight / 2);
	paddles[1].setPosition((gameWidth - 10) - paddleSize.x / 2, gameHeight / 2);
	// reset Ball Position
	ball.setPosition(gameWidth / 2, gameHeight / 2);
	ballVelocity = { (server ? 100.0f : -100.0f), 60.0f };

	// Update Score Text
	text.setString(to_string(scoreLeft) + "		" + to_string(scoreRight));
	// Keep Score Text Centered
	text.setPosition((gameWidth * .5f) - (text.getLocalBounds().width * .5f), 0);
}

void AI(RectangleShape& paddle, float dt)
{
	float direction = 0.0f;

	if (ball.getPosition().y > paddle.getPosition().y)
	{
		direction++;
	}
	else if (ball.getPosition().y < paddle.getPosition().y)
	{
		direction--;
	}

	paddle.move(0, direction * paddleSpeed * dt);
}

void Update(RenderWindow& window) {
	// Reset clock, recalculate deltatime
	static Clock clock;
	float dt = clock.restart().asSeconds();
	// check and consume events
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

	// handle paddle movement
	float directionLeft = 0.0f;
	float directionRight = 0.0f;
	if (Keyboard::isKeyPressed(controls[0])) {
		directionLeft--;
	}
	if (Keyboard::isKeyPressed(controls[1])) {
		directionLeft++;
	}
	if (Keyboard::isKeyPressed(controls[2])) {
		directionRight--;
	}
	if (Keyboard::isKeyPressed(controls[3])) {
		directionRight++;
	}
	if (Keyboard::isKeyPressed(Keyboard::LShift))
	{
		if (ai)
		{
			ai = false;
		}
		else
			ai = true;
	}

	if (!ai)
	{
		paddles[0].move(0, directionLeft * paddleSpeed * dt);
		paddles[1].move(0, directionRight * paddleSpeed * dt);
	}
	else if (ai)
	{
		paddles[0].move(0, directionLeft * paddleSpeed * dt);
		AI(paddles[1],dt);
	}

	ball.move(ballVelocity * dt);
	// check ball collision
	const float bx = ball.getPosition().x;
	const float by = ball.getPosition().y;
	if (by > gameHeight) { //bottom wall
	  // bottom wall
		ballVelocity.x *= 1.1f;
		ballVelocity.y *= -1.1f;
		ball.move(0, -10);
	}
	else if (by < 0) { //top wall
   // top wall
		ballVelocity.x *= 1.1f;
		ballVelocity.y *= -1.1f;
		ball.move(0, 10);
	}
	else if (bx > gameWidth) {
		// right wall
		server = false;
		scoreLeft++;
		Reset();
	}
	else if (bx < 0) {
		// left wall
		server = true;
		scoreRight++;
		Reset();
	}
	else if (
		//ball is inline or behind paddle
		bx < paddles[0].getPosition().x + paddleSize.x * 0.5 &&
		//AND ball is below top edge of paddle
		by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
		//AND ball is above bottom edge of paddle
		by < paddles[0].getPosition().y + (paddleSize.y * 0.5)
		)
	{
		// Left paddle
		ballVelocity.x *= -1.1f;
		ballVelocity.y *= 1.1f;
		ball.move(10, 0);
	}
	else if (
		//ball is inline or behind paddle
		bx > paddles[1].getPosition().x - paddleSize.x * 0.5 &&
		//AND ball is below top edge of paddle
		by > paddles[1].getPosition().y - (paddleSize.y * 0.5) &&
		//AND ball is above bottom edge of paddle
		by < paddles[1].getPosition().y + (paddleSize.y * 0.5)
		)
	{
		// Right paddle
		ballVelocity.x *= -1.1f;
		ballVelocity.y *= 1.1f;
		ball.move(-10, 0);
	}
	for (auto& p : paddles)
	{
		if (p.getPosition().y - paddleSize.y * 0.5 < 0)
		{
			p.setPosition(p.getPosition().x, 0 + paddleSize.y * 0.5);
		}
		if (p.getPosition().y + paddleSize.y * 0.5 > gameHeight)
		{
			p.setPosition(p.getPosition().x, gameHeight - paddleSize.y * 0.5);
		}
	}
	
}

void Render(RenderWindow& window) {
	// Draw Everything
	window.draw(paddles[0]);
	window.draw(paddles[1]);
	window.draw(ball);
	window.draw(text);
}

int main() {
	RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
	Load();
	Reset();
	while (window.isOpen()) {
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}
	return 0;
}