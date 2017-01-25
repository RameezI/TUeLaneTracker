#include "LaneObservation.h"

LaneObservation::LaneObservation(const shared_ptr<const Lane>  lane, const shared_ptr<const Camera> camera)
: mLane(lane),
  mCamera(camera)
{
	
	createLaneObservationModel(laneFilter->mBins);
	
	
}


template <typename Derived>
void LaneObservation::createLaneObservationModel(const MatrixBase<Derived>& Bins)
{
	float width;
	
	VectorXd bins=Bins.tail(mLaneFilter->mNbins_oneside);
	

		
	
	//VectorXd bins = mLaneFilter->mBins
	
	//VectorXd bins = mLaneFilter->mBins.tail(mLaneFilter->mNbins_oneside)
}



LaneObservation::~LaneObservation()
{
}

