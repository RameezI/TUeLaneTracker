#ifndef LANEFILTER_H
#define LANEFILTER_H

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

#include <Eigen/Dense>
#include "opencv2/opencv.hpp"
#include "Lane.h"
#include "Camera.h"

using namespace Eigen;

struct BaseHistogramModel
{
	int   leftOffsetIdx;
	int   rightOffsetIdx;
	int   leftOffset;
	int   rightOffset;
	float width_cm;
		
	int  binID_leftBoundary;
	int  binID_rightBoundary;
	
	int  binID_NegBoundaryLeft;
	int  nbNonBoundaryBinsLeft;
	int  binID_NegBoundaryRight;
	int  nbNonBoundaryBinsRight;
		
	
	BaseHistogramModel()
	: leftOffsetIdx(-1),
	  rightOffsetIdx(-1),
	  leftOffset(-1),
	  rightOffset(-1),
	  width_cm(0),
	  binID_leftBoundary(-1),
	  binID_rightBoundary(-1)
	{

	}
	
};


/* This class provides LaneFilter expressed in Vanishing point coordinate system. */
class LaneFilter
{
	
private:	
		const LaneParameters   	mLANE;
		const Camera 		mCAMERA;
		const int		mSTEP_CM;     // BaseHistogram Step in cm

public:	 //Public Interface 
		const int 	STEP;   		// BaseHistogram Step in Pixels 
		const int 	mBIN_MAX;		// BaseHistogram Max Bin Value
		const int   	mNb_HISTOGRAM_BINS;    	// number of bins in the Base Histogram.
		const int   	mNb_OFFSET_BINS;	// number of offset bins on one side.
		
public:	 //Public Interface of the class	

		const int 	OFFSET_V;   		// Vertical Offset (BaseHistogram) in VP coordinate System
		const VectorXi  HISTOGRAM_BINS;    	// -PX_MAX:STEP: PX_MAX
		const VectorXi  OFFSET_BINS;       	//  0 : STEP : PX_MAX
		
		cv::Mat prior;
		cv::Mat filter;

		std::vector<BaseHistogramModel>  baseHistogramModels; 	

private:	
		void  createHistogramModels();	

public:	
		LaneFilter(const LaneParameters& LANE,  const Camera& CAMERA);
	   	~LaneFilter();
};
	
#endif // LANEFILTER_H
