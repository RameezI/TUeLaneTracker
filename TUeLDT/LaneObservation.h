#ifndef LANEOBSERVATION_H
#define LANEOBSERVATION_H
#include <memory>
#include <Eigen/Dense>
#include <math.h>
#include "LaneFilter.h"


using namespace std;
using namespace Eigen;


class LaneObservation
{
private:	
       const shared_ptr<const LaneFilter> mLaneFilter;
	
public:

	LaneObservation(const shared_ptr<const LaneFilter>);
	~LaneObservation();
	void createLaneObservationModel();

};

#endif // LANEOBSERVATION_H
