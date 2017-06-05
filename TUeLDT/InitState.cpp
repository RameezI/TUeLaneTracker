#include "InitState.h"
 

InitState::InitState()
: mLaneFilterCreated(false),
  mVpFilterCreated  (false),
  mTemplatesCreated (false)
{
	// This state is ready at instantiations to do the tasks
	// No Dependncies required
	// Generate Dependencies and flag DONE when all dependencies are generated
	
	this->currentStatus = StateStatus::ACTIVE;
}


unique_ptr<LaneFilter> InitState::createLaneFilter()
{
	
	Camera camera;
	Lane   lane;
	
	unique_ptr<LaneFilter> laneFilter( new LaneFilter (lane, camera) );
	mLaneFilterCreated = true;
	
	if (checkCreationStatus())
		currentStatus= StateStatus::DONE;
	
	return laneFilter;	
}


unique_ptr<VanishingPtFilter> InitState::createVanishingPtFilter()
{
	Camera camera;
	Lane   lane;
	LaneFilter laneFilter(lane, camera);
	
	unique_ptr<VanishingPtFilter> vanishingPtFilter(new VanishingPtFilter (laneFilter.HISTOGRAM_BINS, 
																	       laneFilter.OFFSET_V,																		   camera) );
	mVpFilterCreated =true;
		
	if (checkCreationStatus())
		currentStatus= StateStatus::DONE;
	
	return vanishingPtFilter; 	
}


unique_ptr<Templates> InitState::createTemplates()
{
	Camera camera;
	Lane   lane;
	LaneFilter 		  laneFilter(lane, camera);
	VanishingPtFilter vanishingPtFilter(laneFilter.HISTOGRAM_BINS, laneFilter.OFFSET_V, camera);
	
	unique_ptr<Templates> templates ( new  Templates (camera.RES_VH(0),
													  camera.RES_VH(1),
													  vanishingPtFilter.VP_RANGE_V) );
	mTemplatesCreated= true;
	
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
