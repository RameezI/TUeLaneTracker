#include "Camera.h"

Camera::Camera()
: mCM_TO_PIXEL(CM_TO_PIXEL())
{
	
}


double Camera::CM_TO_PIXEL()
{
	float 	H_FOV_V     = mPROPERTIES.FOV[0]/2;
	int 	H_RES_V     = mPROPERTIES.RES_VH[0]/2;  
    float 	H_FOV_H     = mPROPERTIES.FOV[1]/2;    
    int 	H_RES_H     = mPROPERTIES.RES_VH[1]/2;  
	double  PX_SIZE     	 = tan(H_FOV_V * M_PI /180.0)*mPROPERTIES.FOCAL_LENGTH/H_RES_V;
	const 	VectorXd vRows 	 = VectorXd::LinSpaced(mPROPERTIES.RES_VH[0],1,mPROPERTIES.RES_VH[0]);
    const   VectorXd PX_ANG  = atan( PX_SIZE * (vRows.array()/mPROPERTIES.FOCAL_LENGTH) ) * 180.0 /M_PI ;
	const	VectorXd DEPTH_P = mPROPERTIES.HEIGHT * tan( (90 - PX_ANG.array()) * M_PI/180.0 );
	return 	H_RES_H/(100*tan(H_FOV_H * M_PI /180.0)*DEPTH_P(H_RES_V-2)); //The horizontal pixel to cm ratio at the bottom line of the image
    
}


Camera::~Camera()
{
}

