#ifndef MANHATTAN_HEURISTIC_H
#define MANHATTAN_HEURISTIC_H

#include "Algorithm.h"

#include <vector>
#include <set>
#

class BFSAlgo : public Algorithm
{
public:
    void start() override;
    SearchState next() override;
    std::vector<SearchState> getSolution()  override;
    bool finished() override;
    std::deque<SearchState> getFringe() override;
private:
    int manhattan (sf::Vector2i a, sf::Vector2i b);
    int dist_to_keys();
};
#endif