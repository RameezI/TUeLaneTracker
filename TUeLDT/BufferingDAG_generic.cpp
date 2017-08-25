#include "BufferingDAG_generic.h"

BufferingDAG_generic::BufferingDAG_generic()
:mTemplatesReady(false),
 mBufferReady(false)
{
	
}

int BufferingDAG_generic::init_DAG()
{
        int16_t   ORIGIN_Y_CRS   =  mCAMERA.FRAME_CENTER(0) - mSPAN;
        int16_t   ORIGIN_X_CRS   = -mCAMERA.FRAME_CENTER(1);

        bufferingGraph.mY_VPRS   = -(mY_IRS + ORIGIN_Y_CRS);
        bufferingGraph.mX_VPRS   =   mX_IRS + ORIGIN_X_CRS;
}


void BufferingDAG_generic::buffer()
{


#ifdef PROFILER_ENABLED
mProfiler.start("GRAY_FRAME_CONVERSION");
#endif				

	cvtColor(mFrameRGB, mFrameGRAY, cv::COLOR_BGR2GRAY);
	int rowIndex= mCAMERA.RES_VH(0) - mSpan;
	Rect ROI;
	ROI = Rect(0, rowIndex, mCAMERA.RES_VH(1), mSpan);	
	mFrameGRAY_ROI = mFrameGRAY(ROI);
			 
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "GRAY Frame Conversion." <<endl
				<<  "Conversion Time: " << mProfiler.getAvgTime("GRAY_FRAME_CONVERSION")<<endl
				<<"******************************"<<endl<<endl;	
				#endif







#ifdef PROFILER_ENABLED
mProfiler.start("GAUSSIAN_BLUR");
#endif 
	
	GaussianBlur( mFrameGRAY_ROI, mFrameGRAY_ROI, Size( 5, 5 ), 1.5, 1.5, BORDER_REPLICATE);

				
 #ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Gaussian Filtering." <<endl
				<<  "Gaussian(5x5) Time: " << mProfiler.getAvgTime("GAUSSIAN_BLUR")<<endl
				<<"******************************"<<endl<<endl;	
				#endif







#ifdef PROFILER_ENABLED
mProfiler.start("TEMPLATES_WAIT");
#endif 							

		WriteLock  wrtLock(_mutex);
		_sateChange.wait(wrtLock,[this]{return (mTemplatesReady && mBufferReady) ;} );
				
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
				<<"******************************"<<endl
				<<  "Waiting For Worker thread." <<endl
				<<  "Wait Time: " << mProfiler.getAvgTime("TEMPLATES_WAIT")<<endl
				<<"******************************"<<endl<<endl;	
				#endif	






				
#ifdef PROFILER_ENABLED
mProfiler.start("GRADIENT_COMPUTATION");
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
				<<  "Gradients compute Time: " << mProfiler.getAvgTime("GRADIENT_COMPUTATION")<<endl
			  	<<"******************************"<<endl<<endl;	
			 	#endif






#ifdef PROFILER_ENABLED
mProfiler.start("COMPUTE_PROBABILITIES");
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


	// Intermediate Probability Map
	mBufferPool->Probability[bufferPos] = mProbMap_GradMag + mProbMap_Gray;
	mMask = mBufferPool->Probability[bufferPos] <0 ;
	mBufferPool->Probability[bufferPos].setTo(0,mMask);


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
	
	mTemplatesReady = false;	
	mBufferReady    = false;
	
	wrtLock.unlock();

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Compute total LaneMarker Proability ." <<endl
				<<  "Max Time: " << mProfiler.getMaxTime("COMPUTE_PROBABILITIES")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("COMPUTE_PROBABILITIES")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("COMPUTE_PROBABILITIES")<<endl
				<<"******************************"<<endl<<endl;	
				#endif
}


/** 
Parallel Execution Path for Buffering Graph.
Description of Modes:
*/

void BufferingDAG_generic::auxillaryTasks()
{
	const int OFFSET =  mCAMERA.RES_VH(0)-mSpan;
	int rowIndex= mCAMERA.RES_VH(0) - mCAMERA.FRAME_CENTER(0) -mVanishPt.V + OFFSET ;
	int colIndex= mCAMERA.RES_VH(1) - mCAMERA.FRAME_CENTER(1) -mVanishPt.H ;

	WriteLock  wrtLock(_mutex, std::defer_lock);	

/* MODE: (A + C) */	

	wrtLock.lock();

		Rect ROI = Rect(colIndex, rowIndex, mCAMERA.RES_VH(1), mSpan);
		mGRADIENT_TAN_ROOT(ROI).copyTo(mGradTanTemplate);

		ROI = Rect(0,rowIndex,mCAMERA.RES_VH(1), mSpan);
		mDEPTH_MAP_ROOT(ROI).copyTo(mDepthTemplate);

		rowIndex = mVP_Range_V-mVanishPt.V;
		ROI = Rect(0, rowIndex, mCAMERA.RES_VH(1), mSpan);
		mFOCUS_MASK_ROOT(ROI).copyTo(mFocusTemplate);	

		for ( std::size_t i = 0; i< mBufferPool->Probability.size()-1 ; i++ )
		{
			mBufferPool->Probability[i+1].copyTo(mBufferPool->Probability[i]);		
			mBufferPool->GradientTangent[i+1].copyTo(mBufferPool->GradientTangent[i]);
		}	
		
		mTemplatesReady = true;
		mBufferReady    = true;


	wrtLock.unlock();
	_sateChange.notify_one();

}



/*^TODO: Define Grabing mechanism in case of camera */
int BufferingDAG_generic::grabFrame()	
{		

#ifdef DIRECTORY_INPUT

	#ifdef PROFILER_ENABLED
	mProfiler.start("IMAGE_READ");
	#endif 
	
		mFrameRGB = imread(mFiles[mFrameCount]);
		cout<<"Processing Frame: "<<mFrameCount<<endl;
	 

	#ifdef PROFILER_ENABLED
	mProfiler.end();

	const std::string str = mFiles[mFrameCount];	
	LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
				<<"******************************"<<endl
				<<  "Reading frame from directory." <<endl <<str<<endl
				<<  "Read time: " << mProfiler.getAvgTime("IMAGE_READ")<<endl
				<<"******************************"<<endl<<endl;
				#endif
						
	if (mFrameCount+1 < mFiles.size())
		 mFrameCount ++;

	if(!mFrameRGB.data)
	   return -1;
	else	
	   return 0;

#else

					
#endif


}



