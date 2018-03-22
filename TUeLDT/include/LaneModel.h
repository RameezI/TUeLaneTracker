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

public:
	int 	boundaryLeft;	  	/*< Selected, according to max-posterior, distance to left boundary in Image-Center-CS [pixel] */
	int 	boundaryRight;  	/*< Selected, according to max-posterior, distance to left boundary in Image-Center-CS [pixel] */

	VanishingPt vanishingPt; 	/*< Selected, according to max-posterior, distance to VanishingPoint in Image-Center-CS [pixel] */

	float 	lookAheadErr_m;		/*< The mid-Line at the purview line in Vehicle-Symmetery-CS [m]*/
	

	void setModel(const int&  boundaryL, 	const int& boundaryR, const VanishingPt& vanishPt, const float& Err)
	{

	  boundaryLeft  	= boundaryL;
	  boundaryRight 	= boundaryR;
	  vanishingPt	  	= vanishPt;
	  lookAheadErr_m	= Err;
	}

	float getLateralError()
	{
	  return lookAheadErr_m;
	}

	LaneModel():
		   boundaryLeft(-1),
		   boundaryRight(-1),
		   lookAheadErr_m(0)
		   {

		   }

};

#endif
