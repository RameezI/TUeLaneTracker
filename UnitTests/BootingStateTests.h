#include "UnitTest++/UnitTest++.h"
#include  "Tests.h"
#include  "Camera.h"
#include  "Lane.h"
#include  "LaneFilter.h"
#include  "VanishingPtFilter.h"
#include  "InitState.h"



class TEST_InitState:Tests
{
	
public:	
	
	VectorXd exp_BINS_HISTOGRAM;
	VectorXi act_BINS_HISTOGRAM;
	  
	
	  int testResult_prior;
	  int testResult_filter;
	  int testResult_transition;
	  
	  int testResult_GradTangentRoot;
	  int testResult_DepthRoot;
	  int testResult_FocusRoot;

	 States  mCurrentState;
	  
	  TEST_InitState()
		
		{ 
			
			Camera	camera;
			Lane    lane;
	
			unique_ptr<LaneFilter>  		laneFilter;
			unique_ptr<VanishingPtFilter>   vanishingPtFilter;
			unique_ptr<Templates> 			templates;
			
			InitState		bootingState;
				
			/*******************************************/
						  // INITIALISING //
		
			laneFilter 			= bootingState.createLaneFilter();
			vanishingPtFilter	= bootingState.createVanishingPtFilter();
			templates           = bootingState.createTemplates();
			
			
			Mat comparisonPrior;
			Mat comparisonFilter;
			Mat comparisonTransition;
			
			Mat comparisonGradientTanRoot;
			Mat comparisonFocusRoot;
			Mat comparisonDepthRoot;
			
			
			exp_BINS_HISTOGRAM = readCSV("LANE_BINS_H.csv", 153);
			act_BINS_HISTOGRAM = laneFilter->HISTOGRAM_BINS;

			Mat exp_LanePrior = loadCSV("LANE_PRIOR.csv", CV_32SC1);
			cv::compare(exp_LanePrior, laneFilter->prior, comparisonPrior, cv::CMP_NE);
			testResult_prior = cv::countNonZero(comparisonPrior);
			
			Mat exp_LaneFilter = loadCSV("LANE_FILTER.csv", CV_32SC1);
			cv::compare(exp_LaneFilter, laneFilter->filter, comparisonFilter, cv::CMP_NE);
			testResult_filter = cv::countNonZero(comparisonFilter);

			Mat exp_LaneTransition = loadCSV("LANE_TRANSITION.csv", CV_32SC1);
			saveMatToCsv(exp_LaneTransition, "LANE_TRANSITION.csv");
			cv::compare(exp_LaneTransition, laneFilter->transition, comparisonTransition, cv::CMP_NE);
			testResult_transition = cv::countNonZero(comparisonTransition);
			
			
			Mat exp_GradientTangent = loadCSV("GRADIENT_TANGENT_ROOT.csv", CV_16SC1);
			cv::compare(exp_GradientTangent, templates->GRADIENT_TAN_ROOT , comparisonGradientTanRoot, cv::CMP_NE);
			testResult_GradTangentRoot = cv::countNonZero(comparisonGradientTanRoot);
			
			
			Mat exp_DepthRoot = loadCSV("DEPTH_ROOT.csv", CV_8UC1);
			cv::compare(exp_LaneFilter, laneFilter->filter, comparisonDepthRoot, cv::CMP_NE);
			testResult_DepthRoot = cv::countNonZero(comparisonDepthRoot);
			
			Mat exp_FocusRoot = loadCSV("FOCUS_ROOT.csv", CV_8UC1);
			cv::compare(exp_FocusRoot, templates->FOCUS_MASK_ROOT, comparisonFocusRoot, cv::CMP_NE);
			testResult_FocusRoot = cv::countNonZero(comparisonFocusRoot);
			

			
			/*ofstream file("DIR_ROOT_Eigen.csv");
			const static IOFormat CSVFormat(StreamPrecision, DontAlignCols, ", ", "\n");
			file<<initState->mTemplates->GRADIENT_DIR_ROOT.format(CSVFormat); */
		}
		
		~TEST_InitState()
		{
			
			
			
			
		}
};



