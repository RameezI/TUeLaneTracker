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



Mat  Gray_double_ARM, Result_ARM;
UMat GRAY_double_GPU, Result_GPU;

Mat   Ones_ARM = Mat::ones(480,640,CV_32FC1);
UMat  Ones_GPU ;

mFrameGRAY.convertTo(Gray_double_ARM, CV_32FC1, 1.0/255);
Gray_double_ARM.copyTo(GRAY_double_GPU);
Ones_ARM.copyTo(Ones_GPU);




#ifdef PROFILER_ENABLED
mProfiler.start("TestGPU");
#endif

ocl::setUseOpenCL(true);
for (int i=0; i<200; i++)
{				
	//cv::exp(GRAY_double_GPU, Result_GPU );
	//cv::add(GRAY_double_GPU, Ones_GPU, Result_GPU);
	int a = cv::countNonZero(GRAY_double_GPU);
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

for (int i=0; i<200; i++)
{	
  
  //cv::exp(Gray_double_ARM, Result_ARM);
  //cv::add(Gray_double_ARM, Ones_ARM, Result_ARM);
  int a = cv::countNonZero(Gray_double_ARM);
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