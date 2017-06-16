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
	  
	  int testResult_BINS_V;
	  int testResult_BINS_H;
	  int testResult_HistogramBins;
	  int testResult_prior=-1;
	  int testResult_filter=-1;
	  int STEP =-1;
	   
	TEST_VPFilter()		 
	{
			
	Mat comparison;
	Mat exp_vpFilter, exp_vpPrior, exp_HistogramBINS, exp_BINS_V, exp_BINS_H;
	Mat BINS_V, BINS_H, HistogramBINS;
		
	Camera 	   			camera;
	LaneParameters      lane;
	LaneFilter 			laneFilter(lane, camera);
	
		
	VanishingPtFilter vpFilter(laneFilter.HISTOGRAM_BINS, laneFilter.OFFSET_V, camera);

	STEP 				= vpFilter.STEP;
		
	cv::eigen2cv(vpFilter.HISTOGRAM_BINS, HistogramBINS);
	cv::eigen2cv(vpFilter.VP_BINS_V, BINS_V);
	cv::eigen2cv(vpFilter.VP_BINS_H, BINS_H);
		
	exp_HistogramBINS 	= loadCSV("PURVIEW_HISTOGRAM_BINS.csv",CV_32SC1);
	cv::compare(exp_HistogramBINS, HistogramBINS, comparison, cv::CMP_NE);
	testResult_HistogramBins = cv::countNonZero( comparison);
		
		
	exp_BINS_V 	=	loadCSV("VP_BINS_V.csv", CV_32S);
	cv::compare(exp_BINS_V, BINS_V, comparison, cv::CMP_NE);
	testResult_BINS_V =  cv::countNonZero( comparison);
		
	exp_BINS_H 	=	loadCSV("VP_BINS_H.csv", CV_32S);
	cv::compare(exp_BINS_H, BINS_H, comparison, cv::CMP_NE);
	testResult_BINS_H =  cv::countNonZero( comparison);
		
		
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

		CHECK_EQUAL(0, testVpFilter.testResult_BINS_V);
		CHECK_EQUAL(0, testVpFilter.testResult_HistogramBins);
		
		CHECK_EQUAL(0,  testVpFilter.testResult_prior);		
		CHECK_EQUAL(0,  testVpFilter.testResult_filter);


    }