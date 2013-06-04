#include <cppunit/extensions/HelperMacros.h>

class test_thread : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( test_thread );
		//test thread
		CPPUNIT_TEST( TC_01_01 );
		//test mutex
		CPPUNIT_TEST( TC_02_01 );
		CPPUNIT_TEST( TC_02_02 );
		CPPUNIT_TEST( TC_02_03 );
		//test spinlock
		CPPUNIT_TEST( TC_03_01 );
		//test semaphore
		CPPUNIT_TEST( TC_04_01 );
		//test event
		CPPUNIT_TEST( TC_05_01 );
		CPPUNIT_TEST( TC_05_02 );
		//test atomic
		CPPUNIT_TEST( TC_06_01 );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
public:
	void TC_01_01();
	void TC_02_01();
	void TC_02_02();
	void TC_02_03();
	void TC_03_01();
	void TC_04_01();
	void TC_05_01();
	void TC_05_02();
	void TC_06_01();
};
