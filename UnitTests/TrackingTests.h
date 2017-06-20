#ifndef TRACKINGSTATETESTS_H
#define TRACKINGSTATETESTS_H
#include "UnitTest++/UnitTest++.h"
#include  "Tests.h"
#include  "LaneFilter.h"
#include  "VanishingPtFilter.h"
#include  "InitState.h"
#include  "BufferingState.h"
#include  "TrackingLaneState.h"

class TEST_TrackingState:Tests
{

public:


// Test results from Buffering State

	int testResult_GradTangentRoot=-1;
	int testResult_DepthRoot=-1;
	int testResult_FocusRoot=-1;

	int testResult_GradTangentTemplate=-1;
	int testResult_DepthTemplate=-1;
	int testResult_FocusTemplate=-1;


	int testResult_BlurFrame=-1;
	int testResult_GradX=-1;
	int testResult_GradY=-1;
	int testResult_Mag=-1;
	int testResult_GradTan=-1;
	int testResult_ProbGray=-1;
	int testResult_ProbMag=-1;
	int testResult_ProbDir=-1;
	int testResult_ProbFrame=-1;
	

	


// Test results from Tracking state
	
	int testResult_MaxProbFocussed=-1;
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
	
	
	int testResult_laneModel=-1;
	int testResult_vp=-1;
	
	  
	  
	TEST_TrackingState()
		
