#ifndef LANE_H
#define LANE_H
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
#include <iostream>
#include "Config.h"

using namespace std;

/**Describes Lane Properties*/
struct LaneProperties
{

friend ostream& operator<<(ostream& os, const LaneProperties& laneProperties);

public:
	const float AVG_WIDTH;		/**< Average with of a Lane [cm] */
	const float STD_WIDTH;		/**< Standard deviation in Lane width [cm] */
	const float MIN_WIDTH;		/**< Minimum width of a Lane [cm] */
	const float MAX_WIDTH;  	/*< Maximum width of a Lane [cm] */
	const float AVG_WIDTH_LM;  	/**< Average Width of the Lane Markings on the lane */
	
	LaneProperties(const LaneTracker::Config& Config) //Lane Widths in cm
	: AVG_WIDTH(Config.lane_avg_width),
	  STD_WIDTH(Config.lane_std_width),
	  MIN_WIDTH(Config.lane_min_width),
	  MAX_WIDTH(Config.lane_max_width),
	  AVG_WIDTH_LM(2*Config.lane_marker_width){}
};


/**
Describes pixel chracteristics for assigning their membership to Lane-Boundary or Non-Boundary regions 

*/
struct LaneMembership 
	{
		public:
		const uint8_t   TIPPING_POINT_GRAY; 	/*< Tipping point of gray shade fitness for a lane bounday */
		const uint8_t   TIPPING_POINT_GRAD_Mag; /*< Tipping point of gradient magnitude fitness for a lane boundary */ 
		
		private:
		const float 	WIDTH_STD;		
		
		public:
		const float 	WIDTH_DIFF_NORMA;
		const float 	WIDTH_DIFF_NOMIN;
		
		private:
		const float 	NEG_BOUNDARY_STD;
		
		public:
		const float 	NEG_BOUNDARY_NORMA;
		const float 	NEG_BOUNDARY_NOMIN;
		
		LaneMembership() //Initialization of  members through initialization list
		:TIPPING_POINT_GRAY(100),
		 TIPPING_POINT_GRAD_Mag(40),
		 
		 WIDTH_STD(15),
		 WIDTH_DIFF_NORMA( (1/sqrt( 2*M_PI*pow(WIDTH_STD,2) )) ),
		 WIDTH_DIFF_NOMIN( 2*pow(WIDTH_STD,2) ),
		 
		 NEG_BOUNDARY_STD(0.2),
		 NEG_BOUNDARY_NORMA( (2/sqrt(2*M_PI*pow(NEG_BOUNDARY_STD,2)))),
		 NEG_BOUNDARY_NOMIN(2*pow(NEG_BOUNDARY_STD,2)){}
	};



inline ostream& operator<<(ostream& os, const LaneProperties& laneProperties)
{
  os<<"[Lane Properties]"<<endl;
  os<<"****************************"<<endl;
  os<<"Average Width		: "<< laneProperties.AVG_WIDTH<<endl;
  os<<"Standard Deviaion 	: "<< laneProperties.STD_WIDTH<<endl;
  os<<"Minimum  Width 	  	: "<< laneProperties.MIN_WIDTH<<endl;
  os<<"Maximum  Width 		: "<< laneProperties.MAX_WIDTH<<endl;
  os<<"Lane Marking  Width 	: "<< laneProperties.AVG_WIDTH_LM<<endl;
  os<<"****************************"<<endl;
  return os;
}

#endif // LANE_H
