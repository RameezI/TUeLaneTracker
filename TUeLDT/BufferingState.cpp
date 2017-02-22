#include "BufferingState.h"

BufferingState::BufferingState()
: //mFrameRGB_double(make_shared<Mat>()),
  mFrameRGB(make_shared<Mat>()),
  mFrameGRAY(make_shared<Mat>()),
  mFrameGRAY_float(make_shared<Mat>()),
  mFrameGradMag_Gray(make_shared<Mat>()),
  mFrameGradAng_Gray(make_shared<Mat>())
#ifdef DIRECTORY_INPUT
  , mCountFrame(0)
#endif

{

	
}



int BufferingState::grabFrame()
{
	
#ifdef DIRECTORY_INPUT
 
#ifdef PROFILER_ENABLED
mProfiler.start("ImageRead");
#endif 

	*mFrameRGB = imread(mFiles[mCountFrame]);	
 
 #ifdef PROFILER_ENABLED
 mProfiler.end();
 #endif 


#ifdef PROFILER_ENABLED
	const std::string str = mFiles[mCountFrame];
	
	LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Reading frame from directory." <<endl <<str<<endl
										  <<  "Read time: " << mProfiler.getTiming("ImageRead")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif

	

	if(!mFrameRGB->data)
			return -1;
			
	else	mCountFrame ++;
			return 0;
			

#endif

}




void BufferingState::conclude()
{
	
	
	
}




void BufferingState::run()
{

#ifdef DIRECTORY_INPUT

	if (mCountFrame < mFiles.size())
		sStateCounter++;
	else
	{
		mStateStatus = StateStatus::DONE;	
		return;
	} 

#else 
	//^TODO: Check for camera errors or signals to finalise this state
	mStateStatus = StateStatus::DONE;
	sStateCounter++;
	
#endif



#ifdef PROFILER_ENABLED
	mProfiler.start("SingleRun");
#endif

if (0==grabFrame())
	
{
				
		
#ifdef PROFILER_ENABLED
mProfiler.start("ConversionToGRAY");
#endif				
				
				//ColorConversion ^TODO: Port to ISP
				 ColorTransformer 	 rgb2gray(*mFrameRGB, *mFrameGRAY, cv::COLOR_BGR2GRAY);
				 
				 
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "GRAY Frame Converion." <<endl
										  <<  "GrayFrame conversion Time: " << mProfiler.getTiming("ConversionToGRAY")<<endl
										  <<"******************************"<<endl<<endl;	
									   #endif						 


#ifdef PROFILER_ENABLED
mProfiler.start("ConversionToFloat");
#endif				 
				//Converting to float
				mFrameGRAY->convertTo(*mFrameGRAY_float, CV_32FC1, 1/255.0); // Rescaling image to the range [0 1]
				
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "FloatingPoint Converion." <<endl
										  <<  "FoatingPoint conversion Time: " << mProfiler.getTiming("ConversionToFloat")<<endl
										  <<"******************************"<<endl<<endl;	
									   #endif				




#ifdef PROFILER_ENABLED
mProfiler.start("GaussianFiltering");
#endif 
				//GaussianPreProcessor
				GaussianPreProcessor filterGauss(*mFrameGRAY_float, *mFrameGRAY_float);
				
 #ifdef PROFILER_ENABLED
 mProfiler.end();
 LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Gaussian Filtering." <<endl
										  <<  "Gaussian(11x11) Filtering Time: " << mProfiler.getTiming("GaussianFiltering")<<endl
										  <<"******************************"<<endl<<endl;	
									   #endif				
			
				
#ifdef PROFILER_ENABLED
mProfiler.start("GradientsComputation");
#endif 				
				
				//Gradients Computation
				GradientsExtractor   GradientGray(*mFrameGRAY_float, *mFrameGradMag_Gray , *mFrameGradAng_Gray);
				
	
 #ifdef PROFILER_ENABLED
 mProfiler.end();
 LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Gradients Computation." <<endl
										  <<  "Gray channel gradient computation time: " << mProfiler.getTiming("GradientsComputation")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif
										 
	
#ifdef PROFILER_ENABLED
mProfiler.start("ExtractTemplates");
#endif 				
		
									 

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Extracting from Root Templates." <<endl
										  <<  "Time: " << mProfiler.getTiming("ExtractTemplates")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif
										 
										 



#ifdef PROFILER_ENABLED
mProfiler.start("LaneMarker_P");
#endif 				
		
									 
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Compute total LaneMarker Proabilities ." <<endl
										  <<  "Time: " << mProfiler.getTiming("LaneMarker_P")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif
										 


#ifdef PROFILER_ENABLED
mProfiler.start("Innovation");
#endif 				
		
									 
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Compute and Compare Frame Innovation" <<endl
										  <<  "Time: " << mProfiler.getTiming("Innovation")<<endl
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
										  <<  "Time: " << mProfiler.getTiming("UpdateLikeLihoods")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif		

				
				
				#ifndef s32v2xx
				 //mFrameHS->convertTo(*mFrameHS, CV_32FC1, 360);
				 mFrameGradMag_Gray->convertTo(*mFrameGradMag_Gray, CV_8UC1, 255); // Rescaling image to the range [0 255]
				 imshow( "Display window", *mFrameGradMag_Gray);
				 waitKey(1);
				
				#else
				
				#ifdef PROFILER_ENABLED
				 mProfiler.start("Display");
				#endif
	
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
												
												
				 #ifdef PROFILER_ENABLED
					mProfiler.end();
					LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Screen Display." <<endl
										  <<  "Display update time: " << mProfiler.getTiming("Display")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif
												 
				
				
				
				#endif
				
}
		
else
	cerr << "Problem loading image!!!" << endl;
												

 #ifdef PROFILER_ENABLED
 mProfiler.end();
 LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Completing a run loop." <<endl
										  <<  "Single run-loop time: " << mProfiler.getTiming("SingleRun")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif
}




#ifdef DIRECTORY_INPUT
void BufferingState::setSource(const vector<cv::String>& files)
{
	mFiles = files;
	mStateStatus= StateStatus::ACTIVE;
}
#else

void BufferingState::setSource()
{
	mStateStatus= StateStatus::ACTIVE;
}
	
#endif

BufferingState::~BufferingState()
{
	
	
}

