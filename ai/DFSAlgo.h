#ifndef DFSALGO_H
#define DFSALGO_H

#include "Algorithm.h"

#include <vector>
#include <set>

class DFSAlgo : public Algorithm
{
public:
    void start() override;
    SearchState next() override;
    std::vector<SearchState> getSolution()  override;
    bool finished() override;
    std::deque<SearchState> getFringe() override;
};
#endif