#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <SFML/System.hpp>

class GameObject
{
public:
	GameObject(){};
	virtual void handleInput(sf::Event event, sf::RenderWindow& window) {};
	virtual void update(sf::Time deltaTime) = 0;
};
#endif