#ifndef BUTTON_H
#define BUTTON_H
#include <functional>
#include <SFML/Graphics.hpp>

#include "GameSprite.h"
#include "../Game.h"

class Game;

class Button : public GameSprite
{
private:
	std::function<void(Game*)> onClick;
	sf::RectangleShape sprite;
	sf::Text label;
	Game* game;
	bool engaged;
	enum BUTTON_STATE {NORMAL, HOVER, CLICK};
	void setState(BUTTON_STATE buttonState)
	{
		if (buttonState == NORMAL)
		{
			sprite.setOutlineColor(sf::Color(255,255,255));
			label.setFillColor(sf::Color(255,255,255));
			label.setStyle(sf::Text::Bold);
		}
		else if (buttonState == HOVER)
		{
			sprite.setOutlineColor(sf::Color(0,150,255));
			label.setFillColor(sf::Color(0,150,255));
			label.setStyle(sf::Text::Bold | sf::Text::Underlined);
		}
		else if (buttonState == CLICK)
		{
			sprite.setOutlineColor(sf::Color(255,255,150));
			label.setFillColor(sf::Color(255,255,150));
			label.setStyle(sf::Text::Bold | sf::Text::Underlined);
		}
	}
	void updateLabelPosition()
	{
		sf::FloatRect labelBounds = label.getLocalBounds();
		label.setOrigin(labelBounds.left + labelBounds.width/2, labelBounds.top + labelBounds.height/2);
		sf::FloatRect buttonBounds = sprite.getGlobalBounds();
		label.setPosition(buttonBounds.left + buttonBounds.width/2, buttonBounds.top + buttonBounds.height/2);
	}
public:
	Button(Game* game, std::string labelText = "") 
	{
		this->game = game;
		sprite.setSize(sf::Vector2f(250,50));
		sprite.setFillColor(sf::Color::Black);
		sprite.setOutlineThickness(4);
		engaged = false;
		setState(NORMAL);
	}

	void setPosition(float x, float y) {sprite.setPosition(x, y); updateLabelPosition();}
	void setPosition(sf::Vector2f position) {sprite.setPosition(position); updateLabelPosition();}
	void setText(sf::Font* font, std::string labelText)
	{
		label.setFont(*font);
		label.setString(labelText);
		label.setCharacterSize(20);
		label.setStyle(sf::Text::Bold);
		updateLabelPosition();
	}
	void setOnClick(std::function<void(Game*)> function)
	{
		onClick = function;
	}
	void handleInput(sf::Event event, sf::RenderWindow& window, GameState gameState) override
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i pixelCoordinates = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
				sf::Vector2f worldCoordinates = window.mapPixelToCoords(pixelCoordinates);
				if (sprite.getGlobalBounds().contains(worldCoordinates))
				{
					// set clicked graphics
					this->setState(CLICK);
					engaged = true;
				}
			}
			else engaged = false;
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			engaged = false;
			sf::Vector2i pixelCoordinates = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
			sf::Vector2f worldCoordinates = window.mapPixelToCoords(pixelCoordinates);
			if (sprite.getGlobalBounds().contains(worldCoordinates))
			{
				if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					// set rollover graphics
					this->setState(HOVER);
				}
			}
			else
			{
				// set neutral graphics
				this->setState(NORMAL);
			}	
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (engaged)
			{
				if (onClick != nullptr)
				{
					onClick(game);
				}
			}
			// set neutral graphics
			this->setState(NORMAL);
			engaged = false;
		}
	}
	void update(sf::Time deltaTime) {}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(sprite, states);
		target.draw(label, states);
	}
};

#endif