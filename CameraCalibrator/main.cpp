#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>

using namespace cv;
using namespace std;

#define  RES_V	600   // Vertical Resolution in pixels.
#define  RES_H  960   // Horizental Resoluttion in pixels.
#define  PITCH  0.5    // Pitch angle in degrees.
#define  YAW    0.0    // Yaw angle in degrees.

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
    Mat CameraMatrixIntrinsic = (Mat_<float>(3,3) << 554.25626,    0,  		0,
						     	0,  	579.41125,  	0,
						    	0,  	   0,  		1 );

    // 4x4  Extrinsic Parameters [Homogenous Transformation]
    // This Matrix is specific to the world coordinates definition
    // set the value of camera pitch and yaw to calculate this matrix acoordingly
    // ^TODO: calculate from Pitch and Yaw 
    Mat CameraMatrixExtrinsic = (Mat_<float>(4,4)<< 1,  0,  0,   0,
						    0,  0, -1,  1.5,
						    0,  1,  0,   0,
						    0,  0,  0,   1);

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
