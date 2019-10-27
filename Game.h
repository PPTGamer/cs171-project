#ifndef GAME_H
#define GAME_H
// Standard libraries
#include <iostream>
#include <vector>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

// Resource managers
#include "managers/TextureManager.h"

// Game objects
#include "gameobj/GameObject.h"
#include "gameobj/GameSprite.h"
#include "gameobj/AllObjects.h"

class Game
{
public:
	enum GameState
	{
		SETUP, RUNNING, PAUSED
	};
	Game();
	void changeState(Game::GameState newGameState);
	void handleInput(sf::RenderWindow& window);
	void update(sf::Time deltaTime);
	void draw(sf::RenderTarget& target);
private:
	GameState gameState;
	void enterState(Game::GameState gameState);
	void exitState(Game::GameState gameState);

	// Resource managers
	TextureManager textureManager;

	// Game objects
	std::vector<GameObject*> gameObjects;
	Robot* robot;
	MazeDisplay* mazeDisplay;
	sf::RectangleShape indicator;
	sf::Text pauseIndicator;

	// Temporary, add to a font manager later
	sf::Font HUDFont;
};

#endif
