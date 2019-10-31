#ifndef SEARCHSTATE_H
#define SEARCHSTATE_H

#include<unordered_set>

struct SearchState{
    std::unordered_set<sf::Vector2i> keys;
    sf::Vector2i location;

    SearchState(int x, int y) : location(x, y){};

    bool operator== (const SearchState& lh, const SearchState& rh){
        return lh.location == rh.location and lh.keys == rh.keys;
    }
};

#endif