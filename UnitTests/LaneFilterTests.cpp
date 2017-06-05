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
	  
	  Mat comparisonPrior;
	  int testResult_prior;
      
	  Mat comparisonFilter;
	  int testResult_filter;
	  
	  Mat comparisonTransition;
	  int testResult_transition;
	

	  unique_ptr<LaneFilter> laneFilter;
	  
	  TEST_LaneFilter()
		{

			Camera camera;
			Lane   lane;
			
			laneFilter.reset(new LaneFilter(lane, camera));
			exp_BINS_HISTOGRAM = readCSV("LANE_BINS_H.csv", 153);
			
			Mat exp_LanePrior = loadCSV("LANE_PRIOR.csv", CV_32SC1);
			cv::compare(exp_LanePrior, laneFilter->prior, comparisonPrior, cv::CMP_NE);
			testResult_prior = cv::countNonZero(comparisonPrior);
			
			//saveMatToCsv(exp_LanePrior, "LANE_PRIOR.csv");
			//saveMatToCsv(laneFilter->prior, "LANE_PRIOR2.csv");

			Mat exp_LaneFilter = loadCSV("LANE_FILTER.csv", CV_32SC1);
			cv::compare(exp_LaneFilter, laneFilter->filter, comparisonFilter, cv::CMP_NE);
			testResult_filter = cv::countNonZero(comparisonFilter);
			
			
			Mat exp_LaneTransition = loadCSV("LANE_TRANSITION.csv", CV_32SC1);
			//saveMatToCsv(exp_LaneTransition, "LANE_TRANSITION.csv");
			cv::compare(exp_LaneTransition, laneFilter->transition, comparisonTransition, cv::CMP_NE);
			testResult_transition = cv::countNonZero(comparisonTransition);
		
		}
		
		~TEST_LaneFilter()
		{
			
		}			
};


TEST_LaneFilter testLaneFilter;

TEST(LaneFilter)

    {
				
		 CHECK_EQUAL(10, testLaneFilter.laneFilter->STEP) ;
		 
		 
		 CHECK_EQUAL(2221, testLaneFilter.laneFilter->baseHistogramModels.size()) ;
		 
		 CHECK_ARRAY_CLOSE(testLaneFilter.exp_BINS_HISTOGRAM.data(), 
						   testLaneFilter.laneFilter->HISTOGRAM_BINS.data(),
						   testLaneFilter.exp_BINS_HISTOGRAM.size(), 1.0e-4);
		 
		 CHECK_EQUAL(0,testLaneFilter.testResult_prior);
		 
		 CHECK_EQUAL(0,testLaneFilter.testResult_filter);
		 
		CHECK_EQUAL(0,testLaneFilter.testResult_transition);
		 
    }

