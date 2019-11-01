#ifndef SEARCHSTATE_H
#define SEARCHSTATE_H

#include<set>
#include<utility>
#include <SFML/Graphics.hpp>

struct SearchState{
    sf::Vector2i location;
    std::set<std::pair<int, int> > keys;
    int cost;
    SearchState() : location(0, 0), cost(0){};
    SearchState(int x, int y) : location(x, y), cost(0){};

    friend bool operator== (const SearchState lh, const SearchState rh){
        return lh.location == rh.location and lh.keys == rh.keys;
    }

    friend bool operator< (const SearchState lh, const SearchState rh){
        return lh.cost > rh.cost;
    }

    friend std::ostream& operator<< (std::ostream& stream, const SearchState& s){
        stream << '(' << s.location.x << ' ' << s.location.y << ')';
        stream << " Keys: ";
        for (auto key : s.keys){
            stream << key.first << ',' << key.second << ' ';
        }
        return stream;
    }
};

#endif