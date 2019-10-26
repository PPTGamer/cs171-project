#ifndef MAZE_DISPLAY_H
#define MAZE_DISPLAY_H
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include "../ai/Maze.h"
class MazeDisplay : public GameSprite
{
private:
	const int TILE_SIZE = 64;
	sf::Vector2f position;
	Maze maze;
	sf::Texture* wallTexture;
	sf::Texture* groundTexture;
	std::vector<std::vector<sf::RectangleShape>> tiles;
	void refreshTiles()
	{
		sf::Vector2i mazeSize = maze.getSize();
		tiles.resize(mazeSize.x);
		for(int x = 0; x < mazeSize.x; x++)
		{
			tiles[x].resize(mazeSize.y);
			for(int y = 0; y < mazeSize.y; y++)
			{
				tiles[x][y].setSize(sf::Vector2f(TILE_SIZE,TILE_SIZE));
				tiles[x][y].setPosition(sf::Vector2f(x*TILE_SIZE,y*TILE_SIZE) + position);
				if(maze(x,y) == Maze::EntryType::WALL)
				{
					if(wallTexture != NULL)
					{
						int numTiles = wallTexture->getSize().x / TILE_SIZE;
						tiles[x][y].setTexture(wallTexture);
						tiles[x][y].setTextureRect(sf::IntRect((rand() % numTiles)*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
					}
					else
					{
						tiles[x][y].setFillColor(sf::Color::Red);
					}
				}
				else
				{
					if(groundTexture != NULL)
					{
						int numTiles = groundTexture->getSize().x / TILE_SIZE;
						tiles[x][y].setTexture(groundTexture);
						tiles[x][y].setTextureRect(sf::IntRect((rand() % numTiles)*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
					}
					else
					{
						tiles[x][y].setFillColor(sf::Color::White);
					}
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
			}
		}
	}
public:
	MazeDisplay(sf::Texture* wallTexture = NULL, sf::Texture* groundTexture = NULL, Maze maze = Maze())
	{
		this->maze = maze;
		this->wallTexture = wallTexture;
		refreshTiles();
	}
	sf::Vector2i getSize()
	{
		sf::Vector2i mazeSize = maze.getSize();
		return sf::Vector2i(mazeSize.x * TILE_SIZE, mazeSize.y * TILE_SIZE);
	}
	void setPosition(sf::Vector2f position)
	{
		this->position = position;
		repositionTiles();
	}
	sf::Vector2f getPosition() {return position;}
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
	}
};
#endif