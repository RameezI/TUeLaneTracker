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
	  MatrixXd exp_FilterMatrix;
	  MatrixXd exp_LanePrior;
	  VectorXd exp_BINS_HISTOGRAM;
	  MatrixXd exp_LaneTransition;
	  	  
	  unique_ptr<LaneFilter> laneFilter;
	  
	  TEST_LaneFilter()
		{

			Camera camera;
			Lane   lane;
			
			laneFilter.reset(new LaneFilter(lane, camera));
		 
			exp_BINS_HISTOGRAM = readCSV("LANE_BINS_H.csv", 153);
			exp_LanePrior = readCSV("LANE_PRIOR.csv", 77, 77);
			exp_LaneTransition = readCSV("LANE_TRANSITION.csv", 7, 7);
			exp_FilterMatrix   = exp_LanePrior;
		}
		~TEST_LaneFilter()
		{
			
		}			
};


TEST_LaneFilter testLaneFilter;

TEST(LaneFilter)

    {
				
		 CHECK_EQUAL(10, testLaneFilter.laneFilter->STEP) ;
		 
		 CHECK_ARRAY_CLOSE(testLaneFilter.exp_BINS_HISTOGRAM.data(), 
						   testLaneFilter.laneFilter->HISTOGRAM_BINS.data(),
						   testLaneFilter.exp_BINS_HISTOGRAM.size(), 1.0e-4);
		 
		 CHECK_ARRAY_CLOSE(testLaneFilter.exp_LanePrior.data(),
						   testLaneFilter.laneFilter->Prior.data(),
						   77*77, 1.0e-6);
		 
		 CHECK_ARRAY_CLOSE(testLaneFilter.exp_LaneTransition.data(),
						   testLaneFilter.laneFilter->Transition.data(),
						   7*7, 1.0e-6);
    }

