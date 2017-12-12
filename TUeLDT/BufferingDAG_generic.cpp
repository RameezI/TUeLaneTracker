#include "BufferingDAG_generic.h"

BufferingDAG_generic::BufferingDAG_generic()
:mTemplatesReady(false),
 mBufferReady(false),
 mFrameCount(0)
{
	
}

int BufferingDAG_generic::init_DAG()
{
        int16_t   ORIGIN_Y_CRS   =  mCAMERA.FRAME_CENTER(0) - mSpan;
        int16_t   ORIGIN_X_CRS   = -mCAMERA.FRAME_CENTER(1);

        mY_VPRS   = -(mY_ICS + ORIGIN_Y_CRS);
        mX_VPRS   =   mX_ICS + ORIGIN_X_CRS;
	
	return 0;
}

void BufferingDAG_generic::buffer( )
{

#ifdef PROFILER_ENABLED
mProfiler.start("GRAY_FRAME_CONVERSION");
#endif
				
	cvtColor(mFrameRGB, mFrameGRAY, CV_BGR2GRAY);
	int lRowIndex= mCAMERA.RES_VH(0) - mSpan;
	Rect ROI;
	ROI = Rect(0, lRowIndex, mCAMERA.RES_VH(1), mSpan);	
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
	subtract(mFrameGRAY_ROI, mLaneMembership.TIPPING_POINT_GRAY, mTempProbMat, noArray(), CV_32S);
	mMask = mTempProbMat <0 ;
	mTempProbMat.setTo(0,mMask);
	mTempProbMat.copyTo(mProbMap_Gray);
	mTempProbMat = mTempProbMat + 10;
	
	divide(mProbMap_Gray, mTempProbMat, mProbMap_Gray, 255, -1);
	

	//GradientMag Probabilities
	subtract(mFrameGradMag, mLaneMembership.TIPPING_POINT_GRAD_Mag, mTempProbMat, noArray(), CV_32S);
	mTempProbMat.copyTo(mProbMap_GradMag);
	mTempProbMat= abs(mTempProbMat) + 10;
	divide(mProbMap_GradMag, mTempProbMat, mProbMap_GradMag, 255, -1);


	// Intermediate Probability Map
	mBufferPool->Probability[bufferPos] = mProbMap_GradMag + mProbMap_Gray;
	mMask = mBufferPool->Probability[bufferPos] <0 ;
	mBufferPool->Probability[bufferPos].setTo(0,mMask);


	//Gradient Tangent Probability Map
	subtract(mGradTanTemplate, mBufferPool->GradientTangent[bufferPos], mTempProbMat, noArray(), CV_32S);
	mTempProbMat= abs(mTempProbMat);
	mTempProbMat.copyTo(mProbMap_GradDir);
	mTempProbMat = mTempProbMat + 60;
	divide(mProbMap_GradDir, mTempProbMat, mProbMap_GradDir, 255, -1);
	subtract(255, mProbMap_GradDir, mProbMap_GradDir, noArray(), -1);

	
	//Final Probability Map
	multiply(mBufferPool->Probability[bufferPos], mProbMap_GradDir, mBufferPool->Probability[bufferPos]);
	mBufferPool->Probability[bufferPos].convertTo(mBufferPool->Probability[bufferPos], CV_8U, 1.0/255, 0);

	bitwise_and(mBufferPool->Probability[bufferPos], mFocusTemplate, mBufferPool->Probability[bufferPos]);
	
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
void BufferingDAG_generic::runAuxillaryTasks()
{
	int lRowIndex;
	int lColIndex;

	WriteLock  wrtLock(_mutex, std::defer_lock);	

/* MODE: (A + C) */	

	wrtLock.lock();

		//Extract Gradient Orientation Template 
		lRowIndex =  mCAMERA.RES_VH(0) + mMargin - mVanishPt.V ; 
		lColIndex= mCAMERA.RES_VH(1) - mCAMERA.FRAME_CENTER(1) -mVanishPt.H ;
		Rect ROI = Rect(lColIndex, lRowIndex, mCAMERA.RES_VH(1), mSpan);
		mGRADIENT_TAN_ROOT(ROI).copyTo(mGradTanTemplate);

		//Extract Depth Template
		lRowIndex = mCAMERA.RES_VH(0) -  mSpan; 
		ROI = Rect(0,lRowIndex,mCAMERA.RES_VH(1), mSpan);
		mDEPTH_MAP_ROOT(ROI).copyTo(mDepthTemplate);

		//Extract Focus Template
		lRowIndex = mVP_Range_V	 - mVanishPt.V;
		ROI = Rect(0, lRowIndex, mCAMERA.RES_VH(1), mSpan);
		mFOCUS_MASK_ROOT(ROI).copyTo(mFocusTemplate);	

		// Shift Buffers
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



int BufferingDAG_generic::grabFrame()	
{


	int lReturn = 0;
		
	#ifdef PROFILER_ENABLED
	mProfiler.start("IMAGE_READ");
	#endif 


	   if(mSource == FrameSource::DIRECTORY)
	   {
		mFrameRGB = imread(mFiles[mFrameCount]);
		cout<<"Processing Frame: "<<mFrameCount<<endl;
		
		if (mFrameCount+1 < mFiles.size())
	   	   mFrameCount ++;
	   }

	   else if (mSource == FrameSource::RTSP)

	   {
		mRTSP_CAPTURE >> mFrameRGB;
		cout<<"Processing Frame: "<<mFrameCount<<endl;
	   	mFrameCount ++;
	   }

	   else if (mSource == FrameSource::GMSL)
	   {
		cout<<"Undefined Input Mode"<<endl;
		lReturn = -1;
	   }

	   else
	   {
		cout<<"Undefined Input Mode"<<endl;
		lReturn =-1;
	   }

				
	   if(!mFrameRGB.data)
	   	lReturn = -1;

	   std::string str;

	   if (mSource==FrameSource::DIRECTORY)
	   	str = mFiles[mFrameCount];

	   else if (mSource == FrameSource::RTSP)
	   	str = "RTSP";

	   else if (mSource == FrameSource::GMSL)
	   	str = "GMSL";

	   else
	   	str = "Undefined";


	#ifdef PROFILER_ENABLED
	mProfiler.end();
	LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Frame Source:   "<<endl<<str<<endl
				<<  "Read time: " << mProfiler.getAvgTime("IMAGE_READ")<<endl
				<<"******************************"<<endl<<endl;
				#endif 

	return lReturn;
}
