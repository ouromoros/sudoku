#pragma once
#include <vector>
#include "State.h"
class Solver
{
public:
	Solver(int num = 1, int board[9][9] = NULL);
	std::vector<int[9][9]> GetSolutions();
	~Solver();
private:
	std::vector<int[9][9]> solutions_;
	void BackTrack(State *s);
	int target;
};

