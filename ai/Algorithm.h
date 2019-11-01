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
    virtual void formSolution() = 0;
    std::vector<sf::Vector2i> getSolution() = 0;
    bool finished(){
        SearchState test = fringe.front();
        return test == this->goalstate;
    };
    virtual std::deque<SearchState> getFringe(){
        return this->fringe;
    }
protected:
    void fillGoalState{
        for(int i = 0; i < this->maze.getSize().x; ++i){
            for (int j = 0; j < this->maze.getSize().y; ++j){
                if (maze(i, j) == Maze::END){
                    goalState.location = sf::Vector2i(i, j);
                }
                if (maze(i, j) == Maze::KEY){
                    goalState.keys.add(sf::Vector2i(i, j));
                }
            }
        }
    }
};

#endif