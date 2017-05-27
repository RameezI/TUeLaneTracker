#include "UnitTest++/UnitTest++.h"
#include  "Tests.h"
#include  "Camera.h"
#include  "Lane.h"
#include  "LaneFilter.h"
#include  "VanishingPtFilter.h"

using namespace Eigen;
using namespace std;



class TEST_VPFilter:Tests
{
public:
	  Mat exp_vpFilter;
	  Mat exp_vpPrior;
	  Mat exp_vpTransition;
	  
	  MatrixXd exp_BINS_HISTOGRAM;
	  
	  Mat comparisonPrior;
	  int testResult_prior;
	  
	  Mat comparisonFilter;
	  int testResult_filter;
	  
	  
	   Mat comparisonTransition;
	   int testResult_transition;
	  
	  unique_ptr<VanishingPtFilter> 	vpFilter;
	   
	  TEST_VPFilter()		 
		{
			
			Camera 	   camera;
			Lane       lane;
			LaneFilter laneFilter(lane, camera);
			
			vpFilter.reset(new VanishingPtFilter(laneFilter.HISTOGRAM_BINS, laneFilter.OFFSET_V, camera));
			
			exp_BINS_HISTOGRAM 	= readCSV("VP_BINS_HST.csv",153);
			
			
			exp_vpPrior 		= loadCSV("VP_PRIOR.csv", CV_32SC1);			
			cv::compare(exp_vpPrior, vpFilter->prior, comparisonPrior, cv::CMP_NE);
			testResult_prior = cv::countNonZero(comparisonPrior);
			//saveMatToCsv(exp_LanePrior, "VP_PRIOR.csv");
			//saveMatToCsv(vpFilter->prior, "VP_PRIOR2.csv");
			
			
			exp_vpFilter 		= loadCSV("VP_FILTER.csv", CV_32SC1);			
			cv::compare(exp_vpFilter, vpFilter->filter, comparisonFilter, cv::CMP_NE);
			testResult_filter = cv::countNonZero(comparisonFilter);
			
			
			exp_vpTransition 		= loadCSV("VP_TRANSITION.csv", CV_32SC1);			
			cv::compare(exp_vpTransition, vpFilter->transition, comparisonTransition, cv::CMP_NE);
			testResult_transition = cv::countNonZero(comparisonTransition);

		}
		
		~TEST_VPFilter()
		{
			
		}
};



TEST_VPFilter testVpFilter;


TEST(VanishPointFilter)
    {

		CHECK_EQUAL(5,  testVpFilter.vpFilter->STEP) ;
		
		CHECK_EQUAL(0,  testVpFilter.testResult_prior);
		
		CHECK_EQUAL(0,  testVpFilter.testResult_filter);
		
		CHECK_EQUAL(0,  testVpFilter.testResult_transition);
		


    }
