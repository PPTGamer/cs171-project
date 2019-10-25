#ifndef MAZE_DISPLAY_H
#define MAZE_DISPLAY_H
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include "../ai/Maze.h"
class MazeDisplay : public GameSprite
{
private:
	sf::Vector2f position;
	Maze maze;
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
				tiles[x][y].setSize(sf::Vector2f(64.0,64.0));
				tiles[x][y].setPosition(sf::Vector2f(x*64,y*64) + position);
				if(maze(x,y) == Maze::EntryType::EMPTY)
					tiles[x][y].setFillColor(sf::Color(255,255,255));
				else
					tiles[x][y].setFillColor(sf::Color(255,0,0));
			}
		}
	}
public:
	MazeDisplay(Maze maze = Maze())
	{
		this->maze = maze;
		refreshTiles();
	}
	void setPosition(sf::Vector2f position)
	{
		this->position = position;
		refreshTiles();
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