#include "SearchState.h"
#include "Maze.h"
#include "Algorithm.h"
#include "DFSAlgo.h"

#include <vector>
#include <SFML/Graphics.hpp>

std::string DFSAlgo::getName()
{
	return "DFS";
}

void DFSAlgo::start(){
	statesExpanded = 0;
	first.location = maze.getStart();
	fringe.push_front(first);
	parent[first] = SearchState(-2, -2);
	fillGoalState();
	std::cout << "Starting: " << first.location.x << ' ' << first.location.y << std::endl;
	std::cout << "Ending: " << goalstate.location.x << ' ' << goalstate.location.y << std::endl;
}
SearchState DFSAlgo::next(){
	if (fringe.empty()){
		return SearchState(-1, -1);
	}
	statesExpanded++;
	int dx[4] = {0, -1, 0, 1};
	int dy[4] = {-1, 0, 1, 0};
	SearchState s = fringe.front(); fringe.pop_front();
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
				fringe.push_front(t);
			}
		}
	}
	return s;
}
std::vector<SearchState> DFSAlgo::getSolution(){
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
std::deque<SearchState> DFSAlgo::getFringe(){
		return this->fringe;
}
bool DFSAlgo::finished(){
	return fringe.front() == goalstate;
}
