#ifndef VANSIHINGPOINFILTER_H
#define VANSIHINGPOINFILTER_H
#include  "LaneFilter.h"
#include <memory>
#include <Eigen/Dense>
#include <math.h>
#include "Lane.h"
#include "Camera.h"

using namespace std;
using namespace Eigen;

typedef Matrix<int, 6, 1> Vector6i;


struct PurviewHistogramModel
{
	Vector6i binIDs_Boundary;
	Vector6i Weights_Boundary;	
	std::vector<int> binIDs_NegBoundary;
	
	PurviewHistogramModel()
	{
		binIDs_Boundary    << -1, -1, -1, -1, -1, -1;
		Weights_Boundary   << 0.25,1,0.25, 0.25,1,0.25 ;
	}
	
};




	class VanishingPtFilter
{

		
	public:

		VanishingPtFilter(const Ref<const VectorXi>& LANE_HISTOGRAM_BINS, 
						  const int&  LANE_FILTER_OFFSET_V,
						  const Camera& CAMERA);
						  
		~VanishingPtFilter();




private: //Internal variables
	        const int       mVP_STEP;         // Step for VP Bins

public: 
			const int 	 	VP_RANGE_V;
			const int 	 	VP_RANGE_H;

private:						
			const int    	mNb_VP_BINS_V;    // number of bins in the  vertical   direction.
			const int    	mNb_VP_BINS_H;    // number of bins in the horizental direction.

public:
			const VectorXi 	VP_BINS_V;	    //Histogram Bins in Pixels.
			const VectorXi 	VP_BINS_H;     	//Histogram Bins in Pixels.
			const int  	 	OFFSET_V;
			
private:	const float     mVP_LANE_RATIO;			


public:			
			
			const VectorXi  HISTOGRAM_BINS;	    //Purview Histogram Bins in Pixels.
			const int    	STEP;  				//Purview Histogram STEP
			      
		    Mat  prior;
		    Mat  filter;
		    Mat  transition; 
				  		
private:		 
			void  createPrior();

};
	
#endif // VANSIHINGPOINFILTER_H
