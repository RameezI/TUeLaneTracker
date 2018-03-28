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
#include "UnitConversion.h"
#include <opencv2/core/eigen.hpp>

	///cm to pixel conversion, for a particular row in the image, of the #BINS_cm [Vehicle-Symmetry-CS <---> Image-Center-CS]
cv::Mat toPixelBINS(const Ref<const VectorXi>& BINS_cm, const Camera& CAM, const int Y_ICCS )
{
	float cm2px;
	UnitConversion mUnit;
	cm2px = 1/mUnit.getPixToCm(Y_ICCS, CAM);
	cout << "cm2px " << cm2px << endl;

	cv::Mat  lMat = cv::Mat(BINS_cm.size(),1,CV_32S);
	cv::Mat  lWorldPt	= cv::Mat(3,1, CV_32F);
	for(int i=0; i< BINS_cm.size(); i++)
	   {
		  lWorldPt.at<float>(0)	= BINS_cm[i];
	 	  lMat.at<int32_t>(i,0) = floor(lWorldPt.at<float>(0,0) * cm2px) ;
	   }

	return lMat;
}

LaneFilter::LaneFilter(const LaneProperties& LANE,  const Camera& CAMERA)

: mLANE(LANE),

  mCAMERA(CAMERA),

  O_ICCS_ICS(mCAMERA.O_ICCS_ICS),

  O_ICS_ICCS(mCAMERA.O_ICS_ICCS),

  O_IBCS_ICS(cv::Point(0, mCAMERA.RES_VH(0))),

  BASE_LINE_ICCS(-BASE_LINE_IBCS + O_IBCS_ICS.y + O_ICS_ICCS.y ),  

  PURVIEW_LINE_ICCS(-PURVIEW_LINE_IBCS + O_IBCS_ICS.y + O_ICS_ICCS.y ),
  
  BINS_STEP_cm(15), // This value must be above 10 for the current implementation.
  
  BINS_MAX_cm(round(mLANE.MAX_WIDTH/BINS_STEP_cm)*BINS_STEP_cm),

  COUNT_BINS( (int)( (2*BINS_MAX_cm)/BINS_STEP_cm ) + 1),

  BINS_cm( VectorXi::LinSpaced( COUNT_BINS, -BINS_MAX_cm, BINS_MAX_cm) ),
  
  BASE_BINS(toPixelBINS(BINS_cm, mCAMERA, BASE_LINE_ICCS)),

  PURVIEW_BINS(toPixelBINS(BINS_cm, mCAMERA, PURVIEW_LINE_ICCS)),

/*^TODO: Calculate using calcPixelWidth */
 // BASE_BINS(),
  //PURVIEW_BINS(),
  
  prior( cv::Mat::zeros( (int)(BINS_MAX_cm/BINS_STEP_cm) +1, (int)(BINS_MAX_cm/BINS_STEP_cm) +1 , CV_32SC1) ),
  
  filter(cv::Mat::zeros( (int)(BINS_MAX_cm/BINS_STEP_cm) +1, (int)(BINS_MAX_cm/BINS_STEP_cm) +1 , CV_32SC1) )
  
  {
	createPrior();
	filter = prior.clone(); // Initially the posterior probabilities are equal to the prior estimate.
  }



void LaneFilter::createPrior()
{

 	VectorXi  lBINS_RHP_cm(BINS_cm.tail(prior.rows));  //Take only the bins in the right half plane [only the +ve BINS]

	/* Create Histogram Models for the BaseHistogram */
	/* Assign probabilities to States  */
		
	const float 	 lMean =  mLANE.AVG_WIDTH/2;
	const float    	 lSigma = mLANE.STD_WIDTH;
	
	float lProb_left, lProb_right, lWidth_cm;
	
	for (int left = 0; left < lBINS_RHP_cm.size(); left++)
	{
	  for (int right = 0; right < lBINS_RHP_cm.size(); right++)
	  {
	     // prior on location
	     lProb_left  = (exp( -pow(lMean-lBINS_RHP_cm(left), 2) / (2*pow(8*lSigma,2)) )  / ( sqrt(2*M_PI)*8*lSigma ) )*SCALE_FILTER;

	     lProb_right = (exp( -pow(lMean-lBINS_RHP_cm(right), 2) / (2*pow(8*lSigma,2)) ) / ( sqrt(2*M_PI)*8*lSigma ) )*SCALE_FILTER;

 	    //prior on lane width
	     lWidth_cm = lBINS_RHP_cm(left)+lBINS_RHP_cm(right);

	    if (mLANE.MIN_WIDTH <= lWidth_cm && lWidth_cm <= mLANE.MAX_WIDTH)
	    {
	       /* To Histogram Bins-IDs*/
	       size_t idxL = (lBINS_RHP_cm.size()  -1) - left;
	       size_t idxR = (lBINS_RHP_cm.size()  -1) + right;

	       size_t idxM = round((idxL+idxR)/2.0);

	       size_t lNonBoundaryBinsCount_left  = (idxM-3) - (idxL +2);
	       size_t lNonBoundaryBinsCount_right = (idxR-2) - (idxM +3);

	       if( 0 < idxL && idxR < COUNT_BINS-1 )
	       {
		   baseHistogramModels.push_back( BaseHistogramModel());

		   baseHistogramModels.back().rowIdxFilter  			= left;
		   baseHistogramModels.back().colIdxFilter 			= right;

		   baseHistogramModels.back().binIdxBoundary_left 		= idxL;
		   baseHistogramModels.back().binIdxBoundary_right		= idxR;
		
		   //^TODO: Make non-boundary index dependent on the bin size
		   baseHistogramModels.back().binIdxNonBoundary_left  		= idxL+2;
		   baseHistogramModels.back().nonBoundaryBinsCount_left  	= lNonBoundaryBinsCount_left;

		   //^TODO: Make non-boundary index dependent on the bin size
		   baseHistogramModels.back().binIdxNonBoundary_right  		= idxM+4;
		   baseHistogramModels.back().nonBoundaryBinsCount_right 	= lNonBoundaryBinsCount_right;

		   baseHistogramModels.back().boundary_left 			=  BASE_BINS.at<int32_t>(idxL,0);
		   baseHistogramModels.back().boundary_right 			=  BASE_BINS.at<int32_t>(idxR,0);

		   baseHistogramModels.back().boundary_left_cm 			=  BINS_cm(idxL);
		   baseHistogramModels.back().boundary_right_cm 		=  BINS_cm(idxR);
		   baseHistogramModels.back().width_cm				=  lWidth_cm;

		   this->prior.at<int>(left,right)				= (int)(std::round(lProb_left*lProb_right));
	       }
	    }

	  } //for ends
	}//for ends

    
    	// Normalize
	int32_t SUM = cv::sum(this->prior)[0];
	this->prior.convertTo(this->prior,CV_32SC1,SCALE_FILTER);
    	this->prior = this->prior/(SUM);
}

