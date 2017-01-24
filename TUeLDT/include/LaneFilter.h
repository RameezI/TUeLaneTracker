#ifndef LANEFILTER_H
#define LANEFILTER_H
#include <memory>
#include <Eigen/Dense>
#include <math.h>
#include "Lane.h"
#include "Camera.h"

using namespace std;
using namespace Eigen;
typedef Matrix<double, 7, 7> Matrix7d;


class LaneFilter
{
	
public:  //make public for testing
		//shared_ptr<Lane>  mLane;
		const shared_ptr<const Lane>  mLane;
		const shared_ptr<const Camera> mCamera;
		void createLanePrior();
	

public:
	const int 	 STEP_CM;
	const int 	 STEP_PX;
	const int 	 PX_MAX;
	const int  	 FILTER_OFFSET;
	const float	 CONF_THRESH;
	VectorXd     mBins;         //Histogram Bins in Pixels
	MatrixXd     mPrior;
	MatrixXd 	 mFilter;
	Matrix7d	 mTransition;
	

	//const Ref<const MatrixXd>& getFilter();
	//shared_ptr<MatrixXd> getPrior();
	//shared_ptr<MatrixXd> getTransition(); //^TODO: Make it Private Memeber if only used by LaneFilter class.
	
	
	LaneFilter(const shared_ptr<const Lane>, const shared_ptr<const Camera>);
	
	
   ~LaneFilter();




};

#endif // LANEFILTER_H
