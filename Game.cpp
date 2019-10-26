#include "Game.h"

Game::Game(): gameState(DEFAULT)
{
	sf::Clock loadingTime;
	sf::Time startTime = loadingTime.restart();

	// do all loading here
	textureManager.loadTexture("crate.png");
	gameObjects.push_back(new MazeDisplay(textureManager.getTexture("crate.png")));
	textureManager.loadTexture("robotsprite.png");
	robot = new Robot(textureManager.getTexture("robotsprite.png"));
	gameObjects.push_back(robot);
	
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

		if(event.type == sf::Event::KeyPressed)
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
}

/**
	Updates GameObjects, given that there has been deltaTime since the last update.
**/
void Game::update(sf::Time deltaTime)
{
	for(auto&& gameObject : gameObjects)
	{
		gameObject->update(deltaTime);
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
}
