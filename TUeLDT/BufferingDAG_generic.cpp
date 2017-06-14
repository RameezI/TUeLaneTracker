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
LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
					  <<"******************************"<<endl
					  <<  "GRAY Frame Conversion." <<endl
					  <<  "GrayFrame conversion Time: " << mProfiler.getAvgTime("grabGRAYFrame")<<endl
					  <<"******************************"<<endl<<endl;	
					#endif			
					

#ifdef PROFILER_ENABLED
mProfiler.start("GaussianFiltering");
#endif 
	
	GaussianBlur( mFrameGRAY_ROI, mFrameGRAY_ROI, Size( 5, 5 ), 1.5, 1.5, BORDER_REPLICATE);
				
 #ifdef PROFILER_ENABLED
 mProfiler.end();
 LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Gaussian Filtering." <<endl
										  <<  "Gaussian(11x11) Filtering Time: " << mProfiler.getAvgTime("GaussianFiltering")<<endl
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



		// get Gradient Tanget. Dx*2^7/Dy
			int nRows = mFrameGRAY_ROI.rows;
			int nCols = mFrameGRAY_ROI.cols;;
			int16_t* GradX_pixel;
			int16_t* GradY_pixel;

			if (mGradX.isContinuous() && mGradY.isContinuous())
			{
				nCols *= nRows;
				nRows = 1;
			}

			for(int i = 0; i < nRows; ++i)
			{
				GradX_pixel = mGradX.ptr<int16_t>(i);
				GradY_pixel = mGradY.ptr<int16_t>(i);
				
				for ( int j = 0; j < nCols; ++j)
				{
					if (GradX_pixel[j] < -255)
						GradX_pixel[j] = -255;
					if (GradX_pixel[j] > 255)
						GradX_pixel[j] = 255;
						
						
					if (GradY_pixel[j] < -255)
						GradY_pixel[j] = -255;
					if (GradY_pixel[j] > 255)
						GradY_pixel[j] = 255;
				
					if (GradY_pixel[j]==0)
						GradY_pixel[j]=1;
						
				}
			}			
		
		// Compute Dx/Dy			
			int bufferPos = mBufferPool->GradientTangent.size()-1;
			cv::divide(mGradX, mGradY, mBufferPool->GradientTangent[bufferPos], 128, -1);
								
		//convert to absolute scale and add weighted absolute gradients 
			mGradX_abs = abs(mGradX);
			mGradY_abs = abs(mGradY );
			
		//TODO: REMOVE CONVERION IN GENERIC IMPLEMENTATION. MAG IS CLOSE TO EXPECTED	
		//rounding to next number if 0.5 or greater
			// These must be internally handled on APEX side. OpenCV is not rounding.			
			mGradX_abs.convertTo(mGradX_abs, CV_32F, 1.0, 0.5);
			mGradY_abs.convertTo(mGradY_abs, CV_32F, 1.0, 0.5);
			mGradX_abs.convertTo(mGradX_abs, CV_16S);
			mGradY_abs.convertTo(mGradY_abs, CV_16S);
			
			addWeighted( mGradX_abs, 0.5, mGradY_abs, 0.5, 0, mFrameGradMag );
			//convertScaleAbs(mFrameGradMag, mFrameGradMag);
			mFrameGradMag.convertTo(mFrameGradMag, CV_8U);

				
 #ifdef PROFILER_ENABLED
 mProfiler.end();
 LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Gradients Computation." <<endl
										  <<  "Gray channel gradient computation Time: " << mProfiler.getAvgTime("GradientsComputation")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif
										 



							

#ifdef PROFILER_ENABLED
mProfiler.start("TemplatesWait");
#endif 				
		
		//Synchronise Condition Variable
		WriteLock  wrtLock(_mutex);
		_sateChange.wait(wrtLock,[this]{return mTemplatesReady;} );		
		
				
 #ifdef PROFILER_ENABLED
 mProfiler.end();
 LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Waiting For Worker thread to finish Templates." <<endl
										  <<  "Waiting Time: " << mProfiler.getAvgTime("TemplatesWait")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif	
				
