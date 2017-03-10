#include "VanishingPtFilter.h"


VanishingPtFilter::VanishingPtFilter(const Ref<const VectorXi>& LANE_HISTOGRAM_BINS, 
									 const int&  LANE_FILTER_OFFSET_V,
									 const Camera& CAMERA)
: 
  mSTEP(10),
  
  mRANGE_V(25), 
  
  mRANGE_H(300),
  
  mNb_BINS_V(floor((2*mRANGE_V)/mSTEP) +1),
  
  mNb_BINS_H(floor((2*mRANGE_H)/mSTEP) +1),
  
  mBINS_V( VectorXi::LinSpaced(mNb_BINS_V,-mRANGE_V, mRANGE_V).array() + CAMERA.FRAME_CENTER(0)),
  
  mBINS_H( VectorXi::LinSpaced(mNb_BINS_H,-mRANGE_H, mRANGE_H).array() + CAMERA.FRAME_CENTER(1) ),
  
  VP_FILTER_OFFSET_V(-120),
  
  
  mVP_LANE_RATIO((float)VP_FILTER_OFFSET_V / LANE_FILTER_OFFSET_V),
  
  HISTOGRAM_BINS( round((LANE_HISTOGRAM_BINS.cast<float>()*(mVP_LANE_RATIO)).array()).cast<int>() ),
  
  STEP(HISTOGRAM_BINS(1)- HISTOGRAM_BINS(0))
  
{
	createPrior();
	this->Filter = this->Prior;

}

void VanishingPtFilter::createPrior()
{
	
	/*Fill the histogram */
    
	
	this->Prior  = MatrixXf(mNb_BINS_V, mNb_BINS_H);
    float  sigma = 5.0*mRANGE_V/mSTEP;
    double pv,ph,p;

	
    for (int v = 1; v <= mNb_BINS_V; v++)
	{
       for (int h = 1; h <= mNb_BINS_H; h++)
	   {  
            pv = exp( -pow(v-mNb_BINS_V/2.0,2)  / pow(sigma,2) );     
            ph = exp( -pow(h-mNb_BINS_H/2.0,2)  / pow(sigma,2) );     
            p= pv*ph;
		    this->Prior(v-1,h-1) = p;                
	   }    
    }
    
    // normalize
    this->Prior = (this->Prior/this->Prior.sum()).eval();
    
    // Transition Matrix 
     this->Transition = Matrix3d::Constant(3,3,1);
	  
}



VanishingPtFilter::~VanishingPtFilter()
{
}

