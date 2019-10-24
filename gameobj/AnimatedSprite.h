#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H
#include <SFML/Graphics.hpp>
class AnimatedSprite : public sf::Sprite
{
	int animationFrame;
	int animationNumber;
	int maxFrame;
	int width;
	int height;
	void refreshFrame();
public:
	AnimatedSprite(int width=64, int height=64);
	void setAnimationLength(int frames);
	void setAnimationNumber(int number);
	void setSize(int width, int height);
	void setFrame(int frameNumber);
	void advanceFrame(int offset);
	int getFrame();
};

#endif