/*			
			uint8_t* Gray_pixel;
			uint8_t* GradMag_pixel;
			int16_t* GradTan_pixel;
			int16_t* GradTanTemplate_pixel;
			uint8_t* TOT_PROB;

			if(	mFrameGRAY_ROI.isContinuous()
				&& mFrameGradMag.isContinuous()
				&& mBufferPool->GradientTangent[bufferPos].isContinuous()
				&& mBufferPool->Probability[bufferPos].isContinuous()
				&& mGradTanTemplate.isContinuous()
			  )
				  
			{
				nCols *= nRows;
				nRows = 1;
			}
			
			


			// Initialise Variables
				mBufferPool->Probability[bufferPos] 
				= Mat(mFrameGRAY_ROI.rows, mFrameGRAY_ROI.cols, CV_8U);
				
				int32_t GrayPixel;
				int32_t GradMagPixel;
				int32_t GradTanPixel;
				int32_t GradTanTemplatePixel;
			
				int32_t GrayProb, GradMagProb, GradTanProb, Prob;

			for(int i = 0; i < nRows; ++i)
			{
				Gray_pixel  			= mFrameGRAY_ROI.ptr<uint8_t>(i);
				GradMag_pixel 			= mFrameGradMag.ptr<uint8_t>(i);
				GradTanTemplate_pixel	= mGradTanTemplate.ptr<int16_t>(i);
				GradTan_pixel   		= mBufferPool->GradientTangent[bufferPos].ptr<int16_t>(i);
				TOT_PROB				= mBufferPool->Probability[bufferPos].ptr<uint8_t>(i);
				
				for ( int j = 0; j < nCols; j++)
				{
					// casting to 32bit integers
					GrayPixel 				= Gray_pixel[j];
					GradMagPixel    		= GradMag_pixel[j];
					GradTanPixel    		= GradTan_pixel[j];
					GradTanTemplatePixel	= GradTanTemplate_pixel[j];
					
					GrayProb = GrayPixel -  mLaneMembership.TIPPING_POINT_GRAY;
					if(GrayProb < 0) GrayProb=0;
					GrayProb = std::round((GrayProb*255.0)/(10.0+GrayProb));
					
					GradMagProb = GradMagPixel -  mLaneMembership.TIPPING_POINT_GRAD_Mag;
					GradMagProb = std::round( (GradMagProb*255.0)/(10.0 + abs(GradMagProb)) );
					
					Prob = GradMagProb + GrayProb;
					if(Prob < 0) Prob=0;
					
					GradTanProb = abs(GradTanPixel - GradTanTemplatePixel);
					GradTanProb = 255 - std::round( (GradTanProb*255.0)/(60.0 + GradTanProb) );
					
					Prob = std::round(Prob*GradTanProb/255.0);
					TOT_PROB[j] = Prob;
					
				}
			}			
*/
#ifdef PROFILER_ENABLED
mProfiler.start("computeProbabilities");
#endif 		

			//GrayChannel Probabilities
			subtract(mFrameGRAY_ROI, mLaneMembership.TIPPING_POINT_GRAY, mTempProbMat, noArray(), CV_32F);
			threshold(mTempProbMat, mTempProbMat, 0, 255, THRESH_TOZERO );
			mTempProbMat.copyTo(mProbMap_Gray);
			mTempProbMat = mTempProbMat + 10;
			divide(mProbMap_Gray, mTempProbMat, mProbMap_Gray, 255, -1);
			
			//GradientMag Probabilities
			subtract(mFrameGradMag, mLaneMembership.TIPPING_POINT_GRAD_Mag, mTempProbMat, noArray(), CV_32F);
			mTempProbMat.copyTo(mProbMap_GradMag);
			mTempProbMat= abs(mTempProbMat) + 10;
			divide(mProbMap_GradMag, mTempProbMat, mProbMap_GradMag, 255, -1);

			// Intermediate Probability Map
			mBufferPool->Probability[bufferPos] = mProbMap_GradMag + mProbMap_Gray;
			threshold(mBufferPool->Probability[bufferPos], mBufferPool->Probability[bufferPos], 0, 255, THRESH_TOZERO );
			mBufferPool->Probability[bufferPos].convertTo(mBufferPool->Probability[bufferPos], CV_8U);			
			mBufferPool->Probability[bufferPos].convertTo(mBufferPool->Probability[bufferPos], CV_16U);
			
			//Gradient Tangent Probability Map
			subtract(mGradTanTemplate, mBufferPool->GradientTangent[bufferPos], mTempProbMat, noArray(), CV_32F);
			mTempProbMat= abs(mTempProbMat);
			mTempProbMat.copyTo(mProbMap_GradDir);
			mTempProbMat = mTempProbMat + 60;
			divide(mProbMap_GradDir, mTempProbMat, mProbMap_GradDir, 255, -1);
			
			subtract(255, mProbMap_GradDir, mProbMap_GradDir, noArray(), -1);
			mProbMap_GradDir.convertTo(mProbMap_GradDir, CV_16U);
			
			
			//Final Probability Map
			multiply(mBufferPool->Probability[bufferPos], mProbMap_GradDir, mBufferPool->Probability[bufferPos]);
		    mBufferPool->Probability[bufferPos].convertTo(mBufferPool->Probability[bufferPos], CV_8U, 1.0/255, 0);
			


		mTemplatesReady= false;
		wrtLock.unlock();
		
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
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

		
	int offset = mCAMERA.RES_VH(0)-mSpan;
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
		
			mBufferPool->Probability[i] 	= mBufferPool->Probability[i+1];		
			mBufferPool->GradientTangent[i] = mBufferPool->GradientTangent[i+1];
		}	
		
	mTemplatesReady = true;
	//cout<< "Templates Ready"<<endl;
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
	else
			if (mFrameCount+1 < mFiles.size())
				mFrameCount ++;
			return 0;

#else

					
#endif


}



