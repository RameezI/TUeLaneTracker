#include "TrackingLaneState.h"


TrackingLaneState::TrackingLaneState(BufferingDAG_generic&& bufferingGraph)
:mTrackingLanesGraph(std::move(bufferingGraph))

{	
		
} 

void TrackingLaneState::setupDAG(LaneFilter* laneFilter, VanishingPtFilter* vpFilter)
{

//Setting Up observing Filters for the Graph	
 mTrackingLanesGraph.mLaneFilter = laneFilter;
 mTrackingLanesGraph.mVpFilter   = vpFilter;
 
 
 mTrackingLanesGraph.LOWER_LIMIT_IntBase 
 = SCALE_INTSEC*laneFilter->HISTOGRAM_BINS(0) - (SCALE_INTSEC*laneFilter->STEP)/2;
 
 mTrackingLanesGraph.UPPER_LIMIT_IntBase 
 = SCALE_INTSEC*laneFilter->HISTOGRAM_BINS(laneFilter->mNb_HISTOGRAM_BINS-1) +  (SCALE_INTSEC*laneFilter->STEP)/2;
 
  mTrackingLanesGraph.LOWER_LIMIT_IntPurview 
 = -SCALE_INTSEC*vpFilter->VP_RANGE_H - (SCALE_INTSEC*vpFilter->STEP)/2;
 
 mTrackingLanesGraph.UPPER_LIMIT_IntPurview 
 = SCALE_INTSEC*vpFilter->VP_RANGE_H +  (SCALE_INTSEC*vpFilter->STEP)/2;

 mTrackingLanesGraph.SCALED_STEP_LANE_FILTER	 = laneFilter->STEP*SCALE_INTSEC;
 mTrackingLanesGraph.SCALED_STEP_VP_FILTER       = vpFilter->STEP*SCALE_INTSEC;
 mTrackingLanesGraph.SCALED_START_LANE_FILTER    = laneFilter->HISTOGRAM_BINS(0)*SCALE_INTSEC;
 mTrackingLanesGraph.SCALED_START_VP_FILTER	     = vpFilter->HISTOGRAM_BINS(0)*SCALE_INTSEC;


 this->currentStatus= StateStatus::ACTIVE;	
	
}


void TrackingLaneState::run()
{
	#ifdef PROFILER_ENABLED
			mProfiler.start("SingleRun_TRACK");
		#endif
		
		if (mSideExecutor.joinable())
			mSideExecutor.join();
			
		mSideExecutor =
		#ifndef s32v2xx
		std::thread(&TrackingLanesDAG_generic::auxillaryTasks, std::ref(mTrackingLanesGraph));
		#endif
		
		
		//Grab and Buffer Required Frames
		if (0==mTrackingLanesGraph.grabFrame())
		{
			mTrackingLanesGraph.buffer();
			mTrackingLanesGraph.extractLanes();
		}			
		else
			currentStatus = StateStatus::ERROR;	

		
		this->StateCounter++;
		
		 #ifdef PROFILER_ENABLED
			 mProfiler.end();
			 LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
			  <<"******************************"<<endl
			  <<  "Completing a TrackingLanes run." <<endl
			  <<  "Single run-loop time: " << mProfiler.getAvgTime("SingleRun_TRACK")<<endl
			  <<"******************************"<<endl<<endl;	
		 #endif
	
}

TrackingLaneState::~TrackingLaneState()
{
	
		if (mSideExecutor.joinable())
		   mSideExecutor.join();	
}

