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
	
	MazeGenerator mg{};
	maze = mg.generate(MAZESIZE, MAZESIZE);
	mazeDisplay = new MazeDisplay(HUDFont, &textureManager, maze);
	addGameObject(mazeDisplay, GameState::SET_POSITION);
	addGameObject(mazeDisplay, GameState::SET_ALGORITHM);
	addGameObject(mazeDisplay, GameState::RUNNING);
	addGameObject(mazeDisplay, GameState::PAUSED);
	addGameObject(mazeDisplay, GameState::END);
	mazeDisplay->setPosition(sf::Vector2f(
		400-mazeDisplay->getSize().x/2.0, 
		300-mazeDisplay->getSize().y/2.0
	));
	
	textureManager.loadTexture("robotsprite.png");
	robot = new Robot(textureManager.getTexture("robotsprite.png"), mazeDisplay);
	addGameObject(robot, GameState::RUNNING);
	addGameObject(robot, GameState::PAUSED);
	addGameObject(robot, GameState::END);

	Button* button1 = new Button(this);
	button1->setPosition(sf::Vector2f(50, 100));
	button1->setText(&HUDFont, "Breadth-First Search");
	button1->setOnClick([](Game* game){game->setAlgorithm(AlgorithmType::BFS);});
	addGameObject(button1, GameState::SET_ALGORITHM, 0);
		
	Button* button2 = new Button(this);
	button2->setPosition(sf::Vector2f(50, 200));
	button2->setText(&HUDFont, "Depth-First Search");
	button2->setOnClick([](Game* game){game->setAlgorithm(AlgorithmType::DFS);});
	addGameObject(button2, GameState::SET_ALGORITHM, 0);

	Button* button3 = new Button(this);
	button3->setPosition(sf::Vector2f(50, 300));
	button3->setText(&HUDFont, "Uniform-Cost Search");
	button3->setOnClick([](Game* game){game->setAlgorithm(AlgorithmType::UCS);});
	addGameObject(button3, GameState::SET_ALGORITHM, 0);

	Button* button4 = new Button(this);
	button4->setPosition(sf::Vector2f(50, 400));
	button4->setText(&HUDFont, "Informed search (A*)");
	button4->setOnClick([](Game* game){game->setAlgorithm(AlgorithmType::ASTAR);});
	addGameObject(button4, GameState::SET_ALGORITHM, 0);

	textDisplay.setFont(HUDFont);
	textDisplay.setCharacterSize(28);
	textDisplay.setFillColor(sf::Color::White);
	textDisplay.setOutlineColor(sf::Color::Black);
	textDisplay.setOutlineThickness(2);
	
	controlDisplay.setFont(HUDFont);
	controlDisplay.setCharacterSize(14);
	controlDisplay.setFillColor(sf::Color::White);
	controlDisplay.setOutlineColor(sf::Color::Black);
	controlDisplay.setOutlineThickness(1);
	controlDisplay.setPosition(sf::Vector2f(0,520));
	controlDisplay.setString("Drag with the right mouse button: scroll view\nMouse wheel or Z/X: zoom in/out");

	indicator.setColor(sf::Color::Transparent);
	indicator.setTexture(*textureManager.getTexture("robotsprite.png"));
	indicator.setTextureRect(sf::IntRect(0,0,64,64));

	
	textureManager.loadTexture("Background.png");
	Background* background = new Background(textureManager.getTexture("Background.png"), window);
	addGameObject(background, GameState::SET_ALGORITHM, 2);
	addGameObject(background, GameState::SET_POSITION, 2);
	addGameObject(background, GameState::RUNNING, 2);
	addGameObject(background, GameState::PAUSED, 2);
	addGameObject(background, GameState::END, 2);
	
	this->enterState(GameState::SET_ALGORITHM);
	this->gameState = SET_ALGORITHM;
	std::cout<<"loading time:"<<loadingTime.restart().asMilliseconds()<<"ms"<<std::endl;
}

