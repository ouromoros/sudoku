#pragma once
#include <array>
#include <optional>
typedef std::array <std::array<int, 9>, 9> Board;
typedef std::array <std::array<int, 9>, 9> Grids;

class State
{
public:
	State(const std::optional<Board> board = {});
	~State();
	std::optional<State> AddConstraint(int row, int col, int n);
	void Print();
	bool IsComplete();
	Grids GetGrids();
private:
	Grids grids_;
	bool _AddConstraint(int row, int col, int n);
	bool valid();
};

