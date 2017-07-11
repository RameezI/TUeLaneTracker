#include "TrackingLanesDAG_generic.h"


TrackingLanesDAG_generic::TrackingLanesDAG_generic(BufferingDAG_generic&& bufferingGraph)

: 
  BufferingDAG_generic(std::move(bufferingGraph)),
  mStartFiltering(false),
  mFiltersReady(false),
  mMAX_PIXELS_ROI(mFrameGRAY_ROI.size().height * mFrameGRAY_ROI.size().width)
{	
	mX_VPRS.convertTo(mX_VPRS_SCALED, CV_32S, SCALE_INTSEC );
	mBaseBinIdx.reserve(mMAX_PIXELS_ROI);
	mPurviewBinIdx.reserve(mMAX_PIXELS_ROI);
	mWeightBin.reserve(mMAX_PIXELS_ROI);

/*
	//Write Images to a video file
	mOutputVideo.open("TUeLaneTracker.avi", CV_FOURCC('M','P','4','V'), 30, mFrameRGB.size());
*/

}


void TrackingLanesDAG_generic::extractLanes()
{

	WriteLock  wrtLock(_mutex, std::defer_lock);

	//Start Filtering
	wrtLock.lock();
	this->mStartFiltering = true;
	wrtLock.unlock();
	_sateChange.notify_one();





#ifdef PROFILER_ENABLED
mProfiler.start("TemporalFiltering");
#endif	

	mProbMapFocussed = mBufferPool->Probability[0];
	mGradTanFocussed = mBufferPool->GradientTangent[0];
	
	for ( std::size_t i=1; i< mBufferPool->Probability.size(); i++ )
	{	
	    mMask = mProbMapFocussed < mBufferPool->Probability[i];
	    mBufferPool->Probability[i].copyTo(mProbMapFocussed, mMask );
	    mBufferPool->GradientTangent[i].copyTo(mGradTanFocussed, mMask );
	}
	
	bitwise_and(mProbMapFocussed, mFocusTemplate, mProbMapFocussed);
	
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
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
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
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

	{
	   register int32_t* 	IN_basePTR 	    	= mIntBase.ptr<int32_t>(0);
	   register int32_t* 	IN_purviewPTR   	= mIntPurview.ptr<int32_t>(0);
	   register int32_t* 	IN_weightsPTR   	= mIntWeights.ptr<int32_t>(0);
	   register uint8_t* 	IN_maskPTR   		= mMask.ptr<uint8_t>(0);
	
	   register uint16_t*   	OUT_basePTR		= mBaseBinIdx.data();
	   register uint16_t*   	OUT_purviewPTR		= mPurviewBinIdx.data();
	   register int32_t*    	OUT_weights		= mWeightBin.data();

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
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
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

	   for ( std::size_t j = 0; j < mBaseBinIdx.size(); ++j)
	   {
		*(HistBase_pixelPTR 	+ mBaseBinIdx[j])  += mWeightBin[j];
		*(HistPurview_pixelPTR  + mPurviewBinIdx[j]) += mWeightBin[j];
	   }

	    //Normalising Base Histogram
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
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Compute Weighted Histograms." <<endl
				<<  "Histogram Compute Time: " << mProfiler.getAvgTime("ComputeHistograms")<<endl
				<<"******************************"<<endl<<endl;	
				#endif








#ifdef PROFILER_ENABLED
mProfiler.start("FiltersWait");
#endif 				
		
	wrtLock.lock();
	_sateChange.wait(wrtLock,[this]{return mFiltersReady;} );
	mFiltersReady = false; //reset the flag for next loop.
	wrtLock.unlock();

				
 #ifdef PROFILER_ENABLED
 mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
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
		
	   register int32_t* HistBasePTR    	=  mHistBase.ptr<int32_t>(0);
	   vector<BaseHistogramModel>& Models= mLaneFilter->baseHistogramModels;
	   Mat    range;
	
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
//(end)
			

		conditionalProb  = (conditionalProb * mLikelihoodNegBoundary); 
	
	    
		mPosteriorProbBase = round(conditionalProb*
		mTransitLaneFilter.at<int32_t>(Models[i].leftOffsetIdx, Models[i].rightOffsetIdx) );
		
		mLaneFilter->filter.at<int32_t>(Models[i].leftOffsetIdx, Models[i].rightOffsetIdx)
		= mPosteriorProbBase;

		if(mPosteriorProbBase > bestPosteriorProb)
		{
		   bestPosteriorProb=mPosteriorProbBase;
				
		   BestModelIdx =i;
		   mLaneModel.confidenceLeft  = mLikelihoodLeftBoundary;
		   mLaneModel.confidenceRight = mLikelihoodRightBoundary;
		}
				
	   } 
		
	   if (bestPosteriorProb == 0)
	   return;
		  
	   mLaneModel.leftOffset 	= Models[BestModelIdx].leftOffset;
	   mLaneModel.rightOffset	= Models[BestModelIdx].rightOffset;
	   mLaneModel.centerLane       	= round((mLaneModel.leftOffset + mLaneModel.rightOffset)/2.0);
		
	   mLaneModel.laneWidth_cm     = Models[BestModelIdx].width_cm;
	 
	   mLaneModel.confidenceLeft  =  round(100* mLaneModel.confidenceLeft/(float)SCALE_FILTER);
	   mLaneModel.confidenceRight  = round(100* mLaneModel.confidenceRight/(float)SCALE_FILTER);
		
	}//Scope End
		
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Histogram Matching MAP Estimate LaneBoundaries." <<endl
				<<  "Filter Update Time: " << mProfiler.getAvgTime("HistogramMatching")<<endl
				<<"******************************"<<endl<<endl;	
				#endif




