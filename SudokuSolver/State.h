#pragma once
class State
{
public:
	State(int board[9][9] = NULL);
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

