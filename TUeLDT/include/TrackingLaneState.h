#ifndef TRACKING_LANE_STATE_H
#define TRACKING_LANE_STATE_H

/******************************************************************************
* NXP Confidential Proprietary
* 
* Copyright (c) 2017 NXP Semiconductor;
* All Rights Reserved
*
* AUTHOR : Rameez Ismail
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
* ****************************************************************************/ 

#include "State.h"
#include "ScalingFactors.h"

#ifdef   S32V2XX
 #include "TrackingLaneDAG_s32v.h"
#else
 #include "TrackingLaneDAG_generic.h"
#endif


template<typename GRAPH>
class TrackingLaneState: public State
{
	
private:
	uint_fast8_t mRetryGrab;
	std::thread  mSideExecutor;
	GRAPH 	     mGraph;


public:	
	const LaneModel& run(cv::Mat Frame);
	void setupDAG(LaneFilter* laneFilters, VanishingPtFilter* vpFilter);

	template<typename GRAPH_BASE>
	TrackingLaneState(GRAPH_BASE&& lBaseGraph);
};


//******************************************************************/*
// Class Definitions
//Templated class thus declarations and definitions in a single file
//******************************************************************/

template<typename GRAPH>
template<typename GRAPH_BASE>
TrackingLaneState<GRAPH>::TrackingLaneState(GRAPH_BASE&& lBaseGraph)
:  mRetryGrab(0),
   mGraph(std::move(lBaseGraph))
{	
		
}


template<typename GRAPH>
void TrackingLaneState<GRAPH>::setupDAG(LaneFilter* laneFilter, VanishingPtFilter* vpFilter)
{
   if (0 == mGraph.init_DAG(laneFilter, vpFilter))
   this->currentStatus= StateStatus::ACTIVE;	
}


template<typename GRAPH>
const LaneModel& TrackingLaneState<GRAPH>::run(cv::Mat Frame)
{
   try
   {
      mGraph.execute(Frame);
      StateCounter++;
   }
   catch(...)
   {
     currentStatus = StateStatus::ERROR;
   }

   return mGraph.mLaneModel;
}

#endif // TRACKING_LANE_STATE_H
