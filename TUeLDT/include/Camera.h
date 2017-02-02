#ifndef CAMERA_H
#define CAMERA_H
#include <Eigen/Dense>

using namespace Eigen;

struct CameraProperties
	{ 		
		public:
		const Vector2i	RES_VH; 	    	// resolution of camera
		const Vector2i  FRAME_CENTER;		// frame center in image coordinate system
		const Vector2f  FOV;				// field of view of camera
		const float  	HEIGHT;				// camera height in meters
		const double 	FOCAL_LENGTH;    	// camera focal length in meters

		
		CameraProperties() //Initialisation of  members through initialisation list
		:
		 RES_VH(Vector2i(480, 640)),
		
		 FRAME_CENTER(Vector2i(480/2, 640/2)),
		 
		 FOV(Vector2f(45 , 60)),
		 
		 HEIGHT(1.5),
		 
		 FOCAL_LENGTH(6.0e-3)
		{
			//Default Constructor
		}
		 
	};



class Camera
{
	
private:
	double CM_TO_PIXEL();
	
public:
     const CameraProperties mPROPERTIES;
	 const double mCM_TO_PIXEL;			// CM_TO_PIXCEL RATIO
	 Camera();
	~Camera();

};





#endif // CAMERA_H
