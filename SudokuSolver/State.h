#pragma once
#include <array>
typedef std::array <std::array<int, 3>, 3> Board;
class State
{
public:
	State(const Board *board = NULL);
	~State();
	State *AddConstraint(int row, int col, int n);
	void Print();
	bool IsComplete();
	int *GetGrids();
private:
	int grids_[9][9];
	void _AddConstraint(int row, int col, int n);
	bool valid();
};

