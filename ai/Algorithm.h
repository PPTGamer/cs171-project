#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <deque>
#include <vector>
#include <Maze.h>
#include<SearchState.h>

class Algorithm{
private:
    std::deque<SearchState> fringe;
    std::vector<SearchState> solution;
    Maze maze;
public:
    Algorithm(){};
    void setMaze(Maze m){
        this->maze = m;
    };

    virtual void start(){};
    virtual SearchState next() = 0;

    bool finished(){};
};

#endif