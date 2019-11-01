#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <deque>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Maze.h"
#include "SearchState.h"



class Algorithm{
protected:
    std::deque<SearchState> fringe;
    std::vector<SearchState> solution;
    std::map<SearchState, SearchState> parent;
    SearchState goalstate;
    Maze maze;
public:
    Algorithm(){};
    Algorithm(Maze m) : maze(m){};
    void setMaze(Maze m){
        this->maze = m;
    };
    virtual void start() = 0;
    virtual SearchState next() = 0;
    virtual std::vector<SearchState> getSolution() = 0;
    virtual bool finished() = 0;
    
protected:
    void fillGoalState(){
        for(int i = 0; i < this->maze.getSize().x; ++i){
            for (int j = 0; j < this->maze.getSize().y; ++j){
                if (maze(i, j) == Maze::END){
                    goalstate.location = sf::Vector2i(i, j);
                }
                if (maze(i, j) == Maze::KEY){
                    goalstate.keys.insert({i, j});
                }
            }
        }
    }
};

#endif