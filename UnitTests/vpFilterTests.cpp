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
	  
	  MatrixXd exp_BINS_HISTOGRAM;
	  MatrixXi BINS_HISTOGRAM;
	  int testResult_prior=-1;
	  int testResult_filter=-1;
	  int STEP =-1;
	   
	TEST_VPFilter()		 
	{
			
	Mat comparison;
	Mat exp_vpFilter, exp_vpPrior;
			
		Camera 	   			camera;
		LaneParameters      lane;
		LaneFilter 			laneFilter(lane, camera);
	
		
		VanishingPtFilter vpFilter(laneFilter.HISTOGRAM_BINS, laneFilter.OFFSET_V, camera);
		BINS_HISTOGRAM  = vpFilter.HISTOGRAM_BINS;
		STEP 			= vpFilter.STEP;
		exp_BINS_HISTOGRAM 	= readCSV("VP_BINS_HST.csv",153);
		
		
		exp_vpPrior = loadCSV("VP_PRIOR.csv", CV_32SC1);			
		cv::compare(exp_vpPrior, vpFilter.prior, comparison, cv::CMP_NE);
		testResult_prior = cv::countNonZero( comparison);
		saveMatToCsv(vpFilter.prior, "VP_PRIOR.csv");
				
		exp_vpFilter = loadCSV("VP_FILTER.csv", CV_32SC1);			
		cv::compare(exp_vpFilter, vpFilter.filter, comparison, cv::CMP_NE);
		testResult_filter = cv::countNonZero(comparison);

		}
		
		~TEST_VPFilter()
		{
			
		}
};

TEST_VPFilter testVpFilter;

TEST(VanishPointFilter)
    {

		CHECK_EQUAL(5,  testVpFilter.STEP) ;

		CHECK_ARRAY_CLOSE( testVpFilter.exp_BINS_HISTOGRAM.data(), 
						   testVpFilter.BINS_HISTOGRAM.data(),
						   testVpFilter.exp_BINS_HISTOGRAM.size(), 1.0e-4);
		
		CHECK_EQUAL(0,  testVpFilter.testResult_prior);		
		CHECK_EQUAL(0,  testVpFilter.testResult_filter);

    }
