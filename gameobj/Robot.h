#ifndef ROBOT_H
#define ROBOT_H
#include <deque>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include "GameSprite.h"
#include "MazeDisplay.h"
#include "../ai/SearchState.h"

class Robot : public GameSprite
{
	AnimatedSprite sprite;
	enum SpriteState
	{
		IDLE1, IDLE2, LEFT, RIGHT, DOWN,UP,STOP
	} spriteState;
	sf::Time spriteTime;
	float speed = 3;
	std::deque<sf::CircleShape> indicators;
	std::deque<sf::RectangleShape> lines;
	std::deque<sf::Vector2f> movementQueue;
public:
	Robot(sf::Texture* texture);
	void setPosition(float x, float y);
	void setPosition(sf::Vector2f position);

	void handleInput(std::string command);
	
	void moveTo(sf::Vector2f destination);
	void moveTo(std::vector<sf::Vector2f> points);
	void executeSolution(std::vector<SearchState> solution, MazeDisplay* mazeDisplay);

	void handleInput(sf::Event event, sf::RenderWindow& window, GameState gameState) override;
	void update(sf::Time deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif