#ifndef CAMERA_H
#define CAMERA_H
#include <Eigen/Dense>

using namespace Eigen;

class Camera
{
	struct Properties
	{ 		
		public:
		const Vector2f	RES_VH; 	    	// resolution of camera
		const Vector2f  FRAME_CENTER;		// frame center in image coordinate system
		const Vector2f  FOV;				// field of view of camera
		const float  	HEIGHT;				// camera height in meters
		const double 	FOCAL_LENGTH;    	// camera focal length in meters

		
		Properties() //Initialisation of  members through initialisation list
		:
		RES_VH(Vector2f(480, 640)),
		
		FRAME_CENTER(Vector2f(480/2, 640/2)),
		 
		 FOV(Vector2f(45 , 60)),
		 
		 HEIGHT(1.5),
		 
		 FOCAL_LENGTH(6.0e-3)
		{
			//Default Constructor
		}
		 
	};
	
	double CM_TO_PIXEL(Properties);
	
public:
     Properties mProperties;
	 const double mCM_TO_PIXEL;			// CM_TO_PIXCEL RATIO
	 Camera();
	~Camera();

};

#endif // CAMERA_H
