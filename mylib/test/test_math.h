#include <cppunit/extensions/HelperMacros.h>

class test_math : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( test_math );
		CPPUNIT_TEST( TC_01_01 );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
public:
	// test vertex
	void TC_01_01();
};
