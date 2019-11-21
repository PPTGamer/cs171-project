#include <SFML/Graphics.hpp>
#include <random>

#include "ai/Maze.h"
#include "ai/MazeGenerator.h"
#include "ai/SearchState.h"
#include "ai/Algorithm.h"
#include "ai/ManhattanHeuristic.h"
#include "ai/UCSAlgo.h"

void setRandomStarting(Maze& m, std::mt19937 rng){
    std::uniform_int_distribution<std::mt19937::result_type> distx(0, m.getSize().x - 1);
    std::uniform_int_distribution<std::mt19937::result_type> disty(0, m.getSize().y - 1);

    m.setStartingPoint({(int)distx(rng), (int)disty(rng)});
}

int main(){
    MazeGenerator mg{};
    Maze maze = mg.generate(8, 8);
    maze.printMaze();

    std::random_device dev;
    std::mt19937 rng(dev());
    setRandomStarting(maze, rng);
    

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