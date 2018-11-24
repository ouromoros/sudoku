#pragma once
#include "Solver.h"
class Generator
{
public:
	Generator(int n);
	~Generator();
	std::vector<Board> GetBoards();
private:
	int n;
};

