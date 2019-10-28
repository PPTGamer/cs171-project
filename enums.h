#ifndef ENUMS_H
#define ENUMS_H

enum GameState : int
{
	SET_POSITION, SET_ALGORITHM, RUNNING, PAUSED
};

enum AlgorithmType : int
{
	BFS, UCS
};

#endif