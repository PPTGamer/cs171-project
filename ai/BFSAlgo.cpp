#include "SearchState.h"
#include "Maze.h"
#include "Algorithm.h"
#include "BFSAlgo.h"

#include <vector>
#include <SFML/Graphics.hpp>

void BFSAlgo::start(){
    first.location = maze.getStart();
    parent[first] = SearchState(-2, -2);
    fringe.push_front(first);
    fillGoalState();
    visited.resize(maze.getSize().y);
    std::cout << "Starting: " << first.location.x << ' ' << first.location.y << std::endl;
    std::cout << "Ending: " << goalstate.location.x << ' ' << goalstate.location.y << std::endl;
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
            this->parent.find(t) == parent.end()) 
        {
            if (this->maze(nx, ny) == Maze::KEY){
                t.keys.insert({nx, ny});
            }
            this->parent[t] = s;
            fringe.push_back(t);
        }
    }
    return s;
}
std::vector<SearchState> BFSAlgo::getSolution(){
    if (not this->finished()){
        return std::vector<SearchState>(0);
    }
    SearchState last = goalstate;
    std::cout << parent[last] << std::endl;
    solution.push_back(last);
    while(not (parent[last] == SearchState(-2, -2))){
        solution.push_back(parent[last]);
        last = parent[last];
    }
    std::reverse(solution.begin(), solution.end());
    std::cout << "solution produced" << std::endl;
    return solution;
}
std::deque<SearchState> BFSAlgo::getFringe(){
        return this->fringe;
}
bool BFSAlgo::finished(){
    return fringe.front() == goalstate;
}
