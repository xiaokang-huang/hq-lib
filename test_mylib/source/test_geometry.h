#include <cppunit/extensions/HelperMacros.h>

class test_geometry : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( test_geometry );
		CPPUNIT_TEST( TC_01_01 );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
public:
	// test tree node
	void TC_01_01();

};
