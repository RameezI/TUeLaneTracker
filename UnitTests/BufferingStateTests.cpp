#include "UnitTest++/UnitTest++.h"
#include  "Tests.h"
#include  "Camera.h"
#include  "Lane.h"
#include  "LaneFilter.h"
#include  "VanishingPtFilter.h"
#include  "InitState.h"
#include  "BufferingState.h"



class TEST_BufferingState:Tests
{

public:


	  
	  MatrixXd exp_FOCUS_ROOT;
	  MatrixXd exp_DEPTH_ROOT;
	  MatrixXd exp_GRADIENT_DIR_ROOT;
	  
	  MatrixXd exp_FOCUS;
	  MatrixXd exp_DEPTH;
	  MatrixXd exp_GRADIENT_DIR;
	  

	  
	  unique_ptr<BufferingState> bufferingState;
	  
	  TEST_BufferingState()
		
		{ 
		//	InitState bootingState;
		//	bufferingState.reset(new BufferingState()); 
			
		//	bootingState.run();
			
			
		//	vector< cv::String> IMG_filenames;
		//	cv::String folder = "/media/rameez/Linux-Extended/DataSet/eindhoven/PNG_imgs";
		//	glob(folder, IMG_filenames);
			
		//	bufferingState->setSource(IMG_filenames);
		//	bufferingState->setTemplates(std::move(bootingState.templates));
						
			
		//	exp_FOCUS_ROOT			= readCSV("FOCUS_ROOT.csv", 2*480+1, 640);
		//	exp_DEPTH_ROOT			= readCSV("DEPTH_ROOT.csv", 2*480+1, 640);
		//	exp_GRADIENT_DIR_ROOT	= readCSV("GRADIENT_DIR_ROOT.csv", 2*480+1, 2*640 +1);
			
			/*ofstream file("DIR_ROOT_Eigen.csv");
			const static IOFormat CSVFormat(StreamPrecision, DontAlignCols, ", ", "\n");
			file<<initState->mTemplates->GRADIENT_DIR_ROOT.format(CSVFormat); */
		}
		
		~TEST_BufferingState()
		{
			
		}
};




	 TEST_BufferingState testBufferingState;
	
	TEST(BufferingStateTests)
	{
/*		
	//Mapping OpenCv Data to Eigen Maps
	Eigen::Map< Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > 
	FrameGray_Mapped ((float*) mFrameGRAY->data, mRES_VH(0),mRES_VH(1));
		
	Eigen::Map< Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > 
	mFrameGradMag_Mapped ((float*) mFrameGradMag->data, mRES_VH(0),mRES_VH(1));

	Eigen::Map< Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > 
	mFrameGradDir_Mapped ((float*) mFrameGradAng->data, mRES_VH(0),mRES_VH(1));	
*/
	
			 
	}