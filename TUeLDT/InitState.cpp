#include "InitState.h"
 

InitState::InitState()
: mLaneFilterCreated(false),
  mVpFilterCreated  (false),
  mTemplatesCreated (false)
{	
	this->currentStatus = StateStatus::ACTIVE;
}


unique_ptr<LaneFilter> InitState::createLaneFilter()
{
	
	Camera 			camera;
	LaneParameters		lane;

	unique_ptr<LaneFilter> 	laneFilter( new LaneFilter (lane, camera) );
	mLaneFilterCreated 	= true;

	if (checkCreationStatus())
	currentStatus= StateStatus::DONE;

	return laneFilter;	
}


unique_ptr<VanishingPtFilter> InitState::createVanishingPtFilter()
{
	Camera 			camera;
	LaneParameters   	lane;
	LaneFilter 		laneFilter(lane, camera);
	
	unique_ptr<VanishingPtFilter> 
	vanishingPtFilter(new VanishingPtFilter (laneFilter.HISTOGRAM_BINS, laneFilter.OFFSET_V) ); 
	   
	mVpFilterCreated 	=true;
		
	if (checkCreationStatus())
	currentStatus= StateStatus::DONE;
	
	return vanishingPtFilter; 	
}


unique_ptr<Templates> InitState::createTemplates()
{
	Camera 			camera;
	LaneParameters    	lane;
	LaneFilter 		laneFilter(lane, camera);
	VanishingPtFilter 	vanishingPtFilter(laneFilter.HISTOGRAM_BINS, laneFilter.OFFSET_V);
	
	unique_ptr<Templates>
	templates ( new  Templates (camera.RES_VH(0), camera.RES_VH(1), camera.FOV(0), vanishingPtFilter.VP_RANGE_V) );
	
	mTemplatesCreated	= true;
	
	if (checkCreationStatus())
	currentStatus= StateStatus::DONE;		
	
	return templates;
}


bool InitState::checkCreationStatus()
{
	if (mLaneFilterCreated && mVpFilterCreated && mTemplatesCreated)
	return true;
	
	else
	return false;
}

InitState::~InitState()
{	

	
}
