#include "ai/Maze.h"
#include "ai/SearchState.h"
#include "ai/Algorithm.h"
#include "ai/BFSAlgo.h"
#include "ai/UFSAlgo.h"

int main(){
    Maze maze(9, 9);
    maze.generate();
    maze.printMaze();

    Algorithm * bfs = new BFSAlgo();
    bfs->setMaze(maze);
    bfs->start();

    while(not bfs->finished()){
        std::cout << bfs->next() << std::endl;
    }
    for (SearchState s : bfs->getSolution()){
        std::cout << s << std::endl;
    }

    delete bfs;
    return 0;
}