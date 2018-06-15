#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
#include <cmath>

using namespace cv;
using namespace std;



//Camera Resolution
#define RES_V	480   		// Vertical   Resolution  [pixels].
#define RES_H  	640   		// Horizental Resolution  [pixels].

// Camera Intrinsic defines
#define FX	554.25626	//Focal lenght x-axis [pixels]
#define FY	579.41125  	//Focal lenght y-axis [pixels]
#define CX	0	   	//Principal point x-axis [pixels]
#define CY	0		//Principal point x-axis [pixels]


#define PITCH  			0.0    	// Camera Pitch [degrees].
#define YAW    			0.0    	// Camera yaw a [degrees].
#define CAM_HEIGHT    		1.5	//Camera height from the road[meters].
#define CAM_LATERAL_OFFSET	0	//Lateral-Offset of the camera from the vehicle symmetry plane[meters].



double toRad(double deg)
{
  double radians = deg*(180/M_PI); 
  return radians;
}

int main( int argc, char** argv )
{
  int lReturn =0;

  if(argc >= 2)
  {

    Mat lRES_VH	= (Mat_<float>(1,2) << RES_V, RES_H );
    Mat lROT_PY = (Mat_<float>(1,2) << PITCH, YAW );


    ostringstream lStringStream;
    lStringStream <<argv[1]<<"_"<<RES_H<<"x"<<RES_V<<".yaml";


    FileStorage fs (lStringStream.str(), FileStorage::WRITE);

    time_t rawtime; time(&rawtime);
    fs<< "callibrationDate" << asctime(localtime(&rawtime));


    // 3x3 Intrinsic Parameters
    // This Matrix is for performing calculations in Image-Center-CS
    Mat CameraMatrixIntrinsic = (Mat_<float>(3,3) << FX,    0,   CX,
						     0,     FY,  CY,
						     0,     0,    1 );

   // 4x4 Pitch Transformation
    Mat ROT_P = (Mat_<float>(4,4)<< 1,   0,  				  0,   			            0,
				    0,	 cos(-(M_PI/2.0) + toRad(PITCH)), sin(-(M_PI/2.0) + toRad(PITCH)),  0,
				    0,	-sin(-(M_PI/2.0) + toRad(PITCH)), cos(-(M_PI/2.0) + toRad(PITCH)),  0,
				    0,   0,  				  0,   			       	    1);


   // 4x4 Yaw Transformation
    Mat ROT_Y = (Mat_<float>(4,4)<< cos(toRad(YAW)),   0,  	-sin(toRad(YAW)),   0,
				    0,	 	       1,  	0,		   0,
				    sin(toRad(YAW)),   0,  	cos(toRad(YAW)),   0,
				    0,   	       0,  	0,   		   1);



   // 4x4 Translation
    Mat    T  = (Mat_<float>(4,4)<< 1,   0,  	0,  CAM_LATERAL_OFFSET,
				    0,	 1,     0,  CAM_HEIGHT,
				    0,	 0,	1,  0, //longitudnal displacement between world-CS and camera-CS is 0
				    0,   0,     0,  1);


    // 4x4  Extrinsic Parameters [Homogenous Transformation]
    // This Matrix is specific to the world coordinates definition
    // set the value of camera pitch and yaw to calculate this matrix acoordingly
    Mat CameraMatrixExtrinsic = T*ROT_Y*ROT_P; 


    // Store Camera Parameters in a file
    fs << "CAMERA_RES"<< lRES_VH;  	
    fs << "CAMERA_ROT_PY"<<lROT_PY;  	
    fs << "CAMERA_MATRIX_INTRINSIC" << CameraMatrixIntrinsic;
    fs << "CAMERA_MATRIX_EXTRINSIC" << CameraMatrixExtrinsic;
   
    fs.release();
    cout<< "Callibration file written!" <<endl<<endl;
  }

  else
  {
    cerr<<endl<<"Missing arguments!"<<endl;
    cout<<endl<<"Usage:"<<endl;
    cout<< "	  ./cameraCallibrator IDENTIFIER_FOR_THE_CAMERA " <<endl;
    cout<< "Example:"<<endl;
    cout<< "	  ./cameraCallibrator MAXIM_OV10640"<<endl<<endl<<endl;
    
    lReturn = -1;

  }   
    cout << "Program exited with return value: "<< lReturn <<endl<<endl;
    return lReturn;
}
