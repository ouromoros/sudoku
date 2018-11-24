#pragma once
#include <vector>
#include <array>
#include "State.h"
class Solver
{
public:
	Solver(int num = 1, const Board *board = NULL);
	std::vector<Board> GetSolutions();
	~Solver();
private:
	std::vector<Board> solutions_;
	void BackTrack(State *s);
	int target;
};

