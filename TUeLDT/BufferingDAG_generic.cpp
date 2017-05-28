#include "BufferingDAG_generic.h"

BufferingDAG_generic::BufferingDAG_generic( )
: mLikelihoods(Likelihoods(mCAMERA.RES_VH(0), mCAMERA.RES_VH(1)))

#ifdef DIRECTORY_INPUT
	, mCountFrame(0)
#endif

{
	
	
}


void BufferingDAG_generic::executeDAG_buffering()
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




/*^TODO: Define Grabing mechanism in case of camera */
int BufferingDAG_generic::grabFrame()
	
{		

#ifdef DIRECTORY_INPUT
 
			#ifdef PROFILER_ENABLED
			mProfiler.start("ImageRead");
			#endif 

				mFrameRGB = imread(mFiles[mCountFrame]);	
			 
			#ifdef PROFILER_ENABLED
			 mProfiler.end();
				const std::string str = mFiles[mCountFrame];	
				LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
													  <<"******************************"<<endl
													  <<  "Reading frame from directory." <<endl <<str<<endl
													  <<  "Read time: " << mProfiler.getAvgTime("ImageRead")<<endl
													  <<"******************************"<<endl<<endl;
													  #endif
				if(!mFrameRGB.data)
						return -1;
						
				else	mCountFrame ++;
						return 0;

#else

			
					
#endif


}

void BufferingDAG_generic::computeProbabilities()
{
			//cv::subtract(GRAY_double_ARM, constantT, Processing_ARM);			
			//cv::exp(Processing_ARM, Processing_ARM );
			//cv::add(1, Processing_ARM, Processing_ARM);
			//cv::divide(1, Processing_ARM, Result_ARM);
	
}


void BufferingDAG_generic::extractTemplates()
{
		/*
		
		 int rowIndex= (mCAMERA.RES_VH(0)-mVanishingPt.V) - mCAMERA.RES_VH(0)/2;
		 int colIndex= (mCAMERA.RES_VH(1)-mVanishingPt.H) - mCAMERA.RES_VH(1)/2;

	   Rect ROI;

		ROI = Rect(colIndex, rowIndex, mCAMERA.RES_VH(1), mCAMERA.RES_VH(0) );	
		mGradDirTemplate = mGRADIENT_DIR_ROOT(ROI);

		ROI = Rect(0, rowIndex, mCAMERA.RES_VH(1), mCAMERA.RES_VH(0));
		mDepthTemplate   = mDEPTH_MAP_ROOT(ROI);
		
		
		ROI = Rect(0, rowIndex, mCAMERA.RES_VH(1), mCAMERA.RES_VH(0));	
		mFocusTemplate = mFOCUS_MASK_ROOT(ROI);
		
		 */

}

	
	
void BufferingDAG_generic::computeOrientedGradients()
{	
		int scale = 1;
		int delta = 0;
		int ddepth = CV_64F;
		
		Mat grad_x, grad_y;
		
		Sobel( mFrameGRAY, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_REPLICATE );
		Sobel( mFrameGRAY, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_REPLICATE );
		cv::magnitude(grad_x,grad_y, mFrameGradMag);
		cv::phase(grad_x, grad_y, mFrameGradAng);
		
}

BufferingDAG_generic::~BufferingDAG_generic()
{
	

}



