#ifndef MANHATTAN_HEURISTIC_H
#define MANHATTAN_HEURISTIC_H

#include "UCSAlgo.h"

#include <vector>
#include <set>
#include <map>

class ManhattanHeuristic: public UCSAlgo
{
private:
    int heuristic_function(SearchState s);
    int manhattan (sf::Vector2i a, sf::Vector2i b);
    int dist_to_keys(const SearchState& s);
    std::map<SearchState, int> gScore;
    std::map<SearchState, int> fScore;
public:
    void start() override;
    SearchState next() override;
    std::vector<SearchState> getSolution()  override;
    bool finished() override;
    std::deque<SearchState> getFringe() override;
};
#endif