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
	  MatrixXd exp_Filter;
	  MatrixXd exp_Prior;
	  MatrixXd exp_Transition;
	  VectorXd exp_BINS_HISTOGRAM;
	  
	  unique_ptr<VanishingPtFilter> 	vpFilter;
	   
	  TEST_VPFilter()		 
		{
			
			Camera 	   camera;
			Lane       lane;
			LaneFilter laneFilter(lane, camera);
			
			vpFilter.reset(new VanishingPtFilter(laneFilter.HISTOGRAM_BINS, laneFilter.LANE_FILTER_OFFSET_V, camera));
			
			exp_BINS_HISTOGRAM 	= readCSV("VP_BINS_HST.csv",153);
			exp_Filter 			= readCSV("VP_FILTER.csv", 6, 61 );
			exp_Prior 			= readCSV("VP_PRIOR.csv", 6, 61);
			exp_Transition      = readCSV("VP_TRANSITION.csv" , 3, 3 );
		}
		
		~TEST_VPFilter()
		{
			
		}
};



TEST_VPFilter testVpFilter;


TEST(VanishPointFilter)
    {

		CHECK_EQUAL(5,  testVpFilter.vpFilter->STEP) ;
		
		CHECK_ARRAY_CLOSE(testVpFilter.exp_Prior.data(),
						  testVpFilter.vpFilter->Prior.data(), 6*61, 1.0e-6) ;
		
		CHECK_ARRAY_CLOSE(testVpFilter.exp_Filter.data(),
						  testVpFilter.vpFilter->Filter.data(), 6*61, 1.0e-6) ;
		
		CHECK_ARRAY_CLOSE(testVpFilter.exp_Transition.data(),
						  testVpFilter.vpFilter->Transition.data(), 3*3, 1.0e-6) ;

    }
