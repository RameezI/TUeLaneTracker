#include "BufferingState.h"
/* DIRECTED ACYCLIC GRAPHS ARE IMPLEMENTED AS MEMBER FUNCTION*/
/* ^TODO: TO MAKE IT REUSEABLE: Make a Seperate class out of it */


void BufferingState::executeDAG_buffering()
{
	
#ifdef PROFILER_ENABLED
mProfiler.start("grabGRAYFrame");
#endif				

	cvtColor(mFrameRGB, mFrameGRAY, cv::COLOR_BGR2GRAY);
		
		 
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
					  <<"******************************"<<endl
					  <<  "GRAY Frame Conversion." <<endl
					  <<  "GrayFrame conversion Time: " << mProfiler.getAvgTime("grabGRAYFrame")<<endl
					  <<"******************************"<<endl<<endl;	
					#endif			


#ifdef PROFILER_ENABLED
mProfiler.start("GaussianFiltering");
#endif 
	
	GaussianBlur( mFrameGRAY, mFrameGRAY, Size( 5, 5 ), 1.5, 1.5, BORDER_REPLICATE);
				
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
mProfiler.start("computeProbabilities");
#endif 				
		
		
		
	computeProbabilities();
		
		
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Compute total LaneMarker Proabilities ." <<endl
										  <<  "Max Time: " << mProfiler.getMaxTime("computeProbabilities")<<endl
										  <<  "Avg Time: " << mProfiler.getAvgTime("computeProbabilities")<<endl
					                      <<  "Min Time: " << mProfiler.getMinTime("computeProbabilities")<<endl
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