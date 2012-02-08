#include "_StdAfx.h"
#include "Logic.h"
#include "AutoProcessProfiler.h"

Logic::Logic() {
	initGPGPU();
	pointField = new point[MAX_POINT_COUNT];
	for(int i = 0; i < 1024; ++i) {
		createStar();
	}
}

Logic::~Logic() {
	while(starField.size()) {
		delete starField.back();
		starField.pop_back();
	}
	delete [] pointField;
	disposeGPGPU();
}

void Logic::createStar() {
	if(MAX_POINT_COUNT > starField.size()) {
		starField.push_back(new Star(pointField + (starField.size())));
	}
}

void Logic::update( /* seconds */ double elapsedTime) {

	//processGPGPU~~
	static AutoProcessProfiler gravityComputationProcess("Gravity Computation Process");

	// cudaDeviceSynchronize()
	gravityComputationProcess.start_measurement();
	processGPGPU(elapsedTime, pointField, starField.size());
	// cudaDeviceSynchronize()
	gravityComputationProcess.end_measurement();
}

void Logic::initGPGPU() {
#ifdef CUDA_GPGPU
#elif defined OPENCL_GPGPU
#elif defined CPU_GPGPU
#else
	assert(false && "There is no valid processor type defined (CUDA_GPGPU, OPENCL_GPGPU or CPU_GPGPU)");
#endif
}

// 
void Logic::processGPGPU(double elapsedTime, point* points, const int point_count) {
#ifdef CUDA_GPGPU
#elif defined OPENCL_GPGPU
#elif defined CPU_GPGPU
	for(int i = 0; i < point_count; ++i) {
		for(int j = 0; j < point_count; ++j) {
			point& a = points[i];
			a.acceleration = Vector3::Zero;
			if(i != j) {
				point& b = points[j];
				Vector3 range = b.position - a.position;
				double squaredRangeLength = range.squaredLength();
				// Космическая пыль.
				a.velocity = a.velocity * pow(0.997, elapsedTime);
				// Сталкиваются ли объекты.
				if(squaredRangeLength > 0.005) {
					// m*M/r^2
					a.acceleration += range * a.mass * b.mass / (squaredRangeLength*sqrt(squaredRangeLength)) * 20;
					// Тёмная энергия для стабилизации сгустков.
					//a.acceleration -= range * a.mass * b.mass * squaredRangeLength * 0.01;
				} else {
					// Неупругий удар (притормаживаем).
					a.velocity = a.velocity * pow(0.9, elapsedTime);
				}
			}
		}
	}
	for(int i = 0; i < point_count; ++i) {
		point& a = points[i];
		a.velocity += a.acceleration * elapsedTime;
		a.position += a.velocity * elapsedTime;
	}
#else
	assert(false && "There is no valid processor type defined (CUDA_GPGPU, OPENCL_GPGPU or CPU_GPGPU)");
#endif
}

void Logic::disposeGPGPU() {
#ifdef CUDA_GPGPU
#elif defined OPENCL_GPGPU
#elif defined CPU_GPGPU
#else
	assert(false && "There is no valid processor type defined (CUDA_GPGPU, OPENCL_GPGPU or CPU_GPGPU)");
#endif
}