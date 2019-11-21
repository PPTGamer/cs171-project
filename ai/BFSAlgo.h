#ifndef BFSALGO_H
#define BFSALGO_H

#include "Algorithm.h"

#include <vector>
#include <set>

class BFSAlgo : public Algorithm
{
public:
	void start() override;
	SearchState next() override;
	std::vector<SearchState> getSolution()  override;
	bool finished() override;
	std::deque<SearchState> getFringe() override;
	std::string getName() override;
};
#endif