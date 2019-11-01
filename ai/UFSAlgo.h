#ifndef UFSALGO_H
#define UFSALGO_H

#include "Algorithm.h"

#include <vector>
#include <priority_queue>

class UFSAlgo : public Algorithm
{
private:
    vector<vector<SearchState> > parent;
    priority_queue<SearchState> prio;
public:
    void start();
    SearchState next();
    std::vector<SearchState> getSolution();
    std::priority_queue<SearchState> getFringe();
};
#endif