#include "LaneObservation.h"

LaneObservation::LaneObservation(const shared_ptr<const LaneFilter> laneFilter)
: mLaneFilter(laneFilter)
{
}

LaneObservation::~LaneObservation()
{
}

