#include "UnitTest++/UnitTest++.h"
#include  "Camera.h"
#include  "Lane.h"
#include  "LaneFilter.h"
#include <fstream>
#include <stdlib.h>
#include <Eigen/Dense>



using namespace Eigen;
using namespace std;


class Tests 
{

	private:
	string Path;
		
	public:
	Tests()
	: Path("/home/rameez/TUE_Lane_Tracker/")
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


class TEST_LaneFilter:Tests
{

	
public:
	  MatrixXd exp_FilterMatrix;
	  MatrixXd exp_LanePrior;
	  VectorXd exp_BINS;
	  MatrixXd exp_LaneTransition;
	  shared_ptr<Camera> camera= make_shared<Camera>();
	  shared_ptr<Lane>   lane = make_shared<Lane>();
	  
	   LaneFilter laneFilter;
		TEST_LaneFilter()
		: laneFilter (LaneFilter(lane, camera))
		{
			exp_BINS = readCSV("LANE_BINS_H.csv",  floor((2*laneFilter.PX_MAX)/laneFilter.STEP_PX) +1 );
			exp_LanePrior = readCSV("LANE_PRIOR.csv",  floor(laneFilter.PX_MAX/laneFilter.STEP_PX) +1 , floor(laneFilter.PX_MAX/laneFilter.STEP_PX) +1);
			exp_LaneTransition = readCSV("LANE_TRANSITION.csv", 7, 7);
		}
		~TEST_LaneFilter()
		{
			
		}

};



SUITE(TUeLDT_Camera)
{
	
	Camera cam;
	
	float 	H_FOV_V     = cam.mProperties.FOV[0]/2;
	int 	H_RES_V     = cam.mProperties.RES_VH[0]/2;  
    float 	H_FOV_H     = cam.mProperties.FOV[1]/2;    
    int 	H_RES_H     = cam.mProperties.RES_VH[1]/2;  
	double  PX_SIZE     	 = tan(H_FOV_V * M_PI /180.0)*cam.mProperties.FOCAL_LENGTH/H_RES_V;
	const 	VectorXd vRows 	 = VectorXd::LinSpaced(cam.mProperties.RES_VH[0],1,cam.mProperties.RES_VH[0]);
    const   VectorXd PX_ANG  = atan( PX_SIZE * (vRows.array()/cam.mProperties.FOCAL_LENGTH) ) * 180.0 /M_PI ;	
	const	VectorXd DEPTH_P = cam.mProperties.HEIGHT * tan( (90 - PX_ANG.array()) * M_PI/180.0 );
	double a = 	H_RES_H/(100*tan(H_FOV_H * M_PI /180.0)*DEPTH_P(H_RES_V-2)); //The horizontal pixel to cm ratio at the bottom line of the image



	 
    TEST(CameraPrameters)
    {
		

		
        CHECK_EQUAL(240, 	cam.mProperties.FRAME_CENTER(0,0));
		CHECK_EQUAL(320, 	cam.mProperties.FRAME_CENTER(1,0));
		CHECK_EQUAL(0.0060, cam.mProperties.FOCAL_LENGTH);
		CHECK_CLOSE(1.0355e-05, PX_SIZE,1.0e-9);
		CHECK_EQUAL(vRows[5], 6);
		CHECK_CLOSE(0.098886105737275, PX_ANG[0],1.0e-9);
		CHECK_CLOSE(37.834715685796944, PX_ANG[449],1.0e-9);
		CHECK_CLOSE(0.395538532089772, PX_ANG[3],1.0e-9);
		CHECK_CLOSE(30.551412806326798, PX_ANG[341],1.0e-9);		
		CHECK_CLOSE(869.1169, DEPTH_P[0],1.0e-4);
		CHECK_CLOSE(1.931370849898475, DEPTH_P[449],1.0e-4);
		CHECK_CLOSE(217.2792, DEPTH_P[3],1.0e-4);
		CHECK_CLOSE(2.5413, DEPTH_P[341],1.0e-4);

		CHECK_CLOSE(1.5242, a, 1.0e-4);
		
		CHECK_CLOSE(1.5242, cam.mCM_TO_PIXEL, 1.0e-4);
		
    }
	
	
	
	// Tests for Lane Object ^TODO: Need to write Test and cross check with Matlab
	Lane lane;
	
	    TEST(LaneParameters)
    {
        CHECK_EQUAL(300, lane.mProperties.AVG_WIDTH);
    }
	
	
	// Tests for Lane Filter.
	TEST_LaneFilter testLaneFilter;

	TEST(LaneFilter)
    {
		
		
		
		 CHECK_EQUAL(cam.mCM_TO_PIXEL, testLaneFilter.laneFilter.mCamera->mCM_TO_PIXEL);
		 CHECK_EQUAL(5, testLaneFilter.laneFilter.STEP_CM) ;
		 CHECK_EQUAL(10, testLaneFilter.laneFilter.STEP_PX) ;
		 CHECK_EQUAL(760, testLaneFilter.laneFilter.PX_MAX) ;
		 CHECK_EQUAL(-240, testLaneFilter.laneFilter.FILTER_OFFSET) ;
		 CHECK_EQUAL(2, testLaneFilter.laneFilter.CONF_THRESH) ;
		 CHECK_EQUAL(153, floor(2*testLaneFilter.laneFilter.PX_MAX/testLaneFilter.laneFilter.STEP_PX) +1) ;
		 CHECK_EQUAL(77, floor(testLaneFilter.laneFilter.PX_MAX/testLaneFilter.laneFilter.STEP_PX) +1 ) ;
		 
		 CHECK_EQUAL(153, testLaneFilter.laneFilter.mBins.size() );
		 CHECK_EQUAL(153, testLaneFilter.exp_BINS.size());
		 
		 CHECK_EQUAL(77, testLaneFilter.laneFilter.mPrior.rows());
		 CHECK_EQUAL(77, testLaneFilter.laneFilter.mPrior.cols());
		 
		 
		 
		 MatrixXd exp_LanePrior = testLaneFilter.exp_LanePrior;
		 MatrixXd act_LanePrior = testLaneFilter.laneFilter.mPrior;
		 MatrixXd exp_LaneTransition = testLaneFilter.exp_LaneTransition;
		 MatrixXd act_LaneTransition = testLaneFilter.laneFilter.mTransition;
		 
		 CHECK_ARRAY_CLOSE(testLaneFilter.exp_BINS.data(), testLaneFilter.laneFilter.mBins.data(), testLaneFilter.exp_BINS.size(), 1.0e-4);
		 CHECK_ARRAY_CLOSE(exp_LanePrior.data(), act_LanePrior.data(), 77*77, 1.0e-6);
		 CHECK_ARRAY_CLOSE(exp_LaneTransition.data(), act_LaneTransition.data(), 7*7, 1.0e-6);
		 
		 

    }
	
	
	
	
	
}