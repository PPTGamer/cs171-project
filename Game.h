#ifndef GAME_H
#define GAME_H
// Standard libraries
#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

// Resource managers
#include "TextureManager.h"

// Game objects
#include "GameObject.h"
#include "GameSprite.h"
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
	std::vector<GameObject*> gameObjects;
	Robot* robot;
};

#endif
