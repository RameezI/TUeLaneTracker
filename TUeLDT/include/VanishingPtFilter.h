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

	class VanishingPtFilter
{

		
	public:

		VanishingPtFilter(const Ref<const VectorXi>& LANE_HISTOGRAM_BINS, 
						  const int&  LANE_FILTER_OFFSET_V,
						  const Camera& CAMERA);
						  
		~VanishingPtFilter();




private: //Internal variables
	        const int       mSTEP;         // Without adjusting for the ratio between Lane Filter and vpFilter
			const int 	 	mRANGE_V;
			const int 	 	mRANGE_H;
						
			const int    	mNb_BINS_V;    	// number of bins in the  vertical   direction.
			const int    	mNb_BINS_H;    	// number of bins in the horizental direction.

			const VectorXi 	mBINS_V;	    //Histogram Bins in Pixels.
			const VectorXi 	mBINS_H;     	// Histogram Bins in Pixels.
			
			


public: //The Public Interface
			
			const int  	 	VP_FILTER_OFFSET_V;
			
private:	const float     mVP_LANE_RATIO;			

public:			
			
			const VectorXi  HISTOGRAM_BINS;	    //Histogram Bins in Pixels.
			const int    	STEP;  				//After adjustment
			      
				  MatrixXf  Prior;
				  MatrixXf  Filter;
				  Matrix3d	Transition; 
				  		
private:		 
			void  createPrior();

};
	
#endif // VANSIHINGPOINFILTER_H