	{ 
	
	// The Tests will verify the itteration specified
	// Make sure that correspondng validation is present
	int64_t ItterationID =3; 
	
	Mat comparison;	
	
	Mat exp_BlurFrame, exp_GradientX, exp_GradientY,	exp_MagFrame, exp_GradTan;
	Mat BlurFrame, 		GradientX,    GradientY,        MagFrame,     GradTan;
	
	Mat exp_ProbGray, exp_ProbMag, exp_ProbDir, exp_ProbFrame;
	Mat ProbGray, ProbMag, ProbDir, ProbFrame;
	
	Mat  GradientTanTemplate, 	  DepthTemplate, 		FocusTemplate;
	Mat  exp_GradientTanTemplate, exp_DepthTemplate, exp_FocusTemplate;
	
	Mat  MaxProbFocussed,       GradTanFocussed; 
	Mat  exp_MaxProbFocussed,  exp_GradTanFocussed;


	Mat BaseBinIdx,     PurviewBinIdx,     WeightsBin,    	HistBase,      HistPurview;
	Mat exp_BaseBinIdx, exp_PurviewBinIdx, exp_WeightsBin,  exp_HistBase,  exp_HistPurview;
		
	Mat TransitionedFilterLane,     UpdatedFilterLane, 		TransitionedFilterVP, 	 UpdatedFilterVP;
	Mat exp_TransitionedFilterLane, exp_UpdatedFilterLane, exp_TransitionedFilterVP, exp_UpdatedFilterVP;		


	Mat  vp, laneModel;
	Mat  exp_vp , exp_laneModel;
	
	// Setup Files
	vector< cv::String> lFiles;
	//cv::String folder = "../TestData/TestImages";
	cv::String folder = "/media/rameez/Linux-Extended/DataSet/eindhoven/PNG_imgs";
	glob(folder, lFiles);
	
	States lCurrentState = States::BOOTING;

	// Filters and Templates	
	unique_ptr<LaneFilter>  		pLaneFilter;
	unique_ptr<VanishingPtFilter>   pVanishingPtFilter;
	unique_ptr<Templates> 			pTemplates;
	unique_ptr<TrackingLaneState>   pTrackingState = NULL;

	{
		InitState		bootingState;				

		pLaneFilter 			= bootingState.createLaneFilter();
		pVanishingPtFilter		= bootingState.createVanishingPtFilter();
		pTemplates           	= bootingState.createTemplates();
		
		lCurrentState           = States::BUFFERING;
	}

	BufferingState bufferingState;
	
	for (int i=0; i< ItterationID; i++)
	{	
		if (lCurrentState == States::BUFFERING)
		{
			if (bufferingState.currentStatus == StateStatus::INACTIVE)
			{
				bufferingState.setSource(lFiles);
				bufferingState.setupDAG(std::ref(*pTemplates));
			}
			
			if(bufferingState.currentStatus == StateStatus::ACTIVE)
			{
				bufferingState.run();
				
				BlurFrame 			= bufferingState.bufferingGraph.mFrameGRAY_ROI;
				GradientTanTemplate = bufferingState.bufferingGraph.mGradTanTemplate;
				FocusTemplate       = bufferingState.bufferingGraph.mFocusTemplate;
				DepthTemplate	    = bufferingState.bufferingGraph.mDepthTemplate;
				GradientX   		= bufferingState.bufferingGraph.mGradX;
				GradientY   		= bufferingState.bufferingGraph.mGradY;
				MagFrame    		= bufferingState.bufferingGraph.mFrameGradMag;
				GradTan				= bufferingState.bufferingGraph.mBufferPool->GradientTangent[2];
				ProbGray 			= bufferingState.bufferingGraph.mProbMap_Gray;
				ProbMag  			= bufferingState.bufferingGraph.mProbMap_GradMag;
				ProbDir  			= bufferingState.bufferingGraph.mProbMap_GradDir;
				ProbFrame 			= bufferingState.bufferingGraph.mBufferPool->Probability[2];
				
							
									/* Simulated */
				//--------------------------------------------------------//
				exp_BlurFrame = loadCSV("BlurredFrame.csv", CV_8UC1);
				cv::compare(exp_BlurFrame, BlurFrame, comparison, cv::CMP_NE);
				testResult_BlurFrame = cv::countNonZero(comparison);


										/*Actual */
				//--------------------------------------------------------//
				exp_GradientTanTemplate = loadCSV("TemplateGradientTan.csv", CV_16SC1);
				cv::compare(exp_GradientTanTemplate, GradientTanTemplate, comparison, cv::CMP_NE);
				testResult_GradTangentTemplate = cv::countNonZero(comparison);
				
				
				exp_FocusTemplate = loadCSV("TemplateFocus.csv", CV_8UC1);
				cv::compare(exp_FocusTemplate, FocusTemplate, comparison, cv::CMP_NE);
				testResult_FocusTemplate= cv::countNonZero(comparison);
				
				
				exp_DepthTemplate = loadCSV("TemplateDepth.csv", CV_16UC1);
				cv::compare(exp_DepthTemplate, DepthTemplate, comparison, cv::CMP_NE);
				testResult_DepthTemplate = cv::countNonZero(comparison);
				
				
				exp_GradientX = loadCSV("GradientX.csv", CV_16SC1);
				cv::compare(exp_GradientX, GradientX, comparison, cv::CMP_NE);
				testResult_GradX = cv::countNonZero(comparison);
				
				exp_GradientY = loadCSV("GradientY.csv", CV_16SC1);
				cv::compare(exp_GradientY, GradientY, comparison, cv::CMP_NE);
				testResult_GradY = cv::countNonZero(comparison);
				
				exp_MagFrame = loadCSV("MAG_FRAME.csv", CV_8UC1);
				cv::compare(exp_MagFrame, MagFrame , comparison, cv::CMP_NE);
				testResult_Mag = cv::countNonZero(comparison);
				
				exp_GradTan = loadCSV("GradTan_FRAME.csv", CV_16SC1);
				cv::compare(exp_GradTan, GradTan, comparison, cv::CMP_NE);
				testResult_GradTan = cv::countNonZero(comparison);
		
				exp_ProbGray = loadCSV("PROB_GRAY.csv", CV_32SC1);
				cv::compare(exp_ProbGray, ProbGray, comparison, cv::CMP_NE);
				testResult_ProbGray = cv::countNonZero(comparison);
					
				exp_ProbMag = loadCSV("PROB_MAG.csv", CV_32SC1);
				cv::compare(exp_ProbMag, ProbMag, comparison, cv::CMP_NE);
				testResult_ProbMag = cv::countNonZero(comparison);
					
				exp_ProbDir = loadCSV("PROB_DIR.csv", CV_32SC1);
				cv::compare(exp_ProbDir, ProbDir, comparison, cv::CMP_NE);
				testResult_ProbDir = cv::countNonZero(comparison);
					
				exp_ProbFrame = loadCSV("PROB_FRAME.csv", CV_8UC1);
				cv::compare(exp_ProbFrame, ProbFrame, comparison, cv::CMP_NE);
				testResult_ProbFrame = cv::countNonZero(comparison);
				
				// Make all Tracking Tests Pass for Buffering Itterations			
				testResult_MaxProbFocussed	= 0;
				testResult_GradTanFocussed	= 0;

				testResult_BaseBinIdx		= 0;
				testResult_PurviewBinIdx	= 0;
				testResult_WeightsBin		= 0;

				testResult_HistBase			= 0;
				testResult_HistPurview		= 0;

				testResult_TransitionedFilterLane	= 0;
				testResult_UpdatedFilterLane		= 0;
				testResult_TransitionedFilterVP		= 0;
				testResult_UpdatedFilterVP			= 0;
				
				testResult_laneModel				= 0;
				testResult_vp						= 0;
			}
		
			if (bufferingState.currentStatus == StateStatus::DONE)
			{
			
				lCurrentState = States::DETECTING_LANES;
				pTrackingState.reset(new TrackingLaneState(move(bufferingState.bufferingGraph)));
			}
			
		}		
		else if(lCurrentState == States::DETECTING_LANES)
		{
				testResult_MaxProbFocussed	= -1;
				testResult_GradTanFocussed	= -1;

				testResult_BaseBinIdx		= -1;
				testResult_PurviewBinIdx	= -1;
				testResult_WeightsBin		= -1;

				testResult_HistBase			= -1;
				testResult_HistPurview		= -1;

				testResult_TransitionedFilterLane	= -1;
				testResult_UpdatedFilterLane		= -1;
				testResult_TransitionedFilterVP		= -1;
				testResult_UpdatedFilterVP			= -1;
				
				
				testResult_laneModel				= -1;
				testResult_vp						= -1;
			
			
			TrackingLaneState& trackingState = *pTrackingState;			
			
			if (trackingState.currentStatus == StateStatus::INACTIVE)
			   trackingState.setupDAG(pLaneFilter.get(), pVanishingPtFilter.get());
		   
			if (trackingState.currentStatus == StateStatus::ACTIVE)
			{
				trackingState.run();
				
				BlurFrame 			= trackingState.mTrackingLanesGraph.mFrameGRAY_ROI;
				GradientTanTemplate = trackingState.mTrackingLanesGraph.mGradTanTemplate;
				FocusTemplate       = trackingState.mTrackingLanesGraph.mFocusTemplate;
				DepthTemplate	    = trackingState.mTrackingLanesGraph.mDepthTemplate;
				GradientX   		= trackingState.mTrackingLanesGraph.mGradX;
				GradientY  			= trackingState.mTrackingLanesGraph.mGradY;
				MagFrame    		= trackingState.mTrackingLanesGraph.mFrameGradMag;
				GradTan				= trackingState.mTrackingLanesGraph.mBufferPool->GradientTangent[2];
				ProbGray 			= trackingState.mTrackingLanesGraph.mProbMap_Gray;
				ProbMag  			= trackingState.mTrackingLanesGraph.mProbMap_GradMag;
				ProbDir  			= trackingState.mTrackingLanesGraph.mProbMap_GradDir;
				ProbFrame 			= trackingState.mTrackingLanesGraph.mBufferPool->Probability[2];

				

				
				MaxProbFocussed = trackingState.mTrackingLanesGraph.mProbMapFocussed;
				GradTanFocussed = trackingState.mTrackingLanesGraph.mGradTanFocussed;
			
				BaseBinIdx 		= Mat(trackingState.mTrackingLanesGraph.mBaseBinIdx);
				PurviewBinIdx 	= Mat(trackingState.mTrackingLanesGraph.mPurviewBinIdx);			
				WeightsBin 		= Mat(trackingState.mTrackingLanesGraph.mWeightBin);
				
				HistBase 		= trackingState.mTrackingLanesGraph.mHistBase;
				HistPurview 	= trackingState.mTrackingLanesGraph.mHistPurview;
				
				TransitionedFilterLane = trackingState.mTrackingLanesGraph.mTransitLaneFilter;
				UpdatedFilterLane      = trackingState.mTrackingLanesGraph.mLaneFilter->filter;
				
				TransitionedFilterVP = trackingState.mTrackingLanesGraph.mTransitVpFilter;
				UpdatedFilterVP      = trackingState.mTrackingLanesGraph.mVpFilter->filter;
				
				vector<int> VP, Lane; 
				
				VP.push_back(trackingState.mTrackingLanesGraph.mVanishPt.V);
				VP.push_back(trackingState.mTrackingLanesGraph.mVanishPt.H);
				
				Lane.push_back(trackingState.mTrackingLanesGraph.mLaneModel.leftOffset);
				Lane.push_back(trackingState.mTrackingLanesGraph.mLaneModel.rightOffset);
				Lane.push_back(trackingState.mTrackingLanesGraph.mLaneModel.confidenceLeft);
				Lane.push_back(trackingState.mTrackingLanesGraph.mLaneModel.confidenceRight);
				
				vp = Mat(VP);
				laneModel = Mat(Lane);
				
				
									/* Simulated */
				//--------------------------------------------------------//
				exp_BlurFrame = loadCSV("BlurredFrame.csv", CV_8UC1);
				cv::compare(exp_BlurFrame, BlurFrame, comparison, cv::CMP_NE);
				testResult_BlurFrame = cv::countNonZero(comparison);


										/*Actual */
				//--------------------------------------------------------//
				
				exp_GradientTanTemplate = loadCSV("TemplateGradientTan.csv", CV_16SC1);
				cv::compare(exp_GradientTanTemplate, GradientTanTemplate, comparison, cv::CMP_NE);
				testResult_GradTangentTemplate = cv::countNonZero(comparison);
				
				
				exp_FocusTemplate = loadCSV("TemplateFocus.csv", CV_8UC1);
				cv::compare(exp_FocusTemplate, FocusTemplate, comparison, cv::CMP_NE);
				testResult_FocusTemplate= cv::countNonZero(comparison);
				
				
				exp_DepthTemplate = loadCSV("TemplateDepth.csv", CV_16UC1);
				cv::compare(exp_DepthTemplate, DepthTemplate, comparison, cv::CMP_NE);
				testResult_DepthTemplate = cv::countNonZero(comparison);
				
				exp_GradientX = loadCSV("GradientX.csv", CV_16SC1);
				cv::compare(exp_GradientX, GradientX, comparison, cv::CMP_NE);
				testResult_GradX = cv::countNonZero(comparison);
				
				
				exp_GradientY = loadCSV("GradientY.csv", CV_16SC1);
				cv::compare(exp_GradientY, GradientY, comparison, cv::CMP_NE);
				testResult_GradY = cv::countNonZero(comparison);
				
				
				exp_MagFrame = loadCSV("MAG_FRAME.csv", CV_8UC1);
				cv::compare(exp_MagFrame, MagFrame , comparison, cv::CMP_NE);
				testResult_Mag = cv::countNonZero(comparison);
				
				
				exp_GradTan = loadCSV("GradTan_FRAME.csv", CV_16SC1);
				cv::compare(exp_GradTan, GradTan, comparison, cv::CMP_NE);
				testResult_GradTan = cv::countNonZero(comparison);
				
		
				exp_ProbGray = loadCSV("PROB_GRAY.csv", CV_32SC1);
				cv::compare(exp_ProbGray, ProbGray, comparison, cv::CMP_NE);
				testResult_ProbGray = cv::countNonZero(comparison);
				
					
				exp_ProbMag = loadCSV("PROB_MAG.csv", CV_32SC1);
				cv::compare(exp_ProbMag, ProbMag, comparison, cv::CMP_NE);
				testResult_ProbMag = cv::countNonZero(comparison);
				
					
				exp_ProbDir = loadCSV("PROB_DIR.csv", CV_32SC1);
				cv::compare(exp_ProbDir, ProbDir, comparison, cv::CMP_NE);
				testResult_ProbDir = cv::countNonZero(comparison);
				
					
				exp_ProbFrame = loadCSV("PROB_FRAME.csv", CV_8UC1);
				cv::compare(exp_ProbFrame, ProbFrame, comparison, cv::CMP_NE);
				testResult_ProbFrame = cv::countNonZero(comparison);
				
				
				exp_MaxProbFocussed = loadCSV("MAX_PROB_FOCUSSED.csv", CV_8UC1);
				cv::compare(exp_MaxProbFocussed, MaxProbFocussed, comparison, cv::CMP_NE);
				testResult_MaxProbFocussed = cv::countNonZero(comparison);
								

				exp_GradTanFocussed = loadCSV("GradTan_MAX_PROB.csv", CV_16SC1);
				cv::compare(exp_GradTanFocussed, GradTanFocussed, comparison, cv::CMP_NE);
				testResult_GradTanFocussed = cv::countNonZero(comparison);
		
			
		    
				exp_BaseBinIdx = loadCSV("BaseBinIdx.csv", CV_16UC1);
				if (exp_BaseBinIdx.size() ==  BaseBinIdx.size())
				{	cv::compare(exp_BaseBinIdx, BaseBinIdx, comparison, cv::CMP_NE);
					testResult_BaseBinIdx = cv::countNonZero(comparison);
					
				}
				else testResult_BaseBinIdx= -2;
			
		
				exp_PurviewBinIdx = loadCSV("PurviewBinIdx.csv", CV_16UC1);
				if (exp_PurviewBinIdx.size() ==  PurviewBinIdx.size())
				{
					cv::compare(exp_PurviewBinIdx, PurviewBinIdx, comparison, cv::CMP_NE);
					testResult_PurviewBinIdx = cv::countNonZero(comparison);
				}
				else testResult_PurviewBinIdx= -2;


				exp_WeightsBin = loadCSV("WeightsHistogram.csv", CV_32SC1);
				if (exp_WeightsBin.size() ==  WeightsBin.size())
				{
					cv::compare(exp_WeightsBin, WeightsBin, comparison, cv::CMP_NE);
					testResult_WeightsBin = cv::countNonZero(comparison);
				}
				else testResult_WeightsBin=-2;
				
				exp_HistBase = loadCSV("HistogramBase.csv", CV_32SC1);
				cv::compare(exp_HistBase, HistBase, comparison, cv::CMP_NE);
				saveMatToCsv(HistBase,"HistBase");
				testResult_HistBase= cv::countNonZero(comparison);
				
				
				exp_HistPurview= loadCSV("HistogramPurview.csv", CV_32SC1);
				cv::compare(exp_HistPurview, HistPurview, comparison, cv::CMP_NE);
				testResult_HistPurview = cv::countNonZero(comparison);
				
				exp_TransitionedFilterLane= loadCSV("TransitionedLane.csv", CV_32SC1);
				cv::compare(exp_TransitionedFilterLane, TransitionedFilterLane, comparison, cv::CMP_NE);
				testResult_TransitionedFilterLane = cv::countNonZero(comparison);
				
				exp_UpdatedFilterLane= loadCSV("UpdatedLaneFilter.csv", CV_32SC1);
				cv::compare(exp_UpdatedFilterLane, UpdatedFilterLane, comparison, cv::CMP_NE);
				testResult_UpdatedFilterLane = cv::countNonZero(comparison);
				
				
				exp_TransitionedFilterVP= loadCSV("TransitionedVP.csv", CV_32SC1);
				cv::compare(exp_TransitionedFilterVP, TransitionedFilterVP, comparison, cv::CMP_NE);
				testResult_TransitionedFilterVP = cv::countNonZero(comparison);
				
				exp_UpdatedFilterVP= loadCSV("UpdatedVPFilter.csv", CV_32SC1);
				cv::compare(exp_UpdatedFilterVP, UpdatedFilterVP, comparison, cv::CMP_NE);
				saveMatToCsv(UpdatedFilterVP, "UpdatedVPF.csv");
				testResult_UpdatedFilterVP = cv::countNonZero(comparison);
				
				exp_vp= loadCSV("VanishingPt.csv", CV_32SC1);
				cv::compare(exp_vp, vp, comparison, cv::CMP_NE);
				testResult_vp = cv::countNonZero(comparison);
				
				exp_laneModel= loadCSV("LaneModel.csv", CV_32SC1);
				cv::compare(exp_laneModel, laneModel, comparison, cv::CMP_NE);
				testResult_laneModel = cv::countNonZero(comparison);

			}
		}
	}
	
	
	} //Constructor Close
	
		
		~TEST_TrackingState(){	}
};





#endif // TRACKINGSTATETESTS_H
