#include "UnitTest++/UnitTest++.h"
#include  "Camera.h"
#include  "Lane.h"
#include  "LaneFilter.h"
#include  "VanishingPtFilter.h"
#include  "InitState.h"
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


class TEST_LaneFilter:Tests
{
public:
	  MatrixXd exp_FilterMatrix;
	  MatrixXd exp_LanePrior;
	  VectorXd exp_BINS_HISTOGRAM;
	  MatrixXd exp_LaneTransition;
	  
	  Camera camera;
	  Lane   lane;
	  
	   LaneFilter laneFilter;
		TEST_LaneFilter()
		: laneFilter (LaneFilter(lane.mPROPERTIES, camera.mCM_TO_PIXEL))
	
		{

			exp_BINS_HISTOGRAM = readCSV("LANE_BINS_H.csv",  floor((2*laneFilter.mPX_MAX)/laneFilter.mSTEP_PX) +1 );
			exp_LanePrior = readCSV("LANE_PRIOR.csv",  floor(laneFilter.mPX_MAX/laneFilter.mSTEP_PX) +1 , floor(laneFilter.mPX_MAX/laneFilter.mSTEP_PX) +1);
			exp_LaneTransition = readCSV("LANE_TRANSITION.csv", 7, 7);
		}
		~TEST_LaneFilter()
		{
			
		}
		
		
};



class TEST_VPFilter:Tests
{
public:
	  MatrixXd exp_Filter;
	  MatrixXd exp_Prior;
	  MatrixXd exp_Transition;
	  VectorXd exp_VpBINS_V;
	  VectorXd exp_VpBINS_H;
	  VectorXd exp_BINS_HISTOGRAM;
	  
	  
	  Camera camera;
	  Lane   lane;
	  
	   
	   LaneFilter 	laneFilter;
	   VanishingPtFilter* vpFilter;   
	   
	  TEST_VPFilter()
		:laneFilter (LaneFilter(lane.mPROPERTIES, camera.mCM_TO_PIXEL))   //^TODO: Passing EigenMatrix to the constructor in initialisation list doesnot work.  
		{
			vpFilter = new VanishingPtFilter(laneFilter.mBINS_HISTOGRAM, camera.mPROPERTIES);
			exp_VpBINS_V 		= readCSV("VP_BINS_V.csv",  6);
			exp_VpBINS_H 		= readCSV("VP_BINS_H.csv",  61);
			exp_BINS_HISTOGRAM 	= readCSV("VP_BINS_HST.csv",153);
			
			exp_Filter 			= readCSV("VP_FILTER.csv", 6, 61 );
			exp_Prior 			= readCSV("VP_PRIOR.csv", 6, 61);
			exp_Transition   	= readCSV("VP_TRANSITION.csv" , 3, 3 );
		}
		
		~TEST_VPFilter()
		{
			
		}
};



class TEST_InitState:Tests
{

public:

	  Camera camera;
	  Lane   lane;
	  
	  MatrixXd exp_FOCUS_ROOT;
	  MatrixXd exp_DEPTH_ROOT;
	  MatrixXd exp_GRADIENT_DIR_ROOT;

	  
	  shared_ptr<InitState> initState;
	  
	  TEST_InitState()
		
		{ 

			initState   			=   make_shared<InitState>(camera.mPROPERTIES);
			initState->run();
			exp_FOCUS_ROOT			= readCSV("FOCUS_ROOT.csv", 2*480+1, 640);
			exp_DEPTH_ROOT			= readCSV("DEPTH_ROOT.csv", 2*480+1, 640);
			exp_GRADIENT_DIR_ROOT	= readCSV("GRADIENT_DIR_ROOT.csv", 2*480+1, 2*640 +1);
			
			ofstream file("DIR_ROOT_Eigen.csv");
			const static IOFormat CSVFormat(StreamPrecision, DontAlignCols, ", ", "\n");
			file<<initState->mTemplates->GRADIENT_DIR_ROOT.format(CSVFormat); 

		}
		
