#include "SearchState.h"
#include "Maze.h"
#include "Algorithm.h"
#include "UCSAlgo.h"

#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>

std::string UCSAlgo::getName()
{
	return "UCS";
}

void UCSAlgo::start(){
	statesExpanded = 0;
	first.location = maze.getStart();
	first.cost = 0;
	prio.insert(first); visited_without_cost.insert(first);
	parent[first] = SearchState(-2, -2);
	fillGoalState();
	std::cout << "Starting: " << first.location.x << ' ' << first.location.y << std::endl;
	std::cout << "Ending: " << goalstate.location.x << ' ' << goalstate.location.y << std::endl;
}
SearchState UCSAlgo::next(){
	if (prio.empty()){
		return SearchState(-1, -1);
	}
	statesExpanded++;
	int dx[4] = {0, -1, 0, 1};
	int dy[4] = {-1, 0, 1, 0};
	SearchState s = *prio.rbegin(); prio.erase(*prio.rbegin());
	for (int i = 0; i < 4; ++i){
		int nx = s.location.x + dx[i], ny = s.location.y + dy[i];
		SearchState t(nx, ny);
		for (auto key : s.keys){
			t.keys.insert(key);
		}
		if (visited_without_cost.count(t) > 0) continue;
		visited_without_cost.insert(t);
		t.cost = s.cost + 1;
		if (this->maze(nx, ny) != Maze::WALL and 
			not this->maze.out_of_bounds(nx, ny) ) 
		{
			if (this->parent.find(t) == parent.end()){
				if (this->maze(nx, ny) == Maze::KEY){
					t.keys.insert({nx, ny});
				}
				if (this->maze(nx, ny) == Maze::ROCKY){
					t.cost += 6;
				}
				this->parent[t] = s;
				prio.insert(t);
			}
		}
	}
	return s;
}
std::vector<SearchState> UCSAlgo::getSolution(){
	if (not this->finished()){
		return std::vector<SearchState>(0);
	}
	SearchState last = this->arrival;
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
	this->arrival = *prio.rbegin();
	return *prio.rbegin() == goalstate;
}
