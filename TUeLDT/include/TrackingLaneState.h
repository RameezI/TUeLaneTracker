#ifndef TRACKINGLANESTATE_H
#define TRACKINGLANESTATE_H
#include "State.h"

#ifdef   s32v2xx
 #include "BufferingDAG_s32v.h"
 #include "TrackingLaneDAG_s32v.h"
#else
 #include "BufferingDAG_generic.h"
 #include "TrackingLaneDAG_generic.h"
#endif


class TrackingLaneState: public State
{

friend class TEST_TrackingState;	
	
private:
	uint_fast8_t mRetryGrab;
	std::thread mSideExecutor;

#ifdef s32v2xx
	TrackingLaneDAG_s32v	mTrackingLaneGraph;
#else
	TrackingLaneDAG_generic mTrackingLaneGraph;
#endif	

public:	
	void run();
	void setupDAG(LaneFilter* laneFilter, VanishingPtFilter* vpFilter);
	
public:	
#ifdef s32v2xx
	TrackingLaneState(BufferingDAG_s32v&& bufferingGraph);
#else
	TrackingLaneState(BufferingDAG_generic&& bufferingGraph);
#endif
	~TrackingLaneState();

};

#endif // TRACKINGLANESTATE_H
