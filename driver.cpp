#include "ai/Maze.h"
#include "ai/SearchState.h"
#include "ai/Algorithm.h"
#include "ai/BFSAlgo.h"
#include "ai/UCSAlgo.h"

int main(){
    Maze maze(8, 8);
    maze.generate();
    maze.printMaze();

    Algorithm * bfs = new UCSAlgo();
    bfs->setMaze(maze);
    bfs->start();
    int expansions = 0;
    while(not bfs->finished()){
        SearchState s = bfs->next(); expansions++;
        //std::cout << s << std::endl;
    }
    std::cout << "It took " << expansions << " expansions to find the solution." << std::endl;
    std::cout << "Solution" << std::endl;
    for (SearchState s : bfs->getSolution()){
        std::cout << s << std::endl;
    }

    delete bfs;
    return 0;
}