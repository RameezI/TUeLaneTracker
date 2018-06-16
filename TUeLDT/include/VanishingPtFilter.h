#ifndef VANSIHING_POINT_FILTER_H__
#define VANSIHING_POINT_FILTER_H__

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
#include "Config.h"
#include "opencv2/opencv.hpp"


using namespace Eigen;
using namespace std;

typedef Matrix<int, 6, 1> Vector6i;

///This class represents ad 2D-Filter expressed in Image-Center-Coordiante-System
/*

*/
class VanishingPtFilter
{

friend 		ostream& operator<<(ostream& os, const VanishingPtFilter& lVanishingPtFilter);


private:
		void  createPrior();


public:

		VanishingPtFilter(const int HOR_H, const int HOR_V, const LaneTracker::Config& Config);

        	const int       STEP;         	/*< Step for VanishinPtFilter [pixels] */

		const int 	RANGE_V;  	/*< Vertical   range of the VanishingPtFilter [pixels] */
		const int 	RANGE_H;	/*< Horizental range of the VanishingPtFilter [pixels] */

					
		const int    	COUNT_BINS_V;    /*< Number of bins in the  vertical   direction of VanishinPtFilter */
		const int    	COUNT_BINS_H;    /*< Number of bins in the  horizental direction of VanishinPtFilter */

		const VectorXi 	BINS_V;	 	/*< Horizental  bins of the VanishinPtFiltere [pixels].*/
		const VectorXi 	BINS_H;     	/*< Vertical    bins of the VanishinPtFilter  [pixels].*/
			
		      
	    	cv::Mat  prior;			/*< prior probabilities of the 2D VanishinPtFilter */
	    	cv::Mat  filter;		/*< posterior probabilities of the 2D VanishinPtFilter */

				  		
		~VanishingPtFilter();

};


inline ostream& operator<<(ostream& os, const VanishingPtFilter& vanishingPtFilter)
{
  os<<endl<<"[VanishingPtFilter Properties]"<<endl;
  os<<"***********************************************************************"<<endl;

  os<<"VP step size [pixels]:   "<<vanishingPtFilter.STEP<<endl;
  os<<"Vertical-Range:          "<<vanishingPtFilter.RANGE_V<<endl;
  os<<"Horizontal-Range:        "<<vanishingPtFilter.RANGE_H<<endl;
  os<<"Bins-Count Vertical:     "<<vanishingPtFilter.COUNT_BINS_V<<endl;
  os<<"Bins-Count Horizontal:   "<<vanishingPtFilter.COUNT_BINS_H<<endl;

  os<<"Vertical   BINS:         "<<endl<<vanishingPtFilter.BINS_V<<endl;
  os<<"Horizontal BINS:         "<<endl<<vanishingPtFilter.BINS_H<<endl;
  
  os<<"************************************************************************"<<endl;

  return os;
}
	
#endif // VANSIHINGPOINFILTER_H
