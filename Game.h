#ifndef GAME_H
#define GAME_H
// Standard libraries
#include <cmath>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <set>
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
	int oldMouseX = -1, oldMouseY = -1;
	sf::Vector2f viewPosition;
private:
	GameState gameState;
	AlgorithmType algorithmType;
	void enterState(GameState gameState);
	void exitState(GameState gameState);
	void addGameObject(GameObject* gameObjectPtr, GameState gameState);
	void addHUDObject(sf::Drawable* drawablePtr);
	// Resource managers
	TextureManager textureManager;

	// Game objects
	std::map<GameObject*, std::set<GameState>> gameObjects;
	std::vector<sf::Drawable*> HUDObjects;
	Robot* robot;
	MazeDisplay* mazeDisplay;
	Button *button1, *button2;
	sf::RectangleShape indicator;
	sf::Text textDisplay;

	// Temporary, add to a font manager later
	sf::Font HUDFont;
};

#endif
