#pragma once
#include "UnitTest.h"

class Vector2 {
public:
	Vector2(double x, double y, double z);
	Vector2 operator+(const Vector2& sec) const {
		return Vector2(x+sec.x, y+sec.y, z+sec.z);
	}
	Vector2 operator-(const Vector2& sec) const {
		return Vector2(x-sec.x, y-sec.y, z-sec.z);
	}
	double operator*(const Vector2& sec) const {
		return x*sec.x + y*sec.y + z*sec.z;
	}
	Vector2 operator*(const double scalar) const {
		return Vector2(x*scalar, y*scalar, z*scalar);
	}
	Vector2 operator/(const double scalar) const {
		return *this*(1/scalar);
	}
	Vector2& operator+=(const Vector2& sec) {
		x += sec.x;
		y += sec.y;
		z += sec.z;
		return *this;
	}
	bool operator==(const Vector2& sec) {
		return abs(x - sec.x) < epsilon && abs(y - sec.y) < epsilon && abs(z - sec.z) < epsilon;
	}
	bool operator!=(const Vector2& sec) {
		return !(*this == sec);
	}
	double squaredLength() {
		return x*x+y*y+z*z;
	}
public:
	double getX() const;
	double getY() const;
	double getZ() const;
public:
	const static Vector2 Zero;
private:
	double x, y, z;
private:
	static const double epsilon;
};

class Vector2UnitTest : public UnitTest {
	void run();
};