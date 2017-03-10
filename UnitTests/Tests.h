#include "UnitTest++/UnitTest++.h"
#include <fstream>
#include <stdlib.h>
#include <Eigen/Dense>
#include <Eigen/Core>
using namespace Eigen;
using namespace std;

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
	
	
	
		VectorXd readCSV(std::string file,  int size) 
	{

		  string fullFileName= Path + file;	
		  std::ifstream in(fullFileName);
		  std::string line;
		  
		  int element = 0;

		  Eigen::VectorXd res = Eigen::VectorXd(size);
		  
		  if (in.is_open()) 
		  {
			  std::getline(in, line) ;
			  
			  char *ptr = (char *) line.c_str();
			  int len = line.length();

			  const char *start = ptr;
			  for (int i = 0; i < len; i++) 
			  {
				if (ptr[i] == ',') 
				{
					res(element++) = strtod(start, NULL);
					start = ptr + i + 1;
				}
			  }
			res(element) = strtod(start, NULL);
			in.close();
		  
		  return res;
		}
	}
	
	

};
