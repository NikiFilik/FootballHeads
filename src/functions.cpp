#include "functions.hpp"

namespace nf {
	int randIntFromRange(int a, int b) {
		return rand() % (b - a) + a;
	}

	float power(float a, int b) {
		float c = 1;
		for (int i = 1; i <= b; i++) {
			c *= a;
		}
		return c;
	}
}