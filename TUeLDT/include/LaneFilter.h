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
	int   	leftOffsetIdx;	 /**<Offset Index containing distance to left  boundary given the current model [index]*/
	int   	rightOffsetIdx;	 /**<Offset Index containing distance to right boundary given the current model [index]*/
	int   	leftOffset;	 /**< Distance to the left boundary given the current Model [pixels] */
	int   	rightOffset;	 /**< Distance to the right boundary given the current Model [pixels] */

	float 	width_cm;	/**< Width of the Lane given the current model [cm] */
		
	int  	binID_leftBoundary;	/**<Histogram Index representing left left boundary given the current model [index]*/
	int  	binID_rightBoundary;	/**<Histogram Index representing left left boundary given the current model [index]*/
	
	int  	binID_NegBoundaryLeft;	/**<Histogram Index representing left bin of non-boundary region */
	int  	nbNonBoundaryBinsLeft;	/**<Number of non-boundary bins to the left of the center-line */
	int  	binID_NegBoundaryRight;	/**<Histogram Index representing right bin of non-boundary region */
	int  	nbNonBoundaryBinsRight;	/**<Number of non-boundary bins to the right of the center-line*/
		
	
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
		const LaneParameters   	mLANE;	  /*< Describes Lane properties for example average lane widths and its standard deviation*/
		const Camera 		mCAMERA;  /*< Describes Camera properties and configuration */
		const int		mSTEP_CM; /*< BaseHistogram step size [cm] */

public:	 //Public Interface 
		const int 		STEP;   		/*< BaseHistogram step [Pixels] */
		const int 		mBIN_MAX;		/*< BaseHistogram max bin value [Pixels] */
		const int   		mNb_HISTOGRAM_BINS;    	/*< Number of bins in the base Histogram */
		const int   		mNb_OFFSET_BINS;	/*< Number of bins in offsets vector [Half of the mNb_HISTOGRAM_BINS] */
		
public:	 //Public Interface of the class	

		const int 		OFFSET_V;   		/*< Vertical Offset of BaseHistogram in VP coordinate System */
		const VectorXi  	HISTOGRAM_BINS;    	/*< -PX_MAX	:	STEP	: PX_MAX 	*/
		const VectorXi  	OFFSET_BINS;       	/*<  0 		: 	STEP 	: PX_MAX	*/
		
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
