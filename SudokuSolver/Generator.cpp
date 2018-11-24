#include "Generator.h"
#include "Solver.h"

using std::vector;

Generator::Generator(int n) {
	this->n = n;
}
vector<Board> Generator::GetBoards() {
	Board b{};
	b[0][0] = 7;
	Solver s(n, &b);
	return s.GetSolutions();
}


Generator::~Generator() {
}
