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
	  int testResult_GradTangentRoot=-1;
	  int testResult_DepthRoot=-1;
	  int testResult_FocusRoot=-1;
	  
	  int testResult_GradTangentTemplate=-1;
	  int testResult_DepthTemplate=-1;
	  int testResult_FocusTemplate=-1;
	  
	  int Span =-1;
	  int testTypes =-1;

	  int testResult_GradX=-1;
	  int testResult_GradY=-1;
	  int testResult_Mag=-1;
	  int testResult_GradTan=-1;

	 States  mCurrentState;
	  
	 TEST_BufferingState()
		
	{ 
		
		vector< cv::String> lFiles;
		
		cv::String folder = "/home/rameez/TestDataBackup";
		glob(folder, lFiles);
				
		Camera	camera;
		Lane    lane;

		unique_ptr<LaneFilter>  		laneFilter;
		unique_ptr<VanishingPtFilter>   vanishingPtFilter;
		unique_ptr<Templates> 			templates;
				
		BufferingState bufferingState;
			
		{
			InitState		bootingState;
				
			/*******************************************/
						  // INITIALISING //

			laneFilter 			= bootingState.createLaneFilter();
			vanishingPtFilter	= bootingState.createVanishingPtFilter();
			templates           = bootingState.createTemplates();
					
					
					
			/*******************************************/
							//TRANSITION//

			if (bootingState.currentStatus == StateStatus::DONE)						
				mCurrentState =	States::BUFFERING;	
			else 			
				mCurrentState = States::DISPOSING;
		}
			
			
				
			Mat comparison;

			
				// The currenState== States::BUFFERING
		if (bufferingState.currentStatus == StateStatus::INACTIVE)
		{			
			/*Inject Dependencies for Buffering State */								  
				#ifdef DIRECTORY_INPUT 
					bufferingState.setSource(lFiles);
				#else
					bufferingState.setSource();
				#endif				
					
				bufferingState.setupDAG(std::ref(*templates));
		}
	

		bufferingState.run();
		BufferingDAG_generic lBufferingGraph(std::move(bufferingState.bufferingGraph));
		Span = lBufferingGraph.mSpan;
		
		Mat exp_GradientX = loadCSV("GradientX.csv", CV_16SC1);
		cv::compare(exp_GradientX, lBufferingGraph.mGradX, comparison, cv::CMP_NE);
		testResult_GradX = cv::countNonZero(comparison);
		
		Mat exp_GradientY = loadCSV("GradientY.csv", CV_16SC1);
		cv::compare(exp_GradientY, lBufferingGraph.mGradY, comparison, cv::CMP_NE);
		testResult_GradY = cv::countNonZero(comparison);
		
		Mat exp_MagFrame = loadCSV("MAG_FRAME.csv", CV_8UC1);
		cv::compare(exp_MagFrame, lBufferingGraph.mFrameGradMag, comparison, cv::CMP_NE);
		testResult_Mag = cv::countNonZero(comparison);
		
		Mat exp_GradTan = loadCSV("GradTan_FRAME.csv", CV_16SC1);
		cv::compare(exp_GradTan, lBufferingGraph.mBufferPool->GradientTangent[2], comparison, cv::CMP_NE);
		testResult_GradTan = cv::countNonZero(comparison);
		
		
		Mat expFrame = loadCSV("PROB_FRAME.csv", CV_8UC1);
		imshow("ExpectedFrame", expFrame);
		imshow("ActualFrame",  lBufferingGraph.mBufferPool->Probability[2]);
		
	//	Mat expFrame = loadCSV("MAG_FRAME.csv", CV_8UC1);
	//	imshow("ExpectedFrame", expFrame);
	//	imshow("ActualFrame",  lBufferingGraph.mBufferPool->Probability[2]);
		waitKey(0);
		
		if (lBufferingGraph.mGradX.type() == CV_16SC1)
		{
			testTypes=0;		
		}
		
		ofstream ProbabilityGray;
		ProbabilityGray.open("ProbabilityGray.csv");
        ProbabilityGray<< cv::format(lBufferingGraph.mProbMap_Gray, cv::Formatter::FMT_CSV) << std::endl;
        ProbabilityGray.close();
	
	
	//lBufferingGraph.mBufferPool->Probability[2]
		
	  /* ofstream  GrayFrame, GradientX, GradientY;
       GradientX.open("GradientX.csv");
       GradientX<< cv::format(lBufferingGraph.mGradX, cv::Formatter::FMT_CSV) << std::endl;
       GradientX.close();
	   GradientY.open("GradientY.csv");
       GradientY<< cv::format(lBufferingGraph.mGradY, cv::Formatter::FMT_CSV) << std::endl;
       GradientY.close();
	   GrayFrame.open("GrayFrame.csv");
       GrayFrame<< cv::format(lBufferingGraph.mFrameGRAY_ROI, cv::Formatter::FMT_CSV) << std::endl;
       GrayFrame.close();
	*/
	
	}
		
		~TEST_BufferingState(){	}
};



