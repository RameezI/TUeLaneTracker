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

		const Vector2i	RES_VH; 	    		/**< Resolution of the camera image */
		const Vector2f  FOV_VH;				/**  Field-of-View of the camera */

		const cv::Point O_ICCS_ICS;			/**< Origin of Image-Center-CS in Image-CS*/
		const cv::Point O_ICS_ICCS;			/**< Origin of Image-CS in Image-Center-CS*/

		const cv::Mat	MATRIX_INTRINSIC;	/**<  Camera Intrinsic Parameters 3x4 */
		const cv::Mat	MATRIX_EXTRINSIC;	/**<  Camera Extrinsic Parameters 4x4 */


		Camera():
			 NAME ("BUMBLEBEE_640x480"), 
			 RES_VH(Vector2i(CAMERA_RES_V, CAMERA_RES_H)),
			 FOV_VH(Vector2f(CAMERA_FOV_V, CAMERA_FOV_H)), 
			 O_ICCS_ICS( cv::Point( RES_VH[1]/2,  RES_VH[0]/2) ), 
			 O_ICS_ICCS( cv::Point(-RES_VH[1]/2, -RES_VH[0]/2) ),
			 MATRIX_INTRINSIC(getCameraMatrix("CAMERA_MATRIX_INTRINSIC")),
			 MATRIX_EXTRINSIC(getCameraMatrix("CAMERA_MATRIX_EXTRINSIC")) 
		{
		   	if (MATRIX_INTRINSIC.empty() | MATRIX_EXTRINSIC.empty() )
		   	   throw "Camera Instantiation Failed" ;
		}

private:

	   cv::Mat getCameraMatrix(std::string Mat_name)
	   {
		int lSuccess = 0;

		cv::Mat 	lCAMERA_MATRIX;

		stringstream 	formattedString;
		string 		file, path;

		// Read location of Binary
		char lBuff[65536];
		ssize_t len = ::readlink("/proc/self/exe", lBuff, sizeof(lBuff)-1);

		if (len!=-1)
		{
		  path = std::string(lBuff);
		  std::string::size_type Idx = path.find_last_of("/");
		  path = path.substr(0,Idx);
		}
		else
		{
		  #ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
		   <<"Unable to find the path to binary"<<endl
		   <<"[Searching for Camera configuration files]: "<<endl;
		  #endif
		  lSuccess =-1;
		}

		formattedString<<path<<"/ConfigFiles/Camera/"<<NAME<<".yaml";
		file = formattedString.str();

		struct 	stat  	buf;
		int 	statResult = stat(file.c_str(),&buf);

		if ( (statResult != 0) | (lSuccess !=0) )
		{
		  #ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
		   <<"Unable to load camera configuration: "<<endl
		   << "File not found: " << file.c_str() << endl;
		  #endif
		}
		else
		{
		  cv::FileStorage loadFile( file, cv::FileStorage::READ);
		  loadFile[Mat_name]>> lCAMERA_MATRIX;
		}
		
		return lCAMERA_MATRIX;

	   }
};

inline ostream& operator<<(ostream& os, const Camera& lCamera)
{
  os<<endl<<"[Camera Propoerties]"<<endl;
  os<<"Name			: "<<lCamera.NAME<<endl;
  os<<"Resolution[VxH]		: "<<"[ "<<lCamera.RES_VH[0]<<" x "<<lCamera.RES_VH[1]<<" ]"<<endl;
  os<<"Field-of-View [V, H]	: "<<"[ "<<lCamera.FOV_VH[0]<<" , "<<lCamera.FOV_VH[1]<<" ]"<<endl;
  os<<"Origin-ICCS-ICS		: "<<lCamera.O_ICCS_ICS<<endl;
  os<<"Origin-ICS-ICCS		: "<<lCamera.O_ICS_ICCS<<endl;
  os<<"Extrinsic Camera Matrix	: "<<endl<<lCamera.MATRIX_EXTRINSIC<<endl;
  os<<"Intrinsic Camera Matrix	: "<<endl<<lCamera.MATRIX_INTRINSIC;

  return os;
}

#endif // CAMERA_H
