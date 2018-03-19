#define VEDECOMLOGS

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  int lReturn =0;

  if(argc >= 2)
  {

  #ifndef VEDECOMLOGS
    Size lRES = cv::Size(640, 480);
  #else
    Size lRES = cv::Size(960, 600);    
  #endif

    ostringstream lStringStream;
    lStringStream <<argv[1]<<"_"<<lRES.width<<"x"<<lRES.height<<".yaml";


    FileStorage fs (lStringStream.str(), FileStorage::WRITE);

    time_t rawtime; time(&rawtime);
    fs<< "callibrationDate" << asctime(localtime(&rawtime));

  #ifndef VEDECOMLOGS
    // 3x3 Intrinsic Parameters
    Mat CameraMatrixIntrinsic = (Mat_<float>(3,3) << 554.25626,    0,  		0,
						     	0,  	579.41125,  	0,
						    	0,  	   0,  		1 );

    // 4x4  Extrinsic Parameters [Homogenous Transformation]
    Mat CameraMatrixExtrinsic = (Mat_<float>(4,4)<< 1,  0,  0,   0,
						    0,  0, -1,  1.5,
						    0,  1,  0,   0,
						    0,  0,  0,   1);
  #else
// 3x3 Intrinsic Parameters
    Mat CameraMatrixIntrinsic = (Mat_<float>(3,3) << 1039.232,    0,  		488.58893,
						     	0,  	1049.1925,  	309.27204,
						    	0,  	   0,  		1 );
    float pitchAngle = 0.5 / 180 * M_PI;
    // 4x4  Extrinsic Parameters [Homogenous Transformation]
    Mat CameraMatrixExtrinsic = (Mat_<float>(4,4)<< cos(pitchAngle),  -sin(pitchAngle),  0,   0.195,
						    sin(pitchAngle),  cos(pitchAngle), 0,  1.115,
						    0,  0,  1,   1.809,
						    0,  0,  0,   1);
  #endif


    // Store Camera Parameters in a file
    fs << "CAMERA_RES"<< lRES;  	
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
