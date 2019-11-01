#ifndef UCSALGO_H
#define UCSALGO_H

#include "Algorithm.h"

#include <vector>
#include <queue>

class UCSAlgo : public Algorithm
{
private:
    std::set<SearchState> prio;
    SearchState arrival;
public:
    void start() override;
    SearchState next() override;
    std::vector<SearchState> getSolution() override;
    std::deque<SearchState> getFringe() override;
    bool finished() override;
};
#endif