#ifndef GAME_H
#define GAME_H
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "Robot.h"
class Game
{
public:
	Game();
	void handleInput(sf::RenderWindow& window);
	void update(sf::Time deltaTime);
	void draw(sf::RenderTarget& target);
private:
	void resetGame();
	enum GameState
	{
		DEFAULT
	} gameState;

	// Game objects
	std::shared_ptr<Robot> robot;
};

#endif
