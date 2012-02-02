#pragma once
#include <ctime>
#include <string>
#include <cstdlib>
#include <vector>

// �������� ������ � ������� �������� ��������,
// ��� ����������� ������������� ������������� ���������� ������.
class AutoProcessProfiler {
public:
	AutoProcessProfiler(char* name);
	~AutoProcessProfiler();
	void start_measurement();
	void end_measurement();
	void display_info();
	double get_cpu_time();
private:
	// ��� ��������������, ��������� � ����������� ��������.
	const std::string name;
	// ���������� ������ �������.
	long iteration_count;
	// ����� ����� ����������.
	clock_t cpu_time;
	// ����� ���������� ������, �������������� ����,
	// ��� �������� �������� �� ������ � ������ ��������� ������� �������.
	clock_t last_start_measure;
private:
	typedef std::vector<AutoProcessProfiler*> profiler_container_t;
	static profiler_container_t profilers;
};