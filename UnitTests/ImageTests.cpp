#include "UnitTest++/UnitTest++.h"
#include  "InitState.h"
#include  "BufferingState.h"
#include <fstream>
#include <stdlib.h>
#include <Eigen/Dense>
#include <Eigen/Core>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

class Tests 
{

	private:
	string Path;
		
	public:
	Tests()
	: Path("/home/rameez/TUE_Lane_Tracker_v4.0/")
	{
		
	}

	MatrixXd readCSV(std::string file, int rows, int cols) 
	{

		  string fullFileName= Path + file;	
		  std::ifstream in(fullFileName);
		  std::string line;
		  
		  int row = 0;
		  int col = 0;

		  Eigen::MatrixXd res = Eigen::MatrixXd(rows, cols);
		  
		  if (in.is_open()) 
		  {
			while (std::getline(in, line)) 
			{
			  char *ptr = (char *) line.c_str();
			  int len = line.length();
			  col = 0;
			  
			  const char *start = ptr;
			  for (int i = 0; i < len; i++) 
			  {
				if (ptr[i] == ',') 
				{
					res(row, col++) = strtod(start, NULL);
					start = ptr + i + 1;
				}
			  }
			  
			  res(row, col) = strtod(start, NULL);
			  row++;
			}

			in.close();
		  }
		  return res;
	}
};



class TEST_Images:Tests
{
public:

	   BufferingState BF_STATE;
	   double min_H, max_H;

	   
	   
		TEST_Images()	
		{
			
				double min, max;

					
			vector< cv::String> IMG_filenames;
			//cv::String folder = "/media/rameez/Linux-Extended/DataSet/eindhoven/PNG_imgs";
			cv::String folder = "/home/rameez/Downloads/_files";
			
			glob(folder, IMG_filenames);
			
			BF_STATE.setSource(IMG_filenames);
			BF_STATE.run();
//			cv::minMaxLoc(*(BF_STATE.mFrameH), &min_H, &max_H);
		}
		~TEST_Images()
		{
			
		}
		

};


SUITE(TUeLDT_Images)
{

	TEST_Images testImages;
	
    TEST(OPENCV_TYPE_CHECK)
    {
		CHECK_EQUAL(CV_8UC3, 	testImages.BF_STATE.mFrameRGB->type());
		CHECK_EQUAL(CV_32FC1, 	testImages.BF_STATE.mFrameGRAY_float->type());
		//CHECK_EQUAL(CV_32FC3, 	testImages.BF_STATE.mFrameHSV->type());
		
		//CHECK_EQUAL(CV_32FC1, 	testImages.BF_STATE.mFrameH->type());
		//CHECK_EQUAL(CV_32FC1, 	testImages.BF_STATE.mFrameS->type());
		//CHECK_EQUAL(CV_32FC1, 	testImages.BF_STATE.mFrameV->type());
		
		
		//CHECK_CLOSE(1, 	testImages.max_H, 0.01);
    }
	
}