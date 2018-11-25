#pragma once
#include <vector>
#include <optional>
#include "State.h"

class Solver
{
public:
	Solver(const std::optional<Board> board, int num = 1);
	std::vector<Board> GetSolutions();
	~Solver();
private:
	std::vector<Board> solutions_;
	void BackTrack(State s);
	int target;
};

