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
	Game(sf::RenderWindow& window);
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
	void addGameObject(GameObject* gameObjectPtr, GameState gameState, int layer = 1);
	void zoom(int direction);
	
	// Input handling
	int oldMouseX = -1, oldMouseY = -1;
	int zoomLevel = 0;

	// Views
	static const int NUM_LAYERS = 3;
	sf::View layerView[NUM_LAYERS];

	// Resource managers
	TextureManager textureManager;

	// Game objects
	std::map<GameObject*, std::set<GameState>> gameObjects;
	std::map<int, std::vector<GameObject*>> layerMap;
	std::map<int, std::vector<sf::Drawable*>> drawableMap;
	Robot* robot;
	MazeDisplay* mazeDisplay;
	sf::RectangleShape indicator;
	sf::Text textDisplay;

	// Temporary, add to a font manager later
	sf::Font HUDFont;
};

#endif
