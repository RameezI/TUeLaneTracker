#ifndef VANSIHINGPOINFILTER_H
#define VANSIHINGPOINFILTER_H

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

#include  "LaneFilter.h"
#include <Eigen/Dense>

using namespace Eigen;
typedef Matrix<int, 6, 1> Vector6i;


struct VanishingPt
{
  int  V;
  int  H;
  int  V_prev;
  int  H_prev;
  VanishingPt()
  : V(0),H(0),V_prev(0),H_prev(0){}
};

struct PurviewHistogramModel
{
	int  binID_leftBoundary;
	int  binID_rightBoundary;
	
	int  binID_NegBoundaryLeft;
	int  binID_NegBoundaryRight;
	int  nbNonBoundaryBinsLeft;
	int  nbNonBoundaryBinsRight;
	
	PurviewHistogramModel()
	: binID_leftBoundary(-1),binID_rightBoundary(-1),
	 binID_NegBoundaryLeft(-1),binID_NegBoundaryRight(-1),
	 nbNonBoundaryBinsLeft(0), nbNonBoundaryBinsRight(0)
	{

	}
	
};




class VanishingPtFilter
{

		
public:

	VanishingPtFilter(const Ref<const VectorXi>& LANE_HISTOGRAM_BINS, const int&  LANE_FILTER_OFFSET_V);
	~VanishingPtFilter();




private: 
        	const int       mVP_STEP;         // Step for VP Bins

public: 
		const int 	VP_RANGE_V;
		const int 	VP_RANGE_H;
					
		const int    	mNb_VP_BINS_V;    // number of bins in the  vertical   direction.
		const int    	mNb_VP_BINS_H;    // number of bins in the horizental direction.

		const VectorXi 	VP_BINS_V;	 //Histogram Bins in Pixels.
		const VectorXi 	VP_BINS_H;     	 //Histogram Bins in Pixels.
		const int  	OFFSET_V;
		
		const float     mVP_LANE_RATIO;			


public:			
			
		const VectorXi  HISTOGRAM_BINS;	    //Purview Histogram Bins in Pixels.
		const int    	STEP;  		    //Purview Histogram STEP
		      
	    	cv::Mat  prior;
	    	cv::Mat  filter;

				  		
private:		 
		void  createPrior();

};
	
#endif // VANSIHINGPOINFILTER_H
