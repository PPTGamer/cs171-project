#ifndef GAME_SPRITE_H
#define GAME_SPRITE_H
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "../enums.h"

class GameSprite : public GameObject, public sf::Drawable
{
public:
	GameSprite(){};
	virtual void handleInput(sf::Event event, sf::RenderWindow& window, GameState gameState) {};
	virtual void update(sf::Time deltaTime) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};
#endif
