#ifndef ROBOT_H
#define ROBOT_H
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
class Robot : public GameSprite
{
	AnimatedSprite sprite;
	enum SpriteState
	{
		IDLE1, IDLE2, LEFT, RIGHT, DOWN,UP,STOP
	} spriteState;
	sf::Time spriteTime;
	float speed = 3;
public:
	Robot(sf::Texture* texture)
	{
		spriteState = STOP;
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
	void update(sf::Time deltaTime) override
	{
		// Sprite position
		sf::Vector2f offset;
		switch (spriteState)
		{
			case UP:	offset.y = -speed; break;
			case DOWN:	offset.y = speed; break;
			case LEFT:	offset.x = -speed; break;
			case RIGHT:	offset.x = speed; break;
		}
		sprite.setPosition(sprite.getPosition()+offset);

		// Update to correct idle states.
		spriteTime += deltaTime;
		if(spriteState == STOP)
		{
			if(sprite.getFrame()>4 && sprite.getFrame()<=12) spriteState = IDLE2;
			else spriteState = IDLE1;
			sprite.setFrame(0);
		}

		// Set correct animation graphics
		if (spriteState == IDLE1 || spriteState == IDLE2) 
		{
			sprite.setAnimationLength(4);
		}
		else
		{
			sprite.setAnimationLength(16);
		}
		sprite.setAnimationNumber(spriteState);

		// advance animation based on deltaTime
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
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(sprite, states);
	}
};

#endif