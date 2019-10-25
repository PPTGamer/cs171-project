#ifndef MAZE_H
#define MAZE_H

class Maze
{
private:
	int v[3][5] =
	{
	{ 1, 0, 0, 0, 0 },
	{ 0, 0, 1, 1, 0 },
	{ 1, 0, 0, 1, 1 }
	};
	int height = 3;
	int width = 5;
public:
	enum EntryType {EMPTY, WALL};
	sf::Vector2i getSize() const { return sf::Vector2i(width, height); }
	/*
	 * Retrieve the maze entry at a given column and row.
	 */
	EntryType& operator() (int column, int row) {return (EntryType&)v[row][column];}
};

#endif