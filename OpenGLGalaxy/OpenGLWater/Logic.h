#pragma once
#include "Star.h"
#include <vector>

class Logic {
public:
	Logic();
	~Logic();
	void createStar();
	void update( /* seconds */ double elapsedTime);
	void initGPGPU();
	void processGPGPU(double* mass, Vector3 position,
			velocity,
			acceleration;
private:
	std::vector<Star*> field;
};