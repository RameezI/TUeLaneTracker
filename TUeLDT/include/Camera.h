#ifndef CAMERA_H
#define CAMERA_H
#include <Eigen/Dense>

using namespace Eigen;

class Camera
{
	
private:

	   double getCM_TO_PIXEL();
	
public:
		const Vector2i	RES_VH; 	    	// resolution of camera
		const Vector2i  FRAME_CENTER;		// frame center in image coordinate system
		const Vector2f  FOV;				// field of view of camera
		const float  	HEIGHT;		    	// camera height in meters
		const double 	FOCAL_LENGTH;    	// camera focal length in meters
		const double    CM_TO_PIXEL;		// CM_TO_PIXCEL RATIO
				
		Camera();
	   ~Camera();
};
#endif // CAMERA_H
