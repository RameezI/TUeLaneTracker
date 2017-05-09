/*
This class provides LaneFilter expressed in Vanishing point coordinate system.
*/
#include "LaneFilter.h"

LaneFilter::LaneFilter(const Lane& LANE,  const Camera& CAMERA)

: mLANE(LANE),

  mCAMERA(CAMERA),
  
  mSTEP_CM(5),
  
  STEP(ceil((mSTEP_CM*mCAMERA.CM_TO_PIXEL)/10)*10),
  
  mBIN_MAX(round((mLANE.MAX_WIDTH*mCAMERA.CM_TO_PIXEL)/STEP)*STEP ),
  
  mNb_HISTOGRAM_BINS(floor((2*mBIN_MAX)/STEP) +1),
  
  mNb_OFFSET_BINS(floor((mNb_HISTOGRAM_BINS-1)/2) +1),

  OFFSET_V(-240),
  
  HISTOGRAM_BINS(VectorXi::LinSpaced(mNb_HISTOGRAM_BINS,-mBIN_MAX, mBIN_MAX)),
  
  OFFSET_BINS(HISTOGRAM_BINS.tail(mNb_OFFSET_BINS))
  
{
	
	createHistogramModels();
	this->filter = this->prior;
	 
}




void LaneFilter::createHistogramModels()
{
	/* Create Histogram Models for the BaseHistogram */
	/* Assign probabilities to States  */
    
	this->prior  = MatrixXf((int)(mBIN_MAX/this->STEP) +1, (int)(mBIN_MAX/this->STEP) +1);
	
	VectorXf bins_cm = OFFSET_BINS.cast<float>()*(1/mCAMERA.CM_TO_PIXEL);
	
	
	
	
    float 	 hmean =  mLANE.AVG_WIDTH/2;
    float    sigmaL = mLANE.STD_WIDTH;
	
    float pL, pR, width;

	
    for (int left = 0; left < bins_cm.size(); left++)
	{
       for (int right = 0; right < bins_cm.size(); right++)
	   {  
            // prior on location
            pL = exp( -pow(hmean-bins_cm(left), 2) / (2*pow(8*sigmaL,2)) ) / ( sqrt(2*M_PI)*8*sigmaL );     
            pR = exp( -pow(hmean-bins_cm(right), 2) / (2*pow(8*sigmaL,2)) ) / ( sqrt(2*M_PI)*8*sigmaL );     
            
            //prior on lane width
            width = bins_cm(left)+bins_cm(right);
			
            
			if (mLANE.MIN_WIDTH <= width && width <= mLANE.MAX_WIDTH)
                
			{
				
				
				
				/* TO Histogram Bins-IDs*/
				int idxL = (mNb_OFFSET_BINS-1) - left;
				int idxR = (mNb_OFFSET_BINS-1) + right;
				
				int idxM = round((idxL+idxR)/2.0);
				
				int nbLeftNonBoundaryBins  = (idxM-3) - (idxL +2);
				int nbRightNonBoundaryBins = (idxR-2) - (idxM +3);
 				
				
				if( 0 < idxL && idxR < mNb_HISTOGRAM_BINS-1 )
				{	
					baseHistogramModels.push_back( BaseHistogramModel());
				
					baseHistogramModels.back().leftOffsetIdx  = left;
					baseHistogramModels.back().rightOffsetIdx = right; 
					
					baseHistogramModels.back().binIDs_leftBoundary  <<  left-1,  left, left+1 ;
					baseHistogramModels.back().binIDs_rightBoundary <<  right-1,  right, right+1 ;
					
					for(int i=0; i<nbLeftNonBoundaryBins; i++)
						baseHistogramModels.back().binIDs_NegBoundary.push_back(idxL+2 +i);
					for(int i=0; i< nbRightNonBoundaryBins; i++)
						baseHistogramModels.back().binIDs_NegBoundary.push_back(idxM+4 +i);
						
						
						vector<int> v;
						v= baseHistogramModels.back().binIDs_NegBoundary;
						int a= v.at(3);
						
					this->prior(left,right) = pL*pR;
										
				}
				else
					this->prior(left,right) = 0;                
			}    
		}
	}
    
    // normalize
    this->prior = (this->prior/this->prior.sum()).eval();
    
    // Transition Matrix 
     this->transition = Matrix7f::Constant(7,7,1);
     this->transition = (this->transition/ this->transition.sum()).eval(); 
	  
}

LaneFilter::~LaneFilter()
{
	
}