#ifdef PROFILER_ENABLED
mProfiler.start("VP_HistogramMatching");
#endif

	const int delta= round(mLaneFilter->STEP/2.0);
	
	{	
	   int   	bestPosteriorProb  = 0;
	   register int32_t* HistPurviewPTR    =  mHistPurview.ptr<int32_t>(0);

	
	   const float left_VP  = ((-mLaneModel.leftOffset  - delta)
				+(-mLaneModel.leftOffset + delta))
				/2.0;													
	
	   const float right_VP  = ((mLaneModel.rightOffset  - delta)
				 +(mLaneModel.rightOffset  + delta))
				 /2.0;				
		
	   const int    FRAME_CENTER_V	= mCAMERA.FRAME_CENTER(0); 
	   const float  PIXEL_TO_CM     	= 1.0/mCAMERA.CM_TO_PIXEL;

	   const int    VP_FILTER_OFFSET = mVpFilter->OFFSET_V;
	   const int    VP_HIST_STEP	 = mVpFilter->STEP;
	   const int    VP_HIST_START	 = mVpFilter->HISTOGRAM_BINS(0);
	   const int    VP_HIST_SIZE	 = mVpFilter->HISTOGRAM_BINS.size();
	   const int    VP_HIST_END	 = mVpFilter->HISTOGRAM_BINS(VP_HIST_SIZE-1);
	   const float  VP_HIST_RATIO  	 = mVpFilter->mVP_LANE_RATIO;
	   const float  WIDTH_FACTOR   	 = PIXEL_TO_CM* 1.0/VP_HIST_RATIO;
		

	   float  IntSecLeft, IntSecRight; 
	   int    LeftIdx, MidIdx, RightIdx;

	   int   NegLeftIdx, NegRightIdx;
	   int   NbNegLaneLeft, NbNegLaneRight, NegLaneCorrelation;
	   float x, conditionalProb;
	
	   Mat   range;
	
	   float  width_cm;
	
	   /* Store Previous values of VP */
	   mVanishPt.V_prev = mVanishPt.V;
	   mVanishPt.H_prev = mVanishPt.H;
		
	   for(int v=0; v < mVpFilter->mNb_VP_BINS_V;v++)
	   {	for(int h=0; h < mVpFilter->mNb_VP_BINS_H; h++)
		{
			
		   //Convert from Center to VP Coordinate system
		   const	int&  binV = -mVpFilter->VP_BINS_V(v);
		   const 	int&  binH = mVpFilter->VP_BINS_H(h);
		
		   IntSecLeft = ((binH - left_VP)/(float)(binV+FRAME_CENTER_V))
					 *(VP_FILTER_OFFSET - binV) +binH;
		   IntSecLeft = VP_HIST_STEP * round(IntSecLeft/VP_HIST_STEP);
		   LeftIdx= (IntSecLeft - VP_HIST_START)/VP_HIST_STEP;
		
		
		   IntSecRight 	= ((binH - right_VP)/(float)(binV+FRAME_CENTER_V))
				  *(VP_FILTER_OFFSET - binV) +binH;
		   IntSecRight 	= VP_HIST_STEP * round(IntSecRight/VP_HIST_STEP);
		   RightIdx 	= (VP_HIST_SIZE-1) - (VP_HIST_END - IntSecRight)/VP_HIST_STEP;
		
		   MidIdx  	= round( ( LeftIdx+ RightIdx ) /2.0);
		
		   NegLeftIdx  	   = LeftIdx+2;
		   NegRightIdx     = MidIdx+4;
		
		   NbNegLaneLeft   = (MidIdx-3) - (LeftIdx+ 2) ;        
		   NbNegLaneRight  = (RightIdx-2) - (MidIdx+3);
		
		   width_cm= (IntSecRight -IntSecLeft) * WIDTH_FACTOR;
					   
		   if (LeftIdx > 0 && RightIdx < VP_HIST_SIZE-2)
		   {	
			
		      /* VP Probability over Lane Width at Offseted-VP */	
		      mLikelihoodVP_Width = mLaneMembership.WIDTH_DIFF_NORMA 
		       * exp( - pow(mLaneModel.laneWidth_cm - width_cm, 2) / mLaneMembership.WIDTH_DIFF_NOMIN );
				
				
		      /* VP Probability over Left LaneBoundary */
		      mLikelihoodVP_LBoundary =  round(HistPurviewPTR[LeftIdx-1]*0.25
					      + HistPurviewPTR[LeftIdx]
				  	      + HistPurviewPTR[LeftIdx+1]*0.25);
												  
		      /* VP Probability over Right LaneBoundary */				 
		      mLikelihoodVP_RBoundary = round(HistPurviewPTR[RightIdx-1]*0.25
					      + HistPurviewPTR[RightIdx]
					      + HistPurviewPTR[RightIdx+1]*0.25);
					
		      conditionalProb  =  (mLikelihoodVP_LBoundary*mLikelihoodVP_RBoundary) /(float)SCALE_FILTER;	
		
		      /* VP Probability over Negative(NO) Boundary */
		      NegLaneCorrelation=0;
		      range = mHistPurview(Range(NegLeftIdx,  NegLeftIdx  +  NbNegLaneLeft), Range::all());
		      NegLaneCorrelation +=sum(range)[0];
			
		      range = mHistPurview(Range(NegRightIdx, NegRightIdx + NbNegLaneRight), Range::all());
		      NegLaneCorrelation +=sum(range)[0];
		
		      x= (float)NegLaneCorrelation / SCALE_FILTER ;
		      mLikelihoodVP_NegBoundary = mLaneMembership.NEG_BOUNDARY_NORMA 
		       *exp (-pow(x,2)/mLaneMembership.NEG_BOUNDARY_NOMIN);
											 				 
		      /* Conditional Probability */							 
		      conditionalProb  =   conditionalProb
					* mLikelihoodVP_NegBoundary
					* mLikelihoodVP_Width;
					
		      mPosteriorProbVP = round(conditionalProb * mTransitVpFilter.at<int32_t>(v ,h));
			
		      mVpFilter->filter.at<int32_t>(v, h) = mPosteriorProbVP;

		      if(mPosteriorProbVP > bestPosteriorProb)
		      {
			   bestPosteriorProb=mPosteriorProbVP;
			   mVanishPt.V = -binV;
			   mVanishPt.H =  binH;
		      }

		   }
					
		}			
	     }
		
	     if (bestPosteriorProb == 0)
	     {
		mVanishPt.V = mVanishPt.V_prev;
		mVanishPt.H = mVanishPt.H_prev;
		return;
	     }
		
	} // Scope Ends
		
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
	{
	   /*  Transform VP to Image Coordianate System */
	   int VP_V =  mVanishPt.V + mCAMERA.FRAME_CENTER(0);
	   int VP_H =  mVanishPt.H + mCAMERA.FRAME_CENTER(1);	

	   /* Lane Bundaries */
	   Point  Start_leftLaneInner( mCAMERA.FRAME_CENTER(1) -mLaneModel.leftOffset  + delta,  mCAMERA.RES_VH(0) );
	
	   //Point  Start_leftLaneOuter(mCAMERA.FRAME_CENTER(1) -mLaneModel.leftOffset  - delta, mCAMERA.RES_VH(0) );
	
	
	   Point  Start_rightLaneInner( mCAMERA.FRAME_CENTER(1) + mLaneModel.rightOffset - delta,  mCAMERA.RES_VH(0) );
	
	   //Point  Start_rightLaneOuter(mCAMERA.FRAME_CENTER(1) + mLaneModel.rightOffset  + delta, mCAMERA.RES_VH(0) );
 
	   float slopeLeft =  (float)( VP_V-mCAMERA.RES_VH(0) ) / (VP_H- Start_leftLaneInner.x);
	   float slopeRight = (float)( VP_V-mCAMERA.RES_VH(0) ) / (VP_H- Start_rightLaneInner.x);
	
	
	   /*Point End_leftLaneOuter = Start_leftLaneOuter;
	   End_leftLaneOuter.x += - round((mCAMERA.RES_VH(0)*0.35) / slopeLeft);
	   End_leftLaneOuter.y += - round(mCAMERA.RES_VH(0)*0.35);
	   */
	
	   Point End_leftLaneInner = Start_leftLaneInner;
	   End_leftLaneInner.x 	+= -round((mCAMERA.RES_VH(0)*0.35) / slopeLeft);
	   End_leftLaneInner.y 	+= -round( (mCAMERA.RES_VH(0)*0.35));
	
	
	   Point End_rightLaneInner = Start_rightLaneInner;
	   End_rightLaneInner.x += -round ((mCAMERA.RES_VH(0)*0.35) / slopeRight);
	   End_rightLaneInner.y += -round ((mCAMERA.RES_VH(0)*0.35));

	   line(mFrameRGB,
		 Start_leftLaneInner,
		 End_leftLaneInner,
		 CvScalar(0,255,0),
		 3
	   	);
	
	   line(mFrameRGB,
		 Start_rightLaneInner,
		 End_rightLaneInner,
		 CvScalar(0,255,0),
		 3
	   	);
	    imshow( "Display window", mFrameRGB);
	    waitKey(10);

	    //mOutputVideo<<mFrameRGB;
	    
	
		


	}
		
										
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
	
	wrtLock.lock();
	if (mBufferReady == false)
	{
		
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
		
		mBufferReady = true;
	}
	
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
	mTransitLaneFilter = 	mTransitLaneFilter + 0.1*mLaneFilter->prior;


	//Predict VP States
	mVpFilter->filter.convertTo(mVpFilter->filter, CV_64F);
	boxFilter(mVpFilter->filter, mTransitVpFilter, -1, Size(3,3), Point(-1,-1), false, BORDER_REPLICATE );
    	mVpFilter->filter.convertTo(mVpFilter->filter, CV_32S);
	
	SUM = sum(mTransitVpFilter)[0];
	mTransitVpFilter= mTransitVpFilter*SCALE_FILTER;
	mTransitVpFilter.convertTo(mTransitVpFilter, CV_32S, 1.0/SUM);	
	mTransitVpFilter = mTransitVpFilter + 0.5*mVpFilter->prior;
			
	mFiltersReady   = true;
	mStartFiltering = false;
	
	wrtLock.unlock();
	_sateChange.notify_one();



//MODE C:
/*
	wrtLock.lock();
	if (mBufferReady == false)
	{
		
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
	}

	wrtLock.unlock();
*/


//MODE D:
	wrtLock.lock();
	
	wrtLock.unlock();
	

}

TrackingLanesDAG_generic::~TrackingLanesDAG_generic()
{
	

}
