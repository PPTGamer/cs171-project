#ifndef BFSALGO_H
#define BFSALGO_H

#include "Algorithm.h"

#include <vector>

class BFSAlgo : public Algorithm
{
private:
    SearchState first;
    std::vector<std::vector<SearchState> > parent;
    std::vector<std::vector<bool> > visited;
public:
    void start() override;
    SearchState next() override;
    std::vector<SearchState> getSolution()  override;
    bool finished() override;
    std::deque<SearchState> getFringe();
};
#endif