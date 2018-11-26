#include "Solver.h"
#include "util.h"

using std::vector;
using std::optional;

Solver::Solver(const std::optional<Board> board, int num) {
	State init_state = State(board);
	target = num;
	BackTrack(init_state);
}

void Solver::BackTrack(State s) {
	Grids grids = s.GetGrids();
	if (s.IsComplete()) {
		Board board;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				board[i][j] = MsgBitPos(grids[i][j]);
			}
		}
		solutions_.push_back(board);
		return;
	}
	int min_pos = -1, min_count = 10;
	vector<int> a;
	for (int i = 0; i < 9 * 9; i++) {
		int count = NumOfSetBits(grids[i / 9][i % 9]);
		if (count == 1) continue;
		if (count < min_count) {
			min_count = count;
			min_pos = i;
		}
		if (count == 2) {
			break;
		}
	}
	for (int i = 0; i < 9; i++) {
		if (grids[min_pos / 9][min_pos % 9] & (1 << i)) {
			int row = min_pos / 9, col = min_pos % 9;
			optional<State> new_state = s.AddConstraint(row, col, i + 1);
			if (new_state.has_value()) {
				BackTrack(*new_state);
				if (solutions_.size() == target) {
					return;
				}
			}
		}
	}
}

vector<Board> Solver::GetSolutions() {
	return solutions_;
}


Solver::~Solver() {
}
