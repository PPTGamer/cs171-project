#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <deque>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Maze.h"
#include "SearchState.h"



class Algorithm{
protected:
	std::deque<SearchState> fringe;
	std::vector<SearchState> solution;
	std::map<SearchState, SearchState> parent;
	SearchState first;
	SearchState goalstate;
	Maze maze;
	int statesExpanded = 0;
	
public:
	Algorithm(){};
	Algorithm(Maze m) : maze(m){};
	void setMaze(Maze m){
		this->maze = m;
	};
	virtual ~Algorithm() = default;
	virtual void start() = 0;
	virtual SearchState next() = 0;
	virtual std::deque<SearchState> getFringe() = 0;
	virtual std::vector<SearchState> getSolution() = 0;
	virtual bool finished() = 0;
	int getNumStatesExpanded(){return statesExpanded;}
	virtual std::string getName(){return "[ALGORITHM NAME]";}
	int getSolutionCost()
	{
		int totalCost = 0;
		for (auto&& state : solution)
		{
			switch(maze(state.location.x, state.location.y))
			{
				case Maze::EntryType::START: totalCost += 0; break;
				case Maze::EntryType::ROCKY: totalCost += 6; break;
				default: totalCost += 1; break;
			}
		}
		return totalCost;
	}
	
protected:
	void fillGoalState(){
		for(int i = 0; i < this->maze.getSize().x; ++i){
			for (int j = 0; j < this->maze.getSize().y; ++j){
				if (maze(i, j) == Maze::END){
					goalstate.location = sf::Vector2i(i, j);
					std::cout << "Exit is at " << "(" << i << ", " << j << ")" << std::endl;
				}
				if (maze(i, j) == Maze::KEY){
					goalstate.keys.insert({i, j});
				}
			}
		}
	}
};

#endif