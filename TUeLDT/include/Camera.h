#ifndef CAMERA_H
#define CAMERA_H

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
#include "State.h"
#include <sys/stat.h>
#include <Eigen/Dense>
#include "opencv2/opencv.hpp"
#include <cmath>


using namespace Eigen;
using namespace std;


static double toRad(double deg)
{
  double radians = deg*(M_PI/180); 
  return radians;
}


struct Camera
{

friend ostream& operator<<(ostream& os, const Camera& lCamera);


private:

		const LaneTracker::Config& mConfig;


public:
		const cv::Mat	MATRIX_INTRINSIC;		/**<  Camera Intrinsic Parameters 3x3 */
		const cv::Mat	MATRIX_EXTRINSIC;		/**<  Camera Extrinsic Parameters 4x4 */

		const Vector2i	RES_VH; 	    		/**< Resolution of the camera image [pixels]*/
		const Vector2f 	ROT_PY;				/**< Pitch and Yaw angle of the camera [degrees]*/
		const Vector2f  FOV_VH;				/**< Field-of-View of the camera [degrees]*/
		const Vector2i	HORIZON_VH; 	    		/**< Location of Horizon in Image-Center-CS [pixels]*/


		const cv::Point O_ICCS_ICS;			/**< Origin of Image-Center-CS in Image-CS [pixels]*/
		const cv::Point O_ICS_ICCS;			/**< Origin of Image-CS in Image-Center-CS [pixels]*/


		Camera(const LaneTracker::Config& Config)
		: mConfig(Config),

		  MATRIX_INTRINSIC(getIntrinsicMatrix()),

		  MATRIX_EXTRINSIC(getExtrinsicMatrix()), 

		  RES_VH(Vector2i(Config.cam_res_v, Config.cam_res_h)),

		  ROT_PY(Vector2f(Config.cam_pitch, Config.cam_yaw)),

		  FOV_VH(Vector2f(2*atan( (RES_VH(0)/2.0) / (MATRIX_INTRINSIC.at<float>(1,1)) )*180/M_PI ,
				 2*atan( (RES_VH(1)/2.0) / (MATRIX_INTRINSIC.at<float>(0,0)) )*180/M_PI )),

		  HORIZON_VH(Vector2i( round((ROT_PY(0)* RES_VH(0)) / FOV_VH(0) ) ,
				      round((ROT_PY(1)* RES_VH(1)) / FOV_VH(1) ) )),

		  O_ICCS_ICS( cv::Point( RES_VH[1]/2,  RES_VH[0]/2) ), 

		  O_ICS_ICCS( cv::Point(-RES_VH[1]/2, -RES_VH[0]/2) ){ }




private:

	cv::Mat getIntrinsicMatrix()
	{	   

	  const float& FX =  mConfig.cam_fx;
	  const float& FY =  mConfig.cam_fy;
	  const float& CX =  mConfig.cam_cx;
	  const float& CY =  mConfig.cam_cy;

    	  // 3x3 Intrinsic Parameters
    	  // This Matrix is for performing calculations in Image-Center-CS
    	  cv::Mat matrix = (cv::Mat_<float>(3,3) << FX,    0,   CX,
						     0,    FY,  CY,
						     0,    0,    1 );
	  return matrix;
	}


	cv::Mat getExtrinsicMatrix()
	{	   


	  const float& PITCH 		 	= mConfig.cam_pitch;
	  const float& YAW  		 	= mConfig.cam_yaw;
	  const float& CAM_HEIGHT		= mConfig.cam_height;
	  const float& CAM_LATERAL_OFFSET	= mConfig.cam_lateral_offset;
 
   	  // 4x4 Pitch Transformation
    	  cv::Mat ROT_P = (cv::Mat_<float>(4,4)<< 1,   0,  				  0,   			            0,
				    	          0,   cos(-(M_PI/2.0) + toRad(PITCH)),  sin(-(M_PI/2.0) + toRad(PITCH)),   0,
				    	          0,  -sin(-(M_PI/2.0) + toRad(PITCH)),  cos(-(M_PI/2.0) + toRad(PITCH)),   0,
				    	          0,   0,  				  0,   			       	    1);

	  // 4x4 Yaw Transformation
	  cv::Mat ROT_Y = (cv::Mat_<float>(4,4)<< cos(toRad(YAW)),      0,  	-sin(toRad(YAW)),  0,
					    	  0,	 	       	1,      0,	 	   0,
					       	  sin(toRad(YAW)),   	0,      cos(toRad(YAW)),   0,
					          0,   	       		0,      0,   		   1);


   	 // 4x4 Translation
    	 cv::Mat    T  = (cv::Mat_<float>(4,4)<< 1,   0,     0,  CAM_LATERAL_OFFSET,
				    	 0,   1,     0,  CAM_HEIGHT,
				    	 0,   0,     1,  0, //longitudnal displacement between world-CS and camera-CS is 0
				    	 0,   0,     0,  1);


    	 // 4x4  Extrinsic Parameters [Homogenous Transformation]
    	 // This Matrix is specific to the world coordinates definition
    	 // set the value of camera pitch and yaw to calculate this matrix acoordingly
    	 cv::Mat matrix = T*ROT_Y*ROT_P; 
	 return matrix;
	}

};

inline ostream& operator<<(ostream& os, const Camera& lCamera)
{
  os<<endl<<"[Camera Properties]"<<endl;

  os<<"***********************************************"<<endl;
  os<<"Extrinsic Camera Matrix	: "<<endl<<lCamera.MATRIX_EXTRINSIC<<endl<<endl;
  os<<"Intrinsic Camera Matrix	: "<<endl<<lCamera.MATRIX_INTRINSIC<<endl<<endl;
  os<<"Resolution[VxH]		: "<<"[ "<<lCamera.RES_VH[0]<<" x "<<lCamera.RES_VH[1]<<" ]"<<endl;
  os<<"Pitch-Yaw[P,Y]		: "<<"[ "<<lCamera.ROT_PY[0]<<" x "<<lCamera.ROT_PY[1]<<" ]"<<endl;
  os<<"Field-of-View [V, H]	: "<<"[ "<<lCamera.FOV_VH[0]<<" , "<<lCamera.FOV_VH[1]<<" ]"<<endl;
  os<<"Horizon [V, H]	        : "<<"[ "<<lCamera.HORIZON_VH[0]<<" , "<<lCamera.HORIZON_VH[1]<<" ]"<<endl;
  os<<"Origin-ICCS-ICS		: "<<lCamera.O_ICCS_ICS<<endl;
  os<<"Origin-ICS-ICCS		: "<<lCamera.O_ICS_ICCS<<endl;
  os<<"***********************************************"<<endl;

  return os;
}

#endif // CAMERA_H
