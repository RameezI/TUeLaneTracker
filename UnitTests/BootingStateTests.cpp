#include "UnitTest++/UnitTest++.h"
#include  "Tests.h"
#include  "Camera.h"
#include  "Lane.h"
#include  "LaneFilter.h"
#include  "VanishingPtFilter.h"
#include  "InitState.h"



class TEST_InitState:Tests
{

public:


	  
	  MatrixXd exp_FOCUS_ROOT;
	  MatrixXd exp_DEPTH_ROOT;
	  MatrixXd exp_GRADIENT_DIR_ROOT;

	  
	  unique_ptr<InitState> bootingState;
	  
	  TEST_InitState()
		
		{ 
			bootingState.reset(new InitState()); 
			bootingState->run();
			
			exp_FOCUS_ROOT			= readCSV("FOCUS_ROOT.csv", 2*480+1, 640);
			exp_DEPTH_ROOT			= readCSV("DEPTH_ROOT.csv", 2*480+1, 640);
			exp_GRADIENT_DIR_ROOT	= readCSV("GRADIENT_DIR_ROOT.csv", 2*480+1, 2*640 +1);
			
			/*ofstream file("DIR_ROOT_Eigen.csv");
			const static IOFormat CSVFormat(StreamPrecision, DontAlignCols, ", ", "\n");
			file<<initState->mTemplates->GRADIENT_DIR_ROOT.format(CSVFormat); */
		}
		
		~TEST_InitState()
		{
			
		}
};




	 TEST_InitState testInitState;
	
	TEST(InitStateTests)
	{
	
		CHECK_ARRAY_CLOSE(testInitState.exp_FOCUS_ROOT.data(), 
						  testInitState.bootingState->templates->FOCUS_ROOT.data(),
						  961*640, 1.0e-5) ;
		
		CHECK_ARRAY_CLOSE(testInitState.exp_DEPTH_ROOT.data(),
						  testInitState.bootingState->templates->DEPTH_ROOT.data(),
						  961*640, 1.0e-3) ;
						  
		CHECK_ARRAY_CLOSE(testInitState.exp_GRADIENT_DIR_ROOT.data(),
						  testInitState.bootingState->templates->GRADIENT_DIR_ROOT.data(),
						  (2*480+1)*(2*640+1), 0.12);	 
	}