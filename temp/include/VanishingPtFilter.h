#ifndef VANSIHINGPOINFILTER_H
#define VANSIHINGPOINFILTER_H
#include  "LaneFilter.h"
#include <memory>
#include <Eigen/Dense>
#include <math.h>
#include "ScalingFactors.h"
#include "Lane.h"
#include "Camera.h"

using namespace std;
using namespace Eigen;

typedef Matrix<int, 6, 1> Vector6i;


struct VanishingPt
{
  int  V;
  int  H;
  int  V_prev;
  int  H_prev;
  VanishingPt()
  : V(0),H(0),V_prev(0),H_prev(0){}
};

struct PurviewHistogramModel
{
	int  binID_leftBoundary;
	int  binID_rightBoundary;
	
	int  binID_NegBoundaryLeft;
	int  binID_NegBoundaryRight;
	int  nbNonBoundaryBinsLeft;
	int  nbNonBoundaryBinsRight;
	
	PurviewHistogramModel()
	: binID_leftBoundary(-1),binID_rightBoundary(-1),
	 binID_NegBoundaryLeft(-1),binID_NegBoundaryRight(-1),
	 nbNonBoundaryBinsLeft(0), nbNonBoundaryBinsRight(0)
	{

	}
	
};




class VanishingPtFilter
{

		
public:

	VanishingPtFilter(const Ref<const VectorXi>& LANE_HISTOGRAM_BINS, 
					  const int&  LANE_FILTER_OFFSET_V,
					  const Camera& CAMERA);
					  
	~VanishingPtFilter();




private: 
        	const int       mVP_STEP;         // Step for VP Bins

public: 
		const int 	VP_RANGE_V;
		const int 	VP_RANGE_H;
					
		const int    	mNb_VP_BINS_V;    // number of bins in the  vertical   direction.
		const int    	mNb_VP_BINS_H;    // number of bins in the horizental direction.

		const VectorXi 	VP_BINS_V;	 //Histogram Bins in Pixels.
		const VectorXi 	VP_BINS_H;     	 //Histogram Bins in Pixels.
		const int  	OFFSET_V;
		
		const float     mVP_LANE_RATIO;			


public:			
			
		const VectorXi  HISTOGRAM_BINS;	    //Purview Histogram Bins in Pixels.
		const int    	STEP;  		    //Purview Histogram STEP
		      
	    	Mat  prior;
	    	Mat  filter;

				  		
private:		 
		void  createPrior();

};
	
#endif // VANSIHINGPOINFILTER_H
