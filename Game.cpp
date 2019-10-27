#include "Game.h"

Game::Game(): gameState(SETUP)
{
	sf::Clock loadingTime;
	sf::Time startTime = loadingTime.restart();

	// do all loading here
	// Temporary, add to a font manager later
	if(!HUDFont.loadFromFile("fonts/Inconsolata-Regular.ttf"))
	{
		std::cout<<"Unable to load font!"<<std::endl;
	}

	textureManager.loadTexture("crate.png");
	mazeDisplay = new MazeDisplay(HUDFont, textureManager.getTexture("crate.png"));
	gameObjects.push_back(mazeDisplay);
	mazeDisplay->setPosition(sf::Vector2f(
		400-mazeDisplay->getSize().x/2.0, 
		300-mazeDisplay->getSize().y/2.0
	));
	
	textureManager.loadTexture("robotsprite.png");
	robot = new Robot(textureManager.getTexture("robotsprite.png"));
	gameObjects.push_back(robot);

	this->enterState(GameState::SETUP);

	std::cout<<"loading time:"<<loadingTime.restart().asMilliseconds()<<"ms"<<std::endl;
}

/**
	Obtains input events from a RenderWindow.
**/
void Game::handleInput(sf::RenderWindow& window)
{
	sf::Event event;
	while(window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
		if (event.type == sf::Event::Resized) // letterboxing
		{
			float w = event.size.width;
			float h = event.size.height;
			float usedWidth = 4.0f/3.0f*h;
			sf::View currView = window.getView();
			currView.setViewport(sf::FloatRect((w-usedWidth)/(2*w),0.0f,usedWidth/w,1.0f));
			window.setView(currView);
		}

		if (gameState == SETUP)
		{
			if (event.type == sf::Event::MouseMoved)
			{
				sf::Vector2i pixelCoordinates = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
				sf::RectangleShape* rectPtr = mazeDisplay->getTileAtPixel(window.mapPixelToCoords(pixelCoordinates));
				if (rectPtr != NULL)
				{
					indicator.setPosition(rectPtr->getPosition());
				}
				else
				{
					indicator.setPosition(-999,-999);
				}
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				sf::Vector2i pixelCoordinates = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
				sf::RectangleShape* rectPtr = mazeDisplay->getTileAtPixel(window.mapPixelToCoords(pixelCoordinates));
				if (rectPtr != NULL)
				{
					sf::Vector2f robotPosition = rectPtr->getPosition();
					robotPosition.x += rectPtr->getSize().x/2;
					robotPosition.y += rectPtr->getSize().y/2;
					robot->setPosition(robotPosition);
					this->changeState(GameState::RUNNING);
				}
				else
				{
					// do nothing
				}
			}
		}
		else if (gameState == RUNNING)
		{
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				this->changeState(GameState::PAUSED);
			}
			else if(event.type == sf::Event::MouseButtonPressed)
			{
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
				{
					sf::Vector2i pixelCoordinates = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
					sf::Vector2i tileCoordinates = mazeDisplay->getTileIndexAtPixel(window.mapPixelToCoords(pixelCoordinates));
					mazeDisplay->setMark(tileCoordinates.x, tileCoordinates.y, sf::Color::Yellow, "test");
				}
				else if(sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
				{
					sf::Vector2i pixelCoordinates = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
					sf::Vector2i tileCoordinates = mazeDisplay->getTileIndexAtPixel(window.mapPixelToCoords(pixelCoordinates));
					mazeDisplay->clearMark(tileCoordinates.x, tileCoordinates.y);
				}
				else if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i pixelCoordinates = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
					robot->moveTo(window.mapPixelToCoords(pixelCoordinates));
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					sf::Vector2i pixelCoordinates = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
					pixelCoordinates.x = lrint(pixelCoordinates.x/64.0)*64;
					pixelCoordinates.y = lrint(pixelCoordinates.y/64.0)*64;
					robot->moveTo(window.mapPixelToCoords(pixelCoordinates));
				}
			}
			else if(event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Left)
				{
					robot->handleInput("LEFT");
				}
				else if (event.key.code == sf::Keyboard::Right)
				{
					robot->handleInput("RIGHT");
				}
				else if (event.key.code == sf::Keyboard::Up)
				{
					robot->handleInput("UP");
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					robot->handleInput("DOWN");
				}
			}
			else if(event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down)
				{
					robot->handleInput("IDLE");
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					robot->handleInput("LEFT");
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					robot->handleInput("RIGHT");
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					robot->handleInput("UP");
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					robot->handleInput("DOWN");
				}
			}
		}
		else if (gameState == PAUSED)
		{
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				this->changeState(GameState::RUNNING);
			}
		}
	}
}

void Game::changeState(Game::GameState newGameState)
{
	this->exitState(this->gameState);
	this->enterState(newGameState);
	this->gameState = newGameState;
}

void Game::enterState(Game::GameState gameState)
{
	if (gameState == SETUP)
	{
		indicator.setSize(sf::Vector2f(64, 64));
		indicator.setFillColor(sf::Color::Transparent);
		indicator.setOutlineColor(sf::Color::Red);
		indicator.setOutlineThickness(5);
		indicator.setPosition(-999,-999);
	}
	else if (gameState == RUNNING)
	{

	}
	else if (gameState == PAUSED)
	{
		pauseIndicator.setFont(HUDFont);
		pauseIndicator.setCharacterSize(28);
		pauseIndicator.setFillColor(sf::Color::White);
		pauseIndicator.setOutlineColor(sf::Color::Black);
		pauseIndicator.setOutlineThickness(2);
		pauseIndicator.setString("SIMULATION PAUSED");
		pauseIndicator.setPosition(0, 0);
	}
}

void Game::exitState(Game::GameState gameState)
{
	if (gameState == SETUP)
	{
		indicator.setPosition(-999, -999);
	}
	else if (gameState == RUNNING)
	{

	}
	else if (gameState == PAUSED)
	{
		pauseIndicator.setPosition(-999, -999);
	}
}



/**
	Updates GameObjects, given that there has been deltaTime since the last update.
**/
void Game::update(sf::Time deltaTime)
{
	if (gameState == SETUP)
	{

	}
	else if (gameState == RUNNING)
	{
		for(auto&& gameObject : gameObjects)
		{
			gameObject->update(deltaTime);
		}
	}
	else if (gameState == PAUSED)
	{
		
	}
	
}

/**
  Draw the current game state to the window.
**/
void Game::draw(sf::RenderTarget& target)
{
	for(auto const &gameObjectPtr : gameObjects)
	{
		if (GameSprite* gameSprite = dynamic_cast<GameSprite*>(gameObjectPtr))
		{
			gameSprite->draw(target, sf::RenderStates::Default);
		}
	}
	if (gameState == SETUP)
	{
		target.draw(indicator);
	}
	else if (gameState == RUNNING)
	{

	}
	else if (gameState == PAUSED)
	{
		target.draw(pauseIndicator);
	}
}