Game::~Game()
{
	for (auto&& gameObjectPtr : gameObjects)
	{
		delete gameObjectPtr.first;
	}
}
/*
	1 for zoom in, -1 for zoom out.
*/
void Game::zoom(int direction)
{
	int newZoomLevel = zoomLevel + direction;
	newZoomLevel = std::max(newZoomLevel, -1);
	newZoomLevel = std::min(newZoomLevel, 1);
	switch (newZoomLevel - zoomLevel)
	{
		case 2: layerView[1].zoom(0.25); break;
		case 1: layerView[1].zoom(0.50); break;
		case -1: layerView[1].zoom(2); break;
		case -2: layerView[1].zoom(4); break;
	}
	zoomLevel = newZoomLevel;
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
		if (event.type == sf::Event::MouseWheelScrolled) // mouse wheel zooming
		{
			zoom((int)event.mouseWheelScroll.delta);
		}
		if (event.type == sf::Event::KeyPressed) // keyboard zooming
		{
			if (event.key.code == sf::Keyboard::Z) zoom(-1);
			if (event.key.code == sf::Keyboard::X) zoom(1);
		}
		if (event.type == sf::Event::MouseMoved) // drag to scroll layer 1
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
					Maze::EntryType mazeEntry = mazeDisplay->getMazeEntryAtPixel(window.mapPixelToCoords(pixelCoordinates));
					if (mazeEntry == Maze::EntryType::EMPTY || mazeEntry == Maze::EntryType::START)
					{
						indicator.setColor(sf::Color(255,255,255,100));
					}
					else
					{
						indicator.setColor(sf::Color(255,0,0,100));
					}
					indicator.setPosition(rectPtr->getPosition());
				}
				else
				{
					indicator.setColor(sf::Color::Transparent);
				}
			}
			if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2i pixelCoordinates = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
				sf::RectangleShape* rectPtr = mazeDisplay->getTileAtPixel(window.mapPixelToCoords(pixelCoordinates));
				Maze::EntryType mazeEntry = mazeDisplay->getMazeEntryAtPixel(window.mapPixelToCoords(pixelCoordinates));
				if (rectPtr != NULL && (mazeEntry == Maze::EntryType::EMPTY || mazeEntry == Maze::EntryType::START))
				{
					setPosition(mazeDisplay->getTileIndexAtPixel(window.mapPixelToCoords(pixelCoordinates)));
				}
			}
		}
		else if (gameState == SET_ALGORITHM)
		{
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
			{
				MazeGenerator mg{};
				maze = mg.generate(MAZESIZE, MAZESIZE);
				mazeDisplay->setMaze(maze);
			}
		}
		else if (gameState == RUNNING)
		{
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				this->changeState(GameState::PAUSED);
			}
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				robot->clearMovementQueue();
				mazeDisplay->clearAllMarks();
				delete algorithm;
				algorithm = NULL;
				this->changeState(GameState::SET_ALGORITHM);
			}
		}
		else if (gameState == PAUSED)
		{
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				this->changeState(GameState::RUNNING);
			}
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				robot->clearMovementQueue();
				mazeDisplay->clearAllMarks();
				delete algorithm;
				algorithm = NULL;
				this->changeState(GameState::SET_ALGORITHM);
			}
		}
		else if (gameState == END)
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				robot->clearMovementQueue();
				mazeDisplay->clearAllMarks();
				this->changeState(GameState::SET_ALGORITHM);
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
	if (gameState == SET_ALGORITHM)
	{
		textDisplay.setString("Choose an algorithm.");
		controlDisplay.setString("Drag with the right mouse button: scroll view\nMouse wheel or Z/X: zoom in/out\nR - regenerate maze");
		mazeDisplay->setMaze(maze); // reset the maze;
	}
	if (gameState == SET_POSITION)
	{
		textDisplay.setString("Click to set starting position.");
		controlDisplay.setString("Drag with the right mouse button: scroll view\nMouse wheel or Z/X: zoom in/out");
		indicator.setColor(sf::Color::Transparent);
	}
	else if (gameState == RUNNING)
	{
		std::stringstream ss;
		ss<<"Running "<<algorithm->getName()<<".\n"<<algorithm->getNumStatesExpanded()<<" states expanded.";
		textDisplay.setString(ss.str());
		controlDisplay.setString("Drag with the right mouse button: scroll view\nMouse wheel or Z/X: zoom in/out\nSpace: Pause\nEsc: Abort");
	}
	else if (gameState == PAUSED)
	{
		std::stringstream ss;
		ss<<"Paused simulation of "<<algorithm->getName()<<".\n"<<algorithm->getNumStatesExpanded()<<" states expanded.";
		textDisplay.setString(ss.str());
		controlDisplay.setString("Drag with the right mouse button: scroll view\nMouse wheel or Z/X: zoom in/out\nSpace: Unpause\nEsc: Abort");
	}
	else if (gameState == END)
	{
		std::stringstream ss;
		ss<<"Done running "<<algorithm->getName()<<".\n"<<algorithm->getNumStatesExpanded()<<" states expanded.\nTotal cost:"<<algorithm->getSolutionCost()<<"\nPress SPACE to restart.";
		textDisplay.setString(ss.str());
		controlDisplay.setString("Drag with the right mouse button: scroll view\nMouse wheel or Z/X: zoom in/out");
		delete algorithm;
		algorithm = NULL;
	}
}

