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

	Mat comparison;
	int testResult_HistogramBins;  
	int testResult_prior=-1;
	int testResult_filter=-1;
	int STEP =-1;
	int BASE_HISTOGRAM_MODELS_SIZE =-1;

	  
	TEST_LaneFilter()
	{

		Camera 			 camera;
		LaneParameters   lane;
			
		LaneFilter laneFilter(lane, camera);
		
		Mat exp_BINS_HISTOGRAM, exp_LanePrior, exp_LaneFilter;
		Mat BINS_HISTOGRAM;

		STEP	= laneFilter.STEP;
		
		cv::eigen2cv(laneFilter.HISTOGRAM_BINS, BINS_HISTOGRAM);
		BASE_HISTOGRAM_MODELS_SIZE = laneFilter.baseHistogramModels.size();

		exp_BINS_HISTOGRAM = loadCSV("BASE_HISTOGRAM_BINS.csv", CV_32SC1);
		cv::compare(exp_BINS_HISTOGRAM, BINS_HISTOGRAM, comparison, cv::CMP_NE);
		testResult_HistogramBins = cv::countNonZero(comparison);
		
		exp_LanePrior = loadCSV("LANE_PRIOR.csv", CV_32SC1);
		cv::compare(exp_LanePrior, laneFilter.prior, comparison, cv::CMP_NE);
		testResult_prior = cv::countNonZero(comparison);

		exp_LaneFilter = loadCSV("LANE_FILTER.csv", CV_32SC1);
		cv::compare(exp_LaneFilter, laneFilter.filter, comparison, cv::CMP_NE);
		testResult_filter = cv::countNonZero(comparison);
		
	
		
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
		 

		 CHECK_EQUAL(0,testLaneFilter.testResult_HistogramBins);
		 CHECK_EQUAL(0,testLaneFilter.testResult_prior);		 
		 CHECK_EQUAL(0,testLaneFilter.testResult_filter);
		 
		
		 
    }

