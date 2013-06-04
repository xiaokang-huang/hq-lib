#include <cppunit/extensions/HelperMacros.h>

class test_mesh : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( test_mesh );
		CPPUNIT_TEST( TC_01_01 );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
public:
	void TC_01_01();
};
