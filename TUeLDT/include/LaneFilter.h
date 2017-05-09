#ifndef LANEFILTER_H
#define LANEFILTER_H
#include <Eigen/Dense>
#include <math.h>
#include "Lane.h"
#include "Camera.h"

using namespace std;
using namespace Eigen;
typedef Matrix<float, 7, 7> Matrix7f;


struct BaseHistogramModel
{
	int leftOffsetIdx;
	int rightOffsetIdx;
	
	Vector3i binIDs_leftBoundary;
	Vector3i binIDs_rightBoundary;
	Vector3i Weights_leftBoundary;
	Vector3i Weights_rightBoundary;
	
	std::vector<int> binIDs_NegBoundary;
	
	BaseHistogramModel()
	: leftOffsetIdx(-1),
	  rightOffsetIdx(-1)
	{
		binIDs_leftBoundary    << -1,-1,-1;
		binIDs_rightBoundary   << -1,-1,-1;
		Weights_leftBoundary   << 0.25,1,0.25;
		Weights_rightBoundary  << 0.25,1,0.25;

	}
	
};

	class LaneFilter
{
	
private:	
		const Lane   mLANE;
		const Camera mCAMERA;
		const int 	 mSTEP_CM; // BaseHistogram Step in cm


public:	 //Public Interface 
		const int 	 STEP;   // BaseHistogram Step in Pixels

private: 
		 const int 	 mBIN_MAX; 				// BaseHistogram Max Bin Value
		 const int   mNb_HISTOGRAM_BINS;    // number of bins in the Base Histogram.
		 const int   mNb_OFFSET_BINS;       // number of offset bins on one side.
		
		
public:	 //Public Interface of the class	

		const int 		OFFSET_V;   		// Vertical Offset (BaseHistogram) in VP coordinate System
		const VectorXi  HISTOGRAM_BINS;    	// -PX_MAX:STEP: PX_MAX
		const VectorXi  OFFSET_BINS;       	//  0 : STEP : PX_MAX
		
		
		
		MatrixXf     	prior;
		MatrixXf 	 	filter;
		Matrix7f	 	transition;

	    std::vector<BaseHistogramModel>  baseHistogramModels; 	

    private:	
		void  createHistogramModels();	

	public:	
		LaneFilter(const Lane& LANE,  const Camera& CAMERA);
	   ~LaneFilter();
	};
#endif // LANEFILTER_H