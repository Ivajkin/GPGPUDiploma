#pragma once
#include "Vector2.h"

struct point {
	point();
	const double mass;
	Vector2 position,
			velocity,
			acceleration;
private:
	static Vector2 emitter;
};