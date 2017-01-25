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
       const shared_ptr<const Lane> 	mLane;
	   const shared_ptr<const Camera> 	mCamera;
	
public:
	MatrixXd     mLeftLane;
	MatrixXd 	 mRightLane;
	MatrixXd     mNotLane;
	
	
	
	LaneObservation(const shared_ptr<const Lane>, const shared_ptr<const Camera>);
	~LaneObservation();
	
	template <typename Derived>
	void createLaneObservationModel(const MatrixBase<Derived>& a);

};

#endif // LANEOBSERVATION_H
