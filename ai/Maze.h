#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <random>
#include <vector>
#include <deque>
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
	enum EntryType {EMPTY, WALL, START, END, KEY};
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
	void setStartingPoint(const sf::Vector2i& loc){
		this->star = loc;
		this->v[loc.y][loc.x] = START;
	}
	void generate(){
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				this->v[i][j] = WALL;
			}
		}
		randomDFS();
		randomKeys();
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

	sf::Vector2i gen_cell(){
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist_width(1, width - 2);
		std::uniform_int_distribution<std::mt19937::result_type> dist_height(1, height - 2);
		int randx = (int)dist_width(rng), (int)randy = dist_height(rng);
		return {randx, randy};
	}

	void randomDFS(){
		int dx[4] = {0, -1, 0, 1};
		int dy[4] = {-1, 0, 1, 0};
		std::deque<sf::Vector2i> fringe;
		sf::Vector2i generated_startpos = gen_start();
		this->start = generated_startpos;

		v[starty][startx] = START;
		fringe.push_back({startx, starty});
		std:: cout << "starting coordinates: " << this->startx << ' ' << this->starty << std::endl;
		while(not fringe.empty()){
			int cx = fringe.back().x;
			int cy = fringe.back().y; fringe.pop_back();
			DirectionType order[4] = {NORTH,WEST,SOUTH,EAST};
			std::random_shuffle(order, order + 4);
			for (int i = 0; i < 4; ++i){
				int px = cx + dx[order[i]];
				int py = cy + dy[order[i]];
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

	void randomKeys(){
		int empty = 0;
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				if (v[i][j] == EMPTY)
					empty++;

		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist_keys(1, empty / 5); // There are at most 20% of empty cells as cells with keys
		std::uniform_int_distribution<std::mt19937::result_type> dist_width(1, width - 2);
		std::uniform_int_distribution<std::mt19937::result_type> dist_height(1, height - 2);

		int number_of_keys = dist_keys(rng);
		std::cout << "Generating " << number_of_keys << " keys" << std::endl;
		while(number_of_keys > 0){
			int randx = dist_width(rng), randy = dist_height(rng);
			if (v[randx][randy] == EMPTY){
				this->v[randx][randy] = KEY;
				--number_of_keys;
			}
		}

	EntryType chooseTerrain(){
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist_terrain(0, 4); //20% chance that an "EMPTY" cell will be "ROCKY"
		if (dist_terrain(rng) < 4) return EMPTY;
		return ROCKY;
	}

	void randomEnd(){
		auto endCandidate = this->gen_cell();
		while(v[endCandidate.x][endCandidate.y] == KEY){
			endCandidate = this->gen_cell();
		}
		this->end = endCandidate;
		v[endCandidate.x][endCandidate.y] = END;
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