#include "TrackingLanesDAG_generic.h"
#include "ParallelForBodies.h"


TrackingLanesDAG_generic::TrackingLanesDAG_generic(BufferingDAG_generic&& bufferingGraph)

: BufferingDAG_generic(std::move(bufferingGraph)),
  mMAX_PIXELS_ROI(mFrameGRAY_ROI.size().height * mFrameGRAY_ROI.size().width),
  mProbMapFocussed(mFrameGRAY_ROI.size(),CV_8U),
  mGradTanFocussed(mFrameGRAY_ROI.size(),CV_16S)
  
{	
	mX_VPRS.convertTo(mX_VPRS_SCALED, CV_32S, SCALE_INTSEC );
	mBaseBinIdx.reserve(mMAX_PIXELS_ROI);
	mPurviewBinIdx.reserve(mMAX_PIXELS_ROI);
	mWeightBin.reserve(mMAX_PIXELS_ROI);
	
}


void TrackingLanesDAG_generic::extractLanes()
{

	
#ifdef PROFILER_ENABLED
mProfiler.start("FilterFocus");
#endif	
	
	mBufferPool->Probability[0].copyTo(mProbMapFocussed);
	mBufferPool->GradientTangent[0].copyTo(mGradTanFocussed);
	
	for ( int i=1; i< mBufferPool->Probability.size(); i++ )
	{	
		mMask = mProbMapFocussed < mBufferPool->Probability[i];
		
		mBufferPool->Probability[i].copyTo(mProbMapFocussed, mMask );
		mBufferPool->GradientTangent[i].copyTo(mGradTanFocussed, mMask );
	}
	
	bitwise_and(mProbMapFocussed, mFocusTemplate, mProbMapFocussed);
	
	
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
  <<"******************************"<<endl
  <<  "Filter and Focus Based on Focus Mask." <<endl
  <<  "Focussing Time: " << mProfiler.getAvgTime("FilterFocus")<<endl
  <<"******************************"<<endl<<endl;	
#endif





#ifdef PROFILER_ENABLED
mProfiler.start("ComputeIntersections");
#endif	
	
	
	//Base Intersections
	subtract(mLaneFilter->OFFSET_V, mY_VPRS, mIntBase, noArray(), CV_32S);
	divide(mIntBase, mGradTanFocussed, mIntBase, SCALE_INTSEC_TAN, CV_32S);
	add(mIntBase, mX_VPRS_SCALED, mIntBase);
	
	
	//Purview Intersections
	subtract(mVpFilter->OFFSET_V, mY_VPRS, mIntPurview, noArray(), CV_32S);
	divide(mIntPurview,mGradTanFocussed, mIntPurview, SCALE_INTSEC_TAN, CV_32S);
	add(mIntPurview, mX_VPRS_SCALED, mIntPurview);
	
	//Weights of Intersections
	multiply(mDepthTemplate, mProbMapFocussed, mIntWeights, 1, CV_32S);	
	
	//Build Mask for Valid Intersections
	bitwise_and(mProbMapFocussed > 0, mGradTanFocussed !=0,    mMask);
	bitwise_and(mMask, mIntBase    > LOWER_LIMIT_IntBase,     mMask);
	bitwise_and(mMask, mIntPurview > LOWER_LIMIT_IntPurview,  mMask);
    bitwise_and(mMask, mIntBase    < UPPER_LIMIT_IntBase,     mMask);
    bitwise_and(mMask, mIntPurview < UPPER_LIMIT_IntPurview,  mMask);

	int size= countNonZero(mMask);
	

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
  <<"******************************"<<endl
  <<  "Compute Intersections with Bottom and Horizon." <<endl
  <<  "Intersection Compute Time: " << mProfiler.getAvgTime("ComputeIntersections")<<endl
  <<"******************************"<<endl<<endl;	
#endif
		




#ifdef PROFILER_ENABLED
mProfiler.start("ExtractValidBinIds");
#endif	
		
	// Resize vectors to Maximum Limit
	mBaseBinIdx.resize(mMAX_PIXELS_ROI);
	mPurviewBinIdx.resize(mMAX_PIXELS_ROI);	
	mWeightBin.resize(mMAX_PIXELS_ROI);

/* Paralle FOR is Disabled
 * 
	// Fill Vectors With Valid Values using Parallel for Loops. NOT WORKING
	{ //^TODO: The Object instantiation can be moved to SetupGraph Routine
		FillBins_Parallel fillBins(	mIntBase,
									mIntPurview,
									mIntWeights,
									mMask,
									SCALED_STEP_LANE_FILTER,
									SCALED_STEP_VP_FILTER,
									SCALED_START_LANE_FILTER,
									SCALED_START_VP_FILTER,
									mBaseBinIdx.data(),
									mPurviewBinIdx.data(),
									mWeightBin.data()
								  );
		
		parallel_for_(cv::Range(0, mMAX_PIXELS_ROI), fillBins);
	} 
*/
	
	register int32_t* 	IN_basePTR 	    	= mIntBase.ptr<int32_t>(0);
	register int32_t* 	IN_purviewPTR   	= mIntPurview.ptr<int32_t>(0);
	register int32_t* 	IN_weightsPTR   	= mIntWeights.ptr<int32_t>(0);
	register uint8_t* 	IN_maskPTR   		= mMask.ptr<uint8_t>(0);
	
	register uint16_t*   OUT_basePTR		= mBaseBinIdx.data();
	register uint16_t*   OUT_purviewPTR		= mPurviewBinIdx.data();
	register int32_t*    OUT_weights		= mWeightBin.data();
	
	
	for (int i = 0; i < mMAX_PIXELS_ROI; i++, IN_basePTR++, IN_purviewPTR++, IN_weightsPTR++, IN_maskPTR++)
	{
		
		if(!(*IN_maskPTR ==0) )
		{		
			*OUT_basePTR=
				(*IN_basePTR    - SCALED_START_LANE_FILTER + (SCALED_STEP_LANE_FILTER/2) ) / SCALED_STEP_LANE_FILTER;
			
			*OUT_purviewPTR=
				(*IN_purviewPTR -  SCALED_START_VP_FILTER   + (SCALED_STEP_VP_FILTER/2)  ) / SCALED_STEP_VP_FILTER ;
			
			*OUT_weights = *IN_weightsPTR;
			
			OUT_basePTR++;
			OUT_purviewPTR++;
			OUT_weights++;
		}	
			
	}
	
	mBaseBinIdx.resize(size);
	mPurviewBinIdx.resize(size);
	mWeightBin.resize(size);
	
	
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
  <<"******************************"<<endl
  <<  "Extract Valid Intersection Bin IDs." <<endl
  <<  "Paralle-For Compute Time: " << mProfiler.getAvgTime("ExtractValidBinIds")<<endl
  <<"******************************"<<endl<<endl;	
#endif



#ifdef PROFILER_ENABLED
mProfiler.start("ComputeHistograms");
#endif


	 mHistBase 		= Mat::zeros(mLaneFilter->mNb_HISTOGRAM_BINS,  1 , CV_32S);
	 mHistPurview   = Mat::zeros (mLaneFilter->mNb_HISTOGRAM_BINS, 1 , CV_32S);
	
	register int32_t* HistBase_pixelPTR    =  mHistBase.ptr<int32_t>(0);
	register int32_t* HistPurview_pixelPTR =  mHistPurview.ptr<int32_t>(0);
	
	for ( int j = 0; j < mBaseBinIdx.size(); ++j)
	{
		
		*(HistBase_pixelPTR 	+ mBaseBinIdx[j])  += mWeightBin[j];
		*(HistPurview_pixelPTR  + mPurviewBinIdx[j]) += mWeightBin[j];
	}
	


#ifdef PROFILER_ENABLED
mProfiler.end();

		LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
		  <<"******************************"<<endl
		  <<  "Compute Weighted Histograms." <<endl
		  <<  "Histogram Computation Time: " << mProfiler.getAvgTime("ComputeHistograms")<<endl
		  <<"******************************"<<endl<<endl;	
		#endif
		
	


#ifdef PROFILER_ENABLED
mProfiler.start("HistogramMatching");
#endif	







#ifdef PROFILER_ENABLED
mProfiler.end();

		LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
		  <<"******************************"<<endl
		  <<  "Histogram Matching leading to MAP Estimate." <<endl
		  <<  "Histogram Matching Time: " << mProfiler.getAvgTime("HistogramMatching")<<endl
		  <<"******************************"<<endl<<endl;	
		#endif




#ifdef PROFILER_ENABLED
mProfiler.start("Display");
#endif
	
	
		 imshow( "Display window", mProbMapFocussed);
		 waitKey(1);
	

									
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
		  <<"******************************"<<endl
		  <<  "Screen Display." <<endl
		  <<  "Display update time: " << mProfiler.getAvgTime("Display")<<endl
		  <<"******************************"<<endl<<endl;	
		 #endif	
		
}

void TrackingLanesDAG_generic::extractControllerInputs()
{
	
}

void TrackingLanesDAG_generic::auxillaryTasks()
{
		
	int offset   =  mCAMERA.RES_VH(0)  -mSpan;
	int rowIndex = mCAMERA.RES_VH(0) - mCAMERA.FRAME_CENTER(0) -mVanishPt.V +offset ;
	int colIndex = mCAMERA.RES_VH(1) - mCAMERA.FRAME_CENTER(1) -mVanishPt.H ;


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

	wrtLock.unlock();
	_sateChange.notify_one();

}

TrackingLanesDAG_generic::~TrackingLanesDAG_generic()
{
	

}



