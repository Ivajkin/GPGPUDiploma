#include "_StdAfx.h"
#include "Logic.h"
#include "AutoProcessProfiler.h"

Logic::Logic() {
	initGPGPU();
	for(int i = 0; i < 1024; ++i) {
		createStar();
	}
}

Logic::~Logic() {
	while(field.size()) {
		delete field.back();
		field.pop_back();
	}
}

void Logic::createStar() {
	field.push_back(new Star());
}

void Logic::update( /* seconds */ double elapsedTime) {
	
	static AutoProcessProfiler gravityComputationProcess("Gravity Computation Process");

	// cudaDeviceSynchronize()
	gravityComputationProcess.start_measurement();
	for(std::vector<Star*>::iterator i = field.begin(); i != field.end(); ++i) {
		for(std::vector<Star*>::iterator j = field.begin(); j != field.end(); ++j) {
			point& a = (*i)->model;
			a.acceleration = Vector3::Zero;
			if(i != j) {
				point& b = (*j)->model;
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
	for(std::vector<Star*>::iterator i = field.begin(); i != field.end(); ++i) {
		point& a = (*i)->model;
		a.velocity += a.acceleration * elapsedTime;
		a.position += a.velocity * elapsedTime;
	}
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