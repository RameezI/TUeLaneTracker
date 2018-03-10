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

    Size lRES = cv::Size(640, 480);

    ostringstream lStringStream;
    lStringStream <<argv[1]<<"_"<<lRES.width<<"x"<<lRES.height<<".yaml";


    FileStorage fs (lStringStream.str(), FileStorage::WRITE);

    time_t rawtime; time(&rawtime);
    fs<< "callibrationDate" << asctime(localtime(&rawtime));


    // 3x3 Intrinsic Parameters
    Mat CameraMatrixIntrinsic = (Mat_<float>(3,4) << 185.75209,    0,  		0,
						     	0,  	99.41125,  	0,
						    	0,  	   0,  		0 );

    // 4x4  Extrinsic Parameters [Homogenous Transformation]
    Mat CameraMatrixExtrinsic = (Mat_<float>(4,4)<< 1,  0,  0,   0,
						    0,  0, -1,  1.5,
						    0,  1,  0,   0,
						    0,  0,  0,   1);


    // Store Camera Parameters in a file
    fs << "CAMERA_RES"<< lRES;  	
    fs << "CAMERA_MATRIX_INTRINSIC" << CameraMatrixIntrinsic;
    fs << "CAMERA_MATRIX_EXTRINSIC" << CameraMatrixExtrinsic;
   
    fs.release();
  }

  else
  {
    cout<< "Missing Command Line Options" <<endl<<endl;

    cout<< "Usage:"<<endl;
    cout<< "	cameraCallibrator <char*> NAME_OF_THE_CALLIBRAION FILE " <<endl;
    
    lReturn = -1;

  }   
    cout << "Program exited with return value: "<< lReturn <<endl;
    return lReturn;
}
