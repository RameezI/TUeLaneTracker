#include "UnitTest++/UnitTest++.h"
#include <fstream>
#include <stdlib.h>
#include <Eigen/Dense>
#include <Eigen/Core>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/core/eigen.hpp>

using namespace cv;
using namespace Eigen;
using namespace std;

class Tests 
{

	private:
	string Path;
		
	public:
	Tests()
	: Path("../TestData/")
	{
		
	}
	
	void saveMatToCsv(Mat &matrix, string filename)
	{
		ofstream outputFile(filename);
		outputFile << format(matrix, cv::Formatter::FMT_CSV) << endl;
		outputFile.close(); 
	}
	
		Mat loadCSV(std::string file,  int opencv_type) 
	{
	      Mat m;

		  string fullFileName= Path + file;	
		  std::ifstream in(fullFileName);
		  std::string line;
		  
		  int row = 0;
		  int col = 0;
		  
		  if (in.is_open()) 
		  {
			while (std::getline(in, line)) 
			{
				vector<double> dvals;
				
				stringstream ssline(line);
				string val;
				
				while (getline(ssline, val, ','))
				{
				  dvals.push_back(stod(val));
				}

				Mat mline(dvals, true);
				transpose(mline, mline);

				m.push_back(mline);

			}
			
			int ch = CV_MAT_CN(opencv_type);
			m = m.reshape(ch);
			m.convertTo(m, opencv_type);
			in.close();	
		  }
		  
		  return m;
	}

};
