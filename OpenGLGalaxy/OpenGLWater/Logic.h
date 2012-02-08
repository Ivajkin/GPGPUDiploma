#pragma once
#include "Star.h"
#include <vector>

class Logic {
public:
	Logic();
	~Logic();
	void createStar();
	void update( /* seconds */ double elapsedTime);
	static void initGPGPU();
	static void processGPGPU(double elapsedTime, point* points, const int point_count);
	static void disposeGPGPU();
private:
	std::vector<Star*> starField;
	static const int MAX_POINT_COUNT = 4096;
	point* pointField;
};