#include <Algorithm.h>
#include <SearchState.h>
#include <Maze>

class BFSAlgo : public Algorithm
{
    SearchState next(){
        int dx[4] = {0, -1, 0, 1};
		int dy[4] = {-1, 0, 1, 0};
        SearchState s = fringe.front(); fringe.pop_front();
        for (int i = 0; i < 4; ++i){
            int nx = s.location.x + dx[i], ny = s.location.y + dy[i];
            if (this->maze(nx, ny) != maze.EntryType.WALL){
                SearchState t(nx, ny);
                fringe.push_back(t);
            }
        }
    }

    bool finished(){
        SearchState test = fringe.front();
        //if (test.location.x == end.x  and test.location.y == end.y)
        //    check if he has all the keys
        //else
            return false;
    }

};