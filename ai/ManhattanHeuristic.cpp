#include "SearchState.h"
#include "Maze.h"
#include "Algorithm.h"
#include "ManhattanHeuristic.h"

#include <cmath>

void ManhattanHeuristic::start(){
    first.location = maze.getStart();
    fillGoalState();
}

int ManhattanHeuristic::manhattan(sf::Vector2i a, sf::Vector2i b){
    sf::Vector2i c = a - b;
    return std::abs(c.x) + std::abs(c.y);
}

int ManhattanHeuristic::dist_to_keys(const SearchState& s){
    int answer = 0;
    for (auto key in goalstate.keys){
        if (s.keys.find(key) != s.keys.end()){
            answer += manhattan(key, s.location);
        }
    }
    return answer;
}