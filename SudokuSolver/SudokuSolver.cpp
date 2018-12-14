// SudokuSolver.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <vector>

#include "Generator.h"
#include "Solver.h"

using namespace std;

void PrintUsage();
void OutputBoard(ofstream &fout, const Board &b);

int main(int argc, char *argv[]) {
	if (argc != 3) {
		PrintUsage();
		return 0;
	}
	string option(argv[1]);
	if (option == "-c") {
		int n;
		try {
			n = stoi(argv[2]);
		}
		catch (invalid_argument e) {
			cout << "'" << argv[2] << "' is not a valid number!";
			return 0;
		}
		ofstream fout("sudoku.txt", ios::out | ios::trunc);
		if (!fout) {
			cout << "Failed opening file 'sudoku.txt' !" << endl;
			return 0;
		}
		Generator g(n);
		vector<Board> boards = g.GetBoards();
		for (int i = 0; i < n; i++) {
			OutputBoard(fout, boards[i]);
		}
	}
	else if (option == "-s") {
		ifstream fin(argv[2]);
		if (!fin) {
			cout << "Failed opening file '" << argv[2] << "' !" << endl;
			return 0;
		}
		ofstream fout("sudoku.txt", ios::out | ios::trunc);
		if (!fout) {
			cout << "Failed opening file 'sudoku.txt' !" << endl;
			return 0;
		}
		while (true) {
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
				cout << "No solution exists!" << endl;
				return 0;
			}
			Board solution = s.GetSolutions()[0];
			OutputBoard(fout, solution);
		}
	}
	else {
		PrintUsage();
		return 0;
	}

}

void PrintUsage() {
	cout << "Usage:" << endl;
	cout << "  ./sudoku.exe -c <number_of_solutions>" << endl;
	cout << "  ./sudoku.exe -s <path_of_input_file>" << endl;
}

void OutputBoard(ofstream &fout, const Board &b) {
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
	fout << s;
}

