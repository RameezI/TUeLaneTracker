#include "VanishingPtFilter.h"


VanishingPtFilter::VanishingPtFilter(const Ref<const VectorXi>& LANE_HISTOGRAM_BINS, 
									 const int&  LANE_FILTER_OFFSET_V,
									 const Camera& CAMERA)
: 
  mVP_STEP(10),
  
  mVP_RANGE_V(25), 
  
  mVP_RANGE_H(300),
  
  mNb_VP_BINS_V(floor((2*mVP_RANGE_V)/mVP_STEP) +1),
  
  mNb_VP_BINS_H(floor((2*mVP_RANGE_H)/mVP_STEP) +1),
  
  VP_BINS_V( VectorXi::LinSpaced(mNb_VP_BINS_V,-mVP_RANGE_V, mVP_RANGE_V).array() + CAMERA.FRAME_CENTER(0)),
  
  VP_BINS_H( VectorXi::LinSpaced(mNb_VP_BINS_H,-mVP_RANGE_H, mVP_RANGE_H).array() + CAMERA.FRAME_CENTER(1) ),
  
  OFFSET_V(-120),
  
  mVP_LANE_RATIO((float)OFFSET_V / LANE_FILTER_OFFSET_V),
  
  HISTOGRAM_BINS( round((LANE_HISTOGRAM_BINS.cast<float>()*(mVP_LANE_RATIO)).array()).cast<int>() ),
  
  STEP(HISTOGRAM_BINS(1)- HISTOGRAM_BINS(0))
  
{
	createPrior();
	this->filter = this->prior;

}

void VanishingPtFilter::createPrior()
{
	
	/*Fill the histogram */

	this->prior  = MatrixXf(mNb_VP_BINS_V, mNb_VP_BINS_H);
    float  sigma = 5.0*mVP_RANGE_V/mVP_STEP;
    double pv,ph,p;

	
    for (int v = 1; v <= mNb_VP_BINS_V; v++)
	{
       for (int h = 1; h <= mNb_VP_BINS_H; h++)
	   {  
            pv = exp( -pow(v-mNb_VP_BINS_V/2.0,2)  / pow(sigma,2) );     
            ph = exp( -pow(h-mNb_VP_BINS_H/2.0,2)  / pow(sigma,2) );     
            p= pv*ph;
		    this->prior(v-1,h-1) = p;                
	   }    
    }
    
    // normalize
    this->prior = (this->prior/this->prior.sum()).eval();
    
    // Transition Matrix 
     this->transition = Matrix3d::Constant(3,3,1);
	  
}



VanishingPtFilter::~VanishingPtFilter()
{
}

