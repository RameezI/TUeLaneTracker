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


using namespace Eigen;
using namespace std;

struct Camera
{

friend ostream& operator<<(ostream& os, const Camera& lCamera);

public:

		const string	NAME;				/**<  Camera Identifier */
		const cv::Mat	MATRIX_INTRINSIC;		/**<  Camera Intrinsic Parameters 3x3 */
		const cv::Mat	MATRIX_EXTRINSIC;		/**<  Camera Extrinsic Parameters 4x4 */

		const Vector2i	RES_VH; 	    		/**< Resolution of the camera image */
		const Vector2f  FOV_VH;				/**< Field-of-View of the camera */
		const float 	PITCH_ANGLE;			/**< Pitch angle of the camera */
		const int32_t	HORIZON;			/**< Location of Horizon in Image-Center-CS */

		const float 	FOV_HORIZON;			/**< Field-of-View of the road up to the horizon*/
		const float 	RES_HORIZON;			/**< Vertical resolution of image up to the horizon*/

		const cv::Point O_ICCS_ICS;			/**< Origin of Image-Center-CS in Image-CS*/
		const cv::Point O_ICS_ICCS;			/**< Origin of Image-CS in Image-Center-CS*/


		Camera():
			 NAME (CAMERA_NAME),
			 MATRIX_INTRINSIC(getCameraMatrix("CAMERA_MATRIX_INTRINSIC")),
			 MATRIX_EXTRINSIC(getCameraMatrix("CAMERA_MATRIX_EXTRINSIC")), 
			 RES_VH(getCameraRES("CAMERA_RES")),
			 FOV_VH(Vector2f(2*atan( (RES_VH(0)/2.0) / (MATRIX_INTRINSIC.at<float>(1,1)) )*180/M_PI ,
					 2*atan( (RES_VH(1)/2.0) / (MATRIX_INTRINSIC.at<float>(0,0)) )*180/M_PI )),
			 PITCH_ANGLE(atan2(MATRIX_EXTRINSIC.at<float>(1,0),MATRIX_EXTRINSIC.at<float>(0,0)) * 180/M_PI -0.5),
			 HORIZON( round((PITCH_ANGLE * RES_VH(0)) / FOV_VH(0) ) ),
			 FOV_HORIZON(FOV_VH(0) / 2 + PITCH_ANGLE),
			 RES_HORIZON(FOV_HORIZON / FOV_VH(0) * RES_VH(0)),			 
			 O_ICCS_ICS( cv::Point( RES_VH[1]/2,  RES_VH[0]/2) ), 
			 O_ICS_ICCS( cv::Point(-RES_VH[1]/2, -RES_VH[0]/2) ){ }

private:
	   cv::Mat getCameraMatrix(std::string Mat_name)
	   {
		int lSuccess = 0;

		cv::Mat 	lCAMERA_MATRIX;

		stringstream 	lFormattedString;
		string 		lFile, lPath;

		// Read location of Binary
		char lBuff[65536];
		ssize_t lLen = ::readlink("/proc/self/exe", lBuff, sizeof(lBuff)-1);
		//ssize_t lLen = ::readlink("/home/vedecom/TUeLaneTracker/install/", lBuff, sizeof(lBuff)-1);

		if (lLen!=-1)
		{
		  lPath = std::string(lBuff);
		  //std::cout << "Path: " << lPath << endl;
		  std::string::size_type Idx = lPath.find_last_of("/");
		  lPath = lPath.substr(0,Idx);
		}
		else
		{
		  #ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
		   <<"Unable to find the path to binary"<<endl
		   <<"[Searching for Camera configuration files]: "<<endl;
		  #endif

		  std::cout<<"Unable to find the path to binary"<<endl;
		  throw "Camera Instantiation Failed" ;
		}

		lFormattedString<<lPath<<"/ConfigFiles/Camera/"<<NAME<<".yaml";
		lFile = lFormattedString.str();

		struct 	stat  lBufStat;
		lSuccess |= stat(lFile.c_str(),&lBufStat);

		if ( lSuccess !=0 )
		{
		  #ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
		   <<"Unable to load camera configuration: "<<endl
		   << "File not found: " << lFile.c_str() << endl;
		  #endif
		  std::cout<<"Unable to load camera configuration: "<<endl;
		  throw "Camera Instantiation Failed" ;
		}
		else
		{
		  cv::FileStorage loadFile( lFile, cv::FileStorage::READ);
		  loadFile[Mat_name]>> lCAMERA_MATRIX;
		}

		return lCAMERA_MATRIX;
	   }



	  Vector2i getCameraRES(std::string Mat_name)
	  {

	     	int 		lSuccess = 0; 
	     	cv::Size	lRES(0,0);

		

		stringstream 	lFormattedString;
		string 		lFile, lPath;

		// Read location of Binary
		char lBuff[65536];
		ssize_t lLen = ::readlink("/proc/self/exe", lBuff, sizeof(lBuff)-1);
		//ssize_t lLen = ::readlink("/home/vedecom/TUeLaneTracker/install/", lBuff, sizeof(lBuff)-1);

		if (lLen!=-1)
		{
		  lPath = std::string(lBuff);
		  //std::cout << "Path: " << lPath << endl;
		  std::string::size_type Idx = lPath.find_last_of("/");
		  lPath = lPath.substr(0,Idx);
		}
		else
		{
		  #ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
		   <<"Unable to find the path to binary"<<endl
		   <<"[Searching for Camera configuration files]: "<<endl;
		  #endif
		  std::cout<<"Unable to find the path to binary:"<<endl;
		  throw "Camera Instantiation Failed" ;
		}

		lFormattedString<<lPath<<"/ConfigFiles/Camera/"<<NAME<<".yaml";
		lFile = lFormattedString.str();

	     	struct  stat  lBufStat;

	     	lSuccess |= stat(lFile.c_str(),&lBufStat);

	    	if ( lSuccess != 0 )
	    	{
	       	  #ifdef PROFILER_ENABLED
		  LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
		  <<"Unable to load camera configuration: "<<endl
		  << "File not found: " << lFile.c_str() << endl;
	          #endif
		  std::cout<<"Unable to load camera configuration: "<<endl;
		  throw "Camera Instantiation Failed" ;
	    	}
	    	else
	    	{
		  cv::FileStorage loadFile( lFile, cv::FileStorage::READ);
		  loadFile[Mat_name]>> lRES;
	    	}

	   	return Vector2i(lRES.height, lRES.width);
	 }
};

inline ostream& operator<<(ostream& os, const Camera& lCamera)
{
  os<<endl<<"[Camera Properties]"<<endl;

  os<<"***********************************************"<<endl;
  os<<"Name			: "<<lCamera.NAME<<endl;
  os<<"Extrinsic Camera Matrix	: "<<endl<<lCamera.MATRIX_EXTRINSIC<<endl<<endl;
  os<<"Intrinsic Camera Matrix	: "<<endl<<lCamera.MATRIX_INTRINSIC<<endl<<endl;
  os<<"Resolution[VxH]		: "<<"[ "<<lCamera.RES_VH[0]<<" x "<<lCamera.RES_VH[1]<<" ]"<<endl;
  os<<"Field-of-View [V, H]	: "<<"[ "<<lCamera.FOV_VH[0]<<" , "<<lCamera.FOV_VH[1]<<" ]"<<endl;
  os<<"Origin-ICCS-ICS		: "<<lCamera.O_ICCS_ICS<<endl;
  os<<"Origin-ICS-ICCS		: "<<lCamera.O_ICS_ICCS<<endl;
  os<<"***********************************************"<<endl;

  return os;
}

#endif // CAMERA_H
