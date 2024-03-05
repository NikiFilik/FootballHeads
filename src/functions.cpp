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

	float distanceBetween2Points(float x1, float y1, float x2, float y2) {
		return std::sqrt(nf::power(x1 - x2, 2) + nf::power(y1 - y2, 2));
	}

	void findCoordinatesOfUnitNormalVector(float x1, float y1, float x2, float y2, float& unitNormalVectorX, float& unitNormalVectorY) {
		unitNormalVectorX = (x2 - x1) / std::sqrt(power(x2 - x1, 2) + power(y2 - y1, 2));
		unitNormalVectorY = (y2 - y1) / std::sqrt(power(x2 - x1, 2) + power(y2 - y1, 2));
	}

	void solveOneDimensionalCollision(float mass1, float mass2, float speed1, float speed2, float& newSpeed1, float& newSpeed2) {
		newSpeed1 = (speed1 * (mass1 - mass2) + 2 * mass2 * speed2) / (mass1 + mass2);
		newSpeed2 = (speed2 * (mass2 - mass1) + 2 * mass1 * speed1) / (mass1 + mass2);
	}
}