#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <deque>
#include <SFML/Graphics.hpp>
/*
 * Usage: Maze maze(width, height)
 * maze.generate() to do the thing
 */
class Maze
{
private:
	std::vector< std::vector<int> > v;
	int height;
	int width;
	sf::Vector2i start;
	sf::Vector2i end;
public:
	enum EntryType {EMPTY, WALL, START, END, KEY, ROCKY, INVALID};
	enum DirectionType {NORTH, WEST, SOUTH, EAST};
	sf::Vector2i getSize() const { return sf::Vector2i(width, height); }
	/*
	 * Retrieve the maze entry at a given column and row.
	 */
	EntryType& operator() (int column, int row) {return (EntryType&)v[row][column];}
	Maze()
	{
		this->v = {
			{ 1, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 0 },
			{ 1, 0, 0, 1, 1 }
			};
		this->height = 3;
		this->width = 5;
		this->start = {1, 0};
	};
	Maze(int w, int h)
	{
		this->width = w * 2 + 1;
		this->height = h * 2 + 1;
		this->v.resize(height);
		for (int i = 0; i < height; ++i){
			this->v[i].resize(width);
		}
	}
	void setStartingPoint(const sf::Vector2i& loc){
		this->start = loc;
		this->v[loc.y][loc.x] = START;
	}
	sf::Vector2i getStart(){
		return this->start;
	}
	bool out_of_bounds(int x, int y){
		return x <= 0 or x >= width - 1 or y <= 0 or y >= height - 1;
	}
	void printMaze(){
		for (auto row : v){
			for (int cell : row){
				if (cell == WALL)
					std::cout << '#';
				else if (cell == EMPTY)
					std::cout << '.';
				else
					std::cout << cell;
			}
			std::cout << std::endl;
		}
	}

};

#endif