#pragma once
#include <assert.h>
#include <vector>

class UnitTest
{
	typedef std::vector<UnitTest*> container_t;
protected:
	UnitTest();
	static container_t unitTests;
public:
	virtual void run() = 0;
	static void Run();
};