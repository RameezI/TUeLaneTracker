/******************************************************************************
* NXP Confidential Proprietary
* 
* Copyright (c) 2017 NXP Semiconductor;
* All Rights Reserved
*
* AUTHOR : Rameez Ismail
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
* ****************************************************************************/ 

#include "LaneFilter.h"
#include "ScalingFactors.h"

LaneFilter::LaneFilter(const LaneProperties& LANE,  const Camera& CAMERA)

: mLANE(LANE),

  mCAMERA(CAMERA),
  
  mSTEP_CM(10),
  
  STEP(mSTEP_CM*mCAMERA.CM_TO_PIXEL),
  
  mBIN_MAX(round((mLANE.MAX_WIDTH*mCAMERA.CM_TO_PIXEL)/STEP)*STEP ),
  
  mNb_HISTOGRAM_BINS(floor((2*mBIN_MAX)/STEP) +1),
  
  mNb_OFFSET_BINS(floor((mNb_HISTOGRAM_BINS-1)/2) +1),

  OFFSET_V(240),
  
  HISTOGRAM_BINS(VectorXi::LinSpaced(mNb_HISTOGRAM_BINS,-mBIN_MAX, mBIN_MAX)),
  
  OFFSET_BINS(HISTOGRAM_BINS.tail(mNb_OFFSET_BINS)),
  
  prior(  cv::Mat::zeros( (int)(mBIN_MAX/this->STEP) +1, (int)(mBIN_MAX/this->STEP) +1 , CV_32SC1) ),
  
  filter( cv::Mat::zeros( (int)(mBIN_MAX/this->STEP) +1, (int)(mBIN_MAX/this->STEP) +1 , CV_32SC1) )
  
  {
	createHistogramModels();
	this->filter = this->prior.clone();
  }


void LaneFilter::createHistogramModels()
{
	/* Create Histogram Models for the BaseHistogram */
	/* Assign probabilities to States  */
		
	VectorXf bins_cm = OFFSET_BINS.cast<float>()*(1/mCAMERA.CM_TO_PIXEL);
	
	float 	 hmean =  mLANE.AVG_WIDTH/2;
	float    sigmaL = mLANE.STD_WIDTH;
	
	float pL, pR, width;
	
	for (int left = 0; left < bins_cm.size(); left++)
	{
	  for (int right = 0; right < bins_cm.size(); right++)
	  {
		 // prior on location
		 pL = (exp( -pow(hmean-bins_cm(left), 2) / (2*pow(8*sigmaL,2)) ) / ( sqrt(2*M_PI)*8*sigmaL ) )*SCALE_FILTER;

		 pR = (exp( -pow(hmean-bins_cm(right), 2) / (2*pow(8*sigmaL,2)) ) / ( sqrt(2*M_PI)*8*sigmaL ))*SCALE_FILTER;

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

			   baseHistogramModels.back().leftOffsetIdx  			= left;
			   baseHistogramModels.back().rightOffsetIdx 			= right;

			   baseHistogramModels.back().leftOffset 				= OFFSET_BINS(left);
			   baseHistogramModels.back().rightOffset 				= OFFSET_BINS(right);
			   baseHistogramModels.back().width_cm = width;

			   baseHistogramModels.back().binID_leftBoundary  		= idxL;
			   baseHistogramModels.back().binID_rightBoundary 		= idxR;
			
			   baseHistogramModels.back().binID_NegBoundaryLeft  	= idxL+2;
			   baseHistogramModels.back().nbNonBoundaryBinsLeft  	= nbLeftNonBoundaryBins;
			
			   baseHistogramModels.back().binID_NegBoundaryRight  	= idxM+4;
			   baseHistogramModels.back().nbNonBoundaryBinsRight 	= nbRightNonBoundaryBins;
			
			   baseHistogramModels.back().width_cm					= width;

			   this->prior.at<int>(left,right) = (int)(std::round(pL*pR));
		    }
		 }

	  } //for ends
	}//for ends
    
    // Normalize
	int32_t SUM = cv::sum(this->prior)[0];
	this->prior.convertTo(this->prior,CV_32SC1,SCALE_FILTER);
    this->prior = this->prior/(SUM);
}

ostream& operator<<(ostream& os, const LaneFilter& laneFilter)
{
  os<<endl<<"LaneFilter Cofiguration:"<<endl;
  os << "STEP size in cm: "<< laneFilter.mSTEP_CM<<endl;

  return os;

}



LaneFilter::~LaneFilter()
{
	
}
