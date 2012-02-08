#pragma once
#include "point.h"
#include "Actor.h"

class Star : public Actor {
public:
	Star(point* model);
	void draw();
	void update() {}
	point* model;
private:
	float colorDim[5][4];
	float radius[5];
public:
	static bool isPosColoringModeOn;
};
