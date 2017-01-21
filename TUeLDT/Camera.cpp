#include "Camera.h"
Camera::Camera()
: mCM_TO_PIXEL(CM_TO_PIXEL(mProperties))
{
	
}


double Camera::CM_TO_PIXEL(Properties)
{
	
	
	float 	H_FOV_V     = mProperties.FOV[0]/2;
	int 	H_RES_V     = mProperties.RES_VH[0]/2;  
    float 	H_FOV_H     = mProperties.FOV[1]/2;    
    int 	H_RES_H     = mProperties.RES_VH[1]/2;  
    
	double  PX_SIZE     	 = tan(H_FOV_V * M_PI /180.0)*mProperties.FOCAL_LENGTH/H_RES_V;
	const 	VectorXd vRows 	 = VectorXd::LinSpaced(mProperties.RES_VH[0],1,mProperties.RES_VH[0]);
    const   VectorXd PX_ANG  = atan( PX_SIZE * (vRows.array()/mProperties.FOCAL_LENGTH) ) * 180.0 /M_PI ;
	
	const	VectorXd DEPTH_P = mProperties.HEIGHT * tan( (90 - PX_ANG.array()) * M_PI/180.0 );

	return 	H_RES_H/(100*tan(H_FOV_H * M_PI /180.0)*DEPTH_P(H_RES_V-2)); //The horizontal pixel to cm ratio at the bottom line of the image


    
}


Camera::~Camera()
{
}

