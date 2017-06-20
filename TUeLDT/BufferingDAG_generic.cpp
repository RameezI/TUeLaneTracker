#include "BufferingDAG_generic.h"

BufferingDAG_generic::BufferingDAG_generic()
{
	
}

void BufferingDAG_generic::buffer()
{





#ifdef PROFILER_ENABLED
mProfiler.start("grabGRAYFrame");
#endif				

	cvtColor(mFrameRGB, mFrameGRAY, cv::COLOR_BGR2GRAY);

	//int rowIndex= mVanishPt.V + mVP_Range_V;
	int rowIndex= mCAMERA.RES_VH(0) - mSpan;


	Rect ROI;

	ROI = Rect(0, rowIndex, mCAMERA.RES_VH(1), mSpan);	
	mFrameGRAY_ROI = mFrameGRAY(ROI);
			 
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "GRAY Frame Conversion." <<endl
				<<  "Conversion Time: " << mProfiler.getAvgTime("grabGRAYFrame")<<endl
				<<"******************************"<<endl<<endl;	
				#endif





								
#ifdef PROFILER_ENABLED
mProfiler.start("GaussianFiltering");
#endif 
	
	GaussianBlur( mFrameGRAY_ROI, mFrameGRAY_ROI, Size( 5, 5 ), 1.5, 1.5, BORDER_REPLICATE);

	/*std::stringstream formattedString;
	string prefix= "/media/rameez/Linux-Extended/DataSet/eindhoven/GRAY_OPENCV/";
	formattedString<<prefix<<std::setw(6)<<std::to_string(mFrameCount)<<".png";
	imwrite( formattedString.str(), mFrameGRAY_ROI );
	*/	
				
 #ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Gaussian Filtering." <<endl
				<<  "Gaussian(11x11) Time: " << mProfiler.getAvgTime("GaussianFiltering")<<endl
				<<"******************************"<<endl<<endl;	
				#endif





				
#ifdef PROFILER_ENABLED
mProfiler.start("GradientsComputation");
#endif 								

	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	
	Sobel( mFrameGRAY_ROI, mGradX, ddepth, 1, 0, 3, scale, delta, BORDER_REPLICATE );
	Sobel( mFrameGRAY_ROI, mGradY, ddepth, 0, 1, 3, scale, delta, BORDER_REPLICATE );

	mMask = mGradX> 255;
	mGradX.setTo(255, mMask);
	mMask = mGradX <-255;
	mGradX.setTo(-255, mMask);

	mMask = mGradY> 255;
	mGradY.setTo(255, mMask);
	mMask = mGradY <-255;
	mGradY.setTo(-255, mMask);
	mMask = mGradY ==0;
	mGradY.setTo(1, mMask);

	// Compute Dx/Dy			
	int bufferPos = mBufferPool->GradientTangent.size()-1;
	cv::divide(mGradX, mGradY, mBufferPool->GradientTangent[bufferPos], 128, -1);
							
	//convert to absolute scale and add weighted absolute gradients 
	mGradX_abs = abs(mGradX);
	mGradY_abs = abs(mGradY );
	
	//addWeighted( mGradX_abs, 0.5, mGradY_abs, 0.5, 0, mFrameGradMag );
	mFrameGradMag = mGradX_abs + mGradY_abs;			
	//convertScaleAbs(mFrameGradMag, mFrameGradMag);
	mFrameGradMag.convertTo(mFrameGradMag, CV_8U);
			
 #ifdef PROFILER_ENABLED
 mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Gradients Computations." <<endl
				<<  "Gradients compute Time: " << mProfiler.getAvgTime("GradientsComputation")<<endl
			  	<<"******************************"<<endl<<endl;	
			 	#endif





			 
#ifdef PROFILER_ENABLED
mProfiler.start("computeProbabilities");
#endif 		

	//GrayChannel Probabilities
	subtract(mFrameGRAY_ROI, mLaneMembership.TIPPING_POINT_GRAY, mTempProbMat, noArray(), CV_32F);
	threshold(mTempProbMat, mTempProbMat, 0, 255, THRESH_TOZERO );
	mTempProbMat.copyTo(mProbMap_Gray);
	mTempProbMat = mTempProbMat + 10;
	divide(mProbMap_Gray, mTempProbMat, mProbMap_Gray, 255, -1);
	mProbMap_Gray.convertTo(mProbMap_Gray, CV_32S);
	
	//GradientMag Probabilities
	subtract(mFrameGradMag, mLaneMembership.TIPPING_POINT_GRAD_Mag, mTempProbMat, noArray(), CV_32F);
	mTempProbMat.copyTo(mProbMap_GradMag);
	mTempProbMat= abs(mTempProbMat) + 10;
	divide(mProbMap_GradMag, mTempProbMat, mProbMap_GradMag, 255, -1);
	mProbMap_GradMag.convertTo(mProbMap_GradMag, CV_32S);

				
	#ifdef PROFILER_ENABLED
	mProfiler.start("TemplatesWait");
	#endif 							

		WriteLock  wrtLock(_mutex);
		_sateChange.wait(wrtLock,[this]{return mBufferReady;} );
				
	 #ifdef PROFILER_ENABLED
	 mProfiler.end();
	 LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
	 <<"******************************"<<endl
	 <<  "Waiting For Worker thread (Templates)." <<endl
	 <<  "Wait Time: " << mProfiler.getAvgTime("TemplatesWait")<<endl
	 <<"******************************"<<endl<<endl;	
	#endif	


	// Intermediate Probability Map
	mBufferPool->Probability[bufferPos] = mProbMap_GradMag + mProbMap_Gray;
	mMask = mBufferPool->Probability[bufferPos] <0 ;
	mBufferPool->Probability[bufferPos].setTo(0,mMask);
	//mBufferPool->Probability[bufferPos].convertTo(mBufferPool->Probability[bufferPos], CV_8U);			
	//mBufferPool->Probability[bufferPos].convertTo(mBufferPool->Probability[bufferPos], CV_16U);


	//Gradient Tangent Probability Map
	subtract(mGradTanTemplate, mBufferPool->GradientTangent[bufferPos], mTempProbMat, noArray(), CV_32F);
	mTempProbMat= abs(mTempProbMat);
	mTempProbMat.copyTo(mProbMap_GradDir);
	mTempProbMat = mTempProbMat + 60;
	divide(mProbMap_GradDir, mTempProbMat, mProbMap_GradDir, 255, -1);
	mProbMap_GradDir.convertTo(mProbMap_GradDir, CV_32S);
	subtract(255, mProbMap_GradDir, mProbMap_GradDir, noArray(), -1);

	
	//Final Probability Map
	multiply(mBufferPool->Probability[bufferPos], mProbMap_GradDir, mBufferPool->Probability[bufferPos]);
	mBufferPool->Probability[bufferPos].convertTo(mBufferPool->Probability[bufferPos], CV_8U, 1.0/255, 0);
		
	mBufferReady= false;
	wrtLock.unlock();

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Compute total LaneMarker Proability ." <<endl
				<<  "Max Time: " << mProfiler.getMaxTime("computeProbabilities")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("computeProbabilities")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("computeProbabilities")<<endl
				<<"******************************"<<endl<<endl;	
				#endif

}


