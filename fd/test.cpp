// Generic cpp includes
// To compile we need to link -lcppunit
#include "fd.cpp"
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <assert.h> 
#include <sys/stat.h>

//This is our EPSILON for the areSame function
#define EPSILON 0.01

//We are using the CppUnit namespace and std
using namespace CppUnit;
using namespace std;
	 
//-----------------------------------------------------------------------------
// Will be our class tester, inherits from TestFixtures	 
class TestFd : public CppUnit::TestFixture
{
	//Create our test suite
    CPPUNIT_TEST_SUITE(TestFd);
    //Populate it with tests
    CPPUNIT_TEST(testGridSize);
    CPPUNIT_TEST(testValidData);
    CPPUNIT_TEST(testErrorHandling);
    CPPUNIT_TEST(testGaussSeidel);
    CPPUNIT_TEST(testFormLS);
    CPPUNIT_TEST(testPlotter);
    //Close our test suite
    CPPUNIT_TEST_SUITE_END();

	public:
		//We are inheriting these from test fixtures
		//creating our own method of them
        void setUp(void);
        void tearDown(void);

 
	protected:
    	void testGridSize(void);
    	void testValidData(void);
    	void testErrorHandling(void);
    	void testGaussSeidel(void);
    	void testFormLS();
    	void testPlotter();
	 
	private:

	//Create this object every time, declare it in the setUp()
	LaplacianOnGrid *mTestObj;

};

bool areSame(double a, double b)
{
	// Helps determine if two values are similar, checks difference is less than epsilon
	return fabs(a - b) < EPSILON;
}
inline bool fileExists (const std::string& filename) {
  struct stat buffer;   
  return (stat (filename.c_str(), &buffer) == 0); 
}
void TestFd::setUp(void)
{
	//Is our inherited setup function, we set up this object for all tests
   mTestObj = new LaplacianOnGrid(10,10, "test" );
}

void TestFd::tearDown(void)
{
   // Delete the object after the test is written
   delete mTestObj;
}

void TestFd::testGridSize(void){
	/*
		This first test is to check to make sure the size
		of the phi is correct.
	*/
	mTestObj->FormLS();
	mTestObj->GaussSeidel(200, mTestObj->b , mTestObj->phi );
	//Given that we created our LaplacianOnGrid on a 10 x 10 array
	// It should create a vector table of 122 values ([10]+1) x ([10]+1) +1
	CPPUNIT_ASSERT(mTestObj->phi.size() == 122);
	//Do another size check for sanity
	LaplacianOnGrid *test1_1 = new LaplacianOnGrid(5,5, "test1-1" );
	test1_1->FormLS();
	test1_1->GaussSeidel(200, test1_1->b , test1_1->phi );
	//Given that we created our LaplacianOnGrid on a 10 x 10 array
	// It should create a vector table of 37 values ([5]+1) x ([5]+1) +1
	CPPUNIT_ASSERT(test1_1->phi.size() == 37);
	//Clean up the memory afterwards
	delete test1_1;
}

void TestFd::testFormLS(void)
{

	/*
		This  test, is to ensure we can call the formLs option
	*/
	bool no_exception_thrown = true;
	LaplacianOnGrid *test3= new LaplacianOnGrid(2, 2, "testLS");
	try 
	{
		//Call LS , make sure not exception is thrown
		test3->FormLS();
	}
	catch(std::exception& e)
	{
		//Make sure exception is thrown
		no_exception_thrown = false;
	}
	CPPUNIT_ASSERT(no_exception_thrown);
	delete test3;
}

void TestFd::testGaussSeidel(void)
{

	/*
		This test, is to ensure Gauss Seidel works
	*/
	bool no_exception_thrown = true;
	try 
	{
		//Call LS , make sure not exception is thrown
		mTestObj->GaussSeidel(100, mTestObj->b , mTestObj->phi );
	}
	catch(std::exception& e)
	{
		//Make sure exception is thrown
		no_exception_thrown = false;
	}
	CPPUNIT_ASSERT(no_exception_thrown);
}

void TestFd::testValidData(void)
{

	/*
		This second test, is to ensure values are valid
	*/
	mTestObj->FormLS();
	mTestObj->GaussSeidel(100, mTestObj->b , mTestObj->phi );
	//These values are worked out on paper, and should result to
	//the following values
	CPPUNIT_ASSERT(areSame(mTestObj->phi[50], 0.999132));
	CPPUNIT_ASSERT(areSame(mTestObj->phi[100], 1.00000));
	CPPUNIT_ASSERT(areSame(mTestObj->phi[20], 0.999834));
}
void TestFd::testErrorHandling(void)
{
	/*
		This third test, is to ensure proper error handling
		happens
	*/
	//First error handle the constructor
	bool exception_thrown = false;
	LaplacianOnGrid *test3;
	try 
	{
		//Call the constructor with bad arguments
		test3 = new LaplacianOnGrid(-2, 2, "test3");
	}
	catch(std::exception& e)
	{
		//Make sure exception is thrown
		exception_thrown = true;
	}
	//asert that this exception is thrown
	CPPUNIT_ASSERT(exception_thrown);
	delete test3;
}
void TestFd::testPlotter(void)
{
	/*
		This third test, is to ensure proper error handling
		happens
	*/
	//First error handle the constructor
	mTestObj->FormLS();
	mTestObj->GaussSeidel(100, mTestObj->b, mTestObj->phi);
	bool no_exception_thrown = true;
	try 
	{
		mTestObj->plot("phi", mTestObj->phi);
		mTestObj->plot("b", mTestObj->b);
		//mTestObj->plot("A", mTestObj->A);
	}
	catch(std::exception& e)
	{
		//Make sure exception is thrown
		no_exception_thrown = false;
	}
	CPPUNIT_ASSERT(!fileExists("helloworld.txt"));
	CPPUNIT_ASSERT(fileExists("test_phi.plt"));
	CPPUNIT_ASSERT(fileExists("test_b.plt"));
	//asert that this exception is thrown
	CPPUNIT_ASSERT(no_exception_thrown);
}


CPPUNIT_TEST_SUITE_REGISTRATION( TestFd );
int main(int argc, char* argv[])
{
	CPPUNIT_NS::TestResult testresult;

	// register listener for collecting the test-results

    CPPUNIT_NS::TestResultCollector collectedresults;

    testresult.addListener (&collectedresults);
    // register listener for per-test progress output

    CPPUNIT_NS::BriefTestProgressListener progress;

    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry

    CPPUNIT_NS::TestRunner testrunner;

    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());

    testrunner.run(testresult);

    // output results in compiler-format

    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // Output XML for CPPunit plugin

    ofstream xmlFileOut("cppTestFd.xml");

    XmlOutputter xmlOut(&collectedresults, xmlFileOut);

    xmlOut.write();
    // return 0 if tests were successful
	return collectedresults.wasSuccessful() ? 0 : 1;
}
