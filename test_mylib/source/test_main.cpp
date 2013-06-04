#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

#include <iostream>
#include <stdexcept>

CPPUNIT_NS::Test* find_test(const char* pname, CPPUNIT_NS::Test* all_test) {
	/*CPPUNIT_NS::Test* test;
	try {
		test = all_test->findTest(pname);
	} catch (std::invalid_argument &ia) {
		std::cerr<<" Invalid_argument "<< ia.what()<<std::endl;
	}*/
	int count = all_test->getChildTestCount();
	CPPUNIT_NS::Test* test = NULL;
	std::string c(pname);
	for (int i = 0; i < count; ++i) {
		std::string name = all_test->getChildTestAt(i)->getName();
		if (c == name) {
			test = all_test->getChildTestAt(i);
			break;
		}
	}
	return test;
}

int main(int argc, char *argv[]) {
	// Create the event manager and test controller
	CPPUNIT_NS::TestResult controller;

	// Add a listener that colllects test result
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );

	// Add a listener that print dots as test run.
	CPPUNIT_NS::BriefTestProgressListener progress;
	controller.addListener( &progress );

	// Add the top suite to the test runner
	CPPUNIT_NS::TestRunner runner;
	CPPUNIT_NS::Test* all_test = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();
	CPPUNIT_NS::Test* test = NULL;
	if (argc <= 1) {
		runner.addTest(all_test);
		runner.run( controller );
	} else {
		for (int i = 1; i < argc; ++i) {
			test = find_test(argv[i], all_test);
			if (test)
				test->run(&controller);
		}
		delete all_test;
	}


	// Print test in a compiler compatible format.
	CPPUNIT_NS::CompilerOutputter outputter( &result, CPPUNIT_NS::stdCOut() );
	outputter.write();

	return result.wasSuccessful() ? 0 : 1;
}
