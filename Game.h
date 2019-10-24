#ifndef GAME_H
#define GAME_H
// Standard libraries
#include <iostream>
#include <vector>
#include <memory>
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
	std::vector<GameObject*> gameObjects;
	Robot* robot;
};

#endif
