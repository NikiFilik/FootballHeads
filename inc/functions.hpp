#pragma once

#include <cmath>

namespace nf {
	int randIntFromRange(int a, int b);

	float power(float a, int b);

	float distanceBetween2Points(float x1, float y1, float x2, float y2);

	void findCoordinatesOfUnitNormalVector(float x1, float y1, float x2, float y2, float& unitNormalVectorX, float& unitNormalVectorY);

	void solveOneDimensionalCollision(float mass1, float mass2, float speed1, float speed2, float& newSpeed1, float& newSpeed2);
}

/*#pragma once

#include <iostream>

namespace nf {
	int randIntFromRange(int a, int b);
	float power(float a, int b);
}*/