void Game::addGameObject(GameObject* gameObjectPtr, GameState gameState, int layer)
{
	this->layerMap[layer].push_back(gameObjectPtr);
	this->gameObjects[gameObjectPtr].insert(gameState);
}

void Game::exitState(GameState gameState) 
{
	if (gameState == SET_POSITION)
	{
		algorithm->start();
		algorithmTime = sf::Time::Zero;
	}
}

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

	if (gameState == GameState::RUNNING)
	{
		algorithmTime += deltaTime;
		sf::Time timeStep = sf::milliseconds(50);
		while (algorithmTime >= timeStep)
		{
			if (algorithm->finished())
			{
				mazeDisplay->clearAllMarks();
				robot->executeSolution(algorithm->getSolution(), mazeDisplay);
				std::cout<<"Total states expanded: "<<algorithm->getNumStatesExpanded()<<std::endl;
				changeState(GameState::END);
				break;
			}
			else
			{
				SearchState expanded = algorithm->next();
				mazeDisplay->clearAllMarks();
				for (auto state : algorithm->getFringe())
				{
					sf::Color markColor = sf::Color(100,100,105);
					markColor.b += state.keys.size()*50;
					std::string keysString;
					std::stringstream ss;
					for (auto key : state.keys)
					{
						ss<<"("<<key.first<<","<<key.second<<")"<<std::endl;
					}
					keysString = ss.str();
					mazeDisplay->setMark(state.location.x, state.location.y, markColor, keysString);
				}
				mazeDisplay->setMark(expanded.location.x, expanded.location.y, sf::Color::Yellow);
			}
			algorithmTime -= timeStep;
		}

		// text display
		if (algorithm)
		{
			std::stringstream ss;
			ss<<"Running "<<algorithm->getName()<<".\n"<<algorithm->getNumStatesExpanded()<<" states expanded.";
			textDisplay.setString(ss.str());
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
	target.draw(controlDisplay);
}

void Game::setAlgorithm(AlgorithmType algorithmType)
{
	this->algorithmType = algorithmType;
	if (algorithmType == AlgorithmType::BFS)
	{
		algorithm = new BFSAlgo();
	}
	else if (algorithmType == AlgorithmType::DFS)
	{
		algorithm = new DFSAlgo();
	}
	else if (algorithmType == AlgorithmType::UCS)
	{
		algorithm = new UCSAlgo();
	}
	else if (algorithmType == AlgorithmType::ASTAR)
	{
		algorithm = new ManhattanHeuristic();
	}
	changeState(GameState::SET_POSITION);
}

void Game::setPosition(sf::Vector2i tileIndex)
{
	sf::RectangleShape* rectPtr = mazeDisplay->getTileAtIndex(tileIndex);
	sf::Vector2f robotPosition = rectPtr->getPosition();
	robotPosition.x += rectPtr->getSize().x/2;
	robotPosition.y += rectPtr->getSize().y/2;
	robot->setPosition(robotPosition);
	mazeDisplay->setStartingPosition(tileIndex);
	algorithm->setMaze(mazeDisplay->getMaze());
	this->changeState(GameState::RUNNING);
}