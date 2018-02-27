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
	
struct LaneParameters
{

public:
	const float AVG_WIDTH;	/*< Average with of a Lane [cm]	*/
	const float STD_WIDTH;	/*< Standard deviation in Lane width [cm] */
	const float MIN_WIDTH;	/*< Minimum width of a Lane [cm] */
	const float MAX_WIDTH;  /*< Maximum width of a Lane [cm] */
	
	LaneParameters() //Lane Widths in cm
	: AVG_WIDTH(300),STD_WIDTH(50),MIN_WIDTH(250),MAX_WIDTH(500) {}
};

struct LaneModel
{
public:
	    int 	leftOffset;			/*< Distance to left boundary 	[cm] */
		int 	rightOffset;		/*< Distance to right boundary	[cm] */
		int     centerLane;			/*< Distance to center-Line 	[cm] */
		
		int 	confidenceLeft; 	/*< Confidence in left boundary */
		int     confidenceRight;  	/*< Confidence in right boundary */

		float   laneWidth;		/* Width of the lane [cm] */

		
		LaneModel()
		: leftOffset(0),rightOffset(0),confidenceLeft(0), confidenceRight(0), laneWidth(0)
		{
			
		}
};

struct LaneMembership 
	{
		
		public:
		const uint8_t   TIPPING_POINT_GRAY; 
		const uint8_t   TIPPING_POINT_GRAD_Mag;
		
		private:
		const float WIDTH_STD;
		
		public:
		const float WIDTH_DIFF_NORMA;
		const float WIDTH_DIFF_NOMIN;
		
		private:
		const float NEG_BOUNDARY_STD;
		
		public:
		const float NEG_BOUNDARY_NORMA;
		const float NEG_BOUNDARY_NOMIN;
		
		LaneMembership() //Initialization of  members through initialization list
		:TIPPING_POINT_GRAY(100),
		 TIPPING_POINT_GRAD_Mag(40),
		 
		 WIDTH_STD(15),
		 WIDTH_DIFF_NORMA( (1/sqrt( 2*M_PI*pow(WIDTH_STD,2) )) ),
		 WIDTH_DIFF_NOMIN( 2*pow(WIDTH_STD,2) ),
		 
		 NEG_BOUNDARY_STD(0.2),
		 NEG_BOUNDARY_NORMA( (2/sqrt(2*M_PI*pow(NEG_BOUNDARY_STD,2)))),
		 NEG_BOUNDARY_NOMIN(2*pow(NEG_BOUNDARY_STD,2))	 
		{
			//Default Constructor
		}
	};

#endif // LANE_H
