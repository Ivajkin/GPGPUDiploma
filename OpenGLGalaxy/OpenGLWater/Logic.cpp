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
	// Коды ошибок.
	cl_int error = 0;
	// Платформа
	error = oclGetPlatformID(&platform);
	if (error != CL_SUCCESS) {
	   cout << "Ошика получения id платформы: " << errorMessage(error) << endl;
	   exit(error);
	}
	// Устройство
	error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if (err != CL_SUCCESS) {
	   cout << "Ошибка получения id устройств: " << errorMessage(error) << endl;
	   exit(error);
	}
	// Контекст
	context = clCreateContext(0, 1, &device, NULL, NULL, &error);
	if (error != CL_SUCCESS) {
	   cout << "Ошибка создания контекста: " << errorMessage(error) << endl;
	   exit(error);
	}
	// Очередь команд
	queue = clCreateCommandQueue(context, device, 0, &error);
	if (error != CL_SUCCESS) {
	   cout << "Ошибка создания очереди команд: " << errorMessage(error) << endl;
	   exit(error);
	}

	// Создаём программу.
	// Вспомогательные функции NVIDIA для получения содержимого файла и его размера.
	size_t src_size = 0;
	const char* path = shrFindFilePath("vector_add_gpu.cl", NULL);
	const char* source = oclLoadProgSource(path, "", &src_size);
	cl_program program = clCreateProgramWithSource(context, 1, &source, &src_size, &error);
	assert(error == CL_SUCCESS && "Не получилось создать программу");

	// Билдим программу.
	error = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	assert(error == CL_SUCCESS && "Не скомпилировалось");

	// Лог.
	char* build_log;
	size_t log_size;
	// Пробный вызов, для того чтобы узнать размер.
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
	build_log = new char[log_size+1];
	// Получаем теперь лог.
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
	build_log[log_size] = '\0';
	cout << build_log << endl;
	delete[] build_log;

	// Получаем kernel (программу на видеокарте (функцию)).
	kernel = clCreateKernel(program, "evaluate_gravity", &error);
	assert(error == CL_SUCCESS && "Не смог извлечь kernel");
#elif defined CPU_GPGPU
#else
	assert(false && "There is no valid processor type defined (CUDA_GPGPU, OPENCL_GPGPU or CPU_GPGPU)");
#endif
}

// 
void Logic::processGPGPU(double elapsedTime, point* points, const size_t point_count) {
#ifdef CUDA_GPGPU
#elif defined OPENCL_GPGPU
	const int mem_size = sizeof(point)*point_count;
	// Выделяем буффер и копируем в него.
	cl_mem src_d = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_size, src_a_h, &error);
	cl_mem res_d = clCreateBuffer(context, CL_MEM_WRITE_ONLY, mem_size, NULL, &error);
	
	// Собираем параметры, передаём их размер, а не размер того, на что они указывают.
	error = clSetKernelArg(kernel, 0, sizeof(double), &elapsedTime);
	error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &src_d);
	error |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &res_d);
	error |= clSetKernelArg(kernel, 3, sizeof(size_t), &point_count);
	assert(error == CL_SUCCESS && "Не получилось передать аргументы");

	// Запускаем kernel
	// Количество единиц работы на рабочую группу
	const size_t local_ws = 512;
	// shrRoundUp возвращает наименьшее число, большее чем размер, являющееся произведением local_ws на целое число.
	const size_t global_ws = shrRoundUp(local_ws, size);
	// Кол-во элементов работы.
	error = clEnqueueNDRangeKernel(queue, vector_add_k, 1, NULL, &global_ws, &local_ws, 0, NULL, NULL);
	assert(error == CL_SUCCESS && "Не получилось создать очередь работы");

	// Получаем результат, записываем обратно в points.
	clEnqueueReadBuffer(queue, res_d, CL_TRUE, 0, mem_size, points, 0, NULL, NULL);
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

#ifdef OPENCL_GPGPU
cl_platform_id Logic::platform;
cl_context Logic::context;
cl_command_queue Logic::queue;
cl_device_id Logic::device;
cl_kernel Logic::kernel;
#endif