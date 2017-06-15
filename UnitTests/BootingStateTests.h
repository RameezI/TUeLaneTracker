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
	  
	int testResult_GradTangentRoot;
	int testResult_DepthRoot;
	int testResult_FocusRoot;

	 States  mCurrentState;
	  
	TEST_InitState()
	
	{ 
		
		Camera			  camera;
		LaneParameters    lane;

		unique_ptr<LaneFilter>  		laneFilter;
		unique_ptr<VanishingPtFilter>   vanishingPtFilter;
		unique_ptr<Templates> 			templates;
				
		InitState		bootingState;
		
		laneFilter 			= bootingState.createLaneFilter();
		vanishingPtFilter	= bootingState.createVanishingPtFilter();
		templates           = bootingState.createTemplates();
	
		Mat comparison;
		
		Mat exp_GradientTangent, exp_DepthRoot, exp_FocusRoot;
		Mat GradientTangent, DepthRoot, FocusRoot;
		
		GradientTangent = templates->GRADIENT_TAN_ROOT;
		DepthRoot		= templates->DEPTH_MAP_ROOT;
		FocusRoot  		= templates->FOCUS_MASK_ROOT;
		
		exp_GradientTangent = loadCSV("GRADIENT_TANGENT_ROOT.csv", CV_16SC1);
		cv::compare(exp_GradientTangent, GradientTangent , comparison, cv::CMP_NE);
		testResult_GradTangentRoot = cv::countNonZero(comparison);
		
		
		exp_DepthRoot = loadCSV("DEPTH_ROOT.csv", CV_16UC1);
		cv::compare(exp_DepthRoot, DepthRoot, comparison, cv::CMP_NE);
		testResult_DepthRoot = cv::countNonZero(comparison);
		
		exp_FocusRoot = loadCSV("FOCUS_ROOT.csv", CV_8UC1);
		cv::compare(exp_FocusRoot,FocusRoot, comparison, cv::CMP_NE);
		testResult_FocusRoot = cv::countNonZero(comparison);
		
		
	/*	exp_XMat = loadCSV("X_MAT.csv", CV_8UC1);
		cv::compare(exp_X_IRS, XMat , comparison, cv::CMP_NE);
		testResult_XMat = cv::countNonZero(comparison);
	*/	
		
	}
		
	~TEST_InitState()
	{
		
	}
};



