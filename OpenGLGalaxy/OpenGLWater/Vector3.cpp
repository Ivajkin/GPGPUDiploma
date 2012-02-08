#include "_stdafx.h"
#include "Vector3.h"

Vector3::Vector3(double x, double y, double z)
	: x(x), y(y), z(z) {
}

double Vector3::getX() const {
	return x;
}

double Vector3::getY() const {
	return y;
}

double Vector3::getZ() const {
	return z;
}

const Vector3 Vector3::Zero(0,0,0);

const double Vector3::epsilon = 0.0000000001;


void Vector3UnitTest::run() {
	Vector3 c = Vector3(3,4,1) - Vector3(2,-1,1);
	assert(c == Vector3(1,5,0));
	c = Vector3(2.1,-3.25,1) + Vector3(1,2,1);
	assert(c == Vector3(3.1,-1.25,2));
	c = Vector3(2.1,-3.251,1) + Vector3(1,2,1);
	assert(c != Vector3(3.1,1.25,2));
	double scalar = Vector3(3,4,1) * Vector3(2.2,-100,1);
	assert(scalar == -392.4);
	scalar = Vector3(8,6,0).squaredLength();
	assert(scalar == 100);
	c = Vector3(-1,2,1) * 2;
	assert(c == Vector3(-2,4,2));
	c = Vector3(-5,10,25) / 5;
	assert(c == Vector3(-1,2,5));
}
Vector3UnitTest vector2UnitTest;



#define _USE_MATH_DEFINES
#include <cmath>
double random() {
	return pow(rand()%100000 / 100000.0, 1.0/2.87);
}