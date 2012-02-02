#include "StdAfx.h"
#include "UnitTest.h"

UnitTest::UnitTest() {
	unitTests.push_back(this);
}

void UnitTest::Run() {
	for(UnitTest::container_t::iterator i = unitTests.begin(); i != unitTests.end(); ++i) {
		(*i)->run();
	}
}

UnitTest::container_t UnitTest::unitTests;