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
#include <opencv2/core/eigen.hpp>

///cm to pixel conversion, for a particular row in the image, of the #BINS_cm [Vehicle-Symmetry-CS <---> Image-Center-CS]
cv::Mat toPixelBINS(const Ref<const VectorXi>& BINS_cm, const Camera& CAM, const int Y_ICS )
{

	cv::Mat  lMat = cv::Mat(BINS_cm.size(),1,CV_32S);

	{
	   cv::Mat  lIMG_TO_W;	//Image-Center-CS to Vehicle-Symmetry-CS tranformation, planar homography
	   cv::Mat  lW_TO_IMG;	//Vehicle-Symmetry-CS to Image-Center-CS transformation 

	   cv::Mat  lImgPt	= cv::Mat(3,1, CV_32F);
	   cv::Mat  lWorldPt	= cv::Mat(3,1, CV_32F);


	   // 3x3 Planar-Homography from the 4x4 Extrinsic Matrix and 3x3 Intrinsic Matrix
	   cv::Mat a = CAM.MATRIX_EXTRINSIC(cv::Range(0,3), cv::Range(0,2));
	   cv::Mat b = CAM.MATRIX_EXTRINSIC(cv::Range(0,3), cv::Range(3,4));
	   cv::hconcat(a, b, lW_TO_IMG);

	   lW_TO_IMG = CAM.MATRIX_INTRINSIC * lW_TO_IMG;

	   lIMG_TO_W = lW_TO_IMG.inv();

	   lImgPt.at<float>(0,0) = 0;
	   lImgPt.at<float>(1,0) = (float)Y_ICS; 
	   lImgPt.at<float>(2,0) = 1;

	   lWorldPt = lIMG_TO_W * lImgPt;
	   lWorldPt = lWorldPt/lWorldPt.at<float>(2);
	   
	   VectorXi lPixelBins;
	   lPixelBins = BINS_cm;

	   for(int i=0; i< BINS_cm.size(); i++)
	   {
		lWorldPt.at<float>(0)	= BINS_cm[i]/100.0;
		lImgPt = lW_TO_IMG * lWorldPt;
	 	lMat.at<int32_t>(i,0) = round(lImgPt.at<float>(0,0)/lImgPt.at<float>(0,2)) ;
	   }
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
  
  BINS_STEP_cm(10),
  
  BINS_MAX_cm(round(mLANE.MAX_WIDTH/BINS_STEP_cm)*BINS_STEP_cm),

  COUNT_BINS( (int)( (2*BINS_MAX_cm)/BINS_STEP_cm ) + 1),

  BINS_cm( VectorXi::LinSpaced( COUNT_BINS, -BINS_MAX_cm, BINS_MAX_cm) ),
  
  BASE_BINS(toPixelBINS(BINS_cm, mCAMERA, BASE_LINE_ICCS)),

  PURVIEW_BINS(toPixelBINS(BINS_cm, mCAMERA, PURVIEW_LINE_ICCS)),
  
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

