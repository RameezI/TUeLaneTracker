#ifndef CAMERA_H
#define CAMERA_H
#include <Eigen/Dense>
#include <iostream>
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
	
		   //cout<<"vRows"<<endl<<vRows<<endl;

		//The horizontal pixel to cm ratio at the bottom
		  return 	H_RES_H/(100*tan(H_FOV_H * M_PI /180.0)*DEPTH_P(H_RES_V-1)); 
	   }
	
public:
		const Vector2i	RES_VH; 	    	// resolution of camera
		const Vector2i  FRAME_CENTER;		// frame center in the image coordinate system
		const Vector2f  FOV;			// field of view of camera VH
		const float  	HEIGHT;		    	// camera height in meters
		const double 	FOCAL_LENGTH;    	// camera focal length in meters
		const double    CM_TO_PIXEL;		// CM_TO_PIXCEL RATIO
				
		Camera() : RES_VH(Vector2i(480, 640)),
		
			   FRAME_CENTER(Vector2i(RES_VH[0]/2, RES_VH[1]/2)),
		 
			   FOV(Vector2f(45 , 60)),// Field of View of the camera [V H] degrees
		 
			   HEIGHT(1.5),	// Height of the camera in meters
		 
			   FOCAL_LENGTH(6.0e-3), // Focal lenght in meters
  
			   CM_TO_PIXEL(getCM_TO_PIXEL())
		{
			
		}
		
	   ~Camera(){}
};
#endif // CAMERA_H

