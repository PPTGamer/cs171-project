#include "SearchState.h"
#include "Maze.h"
#include "Algorithm.h"
#include "UCSAlgo.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>

void UCSAlgo::start(){
    SearchState first;
    first.location = maze.getStart();
    first.cost = 0;
    prio.push(first);
    fillGoalState();
    parent.resize(maze.getSize().y);
    for (int i = 0; i < maze.getSize().y; i++){
        parent[i].resize(maze.getSize().x, SearchState(0, 0));
    }
}
SearchState UCSAlgo::next(){
    if (fringe.empty()){
        return SearchState(-1, -1);
    }
    int dx[4] = {0, -1, 0, 1};
    int dy[4] = {-1, 0, 1, 0};
    SearchState s = prio.top(); prio.pop();
    this->parent[s.location.y][s.location.x] = SearchState();
    for (int i = 0; i < 4; ++i){
        int nx = s.location.x + dx[i], ny = s.location.y + dy[i];
        SearchState t(nx, ny);
        if (this->maze(nx, ny) != Maze::WALL and 
            not this->maze.out_of_bounds(nx, ny) and 
            this->parent[ny][nx] == SearchState() // this works because 0,0 is guaranteed to be a wall
           ) 
        {
            if (this->maze(nx, ny) == Maze::KEY){
                t.keys.insert({nx, ny});
            }
            if (this->maze(nx, ny) == Maze::ROCKY){
                t.cost += 3;
            }
            else{
                t.cost += 1;
            }
            fringe.push_back(t);
            this->parent[ny][nx] = s;
        }
    }
    return s;
}
std::vector<SearchState> UCSAlgo::getSolution(){
    if (not this->finished()){
        return std::vector<SearchState>(0);
    }
    int gx = this->goalstate.location.x, gy = this->goalstate.location.y;
    solution.push_back(SearchState(gx, gy));
    while(not(parent[gy][gx].location == sf::Vector2i(0, 0))){
        solution.push_back(parent[gy][gx]);
        gy = parent[gy][gx].location.y;
        gx = parent[gy][gx].location.x;
    }
    return solution;
}
std::priority_queue<SearchState> UCSAlgo::getFringe(){
    return this->prio;
}
bool UCSAlgo::finished(){
    return prio.top() == goalstate;
}