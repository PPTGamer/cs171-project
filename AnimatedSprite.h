#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H
#include <SFML/Graphics.hpp>
class AnimatedSprite : public sf::Sprite
{
	int animationFrame;
	int animationNumber;
	int maxFrame;
	void refreshFrame()
	{
		setTextureRect(sf::IntRect(64*animationFrame,64*animationNumber,64,64));
	}
public:
	AnimatedSprite()
	{
		animationFrame = 0;
		animationNumber = 0;
	}
	void setAnimationLength(int frames)
	{
		maxFrame = frames;
	}
	void setAnimationNumber(int number)
	{
		if (animationNumber == number) return;
		animationNumber = number;
		animationFrame = 0;
		refreshFrame();
	}
	void setFrame(int frameNumber)
	{
		animationFrame = frameNumber % maxFrame;
		refreshFrame();
	}
	void advanceFrame(int offset)
	{
		animationFrame = (animationFrame + offset + maxFrame)%maxFrame;
		refreshFrame();
	}
	int getFrame()
	{
		return animationFrame;
	}
};

#endif