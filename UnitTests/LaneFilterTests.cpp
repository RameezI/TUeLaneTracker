#include "UnitTest++/UnitTest++.h"
#include  "Tests.h"
#include  "Camera.h"
#include  "Lane.h"
#include  "LaneFilter.h"
#include  "VanishingPtFilter.h"

#define UNIT_TESTING

using namespace Eigen;
using namespace std;

class TEST_LaneFilter:Tests
{
public:
	
	
	
	VectorXd exp_BINS_HISTOGRAM;
	VectorXi BINS_HISTOGRAM;
	
	Mat comparison;
	  
	int testResult_prior=-1;
	int testResult_filter=-1;
	int STEP =-1;
	int BASE_HISTOGRAM_MODELS_SIZE =-1;
	int testResult_NegLaneModels=-1;

	  
	TEST_LaneFilter()
	{

		Camera camera;
		Lane   lane;
			
		LaneFilter laneFilter(lane, camera);
		
		Mat exp_LanePrior, exp_LaneFilter;
		Mat M_NegLane_0, M_NegLane_12, M_NegLane_567, M_NegLane_2220, M_NegLane_2219, M_NegLane_222;
		Mat exp_M_NegLane_0, exp_M_NegLane_12, exp_M_NegLane_567, exp_M_NegLane_2220, exp_M_NegLane_2219, exp_M_NegLane_222;
		
		
		exp_BINS_HISTOGRAM = readCSV("LANE_BINS_H.csv", 153);
		
		exp_LanePrior = loadCSV("LANE_PRIOR.csv", CV_32SC1);
		cv::compare(exp_LanePrior, laneFilter.prior, comparison, cv::CMP_NE);
		testResult_prior = cv::countNonZero(comparison);

		exp_LaneFilter = loadCSV("LANE_FILTER.csv", CV_32SC1);
		cv::compare(exp_LaneFilter, laneFilter.filter, comparison, cv::CMP_NE);
		testResult_filter = cv::countNonZero(comparison);
		
		STEP= laneFilter.STEP;
		BINS_HISTOGRAM = laneFilter.HISTOGRAM_BINS;
		BASE_HISTOGRAM_MODELS_SIZE = laneFilter.baseHistogramModels.size();
		
		//Check Models NegLane
		M_NegLane_0   	= Mat (laneFilter.baseHistogramModels[0].binIDs_NegBoundary);
		M_NegLane_12  	= Mat (laneFilter.baseHistogramModels[12].binIDs_NegBoundary);
		M_NegLane_567 	= Mat (laneFilter.baseHistogramModels[567].binIDs_NegBoundary);
		M_NegLane_2220	= Mat (laneFilter.baseHistogramModels[2220].binIDs_NegBoundary);
		M_NegLane_2219	= Mat (laneFilter.baseHistogramModels[2219].binIDs_NegBoundary);
		M_NegLane_222	= Mat (laneFilter.baseHistogramModels[222].binIDs_NegBoundary);
		
		
		exp_M_NegLane_0 = loadCSV("NegLaneModel_0.csv", CV_32SC1);		
		cv::compare(exp_M_NegLane_0, M_NegLane_0, comparison, cv::CMP_NE);
		testResult_NegLaneModels = cv::countNonZero(comparison);
		
		exp_M_NegLane_12 = loadCSV("NegLaneModel_12.csv", CV_32SC1);
		cv::compare(exp_M_NegLane_12, M_NegLane_12, comparison, cv::CMP_NE);
		testResult_NegLaneModels += cv::countNonZero(comparison);
		
		exp_M_NegLane_567 = loadCSV("NegLaneModel_567.csv", CV_32SC1);
		cv::compare(exp_M_NegLane_567, M_NegLane_567, comparison, cv::CMP_NE);
		testResult_NegLaneModels += cv::countNonZero(comparison);
		
		exp_M_NegLane_2220 = loadCSV("NegLaneModel_2220.csv", CV_32SC1);
		cv::compare(exp_M_NegLane_2220, M_NegLane_2220, comparison, cv::CMP_NE);
		testResult_NegLaneModels += cv::countNonZero(comparison);
		
		exp_M_NegLane_2219 = loadCSV("NegLaneModel_2219.csv", CV_32SC1);
		cv::compare(exp_M_NegLane_2219, M_NegLane_2219, comparison, cv::CMP_NE);
		testResult_NegLaneModels += cv::countNonZero(comparison);
		
		exp_M_NegLane_222 = loadCSV("NegLaneModel_222.csv", CV_32SC1);
		cv::compare(exp_M_NegLane_222, M_NegLane_222, comparison, cv::CMP_NE);
		testResult_NegLaneModels += cv::countNonZero(comparison);
		
		
		
	
		
				
		
		
		
		}
		
		~TEST_LaneFilter()
		{
			
		}			
};


TEST_LaneFilter testLaneFilter;

TEST(LaneFilter)

    {
				
		 CHECK_EQUAL(10, testLaneFilter.STEP) ;
		 CHECK_EQUAL(2221, testLaneFilter.BASE_HISTOGRAM_MODELS_SIZE) ;
		 
		 CHECK_ARRAY_CLOSE(testLaneFilter.exp_BINS_HISTOGRAM.data(), 
						   testLaneFilter.BINS_HISTOGRAM.data(),
						   testLaneFilter.exp_BINS_HISTOGRAM.size(), 1.0e-4);
		 
		 CHECK_EQUAL(0,testLaneFilter.testResult_prior);		 
		 CHECK_EQUAL(0,testLaneFilter.testResult_filter);
		 
		 CHECK_EQUAL(0, testLaneFilter.testResult_NegLaneModels);
		 
    }

