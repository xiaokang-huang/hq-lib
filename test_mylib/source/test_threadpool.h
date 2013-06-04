#include <cppunit/extensions/HelperMacros.h>

class test_threadpool : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( test_threadpool );
		CPPUNIT_TEST( TC_01_01 );//test function run successfully
		CPPUNIT_TEST( TC_01_02 );//test all the function runs ok
		CPPUNIT_TEST( TC_01_03 );//test dependency

		CPPUNIT_TEST( TC_02_01 );//test group
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
public:
	void TC_01_01();
	void TC_01_02();
	void TC_01_03();
	void TC_02_01();
};
