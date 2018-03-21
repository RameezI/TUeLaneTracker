#ifndef _LANE_MODEL__
#define _LANE_MODEL__

#include "LaneFilter.h"
#include "VanishingPtFilter.h"

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


	
/**
Describes the selected Vanishing Point in Image-Center-CS. 
*/
struct VanishingPt
{
  int  V;
  int  H;
  int  V_prev;
  int  H_prev;
  VanishingPt()
  : V(0),H(0),V_prev(0),H_prev(0){}
};


/**Describes the selected lane model in Image-Center-CS
*and computes lateral error, at a certain look-ahead distance, in Vehicle-Symmetry-CS. 
*/
class  LaneModel
{

private:

	float    mLookAheadError_m; 	/*< Look-ahead-Error of center-line in Vehicle-Symmetry-CS [m] */
	cv::Mat  mImg2W;		/*< Transformation matrix for Img to world transformation*/
        cv::Mat  mW2Img;		/*< Transformation matrix for world to image transformation*/

public:
	int 	boundaryLeft;	  	/*< Selected, according to max-posterior, distance to left boundary in Image-Center-CS [pixel] */
	int 	boundaryRight;  	/*< Selected, according to max-posterior, distance to left boundary in Image-Center-CS [pixel] */

	int 	boundaryLeft_cm;	/*< Selected, according to max-posterior, distance to left boundary in Vehicle-Symmetry-CS [cm] */
	int 	boundaryRight_cm;  	/*< Selected, according to max-posterior, distance to left boundary in Vehicle-Symmetry-CS [cm] */

	VanishingPt vanishingPt; 	/*< Selected, according to max-posterior, distance to VanishingPoint in Image-Center-CS [pixel] */
	

	void setModel(const int&  boundaryL, 	const int& boundaryR,
		      const int&  boundaryL_cm, const int& boundaryR_cm,
		      const VanishingPt& vanishPt)
	{

	  boundaryLeft  	= boundaryL;
	  boundaryRight 	= boundaryR;

	  boundaryLeft_cm  	= boundaryL_cm;
	  boundaryRight_cm 	= boundaryR_cm;

	  vanishingPt	  	= vanishPt;
	}

	float getLateralError(float look_ahead_distance_m)
	{
	  
	  mLookAheadError_m =-100;
	  return mLookAheadError_m;
	}

	LaneModel():
		   mLookAheadError_m(-1),
		   boundaryLeft(-1),
		   boundaryRight(-1),
		   boundaryLeft_cm(-1),
		   boundaryRight_cm(-1)
		   {


		   }

};

#endif
