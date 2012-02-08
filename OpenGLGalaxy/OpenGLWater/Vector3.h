#pragma once
#include "UnitTest.h"

class Vector3 {
public:
	Vector3(double x, double y, double z);
	Vector3 operator+(const Vector3& sec) const {
		return Vector3(x+sec.x, y+sec.y, z+sec.z);
	}
	Vector3 operator-(const Vector3& sec) const {
		return Vector3(x-sec.x, y-sec.y, z-sec.z);
	}
	double operator*(const Vector3& sec) const {
		return x*sec.x + y*sec.y + z*sec.z;
	}
	Vector3 operator*(const double scalar) const {
		return Vector3(x*scalar, y*scalar, z*scalar);
	}
	Vector3 operator/(const double scalar) const {
		return *this*(1/scalar);
	}
	Vector3& operator+=(const Vector3& sec) {
		x += sec.x;
		y += sec.y;
		z += sec.z;
		return *this;
	}
	Vector3& operator-=(const Vector3& sec) {
		x -= sec.x;
		y -= sec.y;
		z -= sec.z;
		return *this;
	}
	bool operator==(const Vector3& sec) {
		return abs(x - sec.x) < epsilon && abs(y - sec.y) < epsilon && abs(z - sec.z) < epsilon;
	}
	bool operator!=(const Vector3& sec) {
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
	const static Vector3 Zero;
private:
	double x, y, z;
private:
	static const double epsilon;
};

class Vector3UnitTest : public UnitTest {
	void run();
};

double random();