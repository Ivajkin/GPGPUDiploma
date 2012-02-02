#include "StdAfx.h"
#include "AutoProcessProfiler.h"
#include <assert.h>

//	Возвращает прошедшее время в секундах.
//	Как получить текущее время:
//		clock_t start = clock();
double difftime(const clock_t start, const clock_t end) {
	double cpu_time = static_cast<double>( end - start ) / CLOCKS_PER_SEC;
	return cpu_time;
}

AutoProcessProfiler::AutoProcessProfiler(char* name) : name(name), iteration_count(0), cpu_time(0), last_start_measure(-1) {
	profilers.push_back(this);
}
AutoProcessProfiler::~AutoProcessProfiler() {
	display_info();
	system("pause");
}
void AutoProcessProfiler::start_measurement() {
	assert(last_start_measure == -1 && "Видимо порядок измерений не тот");
	last_start_measure = clock();
}
void AutoProcessProfiler::end_measurement() {
	const clock_t end_measurement = clock();
	assert(last_start_measure != -1 && "Видимо порядок измерений не тот");
	++iteration_count;
	double elapsed = end_measurement - last_start_measure;
	cpu_time += elapsed;

	last_start_measure = -1;
}
void AutoProcessProfiler::display_info() {
	assert(iteration_count != 0 && "Ни одного измерения не было произведено");
	double cpu_time_seconds = get_cpu_time();
	double average_time_seconds = cpu_time_seconds / (double)(this->iteration_count);
	
	// Здесь нет смысла экономить такты процессора, дублирую вычисления для удобства применения и mantainability.
	double full_time_of_all_the_profilers = 0;
	for(profiler_container_t::iterator i = profilers.begin(); i != profilers.end(); ++i) {
		full_time_of_all_the_profilers += (*i)->get_cpu_time();
	}
	double percentage_time = cpu_time_seconds / full_time_of_all_the_profilers * 100;
	printf("\nProcess named \"%s\": time = %f, iteration count = %d, average time = %f, time percentage = %4.2f%%\n", this->name.c_str(), (float)cpu_time_seconds, this->iteration_count, (float)average_time_seconds, (float)percentage_time);
}
double AutoProcessProfiler::get_cpu_time() {
	return difftime(0, this->cpu_time);
}
AutoProcessProfiler::profiler_container_t AutoProcessProfiler::profilers;