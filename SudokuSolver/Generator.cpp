#include "Generator.h"
#include "Solver.h"

using std::vector;

Generator::Generator(int n) {
	this->n = n;
}
vector<Board> Generator::GetBoards() {
	Board b{};
	b[0][0] = (4 + 1) % 9 + 1;
	Solver s(b, n, false);
	return s.GetSolutions();
}


Generator::~Generator() {
}
