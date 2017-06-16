#include "TrackingLanesDAG_generic.h"
#include "ParallelForBodies.h"


TrackingLanesDAG_generic::TrackingLanesDAG_generic(BufferingDAG_generic&& bufferingGraph)

: mStartFiltering(false),
  mFiltersReady(false),
  BufferingDAG_generic(std::move(bufferingGraph)),
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

	

	WriteLock  wrtLock(_mutex, std::defer_lock);

	//Signal mSideExecutor to start Filtering
	wrtLock.lock();
	this->mStartFiltering = true;
	wrtLock.unlock();
	_sateChange.notify_one();
	
	
#ifdef PROFILER_ENABLED
mProfiler.start("TemporalFiltering");
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
  <<  "Focussing Time: " << mProfiler.getAvgTime("TemporalFiltering")<<endl
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
	bitwise_and(mMask, mIntBase    > mLOWER_LIMIT_IntBase,     mMask);
	bitwise_and(mMask, mIntPurview > mLOWER_LIMIT_IntPurview,  mMask);
    bitwise_and(mMask, mIntBase    < mUPPER_LIMIT_IntBase,     mMask);
    bitwise_and(mMask, mIntPurview < mUPPER_LIMIT_IntPurview,  mMask);
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
*/	{
		register int32_t* 	IN_basePTR 	    	= mIntBase.ptr<int32_t>(0);
		register int32_t* 	IN_purviewPTR   	= mIntPurview.ptr<int32_t>(0);
		register int32_t* 	IN_weightsPTR   	= mIntWeights.ptr<int32_t>(0);
		register uint8_t* 	IN_maskPTR   		= mMask.ptr<uint8_t>(0);
		
		register uint16_t*   OUT_basePTR		= mBaseBinIdx.data();
		register uint16_t*   OUT_purviewPTR		= mPurviewBinIdx.data();
		register int32_t*    OUT_weights		= mWeightBin.data();
		
		for (int i = 0; i < mMAX_PIXELS_ROI; i++,IN_basePTR++,IN_purviewPTR++, IN_weightsPTR++ , IN_maskPTR++)
		{
			
			if(!(*IN_maskPTR ==0) )
			{		
				*OUT_basePTR=
					(*IN_basePTR    - mSCALED_START_LANE_FILTER + (mSCALED_STEP_LANE_FILTER/2) )
					/ mSCALED_STEP_LANE_FILTER;
				
				*OUT_purviewPTR=
					(*IN_purviewPTR -  mSCALED_START_VP_FILTER   + (mSCALED_STEP_VP_FILTER/2)  )
					/ mSCALED_STEP_VP_FILTER ;
				
				*OUT_weights = *IN_weightsPTR;
				
				OUT_basePTR++;
				OUT_purviewPTR++;
				OUT_weights++;
			}	
				
		}
		
	} //Block-Ends
	
	mBaseBinIdx.resize(size);
	mPurviewBinIdx.resize(size);
	mWeightBin.resize(size);	
	
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
  <<"******************************"<<endl
  <<  "Extract Valid Intersection Bin IDs." <<endl
  <<  "Vector Filling Time: " << mProfiler.getAvgTime("ExtractValidBinIds")<<endl
  <<"******************************"<<endl<<endl;	
#endif









