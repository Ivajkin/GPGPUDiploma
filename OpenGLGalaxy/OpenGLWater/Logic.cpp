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
	disposeGPGPU();
}

void Logic::createStar() {
	field.push_back(new Star());
}

void Logic::update( /* seconds */ double elapsedTime) {
	// Буфферы для хранения сконвертированных данных.
	const int count = field.size();
	double* mass = new double[count];
	Vector3* position = new Vector3[count];
	Vector3* velocity = new Vector3[count];
	Vector3* acceleration = new Vector3[count];

	static AutoProcessProfiler dataConvertingProcess("Data Converting Process");
	dataConvertingProcess.start_measurement();
	{
		for(int i = 0; i < count; ++i) {
			mass[i] = field[i]->model.mass;
			position[i] = field[i]->model.position;
			velocity[i] = field[i]->model.velocity;
			acceleration[i] = field[i]->model.acceleration;
		}
	}
	dataConvertingProcess.end_measurement();

	//processGPGPU~~
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


	dataConvertingProcess.start_measurement();
	{
		for(int i = 0; i < count; ++i) {
			field[i]->model.position = position[i];
			field[i]->model.velocity = velocity[i];
			field[i]->model.acceleration = acceleration[i];
		}
	}
	dataConvertingProcess.end_measurement();
	
	delete [] mass;
	delete [] position;
	delete [] velocity;
	delete [] acceleration;
}

void Logic::initGPGPU() {
#ifdef CUDA_GPGPU
#elif defined OPENCL_GPGPU
#elif defined CPU_GPGPU
#else
	assert(false && "There is no valid processor type defined (CUDA_GPGPU, OPENCL_GPGPU or CPU_GPGPU)");
#endif
}

void Logic::processGPGPU(double* mass, Vector3* position, Vector3* velocity, Vector3* acceleration, const int count) {
#ifdef CUDA_GPGPU
#elif defined OPENCL_GPGPU
#elif defined CPU_GPGPU
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