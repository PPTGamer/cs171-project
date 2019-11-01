#include "Algorithm.h"
#include "UFSAlgo.h"
#include "SearchState.h"
#include "Maze.h"

#include <vector>

UFSAlgo::void start(){
    SearchState first;
    first.location = maze.getStart();
    first.cost = 0;
    prio.push(first);
    fillGoalState();
    parent.resize(maze.getSize().y);
    for (int i = 0; i < maze.getSize().y; i++){
        parent[i].resize(maze.getSize().x);
    }
}
UFSAlgo::SearchState next(){
    if (fringe.empty()){
        return SearchState(-1, -1);
    }
    int dx[4] = {0, -1, 0, 1};
    int dy[4] = {-1, 0, 1, 0};
    SearchState s = fringe.top(); fringe.pop();
    this->visited[s.location.y][s.location.x] = true;
    for (int i = 0; i < 4; ++i){
        int nx = s.location.x + dx[i], ny = s.location.y + dy[i];
        SearchState t(nx, ny);
        if (this->maze(nx, ny) != Maze::WALL and 
            not this->maze.outOfBounds(nx, ny) and 
            this->parent[ny][nx].location == sf::Vector2i(0, 0)) // this works because 0,0 is guaranteed to be a wall
        {
            if (this->maze(nx, ny) == Maze::KEY){
                t.keys.append({nx, ny});
            }
            fringe.push_back(t);
            this->parent[ny][nx] = s;
        }
    }
    return s;
}
UFSAlgo::std::vector<SearchState> getSolution(){
    if (not this->finished()){
        return std::vector<sf::SearchState>(0);
    }
    int gx = this->goalState.x, gy = this->goalState.y;
    solution.push_back(Vector2i(gx, gy));
    while(parent[gy][gx] != sf::Vector2i(0, 0)){
        solution.push_back(parent[gy][gx]);
        gy = parent[gy][gx].y;
        gx = parent[gy][gx].x;
    }
    return solution;
}
UFSAlgo::getFringe(){
    return this->prio;
}