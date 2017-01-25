#include "VanishingPtFilter.h"


VanishingPtFilter::VanishingPtFilter(const Ref<const VectorXi>& BINS_LANE_HISTOGRAM, const CameraProperties& CAMERA)
: mRANGE_V(25), 
  mRANGE_H(300),
  mSTEP(10),
  mFRAME_CENTER_V(CAMERA.FRAME_CENTER(0)),
  mFRAME_CENTER_H(CAMERA.FRAME_CENTER(1)),
  mLANE_FILTER_OFFSET_V(-240),
  mVP_FILTER_OFFSET_V(-120),
  mVP_LANE_RATIO((float)mVP_FILTER_OFFSET_V / mLANE_FILTER_OFFSET_V),
  mNb_BINS_V(floor((2*mRANGE_V)/mSTEP) +1),
  mNb_BINS_H(floor((2*mRANGE_H)/mSTEP) +1),
  mBINS_V( VectorXi::LinSpaced(mNb_BINS_V,-mRANGE_V, mRANGE_V).array() + mFRAME_CENTER_V),
  mBINS_H( VectorXi::LinSpaced(mNb_BINS_H,-mRANGE_H, mRANGE_H).array() + mFRAME_CENTER_H ),
  mBINS_HISTOGRAM( round((BINS_LANE_HISTOGRAM.cast<float>()*(mVP_LANE_RATIO)).array()).cast<int>() )
{
	createPrior();
	mFilter = mPrior;

	
}

void VanishingPtFilter::createPrior()
{
	
	/*Fill the histogram */
    
	
	mPrior  = MatrixXd(mNb_BINS_V, mNb_BINS_H);
    float  sigma = 5.0*mRANGE_V/mSTEP;
    double pv,ph,p;

	
    for (int v = 1; v <= mNb_BINS_V; v++)
	{
       for (int h = 1; h <= mNb_BINS_H; h++)
	   {  
            pv = exp( -pow(v-mNb_BINS_V/2.0,2)  / pow(sigma,2) );     
            ph = exp( -pow(h-mNb_BINS_H/2.0,2)  / pow(sigma,2) );     
            p= pv*ph;
		    mPrior(v-1,h-1) = p;                
	   }    
    }
    
    // normalize
    mPrior = (mPrior/mPrior.sum()).eval();
    
    // Transition Matrix 
     mTransition = Matrix3d::Constant(3,3,1);
	  
}



VanishingPtFilter::~VanishingPtFilter()
{
}

