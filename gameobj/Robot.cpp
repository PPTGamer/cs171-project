#include "Robot.h"
Robot::Robot(sf::Texture* texture)
{
	spriteState = STOP;
	sprite.setTexture(*texture);
	sprite.setAnimationLength(16);
	sprite.setOrigin(32,32);
}
void Robot::setPosition(float x, float y) {sprite.setPosition(x, y);}
void Robot::setPosition(sf::Vector2f position) {sprite.setPosition(position);}
void Robot::handleInput(std::string command)
{
	if (command=="LEFT") spriteState = LEFT;
	if (command=="RIGHT") spriteState = RIGHT;
	if (command=="UP") spriteState = UP;
	if (command=="DOWN") spriteState = DOWN;
	if (command=="IDLE") spriteState = STOP;
}

void Robot::moveTo(sf::Vector2f destination)
{
	if (!indicators.empty())
	{
		float distance = hypotf(destination.x - movementQueue.back().x, destination.y - movementQueue.back().y);
		sf::RectangleShape line(sf::Vector2f(distance, 3.0f));
		line.setFillColor(sf::Color::Red);
		line.setOrigin(0,1.5);
		line.setPosition(movementQueue.back());
		line.setRotation(
			atan2(destination.y - movementQueue.back().y, destination.x-movementQueue.back().x) 
			/ acos(-1) * 180.f
			);
		lines.push_back(line);
	}
	movementQueue.push_back(destination);
	sf::CircleShape indicator;
	indicator.setPointCount(6);
	indicator.setRadius(10);
	indicator.setOrigin(indicator.getRadius(), indicator.getRadius());
	indicator.setFillColor(sf::Color::Red);
	indicator.setPosition(destination);
	indicators.push_back(indicator);
}
void Robot::moveTo(std::vector<sf::Vector2f> points)
{
	for (auto point : points) {moveTo(point);}
}

void Robot::update(sf::Time deltaTime)
{
	// Automatically move according to movement queue
	if (!movementQueue.empty())
	{
		sf::Vector2f destination = movementQueue.front();
		sf::Vector2f current = sprite.getPosition();

		indicators.front().setFillColor(sf::Color::Yellow);

		if (fabs(current.x - destination.x)<speed)
		{
			if (fabs(current.y - destination.y)<speed) 
			{
				spriteState = STOP;
				sprite.setPosition(destination);
				movementQueue.pop_front();
				indicators.pop_front();
				if(!lines.empty()) lines.pop_front();
			}
			else if (current.y < destination.y) spriteState = DOWN;
			else if (current.y > destination.y) spriteState = UP;
		}
		else if (current.x < destination.x) spriteState = RIGHT;
		else if (current.x > destination.x) spriteState = LEFT;
	}
	
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
	if(spriteState == STOP)
	{
		if(sprite.getFrame()>4 && sprite.getFrame()<=12) spriteState = IDLE2;
		else spriteState = IDLE1;
		sprite.setFrame(0);
	}
	else
	{
		if (spriteState == IDLE1 || spriteState == IDLE2) 
		{
			sprite.setAnimationLength(4);
		}
		else
		{
			sprite.setAnimationLength(16);
		}
		sprite.setAnimationNumber(spriteState);
	}

	// advance animation based on deltaTime
	spriteTime += deltaTime;
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

	for (auto&& indicator : indicators)
	{
		indicator.rotate(2);
	}
}

void Robot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{ 
	for (auto&& line : lines)
	{
		target.draw(line, states);
	}
	for (auto&& indicator : indicators)
	{
		target.draw(indicator, states);
	}
	target.draw(sprite, states);
}