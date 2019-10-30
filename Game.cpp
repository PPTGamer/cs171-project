#include "Game.h"

Game::Game(sf::RenderWindow& window): gameState(SET_POSITION)
{
	sf::Clock loadingTime;
	sf::Time startTime = loadingTime.restart();
	for (int i = 0; i < NUM_LAYERS; i++)
	{
		this->layerView[i] = window.getDefaultView();
	} 
	// do all loading here
	// Temporary, add to a font manager later
	if(!HUDFont.loadFromFile("fonts/Inconsolata-Regular.ttf"))
	{
		std::cout<<"Unable to load font!"<<std::endl;
	}
	
	Maze maze(4, 4);
	maze.generate();
	mazeDisplay = new MazeDisplay(HUDFont, &textureManager, maze);
	addGameObject(mazeDisplay, GameState::SET_POSITION);
	addGameObject(mazeDisplay, GameState::SET_ALGORITHM);
	addGameObject(mazeDisplay, GameState::RUNNING);
	addGameObject(mazeDisplay, GameState::PAUSED);
	mazeDisplay->setPosition(sf::Vector2f(
		400-mazeDisplay->getSize().x/2.0, 
		300-mazeDisplay->getSize().y/2.0
	));
	
	textureManager.loadTexture("robotsprite.png");
	robot = new Robot(textureManager.getTexture("robotsprite.png"));
	addGameObject(robot, GameState::SET_ALGORITHM);
	addGameObject(robot, GameState::RUNNING);
	addGameObject(robot, GameState::PAUSED);

	button1 = new Button(this);
	button1->setPosition(sf::Vector2f(200, 50));
	button1->setText(&HUDFont, "Breadth-First Search");
	button1->setOnClick([](Game* game){game->setAlgorithm(AlgorithmType::BFS);});
	addGameObject(button1, GameState::SET_ALGORITHM, 0);
		
	button2 = new Button(this);
	button2->setPosition(sf::Vector2f(500, 50));
	button2->setText(&HUDFont, "Uniform-Cost Search");
	button2->setOnClick([](Game* game){game->setAlgorithm(AlgorithmType::UCS);});
	addGameObject(button2, GameState::SET_ALGORITHM, 0);

	this->enterState(GameState::SET_POSITION);

	std::cout<<"loading time:"<<loadingTime.restart().asMilliseconds()<<"ms"<<std::endl;
}

Game::~Game()
{
	for (auto&& gameObjectPtr : gameObjects)
	{
		delete gameObjectPtr.first;
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
			for (int i = 0 ; i < NUM_LAYERS; i++)
			{
				layerView[i].setViewport(sf::FloatRect((w-usedWidth)/(2*w),0.0f,usedWidth/w,1.0f));
			}
		}
		if (event.type == sf::Event::MouseMoved)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				if (oldMouseX < 0 || oldMouseY < 0)
				{
					oldMouseX = event.mouseMove.x;
					oldMouseY = event.mouseMove.y;
				}

				layerView[1].move(oldMouseX-event.mouseMove.x, oldMouseY-event.mouseMove.y);
				layerView[1].setCenter(
					std::max(layerView[1].getCenter().x, 400.0f-mazeDisplay->getSize().x/2),
					std::max(layerView[1].getCenter().y, 300.0f-mazeDisplay->getSize().y/2)
				);
				layerView[1].setCenter(
					std::min(layerView[1].getCenter().x, 400.0f+mazeDisplay->getSize().x/2),
					std::min(layerView[1].getCenter().y, 300.0f+mazeDisplay->getSize().y/2)
				);

				oldMouseX = event.mouseMove.x;
				oldMouseY = event.mouseMove.y;
			}
		}
		if (event.type == sf::Event::MouseButtonReleased)
		{
			oldMouseX = -1;
			oldMouseY = -1;
		}
		for (int currentLayer = NUM_LAYERS - 1; currentLayer >= 0; currentLayer--)
		{
			window.setView(layerView[currentLayer]);
			for(auto const &gameObjectPtr : layerMap[currentLayer])
			{
				// if gameObject is part of the current state, handle input for it
				if (gameObjects[gameObjectPtr].find(gameState) != gameObjects[gameObjectPtr].end())
				{
					gameObjectPtr->handleInput(event, window, gameState);
				}
			}
		}
		window.setView(layerView[1]);
		if (gameState == SET_POSITION)
		{
			if (event.type == sf::Event::MouseMoved)
			{
				sf::Vector2i pixelCoordinates = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
				sf::RectangleShape* rectPtr = mazeDisplay->getTileAtPixel(window.mapPixelToCoords(pixelCoordinates));
				if (rectPtr != NULL)
				{
					indicator.setOutlineColor(sf::Color::Red);
					indicator.setPosition(rectPtr->getPosition());
				}
				else
				{
					indicator.setOutlineColor(sf::Color::Transparent);
				}
			}
			if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
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
		indicator.setOutlineThickness(5);
		indicator.setOutlineColor(sf::Color::Transparent);
	}
	else if (gameState == RUNNING)
	{
		textDisplay.setFont(HUDFont);
		textDisplay.setCharacterSize(28);
		textDisplay.setFillColor(sf::Color::White);
		textDisplay.setOutlineColor(sf::Color::Black);
		textDisplay.setOutlineThickness(2);
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
	}
}

void Game::addGameObject(GameObject* gameObjectPtr, GameState gameState, int layer)
{
	this->layerMap[layer].push_back(gameObjectPtr);
	this->gameObjects[gameObjectPtr].insert(gameState);
}

void Game::exitState(GameState gameState) {}

/**
	Updates GameObjects, given that there has been deltaTime since the last update.
**/
void Game::update(sf::Time deltaTime)
{
	for(auto&& gameObject : gameObjects)
	{
		// if gameObject is part of the current state, update it
		if (gameObject.second.find(gameState) != gameObject.second.end())
		{
			gameObject.first->update(deltaTime);
		}
	}
}

/**
  Draw the current game state to the window.
**/
void Game::draw(sf::RenderTarget& target)
{
	for (int currentLayer = NUM_LAYERS - 1; currentLayer >= 0; currentLayer--)
	{
		target.setView(layerView[currentLayer]);
		for(auto const &gameObjectPtr : layerMap[currentLayer])
		{
			if (gameObjects[gameObjectPtr].find(gameState) != gameObjects[gameObjectPtr].end())
			{
				if (GameSprite* gameSprite = dynamic_cast<GameSprite*>(gameObjectPtr))
				{
					gameSprite->draw(target, sf::RenderStates::Default);
				}
			}
		}
	}
	target.setView(layerView[1]);
	if (gameState == SET_POSITION)
	{
		target.draw(indicator);
	}
	target.setView(layerView[0]);
	target.draw(textDisplay);
}

void Game::setAlgorithm(AlgorithmType algorithmType)
{
	this->algorithmType = algorithmType;
	changeState(GameState::RUNNING);
}