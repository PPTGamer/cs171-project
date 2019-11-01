#ifndef UFSALGO_H
#define UFSALGO_H

#include "Algorithm.h"

#include <vector>
#include <queue>

class UFSAlgo : public Algorithm
{
private:
    std::vector<std::vector<SearchState> > parent;
    std::priority_queue<SearchState> prio;
public:
    void start();
    SearchState next();
    std::vector<SearchState> getSolution();
    std::priority_queue<SearchState> getFringe();
};
#endif