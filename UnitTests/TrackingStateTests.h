#ifndef TRACKINGSTATETESTS_H
#define TRACKINGSTATETESTS_H
#include "UnitTest++/UnitTest++.h"
#include  "Tests.h"
#include  "Camera.h"
#include  "Lane.h"
#include  "LaneFilter.h"
#include  "VanishingPtFilter.h"
#include  "InitState.h"
#include  "BufferingState.h"
#include  "TrackingLaneState.h"




class TEST_TrackingState:Tests
{
	
public:
	
	  int testResult_ProbFrame1=-1;
	  int testResult_ProbFrame2=-1;
	  int testResult_ProbFrame3=-1;
	  int testResult_MaxProbFocussed=-1;
	  
	  
	  int testResult_GradTan1=-1;
	  int testResult_GradTan2=-1;
	  int testResult_GradTan3=-1;
	  int testResult_GradTanFocussed=-1;
	  
	  
	 TEST_TrackingState()
		
	{ 
		
			vector< cv::String> lFiles;
			
			cv::String folder = "/home/rameez/TestDataBackup";
			glob(folder, lFiles);
			
			ofstream Mat_OCV;
					
			Camera	camera;
			Lane    lane;


			
			unique_ptr<LaneFilter>  		pLaneFilter;
			unique_ptr<VanishingPtFilter>   pVanishingPtFilter;
			unique_ptr<Templates> 			pTemplates;
			
			
			
			Mat comparison;	
			Mat ProbFrame1, ProbFrame2, ProbFrame3, MaxProbFocussed,       GradTanFocussed; 
			Mat exp_Prob1, exp_Prob2,   exp_Prob3,  exp_MaxProbFocussed,  exp_GradTanFocussed;
			
			Mat exp_GradTan1, exp_GradTan2, exp_GradTan3;
			Mat GradTan1, 	  GradTan2,     GradTan3;
				
				
			
			InitState		bootingState;				

			pLaneFilter 			= bootingState.createLaneFilter();
			pVanishingPtFilter		= bootingState.createVanishingPtFilter();
			pTemplates           	= bootingState.createTemplates();
			
			
			BufferingState bufferingState;
			
			if (bufferingState.currentStatus == StateStatus::INACTIVE)
			{
				bufferingState.setSource(lFiles);
				bufferingState.setupDAG(std::ref(*pTemplates));
			}
			
			if (bufferingState.currentStatus == StateStatus::ACTIVE)
			{
				bufferingState.run();
				bufferingState.run();
				
			}
		
			if (bufferingState.currentStatus == StateStatus::DONE)
			{
					
				TrackingLaneState trackingState(std::move(bufferingState.bufferingGraph));
				
				if (trackingState.currentStatus == StateStatus::INACTIVE)
				   trackingState.setupDAG(pLaneFilter.get(), pVanishingPtFilter.get());
				   
				if (trackingState.currentStatus == StateStatus::ACTIVE)
				{
					
					trackingState.run();
					
					ProbFrame1 =  trackingState.mTrackingLanesGraph.mBufferPool->Probability[0];
					ProbFrame2 =  trackingState.mTrackingLanesGraph.mBufferPool->Probability[1];
					ProbFrame3 =  trackingState.mTrackingLanesGraph.mBufferPool->Probability[2];
					
					GradTan1 =  trackingState.mTrackingLanesGraph.mBufferPool->GradientTangent[0];
					GradTan2 =  trackingState.mTrackingLanesGraph.mBufferPool->GradientTangent[1];
					GradTan3 =  trackingState.mTrackingLanesGraph.mBufferPool->GradientTangent[2];

					
					MaxProbFocussed = trackingState.mTrackingLanesGraph.mProbMapFocussed;
					GradTanFocussed = trackingState.mTrackingLanesGraph.mGradTanFocussed;
					
					
					exp_Prob1 = loadCSV("Prob_1.csv", CV_8UC1);
					cv::compare(exp_Prob1, ProbFrame1, comparison, cv::CMP_NE);
					testResult_ProbFrame1 = cv::countNonZero(comparison);
					
					exp_Prob2 = loadCSV("Prob_2.csv", CV_8UC1);
					cv::compare(exp_Prob2, ProbFrame2, comparison, cv::CMP_NE);
					testResult_ProbFrame2 = cv::countNonZero(comparison);
					
					exp_Prob3 = loadCSV("Prob_3.csv", CV_8UC1);
					cv::compare(exp_Prob3, ProbFrame3, comparison, cv::CMP_NE);
					testResult_ProbFrame3 = cv::countNonZero(comparison);
					
					
					exp_GradTan1 = loadCSV("GradTan_1.csv", CV_16SC1);
					cv::compare(exp_GradTan1, GradTan1, comparison, cv::CMP_NE);
					testResult_GradTan1 = cv::countNonZero(comparison);
					
					exp_GradTan2 = loadCSV("GradTan_2.csv", CV_16SC1);
					cv::compare(exp_GradTan2, GradTan2, comparison, cv::CMP_NE);
					testResult_GradTan2 = cv::countNonZero(comparison);
					
					exp_GradTan3 = loadCSV("GradTan_3.csv", CV_16SC1);
					cv::compare(exp_GradTan3, GradTan3, comparison, cv::CMP_NE);
					testResult_GradTan3 = cv::countNonZero(comparison);
					
					
					exp_MaxProbFocussed = loadCSV("MAX_PROB_FOCUSSED.csv", CV_8UC1);
					cv::compare(exp_MaxProbFocussed, MaxProbFocussed, comparison, cv::CMP_NE);
					testResult_MaxProbFocussed = cv::countNonZero(comparison);
					
					exp_GradTanFocussed = loadCSV("GradTan_MAX_PROB.csv", CV_16SC1);
					cv::compare(exp_GradTanFocussed, GradTanFocussed, comparison, cv::CMP_NE);
					testResult_GradTanFocussed = cv::countNonZero(comparison);
					
					
					//imshow("ActualFrame",     ProbMap_3);
					//imshow("ActualFrame -1",  ProbMap_2);
					//imshow("ActualFrame -2",  ProbFrame1);
			
					//waitKey(0);
					 
				}
			}

			
		


	}
		
		~TEST_TrackingState(){	}
};





#endif // TRACKINGSTATETESTS_H
