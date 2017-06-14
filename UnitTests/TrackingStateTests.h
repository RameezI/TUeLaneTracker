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
	  
	  int testResult_BaseBinIdx=-1;
	  int testResult_PurviewBinIdx=-1;
	  int testResult_WeightsBin=-1;
	  
	  int testResult_HistBase=-1;
	  int testResult_HistPurview=-1;

	  int testResult_TransitionedFilterLane=-1;
	  int testResult_UpdatedFilterLane=-1;
	  int testResult_TransitionedFilterVP=-1;
	  int testResult_UpdatedFilterVP=-1;
	  
	  
	 TEST_TrackingState()
		
	{ 
		
			
	Mat comparison;	
	Mat ProbFrame1, ProbFrame2, ProbFrame3, MaxProbFocussed,       GradTanFocussed; 
	Mat exp_Prob1, exp_Prob2,   exp_Prob3,  exp_MaxProbFocussed,  exp_GradTanFocussed;

	Mat exp_GradTan1, exp_GradTan2, exp_GradTan3;
	Mat GradTan1, 	  GradTan2,     GradTan3;

	Mat BaseBinIdx,     PurviewBinIdx,     WeightsBin,    	HistBase,      HistPurview;
	Mat exp_BaseBinIdx, exp_PurviewBinIdx, exp_WeightsBin,  exp_HistBase,  exp_HistPurview;
		
	Mat TransitionedFilterLane,     UpdatedFilterLane, 		TransitionedFilterVP, 	 UpdatedFilterVP;
	Mat exp_TransitionedFilterLane, exp_UpdatedFilterLane, exp_TransitionedFilterVP, exp_UpdatedFilterVP;		
	
	
		vector< cv::String> lFiles;
		
		cv::String folder = "/home/rameez/TestDataBackup";
		glob(folder, lFiles);
		
		ofstream Mat_OCV;
				
		Camera	camera;
		Lane    lane;


		
		unique_ptr<LaneFilter>  		pLaneFilter;
		unique_ptr<VanishingPtFilter>   pVanishingPtFilter;
		unique_ptr<Templates> 			pTemplates;
		
		



		
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
				
				BaseBinIdx = Mat(trackingState.mTrackingLanesGraph.mBaseBinIdx);
				PurviewBinIdx = Mat(trackingState.mTrackingLanesGraph.mPurviewBinIdx);
				
				WeightsBin = Mat(trackingState.mTrackingLanesGraph.mWeightBin);
				
				HistBase 	= trackingState.mTrackingLanesGraph.mHistBase;
				HistPurview = trackingState.mTrackingLanesGraph.mHistPurview;
				
				TransitionedFilterLane = trackingState.mTrackingLanesGraph.mTransitLaneFilter;
				
				
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
				
				
				
				exp_BaseBinIdx = loadCSV("BaseBinIdx.csv", CV_16UC1);
				cv::compare(exp_BaseBinIdx, BaseBinIdx, comparison, cv::CMP_NE);
				testResult_BaseBinIdx = cv::countNonZero(comparison);
				
				
				exp_PurviewBinIdx = loadCSV("PurviewBinIdx.csv", CV_16UC1);
				cv::compare(exp_PurviewBinIdx, PurviewBinIdx, comparison, cv::CMP_NE);
				testResult_PurviewBinIdx = cv::countNonZero(comparison);
				
				exp_WeightsBin = loadCSV("WeightsHistogram.csv", CV_32SC1);
				cv::compare(exp_WeightsBin, WeightsBin, comparison, cv::CMP_NE);
				testResult_WeightsBin = cv::countNonZero(comparison);
				
				
				exp_HistBase = loadCSV("HistogramBase.csv", CV_32SC1);
				cv::compare(exp_HistBase, HistBase, comparison, cv::CMP_NE);
				testResult_HistBase= cv::countNonZero(comparison);
				
				
				exp_HistPurview= loadCSV("HistogramPurview.csv", CV_32SC1);
				cv::compare(exp_HistPurview, HistPurview, comparison, cv::CMP_NE);
				testResult_HistPurview = cv::countNonZero(comparison);
				
				// Preccision Error! scaled additions of integer matrices
				exp_TransitionedFilterLane= loadCSV("TransitionedLane.csv", CV_32SC1);
				Mat exp_Diff = Mat::ones(exp_TransitionedFilterLane.size(),CV_32SC1);
				Mat Diff= exp_TransitionedFilterLane - TransitionedFilterLane;
				Diff = abs(Diff);
				cv::compare(Diff, exp_Diff, comparison, cv::CMP_GT);
				testResult_TransitionedFilterLane = cv::countNonZero(comparison);
				
		
			
				saveMatToCsv(HistBase, 				 "Histogram.csv");
				saveMatToCsv(exp_HistBase, 			 "HistogramExpected.csv");
				saveMatToCsv(TransitionedFilterLane, "LaneFilterTransit.csv");
				


				
							
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
