#include "Game.h"

Game::Game(): gameState(SET_POSITION)
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
	robot->setPosition(-999,999);
	gameObjects.push_back(robot);

	button1 = new Button(this);
	button1->setText(&HUDFont, "Breadth-First Search");
	button1->setPosition(-999,-999);
	button1->setOnClick([](Game* game){game->setAlgorithm(AlgorithmType::BFS);});
	gameObjects.push_back(button1);

	button2 = new Button(this);
	button2->setText(&HUDFont, "Uniform-Cost Search");
	button2->setPosition(-999,-999);
	button2->setOnClick([](Game* game){game->setAlgorithm(AlgorithmType::UCS);});
	gameObjects.push_back(button2);

	this->enterState(GameState::SET_POSITION);

	std::cout<<"loading time:"<<loadingTime.restart().asMilliseconds()<<"ms"<<std::endl;
}

Game::~Game()
{
	for (auto&& gameObjectPtr : gameObjects)
	{
		delete gameObjectPtr;
	}
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

		for (auto&& gameObject : gameObjects)
		{
			gameObject->handleInput(event, window, gameState);
		}

		if (gameState == SET_POSITION)
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
					this->changeState(GameState::SET_ALGORITHM);
				}
				else
				{
					// do nothing
				}
			}
		}
		else if (gameState == SET_ALGORITHM)
		{
			// no input events
		}
		else if (gameState == RUNNING)
		{
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				this->changeState(GameState::PAUSED);
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
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

void Game::changeState(GameState newGameState)
{
	this->exitState(this->gameState);
	this->enterState(newGameState);
	this->gameState = newGameState;
}

void Game::enterState(GameState gameState)
{
	if (gameState == SET_POSITION)
	{
		indicator.setSize(sf::Vector2f(64, 64));
		indicator.setFillColor(sf::Color::Transparent);
		indicator.setOutlineColor(sf::Color::Red);
		indicator.setOutlineThickness(5);
		indicator.setPosition(-999,-999);
	}
	else if (gameState == SET_ALGORITHM)
	{
		button1->setPosition(200, 50);
		button2->setPosition(500, 50);
	}
	else if (gameState == RUNNING)
	{
		textDisplay.setFont(HUDFont);
		textDisplay.setCharacterSize(28);
		textDisplay.setFillColor(sf::Color::White);
		textDisplay.setOutlineColor(sf::Color::Black);
		textDisplay.setOutlineThickness(2);
		textDisplay.setPosition(0, 0);
		if (this->algorithmType == AlgorithmType::BFS)
		{
			textDisplay.setString("RUNNING BFS");
		}
		if (this->algorithmType == AlgorithmType::UCS)
		{
			textDisplay.setString("RUNNING UCS");
		}
	}
	else if (gameState == PAUSED)
	{
		textDisplay.setFont(HUDFont);
		textDisplay.setCharacterSize(28);
		textDisplay.setFillColor(sf::Color::White);
		textDisplay.setOutlineColor(sf::Color::Black);
		textDisplay.setOutlineThickness(2);
		textDisplay.setString("SIMULATION PAUSED");
		textDisplay.setPosition(0, 0);
	}
}

void Game::exitState(GameState gameState)
{
	if (gameState == SET_POSITION)
	{
		indicator.setPosition(-999, -999);
	}
	else if (gameState == SET_ALGORITHM)
	{
		button1->setPosition(-999, -999);
		button2->setPosition(-999, -999);
	}
	else if (gameState == RUNNING)
	{
		textDisplay.setPosition(-999, -999);
	}
	else if (gameState == PAUSED)
	{
		textDisplay.setPosition(-999, -999);
	}
}

/**
	Updates GameObjects, given that there has been deltaTime since the last update.
**/
void Game::update(sf::Time deltaTime)
{
	if (gameState == SET_POSITION)
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
	if (gameState == SET_POSITION)
	{
		target.draw(indicator);
	}
	else if (gameState == RUNNING)
	{
		target.draw(textDisplay);
	}
	else if (gameState == PAUSED)
	{
		target.draw(textDisplay);
	}
}

void Game::setAlgorithm(AlgorithmType algorithmType)
{
	this->algorithmType = algorithmType;
	changeState(GameState::RUNNING);
}