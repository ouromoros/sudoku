#include "util.h"

int NumOfSetBits(int n) {
	int count = 0;
	while (n) {
		n &= (n - 1);
		count++;
	}
	return count;
}

int MsgBitPos(int n) {
	int count = 0;
	while (n) {
		n >>= 1;
		count++;
	}
	return count;
}
