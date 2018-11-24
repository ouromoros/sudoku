#pragma once
class State
{
public:
	State();
	State(int board[9][9]);
	~State();
	State AddConstraint(int row, int col, int n);
	void Print();
	bool IsComplete();
	int *GetState();
private:
	int grids_[9][9];
	bool _AddConstraint(int row, int col, int n);
};

