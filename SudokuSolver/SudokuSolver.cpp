// SudokuSolver.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <exception>
#include <vector>

#include "Generator.h"
#include "Solver.h"

using namespace std;

void PrintUsage();
void OutputBoard(FILE *fout, const Board &b);

int main(int argc, char *argv[]) {
	// 检查参数是否完整
	if (argc != 3) {
		PrintUsage();
		return 0;
	}
	string option(argv[1]);
	if (option == "-c") {
		// 尝试解析参数
		int n;
		try {
			n = stoi(argv[2]);
		}
		catch (invalid_argument e) {
			cout << "'" << argv[2] << "' is not a valid number!";
			return 0;
		}
		FILE *fout;
		fopen_s(&fout, "sudoku.txt", "w");
		if (fout == NULL) {
			cout << "Failed opening file 'sudoku.txt' !" << endl;
			return 0;
		}
		// 调用Generator
		Generator g(n);
		vector<Board> boards = g.GetBoards();
		for (int i = 0; i < n; i++) {
			OutputBoard(fout, boards[i]);
		}
		fclose(fout);
	}
	else if (option == "-s") {
		ifstream fin(argv[2]);
		if (!fin) {
			cout << "Failed opening file '" << argv[2] << "' !" << endl;
			return 0;
		}
		FILE *fout;
		fopen_s(&fout, "sudoku.txt", "w");
		if (fout == NULL) {
			cout << "Failed opening file 'sudoku.txt' !" << endl;
			return 0;
		}
		while (true) {
			// 不停读取题目并求解
			Board b;
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					fin >> b[i][j];
					if (fin.eof()) {
						cout << "Solved all puzzles" << endl;
						return 0;
					}
					if (b[i][j] < 0 || b[i][j] > 9 || fin.fail()) {
						cout << "Invalid Sudoku problem!" << endl;
						return 0;
					}
				}
			}
			Solver s(b);
			if (s.GetSolutions().empty()) {
				// 该题目没有可行解，输出错误
				cout << "No solution exists!" << endl;
				return 0;
			}
			Board solution = s.GetSolutions()[0];
			// 打印解到文件
			OutputBoard(fout, solution);
		}
		fclose(fout);
	}
	else {
		// 错误参数，推出
		PrintUsage();
		return 0;
	}

}

void PrintUsage() {
	cout << "Usage:" << endl;
	cout << "  ./sudoku.exe -c <number_of_solutions>" << endl;
	cout << "  ./sudoku.exe -s <path_of_input_file>" << endl;
}

void OutputBoard(FILE* fout, const Board &b) {
	// 输出局面到文件fout
	char s[200];
	int si = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			s[si++] = b[i][j];
			if (j != 8) {
				s[si++] = ' ';
			}
		}
		s[si++] = '\n';
	}
	s[si++] = '\n';
	s[si] = '\0';
	fputs(s, fout);
}

