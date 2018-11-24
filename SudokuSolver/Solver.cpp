#include "Solver.h"
#include "util.h"

using std::vector;

Solver::Solver(int num = 1, const Board *board = NULL) {
	State *init_state = new State(board);
	target = num;
	BackTrack(init_state);
}

void Solver::BackTrack(State *s) {
	int *grids = s->GetGrids();
	if (s->IsComplete()) {
		Board board;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				board[i][j] = MsgBitPos(grids[i * 9 + j]);
			}
		}
		solutions_.push_back(board);
		return;
	}
	int min_pos = -1, min_count = 10;
	vector<int> a;
	for (int i = 0; i < 9 * 9; i++) {
		int count = NumOfSetBits(grids[i]);
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
		if (grids[min_pos] & (1 << i)) {
			int row = min_pos / 9, col = min_pos % 9;
			State *new_state = s->AddConstraint(row, col, i);
			if (new_state != NULL) {
				BackTrack(new_state);
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
