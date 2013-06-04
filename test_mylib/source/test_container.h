#include <cppunit/extensions/HelperMacros.h>

class test_container : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( test_container );
		CPPUNIT_TEST( TC_01_01 );
		CPPUNIT_TEST( TC_01_02 );
		CPPUNIT_TEST( TC_01_03 );

		CPPUNIT_TEST( TC_02_01 );

		CPPUNIT_TEST( TC_03_01 );

		CPPUNIT_TEST( TC_04_01 );
		CPPUNIT_TEST( TC_04_02 );
		CPPUNIT_TEST( TC_04_03 );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
public:
	// test tree node
	void TC_01_01();
	void TC_01_02();
	void TC_01_03();
	// test quad tree
	void TC_02_01();
	// test binary tree
	void TC_03_01();
	// test concurrent ring queue
	void TC_04_01();
	void TC_04_02();
	void TC_04_03();

};
