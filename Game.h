#ifndef GAME_H
#define GAME_H
// Standard libraries
#include <iostream>
#include <vector>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "enums.h"

// Resource managers
#include "managers/TextureManager.h"

// Game objects
#include "gameobj/GameObject.h"
#include "gameobj/GameSprite.h"
#include "gameobj/AllObjects.h"

class Game
{
public:
	Game();
	~Game();
	void changeState(GameState newGameState);
	void handleInput(sf::RenderWindow& window);
	void update(sf::Time deltaTime);
	void draw(sf::RenderTarget& target);
	void setAlgorithm(AlgorithmType algorithmType);
private:
	GameState gameState;
	AlgorithmType algorithmType;
	void enterState(GameState gameState);
	void exitState(GameState gameState);

	// Resource managers
	TextureManager textureManager;

	// Game objects
	std::vector<GameObject*> gameObjects;
	Robot* robot;
	MazeDisplay* mazeDisplay;
	Button *button1, *button2;
	sf::RectangleShape indicator;
	sf::Text textDisplay;

	// Temporary, add to a font manager later
	sf::Font HUDFont;
};

#endif
