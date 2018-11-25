#pragma once
#include <array>
#include <optional>
typedef std::array <std::array<int, 3>, 3> Board;

class State
{
public:
	State(const std::optional<Board> board = {});
	~State();
	std::optional<State> AddConstraint(int row, int col, int n);
	void Print();
	bool IsComplete();
	int *GetGrids();
private:
	int grids_[9][9];
	void _AddConstraint(int row, int col, int n);
	bool valid();
};

