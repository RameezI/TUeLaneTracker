/*
* Copyright (c) 2017 NXP Semiconductor;
* All Rights Reserved
*
* AUTHOR : Rameez Ismail
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
* ****************************************************************************/ 

#include "TrackingLaneDAG_generic.h"
#include "ScalingFactors.h"

TrackingLaneDAG_generic::TrackingLaneDAG_generic(BufferingDAG_generic&& bufferingGraph)
: 
  BufferingDAG_generic(std::move(bufferingGraph)),
  mMAX_PIXELS_ROI(mFrameGRAY_ROI.rows * mFrameGRAY_ROI.cols),
  mLikelihood_LB(0),
  mLikelihood_RB(0),
  mLikelihood_NB(0),
  mLikelihood_W(0),
  mConditionalProb(0),
  mPosterior(0),
  mMaxPosterior(0),
  mCorrelationNB(0),
  mLOWER_LIMIT_BASE(0),
  mLOWER_LIMIT_PURVIEW(0),
  mUPPER_LIMIT_BASE(0),
  mUPPER_LIMIT_PURVIEW(0),
  mSTEP_BASE_SCALED(0),
  mSTEP_PURVIEW_SCALED(0),
  mIdxPurview_LB(0),
  mIdxPurview_RB(0)
{	
	//Write Images to a video file
	//mOutputVideo.open("TUeLaneTracker.avi", CV_FOURCC('M','P','4','V'), 30, mFrameRGB.size());
}


int TrackingLaneDAG_generic::init_DAG(LaneFilter* laneFilter, VanishingPtFilter* vpFilter)
{

	mLaneFilter		 = laneFilter;
	mVpFilter		 = vpFilter;

	const size_t& lCOUNT 	 = mLaneFilter->COUNT_BINS;

	mX_ICCS_SCALED	 	 =  SCALE_INTSEC*mX_ICCS;
	mBASE_BINS_SCALED  	 =  SCALE_INTSEC*mLaneFilter->BASE_BINS;
	mPURVIEW_BINS_SCALED	 =  SCALE_INTSEC*mLaneFilter->PURVIEW_BINS;

	mSTEP_BASE_SCALED	 =  SCALE_INTSEC*mLaneFilter->BASE_STEP;
	mSTEP_PURVIEW_SCALED	 =  SCALE_INTSEC*mLaneFilter->PURVIEW_STEP;

	mLOWER_LIMIT_BASE	 =  mBASE_BINS_SCALED.at<int32_t>(0,0);
	mLOWER_LIMIT_PURVIEW  	 =  mPURVIEW_BINS_SCALED.at<int32_t>(0,0);

	mUPPER_LIMIT_BASE	 =  mBASE_BINS_SCALED.at<int32_t>(lCOUNT-1,0);
	mUPPER_LIMIT_PURVIEW  	 =  mPURVIEW_BINS_SCALED.at<int32_t>(lCOUNT-1,0);


        mHistBase      		 =  cv::Mat::zeros(lCOUNT,  1 ,  CV_32S);
        mHistPurview   		 =  cv::Mat::zeros(lCOUNT,  1 ,  CV_32S);

  	return 0;
}


