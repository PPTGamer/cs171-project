#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Maze.h"
#include "SearchState.h"

#include <deque>
#include <vector>

class Algorithm{
protected:
    std::deque<SearchState> fringe;
    std::vector<SearchState> solution;
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
    bool finished(){
        SearchState test = fringe.front();
        return test == this->goalstate;
    }
    
protected:
    void fillGoalState(){
        for(int i = 0; i < this->maze.getSize().x; ++i){
            for (int j = 0; j < this->maze.getSize().y; ++j){
                if (maze(i, j) == Maze::END){
                    goalstate.location = sf::Vector2i(i, j);
                }
                if (maze(i, j) == Maze::KEY){
                    goalstate.keys.insert(sf::Vector2i(i, j));
                }
            }
        }
    }
};

#endif