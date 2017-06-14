#include "BufferingStateTests.h"

TEST_BufferingState testBufferingState;
	
TEST(BufferingStateTests)
	
{
	CHECK_EQUAL(185,testBufferingState.Span);
	CHECK_EQUAL(0,testBufferingState.testTypes);
	CHECK_EQUAL(0,testBufferingState.testResult_GradX);
	CHECK_EQUAL(0,testBufferingState.testResult_GradY);
	CHECK_EQUAL(0,testBufferingState.testResult_Mag);
	CHECK_EQUAL(0,testBufferingState.testResult_GradTan);
	
}