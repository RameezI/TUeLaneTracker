/*
This class provides LaneFilter expressed in Vanishing point coordinate system.
*/
#include "LaneFilter.h"

LaneFilter::LaneFilter(const Lane& LANE,  const Camera& CAMERA)

: mLANE(LANE),

  mCAMERA(CAMERA),
  
  mSTEP_CM(5),
  
  STEP(ceil((mSTEP_CM*mCAMERA.CM_TO_PIXEL)/10)*10),
  
  mPX_MAX(round((mLANE.MAX_WIDTH*mCAMERA.CM_TO_PIXEL)/STEP)*STEP ),
  
  mNb_BINS(floor((2*mPX_MAX)/STEP) +1),
  
  mNb_OFFSETS(floor((mNb_BINS-1)/2) +1),

  LANE_FILTER_OFFSET_V(-240),
  
  HISTOGRAM_BINS(VectorXi::LinSpaced(mNb_BINS,-mPX_MAX, mPX_MAX)),
  
  OFFSET_BINS(HISTOGRAM_BINS.tail(mNb_OFFSETS))
  
{
	
	createPrior();
	this->Filter = this->Prior;
	 
}


void LaneFilter::createExpectedHistogram()
{
	
	
	//^TODO: Sparce Representation could be used here
	
	//^TODO: Temporary Delay the Implementation, Consult with Gijs
	
	
	/*
	
	VectorXd bins=mBins.tail(mNbins_oneside);
	
	double ObsLeftLane, ObsRightLane, ObsNoLane;
	float width;
	
	double leftOffset, rightOffset;
	
	for(int left =1; left < mNbins_oneside; left++)
	{	
		for(int right =1; right< mNbins_oneside; right++)
		{
		
			
			//Allowed states
			  width = bins(left) + bins(right) * (1/mCamera->mCM_TO_PIXEL);
			  if (mLane->mProperties.MIN_WIDTH <= width && width <= mLane->mProperties.MAX_WIDTH)
			  {
				  leftOffset 	= - bins(left);
				  rightOffset	=   bins(right) 
				 
				  
			  }
		}		
	}	*/



}

void LaneFilter::createPrior()
{
	
	/*Assign probabilities to States */
    
	this->Prior  = MatrixXf((int)(mPX_MAX/this->STEP) +1, (int)(mPX_MAX/this->STEP) +1);
	
	VectorXf bins_cm = OFFSET_BINS.cast<float>()*(1/mCAMERA.CM_TO_PIXEL);
	
    float 	 hmean =  mLANE.AVG_WIDTH/2;
    float    sigmaL = mLANE.STD_WIDTH;
	
    double pL, pR, width;

	
    for (int x = 0; x < bins_cm.size(); x++)
	{
       for (int y = 0; y < bins_cm.size(); y++)
	   {  
            // prior on location
            pL = exp( -pow(hmean-bins_cm(x), 2) / (2*pow(8*sigmaL,2)) ) / ( sqrt(2*M_PI)*8*sigmaL );     
            pR = exp( -pow(hmean-bins_cm(y), 2) / (2*pow(8*sigmaL,2)) ) / ( sqrt(2*M_PI)*8*sigmaL );     
            
            //prior on lane width
            width = bins_cm(x)+bins_cm(y);
            if (mLANE.MIN_WIDTH <= width && width <= mLANE.MAX_WIDTH)
                this->Prior(x,y) = pL*pR;
		    else
                this->Prior(x,y) = 0;                
	   }    
    }
    
    // normalize
    this->Prior = (this->Prior/this->Prior.sum()).eval();
    
    // Transition Matrix 
     this->Transition = Matrix7f::Constant(7,7,1);
     this->Transition = (this->Transition/ this->Transition.sum()).eval(); 
	  
}


LaneFilter::~LaneFilter()
{
	
}




/*
const Ref<const MatrixXd>& LaneFilter::getFilter()
{	 
	return mFilter;

}
*/


/*
	shared_ptr<MatrixXd> LaneFilter::getFilter();
	{
		
	}
	shared_ptr<MatrixXd> LaneFilter::getPrior();
	{
		
	}
	
	shared_ptr<MatrixXd> LaneFilter::getTransition()
	{
	
	}
	*/