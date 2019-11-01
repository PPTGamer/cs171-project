#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <cmath>
#include <SFML/Graphics.hpp>
#include "GameSprite.h"
class Background : public GameSprite
{
private:
	sf::RectangleShape background;
	sf::Time time;
	double t = 0;
	sf::Vector2f origin;
	double getXOffset(double t)
	{
		return 200*sin(5*acos(-1)*t);
	}
	double getYOffset(double t)
	{
		return 150*sin(6*acos(-1)*t);
	}
public:
	Background(sf::Texture* texture, const sf::RenderWindow& window)
	{
		background.setTexture(texture);
		background.setSize(sf::Vector2f(texture->getSize()));
		background.setOrigin(background.getSize().x/2, background.getSize().y/2);
		origin = sf::Vector2f(window.getSize().x/2,window.getSize().y/2);
		background.setPosition(origin);
	}
	void update(sf::Time deltaTime) override
	{
		time += deltaTime;
		while (time >= sf::milliseconds(50))
		{
			t = fmod(t + 0.0003, 30*acos(-1));
			background.setPosition(origin + sf::Vector2f(getXOffset(t), getYOffset(t)));
			time -= sf::milliseconds(50);
		}
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(background, states);
	}
};
#endif
