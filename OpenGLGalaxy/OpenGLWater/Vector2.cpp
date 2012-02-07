#include "_stdafx.h"
#include "Vector2.h"

Vector2::Vector2(double x, double y, double z)
	: x(x), y(y), z(z) {
}

double Vector2::getX() const {
	return x;
}

double Vector2::getY() const {
	return y;
}

double Vector2::getZ() const {
	return z;
}

const Vector2 Vector2::Zero(0,0,0);

const double Vector2::epsilon = 0.0000000001;


void Vector2UnitTest::run() {
	Vector2 c = Vector2(3,4,1) - Vector2(2,-1,1);
	assert(c == Vector2(1,5,0));
	c = Vector2(2.1,-3.25,1) + Vector2(1,2,1);
	assert(c == Vector2(3.1,-1.25,2));
	c = Vector2(2.1,-3.251,1) + Vector2(1,2,1);
	assert(c != Vector2(3.1,1.25,2));
	double scalar = Vector2(3,4,1) * Vector2(2.2,-100,1);
	assert(scalar == -392.4);
	scalar = Vector2(8,6,0).squaredLength();
	assert(scalar == 100);
	c = Vector2(-1,2,1) * 2;
	assert(c == Vector2(-2,4,2));
	c = Vector2(-5,10,25) / 5;
	assert(c == Vector2(-1,2,5));
}
Vector2UnitTest vector2UnitTest;