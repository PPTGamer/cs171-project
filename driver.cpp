#include <SFML/Graphics.hpp>
#include <random>

#include "ai/Maze.h"
#include "ai/SearchState.h"
#include "ai/Algorithm.h"
#include "ai/ManhattanHeuristic.h"
#include "ai/UCSAlgo.h"
int main(){
    Maze maze(8, 8);
    maze.generate();
    maze.printMaze();

    Algorithm * bfs = new ManhattanHeuristic();
    bfs->setMaze(maze);
    bfs->start();
    int expansions = 0;
    while(not bfs->finished()){
        SearchState s = bfs->next(); 
        if (s == SearchState(0, 0) or s == SearchState(-1, -1)){
            break;
        }
        expansions++;
    }
    std::cout << "It took A* " << expansions << " expansions to find the solution." << std::endl;
    
    Algorithm * ucs = new UCSAlgo();
    ucs->setMaze(maze);
    ucs->start();
    expansions = 0;
    while(not ucs->finished()){
        SearchState s = bfs->next();
        if (s == SearchState(0, 0) or s == SearchState(-1, -1)){
            break;
        }
        expansions++;
    }
    std::cout << "It took UCS " << expansions  << " expansions to find the solution." << std::endl;

    delete bfs;
    delete ucs;
    return 0;
}