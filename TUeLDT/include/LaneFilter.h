#ifndef LANEFILTER_H
#define LANEFILTER_H
#include <Eigen/Dense>
#include <math.h>
#include "Lane.h"
#include "Camera.h"

using namespace std;
using namespace Eigen;
typedef Matrix<float, 7, 7> Matrix7f;

	
	class LaneFilter
	{
		
	private:
	
		const Lane   mLANE;
		const Camera mCAMERA;
		const int 	 mSTEP_CM;

public:	 //Public Interface 
		const int 	 STEP;

private: 
		 const int 	 mPX_MAX;
		 const int   mNb_BINS;         	// number of bins in Observation Histogram.
		 const int   mNb_OFFSETS;    		// number of bins in the mFilter.
		
		
	public:	 //Public Interface of the class
	
		const int 		LANE_FILTER_OFFSET_V;   // Offset in VP coordinate System
		const VectorXi  HISTOGRAM_BINS;     	// -PX_MAX:STEP: PX_MAX
		const VectorXi  OFFSET_BINS;        	//  0 : STEP : PX_MAX
		
		
		
		MatrixXf     	Prior;
		MatrixXf 	 	Filter;
		Matrix7f	 	Transition; 		
		
		 
    private:	
		void  createPrior();
		void  createExpectedHistogram();	

	public:	
		LaneFilter(const Lane& LANE,  const Camera& CAMERA);
	   ~LaneFilter();
	};
#endif // LANEFILTER_H