#ifndef ROBOT_H
#define ROBOT_H
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
class Robot : public GameSprite
{
	AnimatedSprite sprite;
	sf::Texture* texture;
	enum SpriteState
	{
		IDLE1, IDLE2, LEFT, RIGHT, DOWN,UP,STOP
	} spriteState;
	sf::Time spriteTime;
	float speed = 5;
public:
	Robot(sf::Texture* texture)
	{
		spriteState = STOP;
		this->texture = texture;
		sprite.setTexture(*texture);
		sprite.setAnimationLength(16);
		sprite.setPosition(0,200);
	}
	void handleInput(std::string command)
	{
		if (command=="LEFT") spriteState = LEFT;
		if (command=="RIGHT") spriteState = RIGHT;
		if (command=="UP") spriteState = UP;
		if (command=="DOWN") spriteState = DOWN;
		if (command=="IDLE") spriteState = STOP;
	}
	void update(sf::Time deltaTime)
	{
		// Sprite position
		sf::Vector2f offset;
		switch (spriteState)
		{
			case UP:
				offset.y = -speed;
				break;
			case DOWN:
				offset.y = speed;
				break;
			case LEFT:
				offset.x = -speed;
				break;
			case RIGHT:
				offset.x = speed;
				break;
		}
		sprite.setPosition(sprite.getPosition()+offset);

		// Sprite graphics
		spriteTime += deltaTime;
		if(spriteState != STOP)
		{
			if (spriteState != IDLE1 && spriteState != IDLE2) sprite.setAnimationLength(16);
			sprite.setAnimationNumber(spriteState);
		}
		else
		{
			if(sprite.getFrame()>4 && sprite.getFrame()<=12) spriteState = IDLE2;
			else 
			{
				spriteState = IDLE1;
			}
			sprite.setAnimationLength(4);
			sprite.setAnimationNumber(spriteState);
		}
		sf::Time timePerFrame;
		if (spriteState == IDLE1 || spriteState == IDLE2)
		{
			timePerFrame = sf::milliseconds(120);
		}
		else
		{
			timePerFrame = sf::milliseconds(30);
		}
		while(spriteTime >= timePerFrame)
		{
			sprite.advanceFrame(1);
			spriteTime -= timePerFrame;
		}
	}
	sf::Sprite getSprite()
	{
		return sprite;
	}
};

#endif