		~TEST_InitState()
		{
			
		}
};








SUITE(TUeLDT_Camera)
{
	
	Camera cam;
	
	float 	H_FOV_V     = cam.mPROPERTIES.FOV[0]/2;
	int 	H_RES_V     = cam.mPROPERTIES.RES_VH[0]/2;  
    float 	H_FOV_H     = cam.mPROPERTIES.FOV[1]/2;    
    int 	H_RES_H     = cam.mPROPERTIES.RES_VH[1]/2;  
	double  PX_SIZE     	 = tan(H_FOV_V * M_PI /180.0)*cam.mPROPERTIES.FOCAL_LENGTH/H_RES_V;
	const 	VectorXd vRows 	 = VectorXd::LinSpaced(cam.mPROPERTIES.RES_VH[0],1,cam.mPROPERTIES.RES_VH[0]);
    const   VectorXd PX_ANG  = atan( PX_SIZE * (vRows.array()/cam.mPROPERTIES.FOCAL_LENGTH) ) * 180.0 /M_PI ;	
	const	VectorXd DEPTH_P = cam.mPROPERTIES.HEIGHT * tan( (90 - PX_ANG.array()) * M_PI/180.0 );
	double a = 	H_RES_H/(100*tan(H_FOV_H * M_PI /180.0)*DEPTH_P(H_RES_V-2)); //The horizontal pixel to cm ratio at the bottom line of the image



	 
    TEST(CameraPrameters)
    {
		

		
        CHECK_EQUAL(240, 	cam.mPROPERTIES.FRAME_CENTER(0,0));
		CHECK_EQUAL(320, 	cam.mPROPERTIES.FRAME_CENTER(1,0));
		CHECK_EQUAL(0.0060, cam.mPROPERTIES.FOCAL_LENGTH);
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
        CHECK_EQUAL(300, lane.mPROPERTIES.AVG_WIDTH);
    }
	
	
	// Tests for Lane Filter.
	TEST_LaneFilter testLaneFilter;

	TEST(LaneFilter)
    {
		
		
		
		 CHECK_EQUAL(5, testLaneFilter.laneFilter.mSTEP_CM) ;
		 CHECK_EQUAL(10, testLaneFilter.laneFilter.mSTEP_PX) ;
		 CHECK_EQUAL(760, testLaneFilter.laneFilter.mPX_MAX) ;
		 CHECK_EQUAL(2, testLaneFilter.laneFilter.mCONF_THRESH) ;
		 CHECK_EQUAL(153, floor(2*testLaneFilter.laneFilter.mPX_MAX/testLaneFilter.laneFilter.mSTEP_PX) +1) ;
		 CHECK_EQUAL(77, floor(testLaneFilter.laneFilter.mPX_MAX/testLaneFilter.laneFilter.mSTEP_PX) +1 ) ;
		 
		 CHECK_EQUAL(153, testLaneFilter.laneFilter.mBINS_HISTOGRAM.size() );
		 
		 CHECK_EQUAL(77, testLaneFilter.laneFilter.mPrior.rows());
		 CHECK_EQUAL(77, testLaneFilter.laneFilter.mPrior.cols());
		 
		 
		 
		 MatrixXd exp_LanePrior = testLaneFilter.exp_LanePrior;
		 MatrixXf act_LanePrior = testLaneFilter.laneFilter.mPrior;
		 MatrixXd exp_LaneTransition = testLaneFilter.exp_LaneTransition;
		 MatrixXf act_LaneTransition = testLaneFilter.laneFilter.mTransition;
		 
		 CHECK_ARRAY_CLOSE(testLaneFilter.exp_BINS_HISTOGRAM.data(), testLaneFilter.laneFilter.mBINS_HISTOGRAM.data(), testLaneFilter.exp_BINS_HISTOGRAM.size(), 1.0e-4);
		 CHECK_ARRAY_CLOSE(exp_LanePrior.data(), act_LanePrior.data(), 77*77, 1.0e-6);
		 CHECK_ARRAY_CLOSE(exp_LaneTransition.data(), act_LaneTransition.data(), 7*7, 1.0e-6);
    }
	
	
	
	TEST_VPFilter testVpFilter;
	
	TEST(VanishPointFilter)
    {
		 CHECK_EQUAL(25,  testVpFilter.vpFilter->mRANGE_V) ;
		 CHECK_EQUAL(300, testVpFilter.vpFilter->mRANGE_H) ;
		 CHECK_EQUAL(10,  testVpFilter.vpFilter->mSTEP) ;
		 CHECK_EQUAL(6,   testVpFilter.vpFilter->mNb_BINS_V) ;
		 
		CHECK_ARRAY_CLOSE(testVpFilter.exp_VpBINS_V.data(), testVpFilter.vpFilter->mBINS_V.data(), 6, 1.0e-6) ;
		CHECK_EQUAL(61, testVpFilter.vpFilter->mNb_BINS_H) ;
		CHECK_ARRAY_CLOSE(testVpFilter.exp_VpBINS_H.data(), testVpFilter.vpFilter->mBINS_H.data(), 61, 1.0e-6) ;
		CHECK_EQUAL(153, testVpFilter.vpFilter->mBINS_HISTOGRAM.size()) ;
		CHECK_ARRAY_CLOSE(testVpFilter.exp_BINS_HISTOGRAM.data(),testVpFilter.vpFilter->mBINS_HISTOGRAM.data(), 153, 1.0e-6) ;
		
		
		CHECK_ARRAY_CLOSE(testVpFilter.exp_Prior.data(),testVpFilter.vpFilter->mPrior.data(), 6*61, 1.0e-6) ;
		CHECK_ARRAY_CLOSE(testVpFilter.exp_Filter.data(),testVpFilter.vpFilter->mFilter.data(), 6*61, 1.0e-6) ;
		CHECK_ARRAY_CLOSE(testVpFilter.exp_Transition.data(),testVpFilter.vpFilter->mTransition.data(), 3*3, 1.0e-6) ;

    }
	
	
	
	 TEST_InitState testInitState;
	
	TEST(InitStateTests)
	{
		CHECK_EQUAL(480,  testInitState.initState->mRES_VH[0]) ;
		/* ^TODO: THe following tests, if two object point to the same data */            
		/* ^TODO: Make these tests to check all shared Pointers behaviour, in a new file */
		//CHECK_EQUAL(testInitState.vpFilter->mFilter.data(),  testInitState.initState->mVanishPtFilter->mFilter.data()) ;
		//CHECK_ARRAY_CLOSE(testVpFilter.vpFilter->mFilter.data(),  testInitState.initState->mVanishPtFilter->mFilter.data(), 6*61, 1.0e-6) ;
		
		//Both points to same data.
		//CHECK_EQUAL(testInitState.initState->mVanishPtFilter->mFilter.data(),
		//			 testInitState.initState->Matlab_vpFilter.mFilter->data);
	
		CHECK_ARRAY_CLOSE(testInitState.exp_FOCUS_ROOT.data(), testInitState.initState->mTemplates->FOCUS_ROOT.data(), 961*640, 1.0e-5) ;		
		CHECK_ARRAY_CLOSE(testInitState.exp_DEPTH_ROOT.data(), testInitState.initState->mTemplates->DEPTH_ROOT.data(), 961*640, 1.0e-3) ;
		CHECK_ARRAY_CLOSE(testInitState.exp_GRADIENT_DIR_ROOT.data(), testInitState.initState->mTemplates->GRADIENT_DIR_ROOT.data(), (2*480+1)*(2*640+1), 0.12);
	 
	}
	
}