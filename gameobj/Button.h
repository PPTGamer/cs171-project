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
	enum BUTTON_STATE {NORMAL, HOVER, CLICK};
	void setState(BUTTON_STATE buttonState)
	{
		if (buttonState == NORMAL)
		{
			sprite.setFillColor(sf::Color(150,150,150));
		}
		else if (buttonState == HOVER)
		{
			sprite.setFillColor(sf::Color(150,150,200));
		}
		else if (buttonState == CLICK)
		{
			sprite.setFillColor(sf::Color(200,150,150));
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
		sprite.setSize(sf::Vector2f(100,50));
		setState(NORMAL);
	}

	void setPosition(float x, float y) {sprite.setPosition(x, y); updateLabelPosition();}
	void setPosition(sf::Vector2f position) {sprite.setPosition(position); updateLabelPosition();}
	void setText(sf::Font* font, std::string labelText)
	{
		label.setFont(*font);
		label.setString(labelText);
		label.setCharacterSize(18);
		label.setFillColor(sf::Color::Black);
		updateLabelPosition();
	}
	// void assignTextures(sf::Texture* normalTexture, sf::Texture* clickedTexture = NULL, sf::Texture* hoverTexture = NULL)
	// {
	// 	this->normalTexture = normalTexture;
	// 	this->clickedTexture = clickedTexture;
	// 	this->hoverTexture = hoverTexture;
	// }
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
					if (onClick != nullptr)
					{
						onClick(game);
					}
				}
			}
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			if (sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
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
			// set neutral graphics
			this->setState(NORMAL);
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