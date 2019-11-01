#include "SearchState.h"
#include "Maze.h"
#include "Algorithm.h"
#include "UCSAlgo.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>

#include "SearchState.h"
#include "Maze.h"
#include "Algorithm.h"
#include "UCSAlgo.h"

#include <vector>
#include <SFML/Graphics.hpp>

void UCSAlgo::start(){
    first.location = maze.getStart();
    prio.insert(first);
    parent[first] = SearchState(-2, -2);
    fillGoalState();
    std::cout << "Starting: " << first.location.x << ' ' << first.location.y << std::endl;
    std::cout << "Ending: " << goalstate.location.x << ' ' << goalstate.location.y << std::endl;
}
SearchState UCSAlgo::next(){
    if (fringe.empty()){
        return SearchState(-1, -1);
    }
    int dx[4] = {0, -1, 0, 1};
    int dy[4] = {-1, 0, 1, 0};
    SearchState s = *prio.rbegin(); prio.rbegin();
    //std::cout << s << std::endl;
    for (int i = 0; i < 4; ++i){
        int nx = s.location.x + dx[i], ny = s.location.y + dy[i];
        SearchState t(nx, ny);
        for (auto key : s.keys){
            t.keys.insert(key);
        }
        if (this->maze(nx, ny) != Maze::WALL and 
            not this->maze.out_of_bounds(nx, ny) ) 
        {
            if (this->parent.find(t) == parent.end()){
                if (this->maze(nx, ny) == Maze::KEY){
                    t.keys.insert({nx, ny});
                }
                this->parent[t] = s;
                fringe.push_back(t);
            }
        }
    }
    return s;
}
std::vector<SearchState> UCSAlgo::getSolution(){
    if (not this->finished()){
        return std::vector<SearchState>(0);
    }
    SearchState last = goalstate;
    std::cout << "solution:" << std::endl;
    while(not (parent[last] == SearchState(-2, -2))){
        solution.push_back(last);
        last = parent[last];
    }
    solution.push_back(last);
    std::reverse(solution.begin(), solution.end());

    return solution;
}
std::deque<SearchState> UCSAlgo::getFringe(){
    std::deque<SearchState> x;
    x.assign(prio.begin(), prio.end());
    return x;
}
bool UCSAlgo::finished(){
    return fringe.front() == goalstate;
}
