#include "VanishingPtFilter.h"


VanishingPtFilter::VanishingPtFilter(const Ref<const VectorXi>& LANE_HISTOGRAM_BINS, 
									 const int&  LANE_FILTER_OFFSET_V,
									 const Camera& CAMERA)
: 
  mVP_STEP(10),
  
  VP_RANGE_V(25), 
  
  VP_RANGE_H(300),
  
  mNb_VP_BINS_V(floor((2*VP_RANGE_V)/mVP_STEP) +1),
  
  mNb_VP_BINS_H(floor((2*VP_RANGE_H)/mVP_STEP) +1),
  
  VP_BINS_V( VectorXi::LinSpaced(mNb_VP_BINS_V,-VP_RANGE_V, VP_RANGE_V).array()),
  
  VP_BINS_H( VectorXi::LinSpaced(mNb_VP_BINS_H,-VP_RANGE_H, VP_RANGE_H).array()),
  
  OFFSET_V(-120),
  
  mVP_LANE_RATIO((float)OFFSET_V / LANE_FILTER_OFFSET_V),
  
  HISTOGRAM_BINS( round((LANE_HISTOGRAM_BINS.cast<float>()*(mVP_LANE_RATIO)).array()).cast<int>() ),
  
  STEP(HISTOGRAM_BINS(1)- HISTOGRAM_BINS(0)),
  
  prior(  Mat::zeros( mNb_VP_BINS_V, mNb_VP_BINS_H , CV_32SC1) ),
  
  filter( Mat::zeros( mNb_VP_BINS_H, mNb_VP_BINS_H,  CV_32SC1) )
{
	createPrior();
	this->filter = this->prior.clone();
}

void VanishingPtFilter::createPrior()
{
	
	float  sigma = 5.0*VP_RANGE_V/mVP_STEP;
	double pv,ph,p;
	
	for (int v = 1; v <= mNb_VP_BINS_V; v++)
   	{
           for (int h = 1; h <= mNb_VP_BINS_H; h++)
	   {  
            pv = exp( -pow(v-mNb_VP_BINS_V/2.0,2)  / pow(sigma,2) ) *128 ;     
            ph = exp( -pow(h-mNb_VP_BINS_H/2.0,2)  / pow(sigma,2) ) *128;  
			
	    this->prior.at<int>(v-1,h-1) = (int)(std::round(pv*ph));                
	   }    
    	}

	// Normalize
	int32_t SUM = cv::sum(this->prior)[0];
	this->prior.convertTo(this->prior,CV_32SC1,SCALE_FILTER);
	this->prior = this->prior/(SUM);
	  
}



VanishingPtFilter::~VanishingPtFilter()
{
}

