#ifndef SEARCHSTATE_H
#define SEARCHSTATE_H

#include<set>
#include <SFML/Graphics.hpp>

struct SearchState{
    sf::Vector2i location;
    std::set<std::pair<int,int>> keys;
    int cost;
    SearchState() : location(0, 0), cost(0){};
    SearchState(int x, int y) : location(x, y), cost(0){};

    friend bool operator== (const SearchState& lh, const SearchState& rh){
        bool res = true;
        res = res && (lh.location == rh.location);
        for (auto x : lh.keys)
        {
            res = res && (rh.keys.find(x) != rh.keys.end());
        }
        for (auto x : rh.keys)
        {
            res = res && (lh.keys.find(x) != lh.keys.end());
        }
        return res;
    }

    friend bool operator< (const SearchState& lh, const SearchState& rh){
        return lh.cost > rh.cost;
    }
};

#endif