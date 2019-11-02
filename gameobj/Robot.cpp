#include "Robot.h"
Robot::Robot(sf::Texture* texture, MazeDisplay* mazeDisplay)
{
	this->mazeDisplay = mazeDisplay;
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
	indicator.setRadius(7);
	indicator.setOrigin(indicator.getRadius(), indicator.getRadius());
	indicator.setFillColor(sf::Color::Red);
	indicator.setPosition(destination);
	indicators.push_back(indicator);
}

void Robot::executeSolution(std::vector<SearchState> solution, MazeDisplay* mazeDisplay)
{
	std::vector<sf::Vector2f> path; 
	for (int i = 0; i < solution.size(); i++)
	{
		sf::RectangleShape* rectPtr = mazeDisplay->getTileAtIndex(solution[i].location);
		sf::Vector2f robotPosition = rectPtr->getPosition();
		robotPosition.x += rectPtr->getSize().x/2;
		robotPosition.y += rectPtr->getSize().y/2;
		moveTo(robotPosition);
	}
}

void Robot::clearMovementQueue()
{
	indicators.clear();
	movementQueue.clear();
	lines.clear();
	spriteState = STOP;
}

void Robot::handleInput(sf::Event event, sf::RenderWindow& window, GameState gameState)
{
	if (gameState != GameState::RUNNING) return;
	/*
	if(event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2i pixelCoordinates = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
			this->moveTo(window.mapPixelToCoords(pixelCoordinates));
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			sf::Vector2i pixelCoordinates = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
			pixelCoordinates.x = lrint(pixelCoordinates.x/64.0)*64;
			pixelCoordinates.y = lrint(pixelCoordinates.y/64.0)*64;
			this->moveTo(window.mapPixelToCoords(pixelCoordinates));
		}
	}
	else if(event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Left)
		{
			this->handleInput("LEFT");
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			this->handleInput("RIGHT");
		}
		else if (event.key.code == sf::Keyboard::Up)
		{
			this->handleInput("UP");
		}
		else if (event.key.code == sf::Keyboard::Down)
		{
			this->handleInput("DOWN");
		}
	}
	else if(event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down)
		{
			this->handleInput("IDLE");
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			this->handleInput("LEFT");
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			this->handleInput("RIGHT");
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			this->handleInput("UP");
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			this->handleInput("DOWN");
		}
	}
	*/
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
				// arrived at destination, remove keys
				sf::Vector2i tileIndex = mazeDisplay->getTileIndexAtPixel(destination);
				if (mazeDisplay->getMazeEntry(tileIndex.x, tileIndex.y) == Maze::EntryType::KEY)
				{
					mazeDisplay->setMazeEntry(tileIndex.x, tileIndex.y, Maze::EntryType::EMPTY);
				}

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
	
	sf::Vector2i tileIndex = mazeDisplay->getTileIndexAtPixel(sprite.getPosition());
	if (mazeDisplay->getMazeEntry(tileIndex.x, tileIndex.y) == Maze::EntryType::ROCKY)
	{
		speed = 2;
	}
	else speed = 5;

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