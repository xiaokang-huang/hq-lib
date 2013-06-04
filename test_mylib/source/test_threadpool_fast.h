#include <cppunit/extensions/HelperMacros.h>

class test_threadpool_fast : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( test_threadpool_fast );
		CPPUNIT_TEST( TC_01_01 );//test function run successfully
		CPPUNIT_TEST( TC_01_02 );
		CPPUNIT_TEST( TC_01_03 );
		CPPUNIT_TEST( TC_01_04 );
		CPPUNIT_TEST( TC_01_05 );
		CPPUNIT_TEST( TC_01_06 );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
public:
	void TC_01_01();
	void TC_01_02();
	void TC_01_03();
	void TC_01_04();
	void TC_01_05();
	void TC_01_06();
};
