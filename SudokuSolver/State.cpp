#include "State.h"
#include <stdlib.h>
#include <assert.h>
#include "util.h"


State::State(const Board *board = NULL) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			grids_[i][j] = 0b111111111;
		}
	}
	if (board == NULL) return;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if ((*board)[i][j]) {
				_AddConstraint(i, j, (*board)[i][j]);
			}
		}
	}
	if (!valid()) {
		throw "Invalid board!";
	}
}

void State::_AddConstraint(int row, int col, int n) {
	assert(n >= 0 && n < 9);
	int flag = 1 << n;
	assert(grids_[row][col] & flag);
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
			if (grids_[i][j]) {
				return false;
			}
		}
	}
	return true;
}

State *State::AddConstraint(int row, int col, int n) {
	State *new_state = new State;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			new_state->grids_[i][j] = grids_[i][j];
		}
	}
	new_state->_AddConstraint(row, col, n);
	if (!new_state->valid()) return NULL;
	return new_state;
}

int *State::GetGrids() {
	return (int*)grids_;
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
