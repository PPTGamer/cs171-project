#include "Algorithm.h"
#include "BFSAlgo.h"
#include "SearchState.h"
#include "Maze.h"

#include <vector>
#include <SFML/Graphics.hpp>

void BFSAlgo::start(){
    first.location = maze.getStart();
    fringe.push_front(first);
    fillGoalState();
    parent.resize(maze.getSize().y);
    for (int i = 0; i < maze.getSize().y; i++){
        parent[i].resize(maze.getSize().x);
    }
}
SearchState BFSAlgo::next(){
    if (fringe.empty()){
        return SearchState(-1, -1);
    }
    int dx[4] = {0, -1, 0, 1};
    int dy[4] = {-1, 0, 1, 0};
    SearchState s = fringe.front(); fringe.pop_front();
    for (int i = 0; i < 4; ++i){
        int nx = s.location.x + dx[i], ny = s.location.y + dy[i];
        SearchState t(nx, ny);
        if (this->maze(nx, ny) != Maze::WALL and 
            not this->maze.out_of_bounds(nx, ny) and 
            this->parent[ny][nx] == SearchState() // this works because 0, 0 is guaranteed to be a wall
           ) 
        {
            if (this->maze(nx, ny) == Maze::KEY){
                t.keys.insert({nx, ny});
            }
            fringe.push_back(t);
            this->parent[ny][nx] = s;
        }
    }
    return s;
}
std::vector<SearchState> BFSAlgo::getSolution(){
    if (not this->finished()){
        return std::vector<SearchState>(0);
    }
    int gx = this->goalstate.location.x, gy = this->goalstate.location.y;
    solution.push_back(SearchState(gx, gy));
    while(not (parent[gy][gx] == first)){
        solution.push_back(parent[gy][gx]);
        gy = parent[gy][gx].location.y;
        gx = parent[gy][gx].location.x;
    }
    std::reverse(solution.begin(), solution.end());
    return solution;
}
std::deque<SearchState> BFSAlgo::getFringe(){
        return this->fringe;
    }
