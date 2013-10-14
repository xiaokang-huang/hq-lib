#include "test_math.h"

#include <math/hq_math.h>

CPPUNIT_TEST_SUITE_REGISTRATION( test_math );

void test_math::setUp() {
}

void test_math::tearDown() {
}

void test_math::TC_01_01() {
	hq_vertex4 a, b, c;
	REAL ret;

	hq_setvertex(&a, 1, 2, 4, 1);
	hq_setvertex(&b, 5, 1, 6, 1);
	hq_setvertex(&c, 0, 0, 0, 1);

	hq_dot(&ret, a, b);
	CPPUNIT_ASSERT(REAL_EQUAL(ret, 31));
}