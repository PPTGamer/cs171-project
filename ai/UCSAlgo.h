#ifndef UCSALGO_H
#define UCSALGO_H

#include "Algorithm.h"

#include <vector>
#include <queue>

class UCSAlgo : public Algorithm
{
protected:
	std::set<SearchState> prio;
	std::set<SearchState> visited_without_cost;
	SearchState arrival;
public:
	void start() override;
	SearchState next() override;
	std::vector<SearchState> getSolution() override;
	std::deque<SearchState> getFringe() override;
	bool finished() override;
	std::string getName() override;
};
#endif