#ifdef PROFILER_ENABLED
mProfiler.start("ComputeHistograms");
#endif

	{
		int64_t SUM;
		register int32_t* HistBase_pixelPTR    =  mHistBase.ptr<int32_t>(0);
		register int32_t* HistPurview_pixelPTR =  mHistPurview.ptr<int32_t>(0);
	
		for ( int j = 0; j < mBaseBinIdx.size(); ++j)
		{
			*(HistBase_pixelPTR 	+ mBaseBinIdx[j])  += mWeightBin[j];
			*(HistPurview_pixelPTR  + mPurviewBinIdx[j]) += mWeightBin[j];
		}
	
		// Normalising Base Histogram
		 SUM = sum(mHistBase)[0];
		 mHistBase.convertTo(mHistBase, CV_64F, SCALE_FILTER);
		 mHistBase.convertTo(mHistBase, CV_32S, 1.0/SUM );
		 
		 //Normalising Purview Histogram
		 SUM = sum(mHistPurview)[0];
		 mHistPurview.convertTo(mHistPurview, CV_64F, SCALE_FILTER);
		 mHistPurview.convertTo(mHistPurview, CV_32S, 1.0/SUM );
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
mProfiler.start("FiltersWait");
#endif 				
		
	wrtLock.lock();
	_sateChange.wait(wrtLock,[this]{return mFiltersReady;} );
	mFiltersReady = false; //reset the flag for next loop.
	/* Filters are not Locked by the main thread 
	   As the worker is not going to read/write the Filters anymore in this run-loop,
	   This is completely fine.
	*/
	wrtLock.unlock();
				
 #ifdef PROFILER_ENABLED
 mProfiler.end();
 LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Waiting For Worker thread to finish Filters." <<endl
										  <<  "Waiting Time: " << mProfiler.getAvgTime("FiltersWait")<<endl
										  <<"******************************"<<endl<<endl;	
										 #endif	


#ifdef PROFILER_ENABLED
mProfiler.start("HistogramMatching");
#endif
	
	{
		
		int   BestModelIdx=-1;
		int   NegLaneCorrelation;
		float x;
		int   bestPosteriorProb  = 0;
		float conditionalProb;
		
		vector<BaseHistogramModel>& Models= mLaneFilter->baseHistogramModels;
		
		Mat    range;
		register int32_t* HistBasePTR    	=  mHistBase.ptr<int32_t>(0);
		
		int Sum=0;


		for(size_t i=0; i< Models.size(); i++)
		{    
			
			int& LeftIdx    	= Models[i].binID_leftBoundary;
			int& RightIdx		= Models[i].binID_rightBoundary;
			
			int& NegLeftIdx		= Models[i].binID_NegBoundaryLeft;
			int& NegRightIdx    = Models[i].binID_NegBoundaryRight;
			
			int& Nleft          = Models[i].nbNonBoundaryBinsLeft;
			int& Nright		    = Models[i].nbNonBoundaryBinsRight;
	
		

			mLikelihoodLeftBoundary =  round(HistBasePTR[LeftIdx-1]*0.25
											+HistBasePTR[LeftIdx]
											+HistBasePTR[LeftIdx+1]*0.25);
										 
			mLikelihoodRightBoundary = round( HistBasePTR[RightIdx-1]*0.25
											+HistBasePTR[RightIdx]
											+HistBasePTR[RightIdx+1]*0.25);
	
			conditionalProb  =  (mLikelihoodLeftBoundary*mLikelihoodRightBoundary)/(float)SCALE_FILTER;

//TODO:	Put on the side thread.(start)
			NegLaneCorrelation=0;
			range = mHistBase(Range(NegLeftIdx,  NegLeftIdx  +  Nleft), Range::all());
			NegLaneCorrelation +=sum(range)[0];
			
		    range = mHistBase(Range(NegRightIdx, NegRightIdx + Nright), Range::all());
			NegLaneCorrelation +=sum(range)[0];
		
			x= (float)NegLaneCorrelation / SCALE_FILTER ;
			
			mLikelihoodNegBoundary = mLaneMembership.NEG_BOUNDARY_NORMA 
			* exp ( - pow(x,2) / mLaneMembership.NEG_BOUNDARY_NOMIN );

//Put on the  side thread. syynchronise mLikelihoodNegBoundary (end)
			
			conditionalProb  = (conditionalProb * mLikelihoodNegBoundary) / SCALE_FILTER; 
		
		    
			mPosteriorProb = round(conditionalProb*
			mTransitLaneFilter.at<int32_t>(Models[i].leftOffsetIdx, Models[i].rightOffsetIdx) );
			
			mLaneFilter->filter.at<int32_t>(Models[i].leftOffsetIdx, Models[i].rightOffsetIdx)
			= mPosteriorProb;

			if(mPosteriorProb > bestPosteriorProb)
			{
				bestPosteriorProb=mPosteriorProb;
				
				BestModelIdx =i;
				mLaneModel.confidenceLeft  = mLikelihoodLeftBoundary;
			 	mLaneModel.confidenceRight = mLikelihoodRightBoundary;
			}
				
		} 
		
		
		mLaneModel.leftOffset 		= Models[BestModelIdx].leftOffset;
		mLaneModel.rightOffset		= Models[BestModelIdx].rightOffset;
		 
		mLaneModel.confidenceLeft  =  (100* mLaneModel.confidenceLeft)/(float)SCALE_FILTER;
		mLaneModel.confidenceRight  = (100*mLaneModel.confidenceRight)/(float)SCALE_FILTER;
		
	}//Scope End
		
#ifdef PROFILER_ENABLED
mProfiler.end();

		LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
		  <<"******************************"<<endl
		  <<  "Histogram Matching MAP Estimate LaneBoundaries." <<endl
		  <<  "Filter Update Time: " << mProfiler.getAvgTime("HistogramMatching")<<endl
		  <<"******************************"<<endl<<endl;	
		#endif




#ifdef PROFILER_ENABLED
mProfiler.start("VP_HistogramMatching");
#endif
	{	
		int   	bestPosteriorProb  = 0;
		register int32_t* HistPurviewPTR    =  mHistBase.ptr<int32_t>(0);
		int Sum = 0;
		
		
		float delta= mLaneFilter->STEP/2.0;
		
		const float left_VP  = ((-mLaneModel.leftOffset  - delta)
							   +(-mLaneModel.leftOffset + delta))
							   /2.0;													
		
		const float right_VP  = ((mLaneModel.rightOffset  - delta)
								+(mLaneModel.rightOffset  + delta))
								/2.0;
								
		const int   FRAME_CENTER_V		= mCAMERA.FRAME_CENTER(0); 
 		const float PIXEL_TO_CM     	= 1.0/mCAMERA.CM_TO_PIXEL;
		
		const int    VP_FILTER_OFFSET  	= mVpFilter->OFFSET_V;
		const int    VP_STEP_HIST		= mVpFilter->STEP;
		const int    VP_HIST_START		= mVpFilter->HISTOGRAM_BINS(0);
		const int    VP_HIST_SIZE		= mVpFilter->HISTOGRAM_BINS.size();
		const int    VP_HIST_END		= mVpFilter->HISTOGRAM_BINS(VP_HIST_SIZE-1);
		const float  VP_HIST_RATIO      = mVpFilter->mVP_LANE_RATIO;
		const float  WIDTH_FACTOR       = PIXEL_TO_CM* VP_HIST_RATIO;
			
	
		float  IntSecLeft, IntSecRight; 
		int    LeftIdx, MidIdx, RightIdx;
        int    NbNegLaneLeft, NbNegLaneRight;  
		
		float  width, widthProb, boundaryProb;
		
		PurviewHistogramModel Model;

		
		for(int v=0; v < mVpFilter->mNb_VP_BINS_V;v++)
		{	for(int h=0; h < mVpFilter->mNb_VP_BINS_H; h++)
			{
			
				//Convert from Center to VP Coordinate system
				const	int&  binV = -mVpFilter->VP_BINS_V(v);
				const 	int&  binH = mVpFilter->VP_BINS_H(h);
				
				IntSecLeft = ((binH - left_VP)/(float)(binV+FRAME_CENTER_V))
							 *(VP_FILTER_OFFSET - binV) +binH;
				IntSecLeft = VP_STEP_HIST * round(IntSecLeft/VP_STEP_HIST);
				LeftIdx= (IntSecLeft - VP_HIST_START)/VP_HIST_START;
				
				
				IntSecRight = ((binH - right_VP)/(float)(binV+FRAME_CENTER_V))
							 *(VP_FILTER_OFFSET - binV) +binH;
				IntSecRight = VP_STEP_HIST * round(IntSecRight/VP_STEP_HIST);
				RightIdx = (VP_HIST_SIZE-1) - (VP_HIST_END - IntSecRight)/VP_STEP_HIST;
			
			
			
					   
				if (LeftIdx > 0 && RightIdx < VP_HIST_SIZE-2)
				{	
					
					MidIdx  		= round( ( LeftIdx+ RightIdx ) /2.0);
					NbNegLaneLeft   =  (MidIdx-3) - (LeftIdx+ 2) ;        
					NbNegLaneRight  = (RightIdx-2) - (MidIdx+3);
				
				
					width= (IntSecRight -IntSecLeft) * WIDTH_FACTOR;
				
					widthProb = mLaneMembership.WIDTH_DIFF_NORMA 
							  * exp( - pow(mLaneModel.laneWidth - width, 2) / mLaneMembership.WIDTH_DIFF_NOMIN );
				
				
					boundaryProb =  round(HistPurviewPTR[LeftIdx-1]*0.25
										 +HistPurviewPTR[LeftIdx]
										 +HistPurviewPTR[LeftIdx+1]*0.25);
									 
					boundaryProb += round(HistPurviewPTR[RightIdx-1]*0.25
										 +HistPurviewPTR[RightIdx]
										 +HistPurviewPTR[RightIdx+1]*0.25);
										 
										 
//TODO:	Put on the side thread.(start)
		/*			NegLaneCorrelation=0;
					range = mHistBase(Range(NegLeftIdx,  NegLeftIdx  +  Nleft), Range::all());
					NegLaneCorrelation +=sum(range)[0];
			
					range = mHistBase(Range(NegRightIdx, NegRightIdx + Nright), Range::all());
					NegLaneCorrelation +=sum(range)[0];
		
					x= (float)NegLaneCorrelation / SCALE_FILTER ;
					mLikelihoodNegBoundary =  round(( mLaneMembership.NEG_BOUNDARY_NORMA 
											* exp ( -pow(x,2) / mLaneMembership.NEG_BOUNDARY_NOMIN  ) )
											* SCALE_FILTER);
											* 
					 IdxLeft + 2;
					 IdxMid +4;
		*/

//Put on the side thread. syynchronise mLikelihoodNegBoundary (end)					 
										 
										 
										 
										 
										 
										 
												
				
				}
							
											
			}			
		}
		
	}
		
		

#ifdef PROFILER_ENABLED
mProfiler.end();

		LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
		  <<"******************************"<<endl
		  <<  "Histogram Matching MAP Estimate VanishingPt." <<endl
		  <<  "Filter Update Time: " << mProfiler.getAvgTime("VP_HistogramMatching")<<endl
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


// MODE: A
		
	int offset   =  mCAMERA.RES_VH(0)  -mSpan;
	int rowIndex =  mCAMERA.RES_VH(0) - mCAMERA.FRAME_CENTER(0) -mVanishPt.V +offset ;
	int colIndex =  mCAMERA.RES_VH(1) - mCAMERA.FRAME_CENTER(1) -mVanishPt.H ;


	WriteLock  wrtLock(_mutex, std::defer_lock);
	
	
		
		Rect ROI = Rect(colIndex, rowIndex, mCAMERA.RES_VH(1), mSpan);
		mGRADIENT_TAN_ROOT(ROI).copyTo(mGradTanTemplate);
			
		ROI = Rect(0,rowIndex,mCAMERA.RES_VH(1), mSpan);
		mDEPTH_MAP_ROOT(ROI).copyTo(mDepthTemplate);
		
		rowIndex = mVP_Range_V-mVanishPt.V;
		ROI = Rect(0, rowIndex, mCAMERA.RES_VH(1), mSpan);
		mFOCUS_MASK_ROOT(ROI).copyTo(mFocusTemplate);	

	wrtLock.lock();
		for ( int i = 0; i< mBufferPool->Probability.size()-1 ; i++ )
		{		
			mBufferPool->Probability[i] 	= mBufferPool->Probability[i+1];		
			mBufferPool->GradientTangent[i] = mBufferPool->GradientTangent[i+1];			
		}
		
		mBufferReady = true;
	
	wrtLock.unlock();
	_sateChange.notify_one();
	
	
//MODE B:
	wrtLock.lock();
	_sateChange.wait(wrtLock,[this]{return mStartFiltering;} );

	
	int64_t SUM;
	
	//Predict Lane States
	mLaneFilter->filter.convertTo(mLaneFilter->filter, CV_64F);
	boxFilter(mLaneFilter->filter, mTransitLaneFilter, -1, Size(11,11), Point(-1,-1), false, BORDER_REPLICATE );
    mLaneFilter->filter.convertTo(mLaneFilter->filter, CV_32S);

	SUM = sum(mTransitLaneFilter)[0];
	mTransitLaneFilter= mTransitLaneFilter*SCALE_FILTER;
	mTransitLaneFilter.convertTo(mTransitLaneFilter, CV_32S, 1.0/SUM);
	mTransitLaneFilter = 	mTransitLaneFilter + mLaneFilter->prior;


	
	
	//Predict VP States
	mVpFilter->filter.convertTo(mVpFilter->filter, CV_64F);
	boxFilter(mVpFilter->filter, mTransitVpFilter, -1, Size(3,3), Point(-1,-1), false, BORDER_REPLICATE );
    mVpFilter->filter.convertTo(mVpFilter->filter, CV_32S);
	
	SUM = sum(mTransitVpFilter)[0];
	mTransitVpFilter= mTransitVpFilter*SCALE_FILTER;
	mTransitVpFilter.convertTo(mTransitVpFilter, CV_32S, 1.0/SUM);	
	mTransitVpFilter = mTransitVpFilter + mVpFilter->prior;
			
		mFiltersReady   = true;
		mStartFiltering = false;
	
	wrtLock.unlock();
	_sateChange.notify_one();



//MODE C:
	wrtLock.lock();


	wrtLock.unlock();



//MODE D:
	wrtLock.lock();
	
	wrtLock.unlock();
	

}

TrackingLanesDAG_generic::~TrackingLanesDAG_generic()
{
	

}