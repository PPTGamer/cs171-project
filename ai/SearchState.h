#ifndef SEARCHSTATE_H
#define SEARCHSTATE_H

#include<unordered_set>

struct SearchState{
    std::unordered_set<sf::Vector2i> keys;
    sf::Vector2i location;
    int cost;
    SearchState() : location(0, 0), cost(0){};
    SearchState(int x, int y) : location(x, y), cost(0){};

    bool operator== (const SearchState& lh, const SearchState& rh){
        return lh.location == rh.location and lh.keys == rh.keys;
    }

    bool operator< (const SearchState& lh, const SearchState& rh){
        return lh.cost > rh.cost;
    }
};

#endif