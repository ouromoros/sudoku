#include "pch.h"
#include <fstream>
#include <iostream>
#include "../SudokuSolver/Solver.h"
#include "../SudokuSolver/State.h"
#include "../SudokuSolver/Generator.h"
#include "../SudokuSolver/Solver.cpp"
#include "../SudokuSolver/State.cpp"
#include "../SudokuSolver/Generator.cpp"
#include "../SudokuSolver/util.cpp"

using namespace std;

const Board b1 = { {
	{4, 0, 0, 3, 0, 7, 0, 0, 0},
	{0, 0, 0, 1, 0, 0, 0, 0, 0},
	{2, 0, 0, 0, 0, 0, 0, 6, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 2, 0, 3},
	{0, 6, 5, 0, 8, 0, 0, 0, 0},
	{0, 0, 0, 0, 6, 0, 0, 8, 0},
	{0, 1, 0, 0, 0, 5, 0, 0, 0},
	{3, 0, 0, 0, 0, 0, 0, 0, 7}
} };

const Board s1 = { {
	{4, 8, 6, 3, 9, 7, 5, 2, 1},
	{5, 9, 7, 1, 2, 6, 4, 3, 8},
	{2, 3, 1, 4, 5, 8, 7, 6, 9},
	{1, 2, 3, 5, 7, 4, 8, 9, 6},
	{8, 7, 4, 6, 1, 9, 2, 5, 3},
	{9, 6, 5, 2, 8, 3, 1, 7, 4},
	{7, 4, 2, 9, 6, 1, 3, 8, 5},
	{6, 1, 8, 7, 3, 5, 9, 4, 2},
	{3, 5, 9, 8, 4, 2, 6, 1, 7}
} };

bool Valid(const Board &b);
bool Full(const Board &b);
bool Same(const Board &b1, const Board &b2);

TEST(StateTest, EmptyState) {
	State s;
	Grids g = s.GetGrids();
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			EXPECT_EQ(g[i][j], 0b111111111);
		}
	}
}

TEST(StateTest, InvalidStateBeNULL) {
	State s1(b1);
	ASSERT_FALSE(s1.GetGrids()[0][1] & 1);
	optional<State> s2 = s1.AddConstraint(0, 1, 1);
	EXPECT_FALSE(s2.has_value());
	ASSERT_TRUE(s1.GetGrids()[0][1] & (1 << 4));
	s2 = s1.AddConstraint(0, 1, 5);
	EXPECT_TRUE(s2.has_value());
}

TEST(SolverTest, RightSolution) {
	Solver sv1(b1, 2);
	vector<Board> sl1 = sv1.GetSolutions();
	EXPECT_EQ(sl1.size(), 1);
	EXPECT_TRUE(Full(sl1[0]));
	EXPECT_TRUE(Valid(sl1[0]));
	EXPECT_TRUE(Same(b1, sl1[0]));
	EXPECT_EQ(sl1[0], s1);
}

TEST(SolverTest, MultipleSolutions) {
	Board c1 = b1;
	c1[0][0] = 0;
	Solver sv1(c1, 2);
	vector<Board> sl1 = sv1.GetSolutions();
	EXPECT_GT(sl1.size(), 1);
}

TEST(GeneratorTest, RightBoards) {
	Generator g(10000);
	vector<Board> bs = g.GetBoards();
	EXPECT_EQ(bs.size(), 10000);
	for (int i = 0; i < 10000; i++) {
		EXPECT_TRUE(Full(bs[i]));
		EXPECT_TRUE(Valid(bs[i]));
		EXPECT_EQ(bs[i][0][0], 6);
	}
}

TEST(GeneratorTest, NonRepeatBoards) {
	Generator g(1000);
	vector<Board> bs = g.GetBoards();
	ASSERT_EQ(bs.size(), 1000);
	for (int i = 0; i < 1000; i++) {
		for (int j = i + 1; j < 1000; j++) {
			EXPECT_NE(bs[i], bs[j]);
		}
	}
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

bool Full(const Board &b) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (b[i][j] < 1 || b[i][j] > 9) {
				return false;
			}
		}
	}
	return true;
}

bool Valid(const Board &b) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (b[i][j] < 0 || b[i][j] > 9) {
				return false;
			}
		}
	}
	// distinct numbers in every row
	for (int i = 0; i < 9; i++) {
		array<bool, 9> fill{};
		for (int j = 0; j < 9; j++) {
			if (!b[i][j]) continue;
			if (fill[b[i][j] - 1]) {
				return false;
			}
			fill[b[i][j] - 1] = true;
		}
	}
	// distinct numbers in every column
	for (int i = 0; i < 9; i++) {
		array<bool, 9> fill{};
		for (int j = 0; j < 9; j++) {
			if (!b[i][j]) continue;
			if (fill[b[j][i] - 1]) {
				return false;
			}
			fill[b[j][i] - 1] = true;
		}
	}
	// distinct numbers in every 3x3 square
	for (int i = 0; i < 9; i+=3) {
		for (int j = 0; j < 9; j+=3) {
			array<bool, 9> fill{};
			for (int k = 0; k < 3; k++) {
				for (int l = 0; l < 3; l++) {
					int x = i + k;
					int y = j + l;
					if (!b[x][y]) continue;
					if (fill[b[x][y] - 1]) {
						return false;
					}
					fill[b[x][y] - 1] = true;
				}
			}
		}
	}
}

bool Same(const Board &b1, const Board &b2) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (!b1[i][j]) continue;
			if (b1[i][j] != b2[i][j]) return false;
		}
	}
	return true;
}
