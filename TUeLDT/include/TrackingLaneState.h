#ifndef TRACKINGLANESTATE_H
#define TRACKINGLANESTATE_H
#include "State.h"
#include "BufferingDAG_generic.h"
#include "TrackingLanesDAG_generic.h"
#include <thread>


class TrackingLaneState: public State
{

friend class TEST_TrackingState;	
	
private:
	std::thread mSideExecutor;
	TrackingLanesDAG_generic mTrackingLanesGraph;
	
public:	
	void run();
	void setupDAG(LaneFilter* laneFilter, VanishingPtFilter* vpFilter);
	
public:	
	TrackingLaneState(BufferingDAG_generic&& bufferingGraph);
	~TrackingLaneState();

};

#endif // TRACKINGLANESTATE_H
