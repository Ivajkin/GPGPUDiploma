#pragma once
#include "Star.h"
#include <vector>

// All OpenCL headers
#if defined (__APPLE__) || defined(MACOSX)
    #include <OpenCL/opencl.h>
#else
    #include <CL/opencl.h>
#endif 


class Logic {
public:
	Logic();
	~Logic();
	void createStar();
	void update( /* seconds */ double elapsedTime);
	static void initGPGPU();
	static void processGPGPU(double elapsedTime, point* points, const size_t point_count);
	static void disposeGPGPU();
private:
	std::vector<Star*> starField;
	static const int MAX_POINT_COUNT = 4096;
	point* pointField;

#ifdef OPENCL_GPGPU
	static cl_platform_id platform;
	static cl_context context;
	static cl_command_queue queue;
	static cl_device_id device;
	static cl_kernel kernel;
#endif
};