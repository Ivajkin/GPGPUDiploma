#pragma once
#include "Vector3.h"

struct point {
	point();
	const double mass;
	Vector3 position,
			velocity,
			acceleration;
private:
	static Vector3 emitter;
	static Vector3 velocity_emitter;
};