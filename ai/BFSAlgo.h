#ifndef BFSALGO_H
#define BFSALGO_H

#include "Algorithm.h"

#include <vector>

class BFSAlgo : public Algorithm
{
private:
    SearchState first;
    std::vector<std::vector<SearchState> > parent;
public:
    void start();
    SearchState next();
    std::vector<SearchState> getSolution();
    std::deque<SearchState> getFringe();
};
#endif