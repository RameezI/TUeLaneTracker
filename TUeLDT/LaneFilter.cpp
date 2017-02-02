/*
This class provides LaneFilter expressed in Vanishing point coordinate system.
*/
#include "LaneFilter.h"

LaneFilter::LaneFilter(const LaneProperties& LANE,  const double& CM_TO_PIXEL)

: 
  mLANE(LANE),
  mCM_TO_PX(CM_TO_PIXEL),
  mSTEP_CM(5),
  mSTEP_PX(ceil((mSTEP_CM*mCM_TO_PX)/10)*10),
  mNb_BINS(floor((2*mPX_MAX)/mSTEP_PX) +1),
  mNb_OFFSETS(floor((mNb_BINS-1)/2) +1),
  mBINS_HISTOGRAM(VectorXi::LinSpaced(mNb_BINS,-mPX_MAX, mPX_MAX)),
  mBINS_FILTER(mBINS_HISTOGRAM.tail(mNb_OFFSETS)),
  mPX_MAX(round((mLANE.MAX_WIDTH*mCM_TO_PX)/mSTEP_PX)*mSTEP_PX ),
  mCONF_THRESH(2)
{
	
	createPrior();
	mFilter = mPrior;
	 
}


void LaneFilter::createHistograms()
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
	
	/*Fill the histogram */
    
	
	mPrior  = MatrixXf((int)(mPX_MAX/mSTEP_PX) +1, (int)(mPX_MAX/mSTEP_PX) +1);
	
	VectorXf bins_cm = mBINS_FILTER.cast<float>()*(1/mCM_TO_PX);
	
    float 	 hmean = mLANE.AVG_WIDTH/2;
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
                mPrior(x,y) = pL*pR;
		    else
                mPrior(x,y) = 0;                
	   }    
    }
    
    // normalize
    mPrior = (mPrior/mPrior.sum()).eval();
    
    // Transition Matrix 
     mTransition = Matrix7f::Constant(7,7,1);
     mTransition = (mTransition/ mTransition.sum()).eval(); 
	  
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