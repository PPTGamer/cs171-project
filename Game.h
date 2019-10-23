#ifndef GAME_H
#define GAME_H
// Standard libraries
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

// Resource managers
#include "TextureManager.h"

// Game objects
#include "Robot.h"
class Game
{
public:
	Game();
	void handleInput(sf::RenderWindow& window);
	void update(sf::Time deltaTime);
	void draw(sf::RenderTarget& target);
private:
	enum GameState
	{
		DEFAULT
	} gameState;

	// Resource managers
	TextureManager textureManager;

	// Game objects
	std::shared_ptr<Robot> robot;
};

#endif