void TrackingLaneDAG_generic::execute(cv::Mat FrameGRAY)
{


	BufferingDAG_generic::execute(FrameGRAY);


#ifdef PROFILER_ENABLED
mProfiler.start("SETUP_ASYNC_FILTERING");
#endif
	mFuture = std::async([this]
	{
	   int64_t lSUM;

	   WriteLock  lLock(_mutex, std::defer_lock);	

	   //Predict Lane States
	   lLock.lock();	
	    mLaneFilter->filter.convertTo(mLaneFilter->filter, CV_64F);
	    boxFilter(mLaneFilter->filter, mTransitLaneFilter, -1, cv::Size(11,11), cv::Point(-1,-1), false, cv::BORDER_REPLICATE );
    	    mLaneFilter->filter.convertTo(mLaneFilter->filter, CV_32S);

	    lSUM = sum(mTransitLaneFilter)[0];
	    mTransitLaneFilter= mTransitLaneFilter*SCALE_FILTER;
	    mTransitLaneFilter.convertTo(mTransitLaneFilter, CV_32S, 1.0/lSUM);
	    mTransitLaneFilter = 	mTransitLaneFilter + 0.2*mLaneFilter->prior;
	   lLock.unlock();


	   //Predict VP States
	   lLock.lock();	
	    mVpFilter->filter.convertTo(mVpFilter->filter, CV_64F);
	    boxFilter(mVpFilter->filter, mTransitVpFilter, -1, cv::Size(3,3), cv::Point(-1,-1), false, cv::BORDER_REPLICATE );
    	    mVpFilter->filter.convertTo(mVpFilter->filter, CV_32S);

	    lSUM = sum(mTransitVpFilter)[0];
	    mTransitVpFilter= mTransitVpFilter*SCALE_FILTER;
	    mTransitVpFilter.convertTo(mTransitVpFilter, CV_32S, 1.0/lSUM);	
	    mTransitVpFilter = mTransitVpFilter + 0.2*mVpFilter->prior;
	   lLock.unlock();
	   //Local Variables

	});

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Setting up async task for filtering LaneFilter." <<endl
				<<  "Max Time: " << mProfiler.getMaxTime("SETUP_ASYNC_FILTERING")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("SETUP_ASYNC_FILTERING")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("SETUP_ASYNC_FILTERING")<<endl
				<<"******************************"<<endl<<endl;	
				#endif



#ifdef PROFILER_ENABLED
mProfiler.start("TEMPORAL_FILTERING");
#endif	

	mProbMapFocussed = mBufferPool->Probability[0];
	mGradTanFocussed = mBufferPool->GradientTangent[0];
	
	for ( std::size_t i=1; i< mBufferPool->Probability.size(); i++ )
	{	
	    mMask = mProbMapFocussed < mBufferPool->Probability[i];
	    mBufferPool->Probability[i].copyTo(mProbMapFocussed, mMask );
	    mBufferPool->GradientTangent[i].copyTo(mGradTanFocussed, mMask );
	}
	
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Temporal Filtering [Max-Pooling]." <<endl
				<<  "Max Time: " << mProfiler.getMaxTime("TEMPORAL_FILTERING")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("TEMPORAL_FILTERING")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("TEMPORAL_FILTERING")<<endl
				<<"******************************"<<endl<<endl;	
				#endif







#ifdef PROFILER_ENABLED
mProfiler.start("COMPUTE_INTERSECTIONS");
#endif	
	//Base Intersections
	subtract(-mLaneFilter->BASE_LINE_ICCS, -mY_ICCS, mIntBase, cv::noArray(), CV_32S);
	divide(mIntBase, mGradTanFocussed, mIntBase, SCALE_INTSEC_TAN, CV_32S);
	add(mIntBase, mX_ICCS_SCALED, mIntBase);
	
	//Purview Intersections
	subtract(-mLaneFilter->PURVIEW_LINE_ICCS, -mY_ICCS, mIntPurview, cv::noArray(), CV_32S);
	divide(mIntPurview,mGradTanFocussed, mIntPurview, SCALE_INTSEC_TAN, CV_32S);
	add(mIntPurview, mX_ICCS_SCALED, mIntPurview);

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Compute Intersections with Bottom and Horizon." <<endl
				<<  "Max Time: " << mProfiler.getMaxTime("COMPUTE_INTERSECTIONS")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("COMPUTE_INTERSECTIONS")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("COMPUTE_INTERSECTIONS")<<endl
				<<"******************************"<<endl<<endl;	
				#endif

		




#ifdef PROFILER_ENABLED
mProfiler.start("MASK_INVALID_BIN_IDS");
#endif
     {
	//Build Mask for Valid Intersections
	bitwise_and(mProbMapFocussed > 0, mGradTanFocussed !=0, mMask);

	bitwise_and(mMask, mIntBase    >  mLOWER_LIMIT_BASE,    mMask);
	bitwise_and(mMask, mIntPurview >  mLOWER_LIMIT_PURVIEW, mMask);

    	bitwise_and(mMask, mIntBase    <  mUPPER_LIMIT_BASE, 	mMask);
    	bitwise_and(mMask, mIntPurview <  mUPPER_LIMIT_PURVIEW, mMask);


	//^TODO: Put on the side thread
        mHistBase      = cv::Mat::zeros(mLaneFilter->COUNT_BINS,  1 ,  CV_32S);
        mHistPurview   = cv::Mat::zeros(mLaneFilter->COUNT_BINS,  1 ,  CV_32S);
     }		

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Extract Valid Intersection Bin IDs." <<endl
				<<  "Max Time: " << mProfiler.getMaxTime("MASK_INVALID_BIN_IDS")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("MASK_INVALID_BIN_IDS")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("MASK_INVALID_BIN_IDS")<<endl
				<<"******************************"<<endl<<endl;	
				#endif 





#ifdef PROFILER_ENABLED
mProfiler.start("COMPUTE_HISTOGRAMS");
#endif
	//Weights of Intersections
	multiply(mDepthTemplate, mProbMapFocussed, mIntWeights, 1, CV_32S);	
	{
	   int32_t* 	lPtrIntBase 	    	= mIntBase.ptr<int32_t>(0);
	   int32_t* 	lPtrIntPurview   	= mIntPurview.ptr<int32_t>(0);
	   int32_t* 	lPtrWeights   		= mIntWeights.ptr<int32_t>(0);
	   uint8_t* 	lPtrMask   		= mMask.ptr<uint8_t>(0);

	   int32_t* 	lPtrHistBase    	=  mHistBase.ptr<int32_t>(0);
	   int32_t* 	lPtrHistPurview 	=  mHistPurview.ptr<int32_t>(0);

	   uint16_t   	lBaseBinIdx;
	   uint16_t   	lPurviewBinIdx;
	   int32_t    	lWeightBin;

	   for (int i = 0; i < mMAX_PIXELS_ROI; i++,lPtrIntBase++,lPtrIntPurview++, lPtrWeights++ , lPtrMask++)
	   {
	      if(!(*lPtrMask ==0) )
	      {		
		 lBaseBinIdx	= (*lPtrIntBase    - mLOWER_LIMIT_BASE    + (mSTEP_BASE_SCALED/2))/mSTEP_BASE_SCALED;

		 lPurviewBinIdx	= (*lPtrIntPurview - mLOWER_LIMIT_PURVIEW + (mSTEP_PURVIEW_SCALED/2))/mSTEP_PURVIEW_SCALED;
		
	         lWeightBin 	= *lPtrWeights;
		
	         *(lPtrHistBase     + lBaseBinIdx   )  	+= lWeightBin;
	         *(lPtrHistPurview  + lPurviewBinIdx) 	+= lWeightBin;
	      }	
	   }
		
	}//Block Ends

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Compute Weighted Histograms." <<endl
				<<  "Max Time: " << mProfiler.getMaxTime("COMPUTE_HISTOGRAMS")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("COMPUTE_HISTOGRAMS")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("COMPUTE_HISTOGRAMS")<<endl
				<<"******************************"<<endl<<endl;	
				#endif




#ifdef PROFILER_ENABLED
mProfiler.start("FILTERS_WAIT");
#endif 				
	mFuture.wait();

#ifdef PROFILER_ENABLED
 mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Waiting for worker thread to finish transition filters." <<endl
				<<  "Max Time: " << mProfiler.getMaxTime("FILTERS_WAIT")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("FILTERS_WAIT")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("FILTERS_WAIT")<<endl
				<<"******************************"<<endl<<endl;	
				#endif	




#ifdef PROFILER_ENABLED
mProfiler.start("SETUP_ASYNC_BUFFER_SHIFT");
#endif
	mFuture = std::async([this]
	{
	   WriteLock  lLock(_mutex, std::defer_lock);	

	   for ( std::size_t i = 0; i< mBufferPool->Probability.size()-1 ; i++ )
	   {
	 	mBufferPool->Probability[i+1].copyTo(mBufferPool->Probability[i]);		
		mBufferPool->GradientTangent[i+1].copyTo(mBufferPool->GradientTangent[i]);
	   }	

	   lLock.unlock();

	});

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Setting up async task for shifting Buffers." <<endl
				<<  "Max Time: " << mProfiler.getMaxTime("SETUP_ASYNC_BUFFER_SHIFT")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("SETUP_ASYNC_BUFFER_SHIFT")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("SETUP_ASYNC_BUFFER_SHIFT")<<endl
				<<"******************************"<<endl<<endl;	
				#endif




#ifdef PROFILER_ENABLED
mProfiler.start("NORMALIZE_HISTOGRAM");
#endif
	{   
   	    int64_t lSUM = 0;

	    //Normalising Base Histogram
	    lSUM = sum(mHistBase)[0];
	    mHistBase.convertTo(mHistBase, CV_64F, SCALE_FILTER);
	    mHistBase.convertTo(mHistBase, CV_32S, 1.0/lSUM );	 

	    //Normalising Purview Histogram
	    lSUM = sum(mHistPurview)[0];
	    mHistPurview.convertTo(mHistPurview, CV_64F, SCALE_FILTER);
	    mHistPurview.convertTo(mHistPurview, CV_32S, 1.0/lSUM );
	}
	
#ifdef PROFILER_ENABLED
 mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Normalising Hitograms." <<endl
				<<  "Max Time: " << mProfiler.getMaxTime("NORMALIZE_HISTOGRAM")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("NORMALIZE_HISTOGRAM")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("NORMALIZE_HISTOGRAM")<<endl
				<<"******************************"<<endl<<endl;	
				#endif	





#ifdef PROFILER_ENABLED
mProfiler.start("HISTOGRAM_MATCHING");
#endif
	{
	    int32_t   	lBestModelIdx = -1;
	    int32_t* 	lPtrHistBase  = nullptr; 
	    cv::Mat    	lRange;

	    mPosterior		= 0;
	    mMaxPosterior  	= 0;

	    vector<BaseHistogramModel>& Models	= mLaneFilter->baseHistogramModels;
	    lPtrHistBase			=  mHistBase.ptr<int32_t>(0);
	
	   for(size_t i=0; i< Models.size(); i++)
	   {    
			
		int& lIdx_LB    	 = Models[i].binIdxBoundary_left;
		int& lIdx_RB		 = Models[i].binIdxBoundary_right;

		int& lIdx_NLB	 	 = Models[i].binIdxNonBoundary_left;
		int& lIdx_NRB    	 = Models[i].binIdxNonBoundary_right;

		int& lCount_NLB          = Models[i].nonBoundaryBinsCount_left;
		int& lCount_NRB		 = Models[i].nonBoundaryBinsCount_right;

		mLikelihood_LB  	=  round(lPtrHistBase[lIdx_LB-1]*0.25
					   +lPtrHistBase[lIdx_LB]
					   +lPtrHistBase[lIdx_LB+1]*0.25);
										 
		mLikelihood_RB 		=  round(lPtrHistBase[lIdx_RB-1]*0.25
					   +lPtrHistBase[lIdx_RB]
					   +lPtrHistBase[lIdx_RB+1]*0.25);
	
		mConditionalProb  	=  (mLikelihood_LB *mLikelihood_RB)/(float)SCALE_FILTER;


		//TODO:start=> Put this block on the side thread
		mCorrelationNB	= 0;
		lRange = mHistBase(cv::Range(lIdx_NLB,  lIdx_NLB  +  lCount_NLB), cv::Range::all());
		mCorrelationNB +=sum(lRange)[0];
		
	    	lRange = mHistBase(cv::Range(lIdx_NRB, lIdx_NRB + lCount_NRB), cv::Range::all());
		mCorrelationNB +=sum(lRange)[0];
		
		float x= (float)mCorrelationNB / SCALE_FILTER ; 
		mLikelihood_NB = mLaneMembership.NEG_BOUNDARY_NORMA*exp(-pow(x,2)/mLaneMembership.NEG_BOUNDARY_NOMIN );
		//(end)

		mConditionalProb  = (mConditionalProb * mLikelihood_NB); 
	
		size_t lRowIdx = Models[i].rowIdxFilter; 
		size_t lColIdx = Models[i].colIdxFilter; 
	    
		mPosterior = round( mConditionalProb*mTransitLaneFilter.at<int32_t>(lRowIdx, lColIdx) );
		mLaneFilter->filter.at<int32_t>(lRowIdx, lColIdx) = mPosterior;

		//Keep track of the best Model
		if(mPosterior > mMaxPosterior)
		{
		   mMaxPosterior = mPosterior; 
		   lBestModelIdx = i;
		}
				
	   } //Loop over all BaseHistogram Models
		
	   if (mMaxPosterior == 0)
	   {
	      return;
	   }
	   else
	   {
	      mBaseHistModel = Models[lBestModelIdx];
	   }

	}//Scope End

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Histogram Matching MAP Estimate LaneBoundaries." <<endl
				<<  "Max Time: " << mProfiler.getMaxTime("HISTOGRAM_MATCHING")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("HISTOGRAM_MATCHING")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("HISTOGRAM_MATCHING")<<endl
				<<"******************************"<<endl<<endl;	
				#endif




#ifdef PROFILER_ENABLED
mProfiler.start("VP_HISTOGRAM_MATCHING");
#endif
	{
	   const int& 	lBaseLB  		= mBaseHistModel.boundary_left;	
	   const int& 	lBaseRB 		= mBaseHistModel.boundary_right;
	   const int& 	lBaseLine		= mLaneFilter->BASE_LINE_ICCS;
	   const int& 	lPurviewLine		= mLaneFilter->PURVIEW_LINE_ICCS;
	
	   int32_t	lIntSecLeft, lIntSecRight, lWidth_cm;
	   int32_t	lIdx_LB, lIdx_RB;

	   size_t  	lIdx_Mid, lIdx_NLB, lIdx_NRB, lCount_NLB, lCount_NRB;

	   cv::Mat   	lRange;

	   mPosterior	 = 0;	
	   mMaxPosterior = 0;

	   // Save current values of VP 
	   mVanishPt.V_prev = mVanishPt.V;
	   mVanishPt.H_prev = mVanishPt.H;
	
	   int32_t* 	lPtrHistPurview  = mHistPurview.ptr<int32_t>(0);

	   for(int v=0; v < mVpFilter->COUNT_BINS_V;v++)
	   {	for(int h=0; h < mVpFilter->COUNT_BINS_H; h++)
		{
		   const int&   binV 	= mVpFilter->BINS_V(v);
		   const int&   binH 	= mVpFilter->BINS_H(h);
		   const auto&  lSTEP	= mSTEP_PURVIEW_SCALED;
		
		   lIntSecLeft  = SCALE_INTSEC*( ((binH - lBaseLB)/(float)(binV - lBaseLine))*(lPurviewLine - binV) +binH );
		   lIntSecRight = SCALE_INTSEC*( ((binH - lBaseRB)/(float)(binV - lBaseLine))*(lPurviewLine - binV) +binH );

		   lIdx_LB 	= ( lIntSecLeft  - mLOWER_LIMIT_PURVIEW + (lSTEP/2))/lSTEP;
		   lIdx_RB 	= ( lIntSecRight - mLOWER_LIMIT_PURVIEW + (lSTEP/2))/lSTEP;

		   lIdx_Mid  	= round((lIdx_LB+ lIdx_RB)/2.0 );

		   //^TODO:start=> Make non-boundary region dependent on the binwidth
		   lIdx_NLB   	= lIdx_LB   + 2;
		   lIdx_NRB 	= lIdx_Mid  + 4;	
		   lCount_NLB   = (lIdx_Mid - 3) - (lIdx_LB  + 2) ;        
		   lCount_NRB   = (lIdx_RB  - 2) - (lIdx_Mid + 3) ;
		   //(TODO:end)

					   
		   if (lIdx_LB > 0 && lIdx_RB < (int32_t) (mLaneFilter->COUNT_BINS -2) )
		   {			

		      lWidth_cm= mLaneFilter->BINS_cm(lIdx_RB) - mLaneFilter->BINS_cm(lIdx_LB);

		      // VP Probability over lane width difference between base and purview line
		      mLikelihood_W  	= mLaneMembership.WIDTH_DIFF_NORMA;
		      mLikelihood_W    *= exp(-pow(mBaseHistModel.width_cm - lWidth_cm, 2) / mLaneMembership.WIDTH_DIFF_NOMIN );
		
		      // VP Probability over left lane boundary
		      mLikelihood_LB 	=  round(lPtrHistPurview[lIdx_LB-1]*0.25
					      + lPtrHistPurview[lIdx_LB]
				  	      + lPtrHistPurview[lIdx_LB+1]*0.25);
												  
		      // VP Probability over Right LaneBoundary 				 
		      mLikelihood_RB 	= round(lPtrHistPurview[lIdx_RB-1]*0.25
					      + lPtrHistPurview[lIdx_RB]
					      + lPtrHistPurview[lIdx_RB+1]*0.25);
					
		      mConditionalProb  =  (mLikelihood_LB*mLikelihood_RB) /(float)SCALE_FILTER;
		
		      //VP Probability over Non-Boundary Region
		      mCorrelationNB = 0;
		      lRange 		= mHistPurview(cv::Range(lIdx_NLB, lIdx_NLB + lCount_NLB), cv::Range::all());
		      mCorrelationNB   +=sum(lRange)[0];
			
		      lRange 		= mHistPurview(cv::Range(lIdx_NRB, lIdx_NRB + lCount_NRB), cv::Range::all());
		      mCorrelationNB   +=sum(lRange)[0];
		
		      mLikelihood_NB	= mLaneMembership.NEG_BOUNDARY_NORMA;
		      mLikelihood_NB   *= exp(-pow( mCorrelationNB/(float)SCALE_FILTER , 2)/mLaneMembership.NEG_BOUNDARY_NOMIN);
											 				 
		      //Conditional Probability			 
		      mConditionalProb  =   mConditionalProb* mLikelihood_NB * mLikelihood_W;
					
		      mPosterior 	= round(mConditionalProb * mTransitVpFilter.at<int32_t>(v ,h));
			
		      mVpFilter->filter.at<int32_t>(v, h) = mPosterior;

		      if(mPosterior > mMaxPosterior)
		      {
			mMaxPosterior	= mPosterior;
			mVanishPt.V 	= binV;
			mVanishPt.H 	= binH;
  			mIdxPurview_LB  = lIdx_LB;
  			mIdxPurview_RB  = lIdx_RB;
		      }//end, if posterior is greater than existing Max

		   } //end, if Intersections are not in the BIN-Range 
				
		 }//for-end			
	     }//for-end
		
	     if (mMaxPosterior == 0)
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
				<<  "Histogram Matching MAP Estimate VanishingPt."<<endl
				<<  "Max Time: " << mProfiler.getMaxTime("VP_HISTOGRAM_MATCHING")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("VP_HISTOGRAM_MATCHING")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("VP_HISTOGRAM_MATCHING")<<endl
		  		<<"******************************"<<endl<<endl;	
				#endif




#ifdef PROFILER_ENABLED
mProfiler.start("ASSIGN_LANE_MODEL");
#endif
	{
	   const auto& lBINS_cm   = mLaneFilter->BINS_cm;
	   const auto& lBASE_LB	  = mBaseHistModel.boundary_left;
	   const auto& lBASE_RB	  = mBaseHistModel.boundary_right;
	   float lLookAheadErr    = (lBINS_cm(mIdxPurview_LB) + lBINS_cm(mIdxPurview_RB))/2.0;

	   mLaneModel.setModel(lBASE_LB, lBASE_RB, mVanishPt, lLookAheadErr/100.0 );
	}
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
				<<"******************************"<<endl
				<<  "Histogram Matching MAP Estimate VanishingPt."<<endl
				<<  "Max Time: " << mProfiler.getMaxTime("ASSIGN_LANE_MODEL")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("ASSIGN_LANE_MODEL")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("ASSIGN_LANE_MODEL")<<endl
		  		<<"******************************"<<endl<<endl;	
				#endif


#ifdef PROFILER_ENABLED
mProfiler.start("BUFFER_SHIFT_WAIT");
#endif 				
	mFuture.wait();

#ifdef PROFILER_ENABLED
 mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				<<"******************************"<<endl
				<<  "Waiting for async task to finish shifting buffers." <<endl
				<<  "Max Time: " << mProfiler.getMaxTime("BUFFER_SHIFT_WAIT")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("BUFFER_SHIFT_WAIT")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("BUFFER_SHIFT_WAIT")<<endl
				<<"******************************"<<endl<<endl;	
				#endif	


}//extractLanes

