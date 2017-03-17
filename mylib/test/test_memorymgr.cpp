#include "test_memorymgr.h"
CPPUNIT_TEST_SUITE_REGISTRATION( test_memorymgr );

#include <hq_memory_mgr.h>
#include <stdio.h>

void test_memorymgr::setUp() {
}

void test_memorymgr::tearDown() {
}

class test_class : public MemoryAllocatorBase {
public:
	int d1;
	int d2;
	test_class():d1(0),d2(1) {}
};

#define TRACER0 0
void test_memorymgr::TC_01_01() {
	INT32 alloced = MemoryAllocatorBase::GetSizeAllocated(TRACER0);
	CPPUNIT_ASSERT_EQUAL((INT32)0, alloced);
	test_class* ptr = new(TRACER0) test_class;
	//CPPUNIT_ASSERT_EQUAL(HQRESULT_SUCCESS, rc);
	delete ptr;
}
