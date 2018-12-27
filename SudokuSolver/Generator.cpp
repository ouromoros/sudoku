#include "Generator.h"
#include "Solver.h"

using std::vector;

Generator::Generator(int n) {
	// 创建函数，初始化
	this->n = n;
}
vector<Board> Generator::GetBoards() {
	// 调用Solver得到局面
	Board b{};
	b[0][0] = (4 + 1) % 9 + 1;
	Solver s(b, n, false);
	return s.GetSolutions();
}


Generator::~Generator() {
}
