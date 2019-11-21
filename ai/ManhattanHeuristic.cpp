#include "SearchState.h"
#include "Maze.h"
#include "Algorithm.h"
#include "ManhattanHeuristic.h"

#include <cmath>

std::string ManhattanHeuristic::getName()
{
	return "A* (Manhattan Distance Heuristic)";
}

void ManhattanHeuristic::start(){
	statesExpanded = 0;
	first.location = maze.getStart();
	prio.insert(first);
	gScore[first] = 0; fScore[first] = heuristic_function(first);
	first.cost = heuristic_function(first);
	parent[first] = SearchState(-2, -2);
	fillGoalState();
}
// Parts of this code was taken from 
// https://en.wikipedia.org/wiki/A*_search_algorithm

SearchState ManhattanHeuristic::next(){
	if (prio.empty()){
		return SearchState(-1, -1);
	}
	statesExpanded++;
	int dx[4] = {0, -1, 0, 1};
	int dy[4] = {-1, 0, 1, 0};
	SearchState s = *prio.rbegin(); prio.erase(*prio.rbegin());
	for (int i = 0; i < 4; ++i){
		int nx = s.location.x + dx[i], ny = s.location.y + dy[i];
		if (this->maze.out_of_bounds(nx, ny) or this->maze(nx, ny) == Maze::WALL) continue;

		SearchState t(nx, ny);
		for (auto key : s.keys){
			t.keys.insert(key);
		}
		if (this->maze(nx, ny) == Maze::KEY){
			t.keys.insert({nx, ny});
		}

		int tentative_gScore = gScore[s] + 1;
		if (this->maze(nx, ny) == Maze::ROCKY)
			tentative_gScore += 6;

		if (gScore.count(t) == 0 or tentative_gScore < gScore[t]){
			gScore[t] = tentative_gScore;
			fScore[t] = gScore[t] + heuristic_function(t);
			t.cost = fScore[t];
			parent[t] = s;

			if (prio.count(t) == 0){
				prio.insert(t);  
			}
		}
	}
	return s;
}

std::vector<SearchState> ManhattanHeuristic::getSolution(){
	if (not this->finished()){
		return std::vector<SearchState>(0);
	}
	SearchState last = arrival;
	while(not (parent.count(last) == 0 or parent[last] == SearchState(-2, -2))){
		solution.push_back(last);
		last = parent[last];
	}
	solution.push_back(last);
	std::reverse(solution.begin(), solution.end());

	return solution;
}

std::deque<SearchState> ManhattanHeuristic::getFringe(){
	fringe.clear();
	fringe.assign(prio.begin(), prio.end());
	return fringe;
}

bool ManhattanHeuristic::finished(){
	this->arrival = *prio.rbegin();
	return *prio.rbegin() == goalstate;
}

int ManhattanHeuristic::heuristic_function(SearchState s){
	int d = dist_to_keys(s);
	if (d > 0)
		return d;
	return manhattan(goalstate.location, s.location);
}

int ManhattanHeuristic::manhattan(sf::Vector2i a, sf::Vector2i b){
	sf::Vector2i c = a - b;
	return std::abs(c.x) + std::abs(c.y);
}

int ManhattanHeuristic::dist_to_keys(const SearchState& s){
	int answer = 0;
	for (auto key : goalstate.keys){
		if (s.keys.count(key) == 0){
			answer = std::min(answer, manhattan({key.first, key.second}, s.location));
		}
	}
	return answer;
}
