#pragma once
#include <bitset>
typedef std::bitset<9> Grid;
class State
{
public:
	State(int board[9][9] = NULL);
	~State();
	State *AddConstraint(int row, int col, int n);
	void Print();
	bool IsComplete();
	Grid *GetGrids();
private:
	Grid grids_[9][9];
	bool _AddConstraint(int row, int col, int n);
	bool valid();
};

