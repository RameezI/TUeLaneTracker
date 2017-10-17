#ifndef LANEFILTER_H
#define LANEFILTER_H
#include <Eigen/Dense>
#include <math.h>
#include "ScalingFactors.h"
#include "Lane.h"
#include "Camera.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace Eigen;
using namespace cv;

struct BaseHistogramModel
{
	int   leftOffsetIdx;
	int   rightOffsetIdx;
	int   leftOffset;
	int   rightOffset;
	float width_cm;
		
	int  binID_leftBoundary;
	int  binID_rightBoundary;
	
	int  binID_NegBoundaryLeft;
	int  nbNonBoundaryBinsLeft;
	int  binID_NegBoundaryRight;
	int  nbNonBoundaryBinsRight;
		
	
	BaseHistogramModel()
	: leftOffsetIdx(-1),
	  rightOffsetIdx(-1),
	  leftOffset(-1),
	  rightOffset(-1),
	  width_cm(0),
	  binID_leftBoundary(-1),
	  binID_rightBoundary(-1)
	{


	}
	
};

class LaneFilter
{
	
private:	
		const LaneParameters   	mLANE;
		const Camera 		mCAMERA;
		const int		mSTEP_CM;     // BaseHistogram Step in cm

public:	 //Public Interface 
		const int 	STEP;   		// BaseHistogram Step in Pixels 
		const int 	mBIN_MAX;		// BaseHistogram Max Bin Value
		const int   	mNb_HISTOGRAM_BINS;    	// number of bins in the Base Histogram.
		const int   	mNb_OFFSET_BINS;	// number of offset bins on one side.
		
public:	 //Public Interface of the class	

		const int 	OFFSET_V;   		// Vertical Offset (BaseHistogram) in VP coordinate System
		const VectorXi  HISTOGRAM_BINS;    	// -PX_MAX:STEP: PX_MAX
		const VectorXi  OFFSET_BINS;       	//  0 : STEP : PX_MAX
		
		Mat prior;
		Mat filter;

		std::vector<BaseHistogramModel>  baseHistogramModels; 	

private:	
		void  createHistogramModels();	

public:	
		LaneFilter(const LaneParameters& LANE,  const Camera& CAMERA);
	   	~LaneFilter();
};
	
#endif // LANEFILTER_H
