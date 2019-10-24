#ifndef GAME_SPRITE_H
#define GAME_SPRITE_H
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class GameSprite : public GameObject
{
public:
	GameSprite(){};
	virtual void update(sf::Time deltaTime){};
	virtual sf::Sprite getSprite(){return sf::Sprite();};
};
#endif