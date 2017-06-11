#include "TrackingStateTests.h"
TEST_TrackingState  testTrackingState;
TEST(TrackingStateTests)	
{
	
	
	CHECK_EQUAL(0,testTrackingState.testResult_ProbFrame1);
	CHECK_EQUAL(0,testTrackingState.testResult_ProbFrame2);
	CHECK_EQUAL(0,testTrackingState.testResult_ProbFrame3);

	CHECK_EQUAL(0,testTrackingState.testResult_GradTan1);
	CHECK_EQUAL(0,testTrackingState.testResult_GradTan2);
	CHECK_EQUAL(0,testTrackingState.testResult_GradTan3);
	
	
	CHECK_EQUAL(0,testTrackingState.testResult_MaxProbFocussed);
	CHECK_EQUAL(0,testTrackingState.testResult_GradTanFocussed);
	
	CHECK_EQUAL(0,testTrackingState.testResult_BaseBinIdx);
	
}