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
#include "Config.h"
#include <Eigen/Dense>
using namespace Eigen;

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
	

public:
		const Vector2i	RES_VH; 	    	// resolution of camera
		const Vector2i  FRAME_CENTER;		// frame center in the image coordinate system
		const Vector2f  FOV;				// field of view of camera VH
		const float  	HEIGHT;		    	// camera height in meters
		const double 	FOCAL_LENGTH;    	// camera focal length in meters
		const double    CM_TO_PIXEL;		// CM_TO_PIXEL RATIO
				

		Camera() :

			   RES_VH(Vector2i(CAMERA_RES_V, CAMERA_RES_H)),
		
			   FRAME_CENTER(Vector2i(RES_VH[0]/2, RES_VH[1]/2)),
		 
			   FOV(Vector2f(CAMERA_FOV_V , CAMERA_FOV_H)),// Field of View of the camera [V H] degrees
		 
			   HEIGHT(CAMERA_HEIGHT),	// Height of the camera in meters
		 
			   FOCAL_LENGTH(CAMERA_FOCAL_LENGTH), // Focal length in meters
  
			   CM_TO_PIXEL(getCM_TO_PIXEL())
		{
			
		}
		
	   ~Camera(){}
};
#endif // CAMERA_H