void BufferingDAG_generic::auxillaryTasks()
{

	
	int offset =  mCAMERA.RES_VH(0)-mSpan;
	int rowIndex= mCAMERA.RES_VH(0) - mCAMERA.FRAME_CENTER(0) -mVanishPt.V +offset ;
	int colIndex= mCAMERA.RES_VH(1) - mCAMERA.FRAME_CENTER(1) -mVanishPt.H ;
	
	
	WriteLock  wrtLock(_mutex);
	
		Rect ROI = Rect(colIndex, rowIndex, mCAMERA.RES_VH(1), mSpan);
		mGRADIENT_TAN_ROOT(ROI).copyTo(mGradTanTemplate);

	
		ROI = Rect(0,rowIndex,mCAMERA.RES_VH(1), mSpan);
		mDEPTH_MAP_ROOT(ROI).copyTo(mDepthTemplate);

		rowIndex = mVP_Range_V-mVanishPt.V;
		ROI = Rect(0, rowIndex, mCAMERA.RES_VH(1), mSpan);
		mFOCUS_MASK_ROOT(ROI).copyTo(mFocusTemplate);	

		for ( int i = 0; i< mBufferPool->Probability.size()-1 ; i++ )
		{
			mBufferPool->Probability[i+1].copyTo(mBufferPool->Probability[i]);		
			mBufferPool->GradientTangent[i+1].copyTo(mBufferPool->GradientTangent[i]);
		}	
		
	mBufferReady = true;

	wrtLock.unlock();
	_sateChange.notify_one();
}



/*^TODO: Define Grabing mechanism in case of camera */
int BufferingDAG_generic::grabFrame()	
{		

#ifdef DIRECTORY_INPUT

	#ifdef PROFILER_ENABLED
	mProfiler.start("ImageRead");
	#endif 
	
		mFrameRGB = imread(mFiles[mFrameCount]);
	 

	#ifdef PROFILER_ENABLED
	mProfiler.end();
	const std::string str = mFiles[mFrameCount];	
	LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
				<<"******************************"<<endl
				<<  "Reading frame from directory." <<endl <<str<<endl
				<<  "Read time: " << mProfiler.getAvgTime("ImageRead")<<endl
				<<"******************************"<<endl<<endl;
				#endif
			  
		  
	if(!mFrameRGB.data)
		return -1;
						
	else if (mFrameCount+1 < mFiles.size())
		 mFrameCount ++;

		return 0;

#else

					
#endif


}



