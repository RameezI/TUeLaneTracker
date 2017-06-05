#include "BootingStateTests.h"
TEST_InitState testBooting;
	
	TEST(BootingTests)
	{
			CHECK_EQUAL(0,testBooting.testResult_prior);
		 
			CHECK_EQUAL(0,testBooting.testResult_filter);
		 
			CHECK_EQUAL(0,testBooting.testResult_transition);
			
			
			CHECK_EQUAL(0,testBooting.testResult_GradTangentRoot);
		 
			CHECK_EQUAL(0,testBooting.testResult_DepthRoot);
		 
			CHECK_EQUAL(0,testBooting.testResult_FocusRoot);

	}