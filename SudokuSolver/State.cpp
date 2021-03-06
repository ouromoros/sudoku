#include "State.h"
#include <stdlib.h>
#include <assert.h>
#include "util.h"

using std::optional;

State::State(const optional<Board> board, bool puzzle_mode) {
	// 初始化grids_
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			grids_[i][j] = 0b111111111;
		}
	}
	if (!board.has_value()) return;
	// 根据初始局面进行更改
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (!(*board)[i][j]) continue;
			if (!(grids_[i][j] & (1 << ((*board)[i][j] - 1)))) {
				// 若初始局面无效，则抛出异常
				throw "Invalid board!";
			}
			_AddConstraint(i, j, (*board)[i][j]);
		}
	}
	puzzle_mode_ = puzzle_mode;
	if (!valid()) {
		throw "Invalid board!";
	}
}

void State::_AddConstraint(int row, int col, int n) {
	// 增加一个Constraint
	assert(n >= 1 && n <= 9);
	int flag = 1 << (n - 1);
	assert(grids_[row][col] & flag);
	grids_[row][col] = flag;
	// 更改对应行上格子的状态
	for (int i = 0; i < 9; i++) {
		if (i == row) continue;
		int prev = grids_[i][col];
		grids_[i][col] &= ~(flag);
		int after_bits = NumOfSetBits(grids_[i][col]);
		if (prev != grids_[i][col] && after_bits == 1) {
			_AddConstraint(i, col, MsgBitPos(grids_[i][col]));
		}
	}
	// 更改对应列上格子的状态
	for (int i = 0; i < 9; i++) {
		if (i == col) continue;
		int prev = grids_[row][i];
		grids_[row][i] &= ~(flag);
		int after_bits = NumOfSetBits(grids_[row][i]);
		if (prev != grids_[row][i] && after_bits == 1) {
			_AddConstraint(row, i, MsgBitPos(grids_[row][i]));
		}
	}
	// 更改对应九宫格中格子的状态
	int row_start, col_start;
	row_start = row - row % 3;
	col_start = col - col % 3;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (row_start + i == row && col_start + j == col) continue;
			int prev = grids_[row_start + i][col_start + j];
			grids_[row_start + i][col_start + j] &= ~(flag);
			int after_bits = NumOfSetBits(grids_[row_start + i][col_start + i]);
			if (prev != grids_[row_start + i][col_start + j] && after_bits == 1) {
				_AddConstraint(row_start + i, col_start + i, MsgBitPos(grids_[row_start + i][col_start + i]));
			}
		}
	}
}


bool State::TryAddMoreConstraint() {
	// 尝试根据规则推导出更多Constraint
	for (int i = 0; i < 9; i++) {
		int pos_x[9];
		int pos_y[9];
		int count[9] = {0,0,0,0,0,0,0,0,0};
		for (int j = 0; j < 9; j++) {
			for (int k = 0; k < 9; k++) {
				if (grids_[i][j] & (1 << k)) {
					count[k]++;
					pos_x[k] = i;
					pos_y[k] = j;
				}
			}
		}
		for (int k = 0; k < 9; k++) {
			if (count[k] == 1 && NumOfSetBits(grids_[pos_x[k]][pos_y[k]]) != 1) {
				_AddConstraint(pos_x[k], pos_y[k], k + 1);
				return true;
			}
		}
	}
	for (int i = 0; i < 9; i++) {
		int pos_x[9];
		int pos_y[9];
		int count[9] = {0,0,0,0,0,0,0,0,0};
		for (int j = 0; j < 9; j++) {
			for (int k = 0; k < 9; k++) {
				if (grids_[j][i] & (1 << k)) {
					count[k]++;
					pos_x[k] = j;
					pos_y[k] = i;
				}
			}
		}
		for (int k = 0; k < 9; k++) {
			if (count[k] == 1 && NumOfSetBits(grids_[pos_x[k]][pos_y[k]]) != 1) {
				_AddConstraint(pos_x[k], pos_y[k], k + 1);
				return true;
			}
		}
	}
	for (int i = 0; i < 9; i+=3) {
		for (int j = 0; j < 9; j+=3) {
			int pos_x[9];
			int pos_y[9];
			int count[9] = {0,0,0,0,0,0,0,0,0};
			for (int k = 0; k < 3; k++) {
				for (int l = 0; l < 3; l++) {
					int x = i + k;
					int y = j + l;
					for (int m = 0; m < 9; m++) {
						if (grids_[x][y] & (1 << m)) {
							count[m]++;
							pos_x[m] = x;
							pos_y[m] = y;
						}
					}
				}
			}
			for (int k = 0; k < 9; k++) {
				if (count[k] == 1 && NumOfSetBits(grids_[pos_x[k]][pos_y[k]]) != 1) {
					_AddConstraint(pos_x[k], pos_y[k], k + 1);
					return true;
				}
			}
		}
	}
	return false;
}

bool State::valid() {
	// 当前局面是否是一个有效的局面
	// 查看是否有格子中没有可以填入的数字
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (!grids_[i][j]) return false;
		}
	}
	// 检查每行都能填入所有数字
	for (int i = 0; i < 9; i++) {
		int p = 0;
		for (int j = 0; j < 9; j++) {
			p |= grids_[j][i];
		}
		if (p != 0b111111111) return false;
	}
	// 检查每列都能填入所有数字
	for (int i = 0; i < 9; i++) {
		int p = 0;
		for (int j = 0; j < 9; j++) {
			p |= grids_[i][j];
		}
		if (p != 0b111111111) return false;
	}
	// 检查每个九宫格中都能填入所有数字
	for (int i = 0; i < 9; i += 3) {
		for (int j = 0; j < 9; j += 3) {
			int p = 0;
			for (int k = 0; k < 3; k++) {
				for (int l = 0; l < 3; l++) {
					int x = i + k;
					int y = j + l;
					p |= grids_[x][y];
				}
			}
			if (p != 0b111111111) return false;
		}
	}
	return true;
}

optional<State> State::AddConstraint(int row, int col, int n) {
	// 返回增加一个Constraint得到的局面
	if (!(grids_[row][col] & (1 << (n - 1)))) return {};
	State new_state;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			new_state.grids_[i][j] = grids_[i][j];
		}
	}
	new_state.puzzle_mode_ = puzzle_mode_;
	new_state._AddConstraint(row, col, n);
	if (puzzle_mode_) {
		while(new_state.TryAddMoreConstraint());
	}
	if (!new_state.valid()) return {};
	return new_state;
}

Grids State::GetGrids() {
	// 返回内部的状态
	return grids_;
}

bool State::IsComplete() {
	// 是否所有数字都已经填入
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
