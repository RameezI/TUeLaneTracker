#ifndef LANE_FILTERS_H__
#define LANE_FILTERS_H__

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
#include "HistogramModels.h"

using namespace Eigen;

/* This class provides Base and Purview Histograms expressed in Image-Center-Coordinate-System. */
class LaneFilter
{
	
private:	
		const LaneProperties   	mLANE;	  /*< Describes Lane properties for example average lane widths and its standard deviation*/
		const Camera 		mCAMERA;  /*< Describes Camera properties and configuration */

		void  createPrior();	


public:	 	//Public Interface 


		/** Constructs LaneFilter from Camera and LaneProperties */
		LaneFilter(const LaneProperties& LANE,  const Camera& CAMERA);
	
		const cv::Point O_ICCS_ICS;		/** Origin of Image-Center-CS in the Image-CS*/
		const cv::Point O_ICS_ICCS;		/** Origin of Image-CS in the Image-Center-CS*/
		const cv::Point O_IBCS_ICS;		/** Origin of Image-Base-CS in the Image-CS*/
		

  		const int 	BASE_LINE_ICCS;		/**< Base line in Image-Center-CS [pixel-lines] */
  		const int 	PURVIEW_LINE_ICCS;	/**< Base line in Image-Center-CS [pixel-lines] */

		const int	BINS_STEP_cm;  		/**< Step size for the Histogram BINS [cm] */
		const int   	BINS_COUNT;   		/*< Number of bins in each Histogram */

		const VectorXi	BINS_cm;		/**<Describes the Histogram BINS in the Vehicle-Symmetry-CS [cm] */

		const cv::Mat   BASE_BINS;    		/**<Describes the Histogram BINS, in Image-Center-CS, at BASE line [pixels] */
		const cv::Mat   PURVIEW_BINS; 		/**<Describes the Histogram BINS, in Image-Center-CS, at PURVIEW line [pixels] */


		cv::Mat 	prior;			/**< \brief This 2D-filter provides prior prbobility for a certain 
							combination of left and right offsets representing the Lane.
     
 							rowIndex: Offset of left  boundary, in #BINS_STEPS_cm,
						   		  from Vehicle-Symmetry-CS Origin.

							colIndex: Offset of right boundary, in #BINS_STEPS_cm,
								   from Vehicle-Symmetry-CS Origin.*/


		cv::Mat 	filter;			/**< \brief This 2D-filter provides posterior prbobility for a certain
							 combination of left and right offsets representing the Lane.
								
 							rowIndex: Offset of left  boundary, in #BINS_STEPS_cm,
						   		  from Vehicle-Symmetry-CS Origin.

							colIndex: Offset of right boundary, in #BINS_STEPS_cm,
								   from Vehicle-Symmetry-CS Origin.
							*/


		std::vector<BaseHistogramModel>  	baseHistogramModels; /** <Provides a filtered list of Lane models 
									  	constructed from a multimodal base histogram.*/


};
	
#endif // LANEFILTER_H
