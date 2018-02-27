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
#include <sys/stat.h>
#include "Config.h"
#include <Eigen/Dense>


using namespace Eigen;
using namespace std;

struct Camera
{
	
private:

	   double getCM_TO_PIXEL()
	   {
		   float 	H_FOV_V     	 	= FOV[0]/2;
		   int 		H_RES_V     	 	= RES_VH[0]/2;  
		   float 	H_FOV_H     	 	= FOV[1]/2;    
		   int 		H_RES_H     	 	= RES_VH[1]/2;

		   double  	PX_SIZE     	 	= tan(H_FOV_V * M_PI /180.0)*FOCAL_LENGTH/H_RES_V;

		   const 	VectorXd vRows 	 	= VectorXd::LinSpaced(H_RES_V,1,H_RES_V);
		   const   	VectorXd PX_ANG  	= atan( PX_SIZE * (vRows.array()/FOCAL_LENGTH) ) * 180.0 /M_PI ;
		   const	VectorXd DEPTH_P 	= HEIGHT * tan( (90 - PX_ANG.array()) * M_PI/180.0 );
	
		  //The horizontal pixel to cm ratio at the bottom
		   return 	H_RES_H/(100*tan(H_FOV_H * M_PI /180.0)*DEPTH_P(H_RES_V-1));

	   }

	   cv::Mat getCameraMatrix(std::string Mat_name)
	   {

		  stringstream formattedString;
		  string file, prefix, path;

		 // Read location of Binary
		  char lBuff[65536];

/*
		  if (len!=-1)
		  {
			path = std::string(lBuff);
			std::string::size_type Idx = path.find_last_of("/");
			path =	path.substr(0,Idx);
		  }
		  else
		  {
			#ifdef PROFILER_ENABLED
			 LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
			 <<"Unable to find the path to binary [searching for Camera configuration files]: "<<endl;
			#endif

			exit(-2);
		  }

		  prefix= "CONFIG_";

		  formattedString<<path<<"/ConfigFiles/CAMERA/"<<prefix<<CAMERA_NAME;
		  file = formattedString.str();

		  struct stat  buf;
		  int 	 statResult = stat(file.c_str(),&buf);

		  if (statResult != 0)
		  {
			#ifdef PROFILER_ENABLED
			 LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
			 <<"Unable to find the path to binary [searching for Camera configuration files]: "<<endl
			 << "File not found: " << file.c_str() << endl;
			#endif

			exit(-2);
		  }
		  else
		  {
			  cv::FileStorage loadFile( file, cv::FileStorage::READ);
			  loadFile[Mat_name]>> CAMERA_MATRIX_INTRINSIC;
		  }

*/



	   }
	

public:
		const Vector2i	RES_VH; 	    				/*<  Resolution of camera 							*/
		const Vector2i  FRAME_CENTER;					/*<  Frame center in the image coordinate system	*/
		const Vector2f  FOV;							/*<  Field of view of camera VH						*/
		const float  	HEIGHT;		    				/*<  Camera height in meters						*/
		const double 	FOCAL_LENGTH;    				/*<  Camera focal length in meters					*/
		const double    CM_TO_PIXEL;					/*<  Conversion ratio at the base line				*/
		const string	CAMERA_NAME;					/*<  Camera Identifier								*/
		cv::Mat			CAMERA_MATRIX_INTRINSIC;		/*<  Camera Intrinsic Parameters 3x3 				*/
		cv::Mat			CAMERA_MATRIX_EXTRINSIC;		/*<  Camera Extrinsic Parameters 4x4				*/


		Camera() :

		RES_VH(Vector2i(CAMERA_RES_V, CAMERA_RES_H)),

		FRAME_CENTER(Vector2i(RES_VH[0]/2, RES_VH[1]/2)),

		FOV(Vector2f(CAMERA_FOV_V , CAMERA_FOV_H)),// Field of View of the camera [V H] degrees

		HEIGHT(CAMERA_HEIGHT),	// Height of the camera in meters

		FOCAL_LENGTH(CAMERA_FOCAL_LENGTH), // Focal length in meters

		CM_TO_PIXEL(getCM_TO_PIXEL()),
		
		CAMERA_NAME ("BUMBLEBEE_640x480")

		{


				// CAMERA_MATRIX_EXTRINSIC


		}
		
	   ~Camera(){}
};
#endif // CAMERA_H

