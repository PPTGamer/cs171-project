#ifndef MAZEGEN_H
#define MAZEGEN_H

#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <deque>
#include <SFML/Graphics.hpp>
#include "Maze.h"

class MazeGenerator
{
private:
    std::mt19937 rng;
    Maze target_maze;
    sf::Vector2i size;
public:
    MazeGenerator(std::mt19937 r) : rng(r){}
    MazeGenerator(){
        std::random_device dev;
		rng = std::mt19937(dev());
    }
    Maze generate(int width, int height){
		target_maze = Maze(width, height);
		size = target_maze.getSize();
        for (int i = 0; i < size.x; i++){
			for (int j = 0; j < size.y; j++){
				this->target_maze(i, j) = Maze::EntryType::WALL;
			}
		}
		sf::Vector2i start = gen_start();
        randomDFS(start);
        randomKeys();
        randomEnd(start);
		return target_maze;
    }

private:
    sf::Vector2i gen_start(){
		std::uniform_int_distribution<std::mt19937::result_type> dist_width(0, (size.x - 3) / 2);
		std::uniform_int_distribution<std::mt19937::result_type> dist_height(0, (size.y - 3) / 2);
		return sf::Vector2i(2 * dist_width(rng) + 1, 2 * dist_height(rng) + 1);
	}
	sf::Vector2i gen_cell(){
		std::uniform_int_distribution<std::mt19937::result_type> dist_width(1, size.x - 2);
		std::uniform_int_distribution<std::mt19937::result_type> dist_height(1, size.y - 2);
		int randx = (int)dist_width(rng), randy = (int)dist_height(rng);
		return sf::Vector2i(randx, randy);
	}
	void randomDFS(sf::Vector2i start){
		int dx[4] = {0, -1, 0, 1};
		int dy[4] = {-1, 0, 1, 0};

		std::deque<sf::Vector2i> fringe; //data struct for dfs
		target_maze(start.x, start.y) = Maze::EntryType::START;
		std::cout << "starting coordinates: " << start.x << ' ' << start.y << std::endl;
		//dfs proper
		fringe.push_back(start);
		while(not fringe.empty()){
			int cx = fringe.back().x;
			int cy = fringe.back().y; fringe.pop_back();
			Maze::DirectionType order[4] = {
				Maze::DirectionType::NORTH,
				Maze::DirectionType::WEST,
				Maze::DirectionType::SOUTH,
				Maze::DirectionType::EAST
			};
			std::random_shuffle(order, order + 4);
			for (int i = 0; i < 4; ++i){
				int px = cx + dx[order[i]];
				int py = cy + dy[order[i]];
				int nx = px + dx[order[i]];
				int ny = py + dy[order[i]];
				if (target_maze.out_of_bounds(nx, ny)) continue;
				if (target_maze(nx, ny) == Maze::EntryType::WALL){
					target_maze(px, py) = target_maze(nx, ny) = chooseTerrain();
					fringe.push_back(sf::Vector2i(nx, ny));
				}
			}
		}
		//generate random holes
		for (int i = 0; i < size.x*size.y/4; i++)
		{
			sf::Vector2i cell = gen_cell();
			if(target_maze(cell.x, cell.y) == Maze::EntryType::WALL) 
				target_maze(cell.x, cell.y) = chooseTerrain();
		}
	}
	void randomKeys(){
		int empty = 0;
		for (int i = 0; i < size.x; ++i)
			for (int j = 0; j < size.y; ++j)
				if (target_maze(i, j) == Maze::EntryType::EMPTY)
					empty++;

		std::uniform_int_distribution<std::mt19937::result_type> dist_keys(1, 3);
		std::uniform_int_distribution<std::mt19937::result_type> dist_width(1, size.x - 2);
		std::uniform_int_distribution<std::mt19937::result_type> dist_height(1, size.y - 2);

		int number_of_keys = dist_keys(rng);
		//number_of_keys = 1;
		std::cout << "Generating " << number_of_keys << " keys" << std::endl;
		while(number_of_keys > 0){
			int randx = dist_width(rng), randy = dist_height(rng);
			if (target_maze(randx, randy)== Maze::EntryType::EMPTY){
				target_maze(randx, randy) = Maze::EntryType::KEY;
				--number_of_keys;
			}
		}
	}
	Maze::EntryType chooseTerrain(){
		std::uniform_int_distribution<std::mt19937::result_type> dist_terrain(0, 4); //20% chance that an "EMPTY" cell will be "ROCKY"
		if (dist_terrain(rng) < 4) return Maze::EntryType::EMPTY;
		return Maze::EntryType::ROCKY;
	}
	void randomEnd(sf::Vector2i start){
		auto endCandidate = gen_cell();
		while(target_maze(endCandidate.x, endCandidate.y) == Maze::EntryType::KEY || endCandidate == start){
			endCandidate = gen_cell();
		}
		target_maze(endCandidate.x, endCandidate.y) = Maze::EntryType::END;
	}
};

#endif