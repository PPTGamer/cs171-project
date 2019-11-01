#ifndef UCSALGO_H
#define UCSALGO_H

#include "Algorithm.h"

#include <vector>
#include <queue>

class UCSAlgo : public Algorithm
{
private:
    std::vector<std::vector<SearchState> > parent;
    std::priority_queue<SearchState> prio;
public:
    void start() override;
    SearchState next() override;
    std::vector<SearchState> getSolution() override;
    bool finished() override;
    std::priority_queue<SearchState> getFringe();
};
#endif