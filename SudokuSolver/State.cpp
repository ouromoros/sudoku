#include "State.h"
#include <stdlib.h>
#include <assert.h>
#include "util.h"

using std::optional;

State::State(const optional<Board> board) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			grids_[i][j] = 0b111111111;
		}
	}
	if (!board.has_value()) return;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if ((*board)[i][j]) {
				if (!_AddConstraint(i, j, (*board)[i][j])) {
					throw "Invalid board!";
				}
			}
		}
	}
	if (!valid()) {
		throw "Invalid board!";
	}
}

bool State::_AddConstraint(int row, int col, int n) {
	assert(n >= 1 && n <= 9);
	int flag = 1 << (n - 1);
	if (!(grids_[row][col] & flag)) {
		return false;
	}
	grids_[row][col] = flag;
	for (int i = 0; i < 9; i++) {
		if (i == row) continue;
		grids_[i][col] &= ~(flag);
	}
	for (int i = 0; i < 9; i++) {
		if (i == col) continue;
		grids_[row][i] &= ~(flag);
	}
	int row_start, col_start;
	row_start = row - row % 3;
	col_start = col - col % 3;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == row && j == col) continue;
			grids_[i][j] &= ~(flag);
		}
	}
}

bool State::valid() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (!grids_[i][j]) {
				return false;
			}
		}
	}
	return true;
}

optional<State> State::AddConstraint(int row, int col, int n) {
	State new_state;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			new_state.grids_[i][j] = grids_[i][j];
		}
	}
	if (!new_state._AddConstraint(row, col, n)) {
		return {};
	}
	if (!new_state.valid()) return {};
	return new_state;
}

Grids State::GetGrids() {
	return grids_;
}

bool State::IsComplete() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (NumOfSetBits(grids_[i][j]) != 1) {
				return false;
			}
		}
	}
	return true;
}


State::~State() {
}

void State::Print() {
}
