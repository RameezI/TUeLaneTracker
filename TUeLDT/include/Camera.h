#ifndef CAMERA_H
#define CAMERA_H
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
		   const 	VectorXd vRows 	 	= VectorXd::LinSpaced(RES_VH[0],1,RES_VH[0]);
		   const   	VectorXd PX_ANG  	= atan( PX_SIZE * (vRows.array()/FOCAL_LENGTH) ) * 180.0 /M_PI ;
		   const	VectorXd DEPTH_P 	= HEIGHT * tan( (90 - PX_ANG.array()) * M_PI/180.0 );
	
		  return 	H_RES_H/(100*tan(H_FOV_H * M_PI /180.0)*DEPTH_P(H_RES_V-2)); //The horizontal pixel to cm ratio at the bottom
	   }
	
public:
		const Vector2i	RES_VH; 	    	// resolution of camera
		const Vector2i  FRAME_CENTER;		// frame center in image coordinate system
		const Vector2f  FOV;				// field of view of camera
		const float  	HEIGHT;		    	// camera height in meters
		const double 	FOCAL_LENGTH;    	// camera focal length in meters
		const double    CM_TO_PIXEL;		// CM_TO_PIXCEL RATIO
				
		Camera() : RES_VH(Vector2i(480, 640)),
		
				   FRAME_CENTER(Vector2i(480/2, 640/2)),
		 
				   FOV(Vector2f(45 , 60)),
		 
				   HEIGHT(1.5),
		 
				   FOCAL_LENGTH(6.0e-3),
  
				   CM_TO_PIXEL(getCM_TO_PIXEL())
		{
			
		}
		
	   ~Camera(){}
};
#endif // CAMERA_H

