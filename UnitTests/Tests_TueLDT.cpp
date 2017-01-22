#include "UnitTest++/UnitTest++.h"
#include  "Camera.h"
#include  "Lane.h"
#include <Eigen/Dense>
using namespace Eigen;


SUITE(TUeLDT_Camera)
{
	
	Camera cam;
	
	float 	H_FOV_V     = cam.mProperties.FOV[0]/2;
	int 	H_RES_V     = cam.mProperties.RES_VH[0]/2;  
    float 	H_FOV_H     = cam.mProperties.FOV[1]/2;    
    int 	H_RES_H     = cam.mProperties.RES_VH[1]/2;  
	double  PX_SIZE     	 = tan(H_FOV_V * M_PI /180.0)*cam.mProperties.FOCAL_LENGTH/H_RES_V;
	const 	VectorXd vRows 	 = VectorXd::LinSpaced(cam.mProperties.RES_VH[0],1,cam.mProperties.RES_VH[0]);
    const   VectorXd PX_ANG  = atan( PX_SIZE * (vRows.array()/cam.mProperties.FOCAL_LENGTH) ) * 180.0 /M_PI ;	
	const	VectorXd DEPTH_P = cam.mProperties.HEIGHT * tan( (90 - PX_ANG.array()) * M_PI/180.0 );
	double a = 	H_RES_H/(100*tan(H_FOV_H * M_PI /180.0)*DEPTH_P(H_RES_V-2)); //The horizontal pixel to cm ratio at the bottom line of the image

	
	 
    TEST(CameraPrameters)
    {
		

		
        CHECK_EQUAL(240, 	cam.mProperties.FRAME_CENTER(0,0));
		CHECK_EQUAL(320, 	cam.mProperties.FRAME_CENTER(1,0));
		CHECK_EQUAL(0.0060, cam.mProperties.FOCAL_LENGTH);
		CHECK_CLOSE(1.0355e-05, PX_SIZE,1.0e-9);
		CHECK_EQUAL(vRows[5], 6);
		CHECK_CLOSE(0.098886105737275, PX_ANG[0],1.0e-9);
		CHECK_CLOSE(37.834715685796944, PX_ANG[449],1.0e-9);
		CHECK_CLOSE(0.395538532089772, PX_ANG[3],1.0e-9);
		CHECK_CLOSE(30.551412806326798, PX_ANG[341],1.0e-9);		
		CHECK_CLOSE(869.1169, DEPTH_P[0],1.0e-4);
		CHECK_CLOSE(1.931370849898475, DEPTH_P[449],1.0e-4);
		CHECK_CLOSE(217.2792, DEPTH_P[3],1.0e-4);
		CHECK_CLOSE(2.5413, DEPTH_P[341],1.0e-4);

		CHECK_CLOSE(1.5242, a, 1.0e-4);
		
		CHECK_CLOSE(1.5242, cam.mCM_TO_PIXEL, 1.0e-4);
		
    }
	
	
	
	
	Lane lane;
	
	    TEST(LaneParameters)
    {
			
        CHECK_EQUAL(2, lane.mProperties.AVG_WIDTH);

		
    }
	
	
}