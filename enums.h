#ifndef ENUMS_H
#define ENUMS_H

enum GameState : int
{
	SET_ALGORITHM, SET_POSITION, RUNNING, PAUSED, END
};

enum AlgorithmType : int
{
	BFS, UCS
};

#endif