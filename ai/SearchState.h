#ifndef SEARCHSTATE_H
#define SEARCHSTATE_H

using CoordType = sf::Vector2<unsigned int>;

struct SearchState{
    std::vector<CoordType> keys;
    CoordType location;

    SearchState(int x, int y, int num_keys = 0) : location(x, y), keys(numkeys){};    
};

#endif