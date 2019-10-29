#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <random>
#include <vector>
#include <deque>

class Maze
{
private:
	std::vector< std::vector<int> > v;
	int height;
	int width;
	int startx, starty;
public:
	enum EntryType {EMPTY, WALL, START, END};
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
		this->startx = 1;
		this->starty = 0;
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
	void generate(){
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				this->v[i][j] = WALL;
			}
		}
		randomDFS();
		printMaze();
	}
private:
	bool out_of_bounds(int x, int y){
		return x <= 0 or x >= width - 1 or y <= 0 or y >= height - 1;
	}

	std::pair<int, int> gen_start(){
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist_width(0, (width - 3) / 2);
		std::uniform_int_distribution<std::mt19937::result_type> dist_height(0, (height - 3) / 2);
		return {2 * dist_width(rng) + 1, 2 * dist_height(rng) + 1};
		
	}

	void randomDFS(){
		int dx[4] = {0, -1, 0, 1};
		int dy[4] = {-1, 0, 1, 0};
		std::deque<std::pair<int, int>> fringe;
		std::pair<int, int> generated_startpos = gen_start();
		this->startx = generated_startpos.first; this->starty = generated_startpos.second;
		
		v[starty][startx] = EMPTY;
		fringe.push_back({startx, starty});
		std:: cout << "starting coordinates: " << this->startx << ' ' << this->starty << std::endl;
		while(not fringe.empty()){
			int x = fringe.back().first;
			int y = fringe.back().second; fringe.pop_back();
			DirectionType order[4] = {NORTH,WEST,SOUTH,EAST};
			std::random_shuffle(order, order + 4);
			for (int i = 0; i < 4; ++i){
				int px = x + dx[order[i]];
				int py = y + dy[order[i]];
				int nx = px + dx[order[i]];
				int ny = py + dy[order[i]];
				if (out_of_bounds(nx, ny)) continue;
				if (v[ny][nx] == WALL){
					v[py][px] = v[ny][nx] = EMPTY;
					fringe.push_back({nx, ny});
				}
			}
		}
	}
	void printMaze(){
		for (auto row : v){
			for (int cell : row){
				if (cell == WALL)
					std::cout << '#';
				else
					std::cout << ' ';
			}
			std::cout << std::endl;
		}
	}
};

#endif