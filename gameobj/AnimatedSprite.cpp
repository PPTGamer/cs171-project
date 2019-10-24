#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(int width, int height)
{
	animationFrame = 0;
	animationNumber = 0;
	this->width = width;
	this->height = height;
	refreshFrame();
}

void AnimatedSprite::refreshFrame()
{
	setTextureRect(sf::IntRect(width*animationFrame,height*animationNumber,width,height));
}

void AnimatedSprite::setAnimationLength(int frames)
{
	maxFrame = frames;
}
void AnimatedSprite::setAnimationNumber(int number)
{
	if (animationNumber == number) return;
	animationNumber = number;
	animationFrame = 0;
	refreshFrame();
}

void AnimatedSprite::setSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void AnimatedSprite::setFrame(int frameNumber)
{
	animationFrame = frameNumber % maxFrame;
	refreshFrame();
}
void AnimatedSprite::advanceFrame(int offset)
{
	animationFrame = (animationFrame + offset + maxFrame)%maxFrame;
	refreshFrame();
}
int AnimatedSprite::getFrame()
{
	return animationFrame;
}