#include "BufferingState.h"
/* DIRECTED ACYCLIC GRAPHS ARE IMPLEMENTED AS MEMBER FUNCTION*/
/* ^TODO: TO MAKE IT REUSEABLE: Make a Seperate class out of it */
void BufferingState::executeDAG_buffering()
{
	
#ifdef PROFILER_ENABLED
mProfiler.start("ConversionToGRAY");
#endif				

	cvtColor(mFrameRGB, mFrameGRAY, cv::COLOR_BGR2GRAY);
				 
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
					  <<"******************************"<<endl
					  <<  "GRAY Frame Conversion." <<endl
					  <<  "GrayFrame conversion Time: " << mProfiler.getAvgTime("ConversionToGRAY")<<endl
					  <<"******************************"<<endl<<endl;	
					#endif			



Mat  GRAY_double_ARM, Processing_ARM,  Result_ARM;
UMat GRAY_double_GPU, Processing_GPU,  Result_GPU;
Mat  Result_Transferred;

Mat   Ones_ARM = Mat::ones(480,640,CV_32FC1);
UMat  Ones_GPU = UMat::ones(480, 640, CV_32FC1);

Mat   SetPoint_ARM(480,640,CV_32FC1);
SetPoint_ARM.setTo(-0.1*120);

UMat  SetPoint_GPU(480,640,CV_32FC1);
SetPoint_GPU.setTo(-0.1*120);


mFrameGRAY.convertTo(GRAY_double_ARM, CV_32FC1, -0.1);
GRAY_double_ARM.copyTo(GRAY_double_GPU);    


const float constantT= -0.1*120;

#ifdef PROFILER_ENABLED
mProfiler.start("TestGPU");
#endif

ocl::setUseOpenCL(true);
for (int i=0; i<3; i++)
{	
	
	GRAY_double_ARM.copyTo(GRAY_double_GPU); 
	cv::subtract(GRAY_double_GPU, constantT, Processing_GPU);			
	cv::exp(Processing_GPU, Processing_GPU );
	cv::add(1, Processing_GPU, Processing_GPU);
	cv::divide(1, Processing_GPU, Result_GPU);
	//int a = cv::countNonZero(GRAY_double_GPU);
} 
				 
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
					  <<"******************************"<<endl
					  <<  "NonZero Pixels in GRAY Frame computed on GPU." <<endl
					  <<  "Max Time: " << mProfiler.getMaxTime("TestGPU")<<endl
					  <<  "Avg Time: " << mProfiler.getAvgTime("TestGPU")<<endl
					  <<  "Min Time: " << mProfiler.getMinTime("TestGPU")<<endl
					  <<"******************************"<<endl<<endl;	
					#endif


#ifdef PROFILER_ENABLED
mProfiler.start("TestARM");
#endif

for (int i=0; i<3; i++)
{	  
  	cv::subtract(GRAY_double_ARM, constantT, Processing_ARM);			
	cv::exp(Processing_ARM, Processing_ARM );
	cv::add(1, Processing_ARM, Processing_ARM);
	cv::divide(1, Processing_ARM, Result_ARM);
}

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
					  <<"******************************"<<endl
					  <<  "Exponent of GRAY Frme computed on ARM." <<endl
					  <<  "Max Time: " << mProfiler.getMaxTime("TestARM")<<endl
					  <<  "Avg Time: " << mProfiler.getAvgTime("TestARM")<<endl
					  <<  "Min Time: " << mProfiler.getMinTime("TestARM")<<endl
					  <<"******************************"<<endl<<endl;	
					#endif
				 




#ifdef PROFILER_ENABLED
mProfiler.start("GaussianFiltering");
#endif 
	
	GaussianBlur( mFrameGRAY, mFrameGRAY, Size( 11, 11 ), 1.5, 1.5, BORDER_REPLICATE);
				
 #ifdef PROFILER_ENABLED
 mProfiler.end();
 LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Gaussian Filtering." <<endl
										  <<  "Gaussian(11x11) Filtering Time: " << mProfiler.getAvgTime("GaussianFiltering")<<endl
										  <<"******************************"<<endl<<endl;	
									   #endif				
			
				
#ifdef PROFILER_ENABLED
mProfiler.start("GradientsComputation");
#endif 								

	computeOrientedGradients();
				
 #ifdef PROFILER_ENABLED
 mProfiler.end();
 LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Gradients Computation." <<endl
										  <<  "Gray channel gradient computation Time: " << mProfiler.getAvgTime("GradientsComputation")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif
										 

	
#ifdef PROFILER_ENABLED
mProfiler.start("ExtractTemplates");
#endif 				

		extractTemplates();


#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Extracting from Root Templates." <<endl
										  <<  "Time: " << mProfiler.getAvgTime("ExtractTemplates")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif
							
							
							
#ifdef PROFILER_ENABLED
mProfiler.start("LaneMarker_P");
#endif 				
		
	computeProbabilities();
		
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Compute total LaneMarker Proabilities ." <<endl
										  <<  "Time: " << mProfiler.getAvgTime("LaneMarker_P")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif
										 

										 
										 
#ifdef PROFILER_ENABLED
mProfiler.start("UpdateLikeLihoods");
#endif 				
		
									 
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Update Likelihoods Buffers" <<endl
										  <<  "Time: " << mProfiler.getAvgTime("UpdateLikeLihoods")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif		

				

#ifdef PROFILER_ENABLED
mProfiler.start("Display");
#endif
				
	#ifndef s32v2xx
	
		 //mFrameHS->convertTo(*mFrameHS, CV_32FC1, 360);
		// mFrameGradMag->convertTo(*mFrameGradMag, CV_8UC1, 255); // Rescaling image to the range [0 255]
		 imshow( "Display window", mFrameGRAY);
		 waitKey(1);
	
	#else

		//^TODO: How to display single Channel images 
		//mFrameGradMag_Gray->convertTo(*mFrameGradMag_Gray, CV_8UC1, 255); // Rescaling image to the range [0 255]
	/*	io::FrameOutputV234Fb DcuOutput(640, 
										480, 
										io::IO_DATA_DEPTH_08, 
										io::IO_DATA_CH3);
									
		DcuOutput.PutFrame((char *)mFrameRGB->data, false);
	*/	
		std::cout<<"******************************"<<std::endl;
		std::cout<<"	Frame Count : " <<mCountFrame <<std::endl;
		std::cout<<"******************************"<<std::endl<<endl<<endl;							
	#endif

									
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
							  <<"******************************"<<endl
							  <<  "Screen Display." <<endl
							  <<  "Display update time: " << mProfiler.getAvgTime("Display")<<endl
							  <<"******************************"<<endl<<endl;	
							 #endif				
}