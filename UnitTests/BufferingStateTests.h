#ifndef TESTBUFFERINGSTATE_H
#define TESTBUFFERINGSTATE_H

#include "UnitTest++/UnitTest++.h"
#include  "Tests.h"
#include  "Camera.h"
#include  "Lane.h"
#include  "LaneFilter.h"
#include  "VanishingPtFilter.h"
#include  "InitState.h"
#include  "BufferingState.h"

class TEST_BufferingState:Tests
{
	
public:	
	  int testResult_GradTangentRoot=-1;
	  int testResult_DepthRoot=-1;
	  int testResult_FocusRoot=-1;
	  
	  int testResult_GradTangentTemplate=-1;
	  int testResult_DepthTemplate=-1;
	  int testResult_FocusTemplate=-1;
	  
	  int Span =-1;
	  int testTypes =-1;

	  int testResult_GradX=-1;
	  int testResult_GradY=-1;
	  int testResult_Mag=-1;
	  int testResult_GradTan=-1;
	  int testResult_ProbFrame=-1;

	 States  mCurrentState;
	  
	TEST_BufferingState()
		
	{ 
		
		vector< cv::String> lFiles;
		cv::String folder = "/home/rameez/TestDataBackup";
		glob(folder, lFiles);
		
		Camera			  camera;
		LaneParameters    lane;
		
		unique_ptr<LaneFilter>  		laneFilter;
		unique_ptr<VanishingPtFilter>   vanishingPtFilter;
		unique_ptr<Templates> 			templates;
		
		InitState		bootingState;
		BufferingState  bufferingState;
			
	
		laneFilter 			= bootingState.createLaneFilter();
		vanishingPtFilter	= bootingState.createVanishingPtFilter();
		templates           = bootingState.createTemplates();
		

		Mat exp_GradientX, exp_GradientY,	exp_MagFrame, exp_GradTan, exp_ProbFrame;
		

		if (bootingState.currentStatus == StateStatus::DONE)						
		{
			
			Mat comparison;

			if (bufferingState.currentStatus == StateStatus::INACTIVE)
			{			
					bufferingState.setSource(lFiles);				
					bufferingState.setupDAG(std::ref(*templates));
			}
			
			if (bufferingState.currentStatus == StateStatus::ACTIVE)
			{
				bufferingState.run();

				BufferingDAG_generic lBufferingGraph(std::move(bufferingState.bufferingGraph));
				Span = lBufferingGraph.mSpan;
						
						
									/* Simulated */
				//-----------------------------------------------------------------------//		
				exp_GradientX = loadCSV("GradientX.csv", CV_16SC1);
				cv::compare(exp_GradientX, lBufferingGraph.mGradX, comparison, cv::CMP_NE);
				testResult_GradX = cv::countNonZero(comparison);
				
				exp_GradientY = loadCSV("GradientY.csv", CV_16SC1);
				cv::compare(exp_GradientY, lBufferingGraph.mGradY, comparison, cv::CMP_NE);
				testResult_GradY = cv::countNonZero(comparison);
				
									/*Actual */
				//-------------------------------------------------------------------------//		
				exp_MagFrame = loadCSV("MAG_FRAME.csv", CV_8UC1);
				cv::compare(exp_MagFrame, lBufferingGraph.mFrameGradMag, comparison, cv::CMP_NE);
				testResult_Mag = cv::countNonZero(comparison);
				
				exp_GradTan = loadCSV("GradTan_FRAME.csv", CV_16SC1);
				cv::compare(exp_GradTan, lBufferingGraph.mBufferPool->GradientTangent[2], comparison, cv::CMP_NE);
				testResult_GradTan = cv::countNonZero(comparison);
				
				
				exp_ProbFrame = loadCSV("PROB_FRAME.csv", CV_8UC1);
				cv::compare(exp_ProbFrame, lBufferingGraph.mBufferPool->Probability[2], comparison, cv::CMP_NE);
				testResult_ProbFrame = cv::countNonZero(comparison);
				
				if (lBufferingGraph.mGradX.type() == CV_16SC1)
				{
					testTypes=0;		
				}
				
				
				// ^TODO ProbFrame is only varified visually
				//imshow("ExpectedFrame", exp_ProbFrame);
				//imshow("ActualFrame",   lBufferingGraph.mBufferPool->Probability[2]);
				//waitKey(0);
			}
		}



	
	}
		
		~TEST_BufferingState(){	}
};



#endif