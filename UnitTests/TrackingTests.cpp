#include "TrackingTests.h"
TEST_TrackingState  testTrackingState;

TEST(BlurFrame)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_BlurFrame);
}

TEST(GradientTanTemplate)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_GradTangentTemplate);
}

TEST(FocusTemplate)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_FocusTemplate);
}


TEST(DepthTemplate)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_DepthTemplate);
}

TEST(GradientX)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_GradX);
}

TEST(GradientY)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_GradY);
}

TEST(GradientMagnitude)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_Mag);
}

TEST(GradientTangent)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_GradTan);
}

TEST(GrayProbability)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_ProbGray);
}

TEST(GradientMagProbability)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_ProbMag);
}

TEST(GradientDirProbability)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_ProbDir);
}


TEST(TotalProbability)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_ProbFrame);
}
	


TEST(FocussedProbabiity)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_MaxProbFocussed);
}

TEST(FocussedGradTangent)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_GradTanFocussed);
}


TEST(IntBins_Base)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_BaseBinIdx);
}


TEST(IntBins_Purveiw)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_PurviewBinIdx);
}

TEST(Int_Weights)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_WeightsBin);	
}

TEST(HistogramBase)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_HistBase);	
}

TEST(HistogramPurview)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_HistBase);	
}

TEST(TransitionedLaneFilter)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_TransitionedFilterLane);	
}

TEST(TransitionedVPFilter)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_TransitionedFilterVP);
}


TEST(UpdatedLaneFilter)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_UpdatedFilterLane);	
}

TEST(UpdatedVPFilter)	
{	
	CHECK_EQUAL(0,testTrackingState.testResult_UpdatedFilterVP);
}


TEST(LANE)	
{
	CHECK_EQUAL(0,testTrackingState.testResult_laneModel);	
}

TEST(VP)	
{	
	CHECK_EQUAL(0,testTrackingState.testResult_vp);
}
	