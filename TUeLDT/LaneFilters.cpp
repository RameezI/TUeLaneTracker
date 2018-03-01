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

#include "LaneFilters.h"
#include "ScalingFactors.h"

LaneFilters::LaneFilters(const LaneProperties& LANE,  const Camera& CAMERA)

: mLANE(LANE),

  mCAMERA(CAMERA),

  BASE_LINE_ICCS(mCAMERA.FRAME_CENTER(0)    - BASE_LINE_IBCS),  

  PURVIEW_LINE_ICCS(mCAMERA.FRAME_CENTER(0) - PURVIEW_LINE_IBCS),
  
  BINS_STEP_cm(10),

  BINS_COUNT( (int)( (2*mLane.MAX_WIDTH)/BINS_STEP_cm ) + 1),

  BINS_cm( VectorXi::LinSpaced( BINS_COUNT, -mLane.MAX_WIDTH, mLane.MAX_WIDTH) ),
  
  BASE_BINS(toPixelBINS(BINS_cm, mCAMERA, BASE_LINE_ICCS)),

  PURVIEW_BINS(toPixelBINS(BINS_cm, mCAMERA, PURVIEW_LINE_ICCS)),
  
  priorBaseHist( cv::Mat::zeros( (int)(mLane.MAX_WIDTH/STEP_cm) +1, (int)(mLane.MAX_WIDTH/STEP_cm) +1 , CV_32SC1) ),
  
  filterBaseHist(cv::Mat::zeros( (int)(mLane.MAX_WIDTH/STEP_cm) +1, (int)(mLane.MAX_WIDTH/STEP_cm) +1 , CV_32SC1) )
  
  {
	createPriors();
	filterBaseHist = priorBaseHist.clone(); // Initially the posterior probabilities are equal to the prior estimate.
  }



vectorXi toPixelBINS(const Ref<const VectorXi>& BINS_cm, const Camera& CAM, const int Y_ICS )
{


}

vectorXi getPurview_HIST_BINS()
{


}

void LaneFilters::createPriors()
{

 	VectorXf  lBINS_RHP_cm(BINS_cm.tail(priorBaseHist.rows));  //Take only the bins in the right half plane [only the +ve BINS]

	/* Create Histogram Models for the BaseHistogram */
	/* Assign probabilities to States  */
		
	float 	 hmean =  mLANE.AVG_WIDTH/2;
	float    sigmaL = mLANE.STD_WIDTH;
	
	float pL, pR, width;
	
	for (int left = 0; left < lBINS_RHP_cm.size(); left++)
	{
	  for (int right = 0; right < lBINS_RHP_cm.size(); right++)
	  {
		 // prior on location
		 pL = (exp( -pow(hmean-lBINS_RHP_cm(left), 2) / (2*pow(8*sigmaL,2)) ) / ( sqrt(2*M_PI)*8*sigmaL ) )*SCALE_FILTER;

		 pR = (exp( -pow(hmean-lBINS_RHP_cm(right), 2) / (2*pow(8*sigmaL,2)) ) / ( sqrt(2*M_PI)*8*sigmaL ))*SCALE_FILTER;

		 //prior on lane width
		 width = lBINS_RHP_cm(left)+lBINS_RHP_cm(right);

		 if (mLANE.MIN_WIDTH <= width && width <= mLANE.MAX_WIDTH)
		 {
		    /* To Histogram Bins-IDs*/
		    int idxL = (lBINS_RHP_cm.size()  -1) - left;
		    int idxR = (lBINS_RHP_cm.size()  -1) + right;

		    int idxM = round((idxL+idxR)/2.0);

		    int nbLeftNonBoundaryBins  = (idxM-3) - (idxL +2);
		    int nbRightNonBoundaryBins = (idxR-2) - (idxM +3);

		    if( 0 < idxL && idxR < BINS_COUNT )
		    {
			   baseHistogramModels.push_back( BaseHistogramModel());

			   baseHistogramModels.back().rowIdxFilter  		= left;
			   baseHistogramModels.back().colIdxFilter 		= right;

			   baseHistogramModels.back().leftBoundary 		=  BASE_BINS(idxL);
			   baseHistogramModels.back().rightBoundary 		=  BASE_BINS(idxR);

			   baseHistogramModels.back().leftOffset_cm 		=  BINS_cm(idxL);
			   baseHistogramModels.back().rightOffset_cm 		=  BINS_cm(idxR);

			   baseHistogramModels.back().binID_leftBoundary 	= idxL;
			   baseHistogramModels.back().binID_rightBoundary	= idxR;

			   baseHistogramModels.back().binID_nonBoundaryLeft  	= idxL+2;
			   baseHistogramModels.back().nbNonBoundaryBinsLeft  	= nbLeftNonBoundaryBins;
			
			   baseHistogramModels.back().binID_nonBoundaryRight  	= idxM+4;
			   baseHistogramModels.back().nbNonBoundaryBinsRight 	= nbRightNonBoundaryBins;
			
			   baseHistogramModels.back().width_cm			= width;

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

LaneFilters::~LaneFilters()
{
	
}
