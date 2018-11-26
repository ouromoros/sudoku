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
			if (!(*board)[i][j]) continue;
			if (!(grids_[i][j] & (1 << ((*board)[i][j] - 1)))) {
				throw "Invalid board!";
			}
			_AddConstraint(i, j, (*board)[i][j]);
		}
	}
	if (!valid()) {
		throw "Invalid board!";
	}
}

void State::_AddConstraint(int row, int col, int n) {
	assert(n >= 1 && n <= 9);
	int flag = 1 << (n - 1);
	assert(grids_[row][col] & flag);
//	if (!(grids_[row][col] & flag)) {
//		return false;
//	}
	grids_[row][col] = flag;
	for (int i = 0; i < 9; i++) {
		if (i == row) continue;
		int prev_bits = NumOfSetBits(grids_[i][col]);
		grids_[i][col] &= ~(flag);
		int after_bits = NumOfSetBits(grids_[i][col]);
		if (prev_bits > 1 && after_bits == 1) {
			_AddConstraint(i, col, MsgBitPos(grids_[i][col]));
		}
	}
	for (int i = 0; i < 9; i++) {
		if (i == col) continue;
		int prev_bits = NumOfSetBits(grids_[row][i]);
		grids_[row][i] &= ~(flag);
		int after_bits = NumOfSetBits(grids_[row][i]);
		if (prev_bits > 1 && after_bits == 1) {
			_AddConstraint(row, i, MsgBitPos(grids_[row][i]));
		}
	}
	int row_start, col_start;
	row_start = row - row % 3;
	col_start = col - col % 3;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (row_start + i == row && col_start + j == col) continue;
			int prev_bits = NumOfSetBits(grids_[row_start + i][col_start + i]);
			grids_[row_start + i][col_start + j] &= ~(flag);
			int after_bits = NumOfSetBits(grids_[row_start + i][col_start + i]);
			if (prev_bits > 1 && after_bits == 1) {
				_AddConstraint(row_start + i, col_start + i, MsgBitPos(grids_[row_start + i][col_start + i]));
			}
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
	if (!(grids_[row][col] & (1 << (n - 1)))) return {};
	State new_state;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			new_state.grids_[i][j] = grids_[i][j];
		}
	}
	new_state._AddConstraint(row, col, n);
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
