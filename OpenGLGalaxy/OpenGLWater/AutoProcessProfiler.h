#pragma once
#include <ctime>
#include <string>
#include <cstdlib>
#include <vector>

// Собирает данные о времени действия процесса,
// при уничтожении автоматически демонстрирует полученные данные.
class AutoProcessProfiler {
public:
	AutoProcessProfiler(char* name);
	~AutoProcessProfiler();
	void start_measurement();
	void end_measurement();
	void display_info();
	double get_cpu_time();
private:
	// Имя профилировщика, относится к замеряемому процессу.
	const std::string name;
	// Количество полных замеров.
	long iteration_count;
	// Общее время вычислений.
	clock_t cpu_time;
	// Время последнего замера, инициализируем нулём,
	// что послужит сигналом об ошибке в случае неверного порядка замеров.
	clock_t last_start_measure;
private:
	typedef std::vector<AutoProcessProfiler*> profiler_container_t;
	static profiler_container_t profilers;
};