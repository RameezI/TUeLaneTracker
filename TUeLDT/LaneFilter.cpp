/*
This class provides LaneFilter expressed in Vanishing point coordinate system.
*/
#include "LaneFilter.h"

LaneFilter::LaneFilter(const shared_ptr<const Lane> lane, const shared_ptr<const Camera> camera)

: 
  mLane(lane),
  mCamera(camera),
  STEP_CM(5),
  STEP_PX(ceil((STEP_CM* mCamera->mCM_TO_PIXEL)/10)*10),
  mBins(VectorXd::LinSpaced(floor((2*PX_MAX)/STEP_PX) +1 ,-PX_MAX, PX_MAX)),
  PX_MAX(round((mLane->mProperties.MAX_WIDTH*mCamera->mCM_TO_PIXEL)/STEP_PX)*STEP_PX ),
  FILTER_OFFSET(-240),
  CONF_THRESH(2)
{
	
	createLanePrior();
	mFilter = mPrior;
	 
}

void LaneFilter::createLanePrior()
{
	
	/*Fill the histogram */
    
	
	mPrior  = MatrixXd((int)(PX_MAX/STEP_PX) +1, (int)(PX_MAX/STEP_PX) +1);
	
	VectorXd bins_cm = mBins.tail((int)(PX_MAX/STEP_PX) +1)* (1/mCamera->mCM_TO_PIXEL);
	
    float 	 hmean = mLane->mProperties.AVG_WIDTH/2;
    float    sigmaL = mLane->mProperties.STD_WIDTH;
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
            if (mLane->mProperties.MIN_WIDTH <= width && width <= mLane->mProperties.MAX_WIDTH)
                mPrior(x,y) = pL*pR;
		    else
                mPrior(x,y) = 0;                
	   }    
    }
    
    // normalize
    mPrior = (mPrior/mPrior.sum()).eval();
    
    // Transition Matrix 
     mTransition = Matrix7d::Constant(7,7,1);
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