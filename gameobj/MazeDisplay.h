#ifndef MAZE_DISPLAY_H
#define MAZE_DISPLAY_H
#include <cstdlib>
#include <map>
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include "../ai/Maze.h"
#include "../managers/TextureManager.h"
class MazeDisplay : public GameSprite
{
private:
	const int TILE_SIZE = 64;
	sf::Vector2f position;
	Maze maze;
	TextureManager* textureManager;
	std::unordered_map<Maze::EntryType, sf::Texture*> textureMap;
	sf::Font displayFont;
	std::vector<std::vector<sf::RectangleShape>> tiles;
	std::map<std::pair<int,int>, AnimatedSprite> sprites;
	sf::Time spriteTime;
	std::map<std::pair<int,int>, std::pair<sf::RectangleShape, sf::Text>> indicators;
	void refreshTiles()
	{
		sprites.clear();
		sf::Vector2i mazeSize = maze.getSize();
		tiles.resize(mazeSize.x);
		for(int x = 0; x < mazeSize.x; x++)
		{
			tiles[x].resize(mazeSize.y);
			for(int y = 0; y < mazeSize.y; y++)
			{
				tiles[x][y].setSize(sf::Vector2f(TILE_SIZE,TILE_SIZE));
				tiles[x][y].setPosition(sf::Vector2f(x*TILE_SIZE,y*TILE_SIZE) + position);

				if(textureMap.find(maze(x,y)) != textureMap.end())
				{
					if (tiles[x][y].getTexture() != textureMap[maze(x,y)])
					{
						int numTiles = textureMap[maze(x,y)]->getSize().x / TILE_SIZE;
						tiles[x][y].setTexture(textureMap[maze(x,y)]);
						tiles[x][y].setTextureRect(sf::IntRect((rand() % numTiles)*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
					}
				}
				else
				{
					tiles[x][y].setFillColor(sf::Color::Red);
				}

				if (maze(x,y) == Maze::EntryType::KEY)
				{
					AnimatedSprite sprite;
					sprite.setPosition(sf::Vector2f(x*TILE_SIZE,y*TILE_SIZE) + position);
					sprite.setTexture(*textureManager->getTexture("keycard.png"));
					sprite.setAnimationLength(16);
					sprite.setAnimationNumber(rand()%3);
					sprites[{x,y}] = sprite;
				}
				else if (maze(x,y) == Maze::EntryType::END)
				{
					AnimatedSprite sprite;
					sprite.setPosition(sf::Vector2f(x*TILE_SIZE,y*TILE_SIZE) + position);
					sprite.setTexture(*textureManager->getTexture("gate.png"));
					sprite.setAnimationLength(1);
					sprite.setAnimationNumber(0);
					sprites[{x,y}] = sprite;
				}
			}
		}
	}
	void repositionTiles()
	{
		sf::Vector2i mazeSize = maze.getSize();
		for(int x = 0; x < mazeSize.x; x++)
		{
			for(int y = 0; y < mazeSize.y; y++)
			{
				tiles[x][y].setPosition(sf::Vector2f(x*TILE_SIZE,y*TILE_SIZE) + position);
				if (maze(x,y) == Maze::EntryType::KEY || maze(x,y)== Maze::EntryType::END)
				{
					sprites[{x,y}].setPosition(sf::Vector2f(x*TILE_SIZE,y*TILE_SIZE) + position);
				}
			}
		}
	}
public:
	MazeDisplay(sf::Font font, TextureManager* textureManager, Maze maze)
	{
		textureManager->loadTexture("crate.png");
		textureManager->loadTexture("floor.png");
		textureManager->loadTexture("mudfloor.png");
		textureManager->loadTexture("keycard.png");
		textureManager->loadTexture("gate.png");
		this->textureManager = textureManager;
		this->maze = maze;
		this->textureMap[Maze::EntryType::WALL] = textureManager->getTexture("crate.png");
		this->textureMap[Maze::EntryType::EMPTY] = textureManager->getTexture("floor.png");
		this->textureMap[Maze::EntryType::KEY] = textureManager->getTexture("floor.png");
		this->textureMap[Maze::EntryType::START] = textureManager->getTexture("floor.png");
		this->textureMap[Maze::EntryType::END] = textureManager->getTexture("floor.png");
		this->textureMap[Maze::EntryType::ROCKY] = textureManager->getTexture("mudfloor.png");
		this->displayFont = font;
		refreshTiles();
	}
	void setStartingPosition(sf::Vector2i position)
	{
		maze.setStartingPoint(position);
		refreshTiles();
	}
	const Maze getMaze()
	{
		return maze;
	}
	/*
		Get size of maze display, in pixels.
	*/
	sf::Vector2i getSize()
	{
		sf::Vector2i mazeSize = maze.getSize();
		return sf::Vector2i(mazeSize.x * TILE_SIZE, mazeSize.y * TILE_SIZE);
	}
	void setMark(int column, int row, sf::Color color = sf::Color::Yellow, std::string text = "")
	{
		if (column < 0 || row < 0 || column >= maze.getSize().x || row >= maze.getSize().y)
		{
			return;
		}
		sf::RectangleShape newIndicator = sf::RectangleShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
		sf::Text newText;
		newIndicator.setPosition(sf::Vector2f(column*TILE_SIZE,row*TILE_SIZE) + position);

		color.a = 150;
		newIndicator.setFillColor(color);

		if (text!="")
		{
			newText.setFont(displayFont);
			newText.setCharacterSize(24);
			newText.setString(text);
			newText.setOutlineColor(sf::Color::Black);
			newText.setOutlineThickness(2);
			//newText.setOrigin(newText.getLocalBounds().width/2, newText.getLocalBounds().height/2);
			newText.setPosition(sf::Vector2f(column*TILE_SIZE,row*TILE_SIZE) + position);
		}

		indicators[std::make_pair(column, row)] = std::make_pair(newIndicator, newText);
	}
	void clearMark(int column, int row)
	{
		std::pair<int,int> key = std::make_pair(column, row);
		if(indicators.find(key) != indicators.end())
		{
			indicators.erase(key);
		}
	}
	sf::Vector2i getTileIndexAtPixel(sf::Vector2f worldCoordinates)
	{
		sf::Vector2f target = worldCoordinates - position;
		return sf::Vector2i( (int)floor(target.x / TILE_SIZE), (int)floor(target.y / TILE_SIZE) );
	}
	sf::RectangleShape* getTileAtPixel(sf::Vector2f worldCoordinates)
	{
		sf::Vector2i tileIndex = getTileIndexAtPixel(worldCoordinates);
		return getTileAtIndex(tileIndex);
	}
	sf::RectangleShape* getTileAtIndex(sf::Vector2i tileIndex)
	{
		int column = tileIndex.x, row = tileIndex.y;
		if (column < 0 || row < 0 || column >= maze.getSize().x || row >= maze.getSize().y)
		{
			return NULL;
		}
		else
		{
			return &tiles[column][row];
		}
	}
	Maze::EntryType getMazeEntryAtPixel(sf::Vector2f worldCoordinates)
	{
		sf::Vector2i tileIndex = getTileIndexAtPixel(worldCoordinates);
		int column = tileIndex.x, row = tileIndex.y;
		if (column < 0 || row < 0 || column >= maze.getSize().x || row >= maze.getSize().y)
		{
			return Maze::EntryType::EMPTY;
		}
		else
		{
			return maze(column,row);
		}
	}
	void setPosition(sf::Vector2f position)
	{
		this->position = position;
		repositionTiles();
	}
	sf::Vector2f getPosition() {return position;}
	void update(sf::Time deltaTime) 
	{
		sf::Time timePerFrame = sf::milliseconds(120);
		spriteTime += deltaTime;
		
		while(spriteTime >= timePerFrame)
		{
			for (auto&& sprite : sprites)
			{
				sprite.second.advanceFrame(1);
			}
			spriteTime -= timePerFrame;
		}
		
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		sf::Vector2i mazeSize = maze.getSize();
		for(int x = 0; x < mazeSize.x; x++)
		{
			for(int y = 0; y < mazeSize.y; y++)
			{
				target.draw(tiles[x][y], states);
			}
		}
		for (auto&& sprite : sprites)
		{
			target.draw(sprite.second);
		}
		for(auto&& indicator : indicators)
		{
			target.draw(indicator.second.first, states);
			target.draw(indicator.second.second, states);
		}
	}
};
#endif