#ifndef LANE_FILTER_H__
#define LANE_FILTER_H__

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
#include "Lane.h"
#include "Camera.h"
#include "HistogramModels.h"


using namespace Eigen;

/* This class provides Base and Purview Histograms expressed in Image-Center-Coordinate-System. */
class LaneFilter
{

friend 		ostream& operator<<(ostream& os, const LaneFilter& lLaneFilter);

public:	 	//Public Interface 

		const LaneProperties   	LANE;	  /*< Describes Lane properties for example average lane widths and its standard deviation*/
		const Camera 		CAMERA;   /*< Describes Camera properties and configuration */
		void  createPrior();	


		/** Constructs LaneFilter from Camera and LaneProperties */
		LaneFilter(const LaneProperties& LANE,  const Camera& CAMERA, const LaneTracker::Config& Config);
	
		const cv::Point O_ICCS_ICS;		/** Origin of Image-Center-CS in the Image-CS*/
		const cv::Point O_ICS_ICCS;		/** Origin of Image-CS in the Image-Center-CS*/
		const cv::Point O_IBCS_ICS;		/** Origin of Image-Base-CS in the Image-CS*/
		

  		const int 	BASE_LINE_ICCS;		/**< Base line in Image-Center-CS [pixel-lines] */
  		const int 	PURVIEW_LINE_ICCS;	/**< Base line in Image-Center-CS [pixel-lines] */

  		float 		BASE_LINE_cm;		/**< Look-ahead distance at the base line in Vehicle_Symmetry-CS [cm] */
  		float  		PURVIEW_LINE_cm;	/**< Look-ahead distance at the purview line in Vehicle_Symmetry-CS [cm] */

		const float	BINS_STEP_cm;  		/**< Step size for the Histogram BINS [cm] */
  		const float	BINS_MAX_cm;		/**< Max value of the #BINS_cm **/
		const size_t   	COUNT_BINS;   		/**< Number of bins in each Histogram */

		const VectorXf	BINS_cm;		/**<Describes the Histogram BINS in the Vehicle-Symmetry-CS [cm] */

		
		const float	BASE_STEP;		/**< Step size for the base bins */
		const float 	PURVIEW_STEP;		/**< Step size for the purview bins */

		const int	BASE_OFFSET;		/**< Starting Point for Base Bins in Image-Center-CS*/
		const int 	PURVIEW_OFFSET;		/**< Starting Point for Purview Bins in Image-Cener-CS*/

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


		
inline ostream& operator<<(ostream& os, const LaneFilter& laneFilter)
{
  os<<endl<<"[LaneFilter Properties]"<<endl;
  os<<"***********************************************************************"<<endl;
  os<<endl<<laneFilter.LANE<<endl;
  os<<endl<<laneFilter.CAMERA<<endl;

  os<<"Origin of Image-Center-CS in Image-CS:	"<<laneFilter.O_ICCS_ICS<<endl;
  os<<"Origin of Image-CS in Image-Center-CS:	"<<laneFilter.O_ICS_ICCS<<endl;
  os<<"Origin of Image-Base-CS in Image-CS:	"<<laneFilter.O_IBCS_ICS<<endl;

  os<<"Y-coordinate  of base-line in Image-Center-CS:	 "<<laneFilter.BASE_LINE_ICCS<<endl;		
  os<<"Y-coordinate  of purview-line in Image-Center-CS: "<<laneFilter.PURVIEW_LINE_ICCS<<endl;  

  os<<"BINS_cm step size:	"<<laneFilter.BINS_STEP_cm<<endl;
  os<<"Base step size		"<<laneFilter.BASE_STEP<<endl;
  os<<"Purview step size	"<<laneFilter.PURVIEW_STEP<<endl;
  os<<"BINS_cm MAX  value:	"<<laneFilter.BINS_MAX_cm<<endl;
  os<<"Total number of BINS	"<<laneFilter.COUNT_BINS<<endl;
  os<<"BINS in cm:		"<<endl<<laneFilter.BINS_cm<<endl<<endl;	
  os<<"Base-line BINS:		"<<endl<<laneFilter.BASE_BINS<<endl<<endl;
  os<<"Purview-line BINS:	"<<endl<<laneFilter.PURVIEW_BINS<<endl;

  os<<"************************************************************************"<<endl;

  return os;
}

#endif // LANE_FILTER_H
