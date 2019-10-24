#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <SFML/System.hpp>
class GameObject
{
public:
	GameObject(){};
	virtual void update(sf::Time deltaTime){};
};
#endif