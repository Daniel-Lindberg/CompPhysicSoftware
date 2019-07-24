#include "fd.cpp"
#include <assert.h> 
using namespace std;

#define EPSILON 0.01

bool areSame(double a, double b)
{
	return fabs(a - b) < EPSILON;
}

int test1(){
	/*
		This first test is to check to make sure the size
		of the phi is correct.
	*/

	LaplacianOnGrid *test1 = new LaplacianOnGrid(10,10, "test1" );
	test1->FormLS();
	test1->GaussSeidel(200, test1->b , test1->phi );
	//Given that we created our LaplacianOnGrid on a 10 x 10 array
	// It should create a vector table of 122 values ([10]+1) x ([10]+1) +1
	assert(test1->phi.size() == 122);
	//Clean up the memory afterwards
	delete test1;
	//Do another size check for sanity
	LaplacianOnGrid *test1_1 = new LaplacianOnGrid(5,5, "test1-1" );
	test1_1->FormLS();
	test1_1->GaussSeidel(200, test1_1->b , test1_1->phi );
	//Given that we created our LaplacianOnGrid on a 10 x 10 array
	// It should create a vector table of 37 values ([5]+1) x ([5]+1) +1
	assert(test1_1->phi.size() == 37);
	//Clean up the memory afterwards
	delete test1_1;
	return 0;
}

int test2()
{

	/*
		This second test, is to ensure values are valid
	*/

	LaplacianOnGrid *test2 = new LaplacianOnGrid(10,10, "test2" );
	test2->FormLS();
	test2->GaussSeidel(100, test2->b , test2->phi );
	//These values are worked out on paper, and should result to
	//the following values
	assert(areSame(test2->phi[50] , 0.168253));
	assert(areSame(test2->phi[100] , 1.00000));
	assert(areSame(test2->phi[20] , 0.396360));
	//Clean up the memory afterwards
	delete test2;
	return 0;
}
int test3()
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
	assert(exception_thrown);
	return 0;
}

int manufacturedMethod()
{
	/*
		The manufactured method is to obtain the manufactured
		method of solutions for the finite difference code.
	*/
	// My manufactured solution is going to be 10 x 10 array
	// with max iterations of 100

	LaplacianOnGrid *manufactured = new LaplacianOnGrid(10,10, "manufactured" );
	manufactured->FormLS();
	manufactured->GaussSeidel(100, manufactured->b , manufactured->phi );
	// Used to generate the manufactured solution
	//manufactured->plot("manufactured_phi", manufactured->phi);
	//LaplacianOnGrid *manufactured_copy = 

	//Clean up the memory afterwards
	delete manufactured;
	return 0;

} 

int RunTests()
{
	printf("Running test 1\n");
	test1();
	printf("Finished test 1\n");	
	printf("Running test 2\n");
	test2();
	printf("Finished test 2\n");
	printf("Running test 3\n");
	test3();
	printf("Finished test 3\n");	
	manufacturedMethod();
	return 0;
}

int main()
{
	RunTests();
}
