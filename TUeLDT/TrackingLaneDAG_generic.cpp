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
  mInvCorrelationNB(0),
  mLOWER_LIMIT_BASE(0),
  mLOWER_LIMIT_PURVIEW(0),
  mUPPER_LIMIT_BASE(0),
  mUPPER_LIMIT_PURVIEW(0),
  mSTEP_BASE_SCALED(0),
  mSTEP_PURVIEW_SCALED(0),
  mIdxPurview_LB(0),
  mIdxPurview_RB(0)
{	

}


int TrackingLaneDAG_generic::init_DAG(LaneFilter* laneFilter, VanishingPtFilter* vpFilter)
{

	mLaneFilter		= laneFilter;
	mVpFilter		= vpFilter;

	mPtrLaneModel.reset(new LaneModel([=]()->vector<float>
					 {
						vector<float> lookAheadPts;
						lookAheadPts.push_back(laneFilter->BASE_LINE_cm);
						lookAheadPts.push_back(laneFilter->PURVIEW_LINE_cm);
						return lookAheadPts;
					 }()));


        mX_ICCS   		= mX_ICS + mCAMERA.O_ICS_ICCS.x;
        mY_ICCS   		= mY_ICS + mCAMERA.O_ICS_ICCS.y;

	const size_t& lCOUNT    = mLaneFilter->COUNT_BINS;

	mX_ICCS_SCALED	 	=  SCALE_INTSEC*mX_ICCS;
	mBASE_BINS_SCALED  	=  SCALE_INTSEC*mLaneFilter->BASE_BINS;
	mPURVIEW_BINS_SCALED	=  SCALE_INTSEC*mLaneFilter->PURVIEW_BINS;

	mSTEP_BASE_SCALED	=  SCALE_INTSEC*mLaneFilter->BASE_STEP;
	mSTEP_PURVIEW_SCALED	=  SCALE_INTSEC*mLaneFilter->PURVIEW_STEP;

	mLOWER_LIMIT_BASE	=  mBASE_BINS_SCALED.at<int32_t>(0,0);
	mLOWER_LIMIT_PURVIEW  	=  mPURVIEW_BINS_SCALED.at<int32_t>(0,0);

	mUPPER_LIMIT_BASE	=  mBASE_BINS_SCALED.at<int32_t>(lCOUNT-1,0);
	mUPPER_LIMIT_PURVIEW  	=  mPURVIEW_BINS_SCALED.at<int32_t>(lCOUNT-1,0);


        mHistBase      		 =  cv::Mat::zeros(lCOUNT,  1 ,  CV_32S);
        mHistPurview   		 =  cv::Mat::zeros(lCOUNT,  1 ,  CV_32S);

  	return 0;
}


void TrackingLaneDAG_generic::execute(cv::UMat& FrameGRAY)
{

	BufferingDAG_generic::execute(FrameGRAY);



#ifdef PROFILER_ENABLED
mProfiler.start("SETUP_ASYNC_FILTERING");
#endif
	mFuture = std::async(std::launch::async, [this]
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
	    mTransitLaneFilter = 	mTransitLaneFilter + 0.1*mLaneFilter->prior;


	   //Predict VP States
	    mVpFilter->filter.convertTo(mVpFilter->filter, CV_64F);
	    boxFilter(mVpFilter->filter, mTransitVpFilter, -1, cv::Size(3,3), cv::Point(-1,-1), false, cv::BORDER_REPLICATE );
    	    mVpFilter->filter.convertTo(mVpFilter->filter, CV_32S);

	    lSUM = sum(mTransitVpFilter)[0];
	    mTransitVpFilter= mTransitVpFilter*SCALE_FILTER;
	    mTransitVpFilter.convertTo(mTransitVpFilter, CV_32S, 1.0/lSUM);	
	    mTransitVpFilter = mTransitVpFilter + 0.1*mVpFilter->prior;

	   lLock.unlock();

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

      #ifdef TEST_APEX_CODE
        mPROB_FRAME_0           = mBufferPool->Probability[0].clone();          // copy, otherwise overwritten by shift operation.
        mGRAD_FRAME_0           = mBufferPool->GradientTangent[0].clone();      // copy, otherwise overwritten by shift operation.
        mProbMapFocussed        = mBufferPool->Probability[0].clone();          // copy otherwise no test point for temporal pooling.
        mGradTanFocussed        = mBufferPool->GradientTangent[0].clone();      // copy otherwise no test point for temporal pooling.

      #else
        mProbMapFocussed        = mBufferPool->Probability[0];
        mGradTanFocussed        = mBufferPool->GradientTangent[0];
      #endif


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
	//Note: Gradients are not computed in the Image-center-cs. slope is positive in upward direction.
	//Therefore invert the mY_ICCS by subtracting the BASE_LINE
	//Weights of the intersections above the vanishingPt are already set to zero.

	//Base Intersections
	subtract(mY_ICCS, mLaneFilter->BASE_LINE_ICCS, mIntBase, cv::noArray(), CV_32S);
	divide(mIntBase, mGradTanFocussed, mIntBase, SCALE_INTSEC_TAN, CV_32S);
	add(mIntBase, mX_ICCS_SCALED, mIntBase);
	
	//Purview Intersections
	subtract(mY_ICCS, mLaneFilter->PURVIEW_LINE_ICCS, mIntPurview, cv::noArray(), CV_32S);
	divide(mIntPurview,mGradTanFocussed, mIntPurview, SCALE_INTSEC_TAN, CV_32S);
	add(mIntPurview, mX_ICCS_SCALED, mIntPurview);


	bitwise_and(mBufferPool->Probability[mBufferPos], mFocusTemplate, mBufferPool->Probability[mBufferPos]);


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
        mMask =  mIntBase    >  mLOWER_LIMIT_BASE;
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
	   int32_t* 	lPtrIntBase 	    = mIntBase.ptr<int32_t>(0);
	   int32_t* 	lPtrIntPurview      = mIntPurview.ptr<int32_t>(0);
	   int32_t* 	lPtrWeights   	    = mIntWeights.ptr<int32_t>(0);
	   uint8_t* 	lPtrMask            = mMask.ptr<uint8_t>(0);

	   int32_t* 	lPtrHistBase        =  mHistBase.ptr<int32_t>(0);
	   int32_t* 	lPtrHistPurview     =  mHistPurview.ptr<int32_t>(0);

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

		 assert((0<=lBaseBinIdx)&&(lBaseBinIdx<mHistBase.rows ));

		 *(lPtrHistBase       + lBaseBinIdx   )  	+= lWeightBin;
	         *(lPtrHistPurview    + lPurviewBinIdx)         += lWeightBin;
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
	mFuture = std::async(std::launch::async, [this]
	{
	   WriteLock  lLock(_mutex, std::defer_lock);	

	   lLock.lock();
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
	    lSUM = sum(mHistBase)[0] ;
	    mHistBase.convertTo(mHistBase_CV64F, CV_64F, SCALE_FILTER);
	    mHistBase_CV64F.convertTo(mHistBase, CV_32S, 1.0/lSUM );	 

	    //Normalising Purview Histogram
	    lSUM = sum(mHistPurview)[0];
	    mHistPurview.convertTo(mHistPurview_CV64F, CV_64F, SCALE_FILTER);
	    mHistPurview_CV64F.convertTo(mHistPurview, CV_32S, 1.0/lSUM );
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
		mInvCorrelationNB	= 0;
		lRange = mHistBase(cv::Range(lIdx_NLB,  lIdx_NLB  +  lCount_NLB ), cv::Range::all());
		mInvCorrelationNB +=sum(lRange)[0];
		
	    	lRange = mHistBase(cv::Range(lIdx_NRB, lIdx_NRB + lCount_NRB), cv::Range::all());
		mInvCorrelationNB +=sum(lRange)[0];
		
		float x= (float)mInvCorrelationNB / SCALE_FILTER ;
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
		
	   if (mMaxPosterior != 0)
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
	   const int& 	  lBASE_LB  		= mBaseHistModel.boundary_left;	
	   const int& 	  lBASE_RB 		= mBaseHistModel.boundary_right;
	   const int& 	  lBASE_Y		= mLaneFilter->BASE_LINE_ICCS;
	   const int& 	  lPURV_Y		= mLaneFilter->PURVIEW_LINE_ICCS;
		
	   const float&	  lSTEP_cm		= mLaneFilter->BINS_STEP_cm;
	   const float&   lMAX_WIDTH		= mLaneFilter->LANE.MAX_WIDTH;
	   const float&	  lMIN_WIDTH		= mLaneFilter->LANE.MIN_WIDTH;	
	   const float&   lWIDTH_MARK		= mLaneFilter->LANE.AVG_WIDTH_LM;
	
	   int32_t	  lIntSecLeft, lIntSecRight, lWidth_cm;
	   int32_t	  lIdx_LB, lIdx_RB;
	   int32_t  	  lIdx_Mid, lIdx_NLB, lIdx_NRB, lCount_NLB, lCount_NRB;
	   
	   cv::Mat   	  lRange;

	   const size_t   lIDX_MARK = floor(lWIDTH_MARK/lSTEP_cm) ;


	   mPosterior	 = 0;	
	   mMaxPosterior = 0;

	   // Save current values of VP 
	   mVanishPt.V_prev = mVanishPt.V;
	   mVanishPt.H_prev = mVanishPt.H;
	
	   int32_t* 	lPtrHistPurview  = mHistPurview.ptr<int32_t>(0);

	   for(int v=0; v < mVpFilter->COUNT_BINS_V;v++)
	   {	for(int h=0; h < mVpFilter->COUNT_BINS_H; h++)
		{

		   const int&   lBIN_V 	= mVpFilter->BINS_V(v);
		   const int&   lBIN_H 	= mVpFilter->BINS_H(h);
		   const auto&  lSTEP	= mSTEP_PURVIEW_SCALED;
		
		   // x=1/m*(y-c) + x0
		   lIntSecLeft  = SCALE_INTSEC*( ((lBIN_H - lBASE_LB)/(float)(lBIN_V - lBASE_Y))*(lPURV_Y - lBIN_V) +lBIN_H );
		   lIntSecRight = SCALE_INTSEC*( ((lBIN_H - lBASE_RB)/(float)(lBIN_V - lBASE_Y))*(lPURV_Y - lBIN_V) +lBIN_H );

		   lIdx_LB 	= ( lIntSecLeft  - mLOWER_LIMIT_PURVIEW + (lSTEP/2) )/lSTEP;
		   lIdx_RB 	= ( lIntSecRight - mLOWER_LIMIT_PURVIEW + (lSTEP/2) )/lSTEP;

	   	   lIdx_Mid    = round((lIdx_LB + lIdx_RB)/2.0 );
	   	   lCount_NLB  = (lIdx_Mid - lIDX_MARK) - (lIdx_LB  + lIDX_MARK) -1;
	   	   lCount_NRB  = (lIdx_RB  - lIDX_MARK) - (lIdx_Mid + lIDX_MARK) -1;

					   
		   if ( (lIdx_LB > 0) &&  (lIdx_RB < (int32_t)(mLaneFilter->COUNT_BINS -1) ) && (lCount_NLB>0) && (lCount_NRB>0) )
		   {			

		      lWidth_cm= mLaneFilter->BINS_cm(lIdx_RB) - mLaneFilter->BINS_cm(lIdx_LB);

			/*
		   	{

	   		 int 	  	lPURV_LB  		= mLaneFilter->PURVIEW_BINS.at<int32_t>(lIdx_LB,0);	
	   		 int 	  	lPURV_RB 		= mLaneFilter->PURVIEW_BINS.at<int32_t>(lIdx_RB,0);

			  cv::Mat Frame;
			  cv::cvtColor(FrameGRAY, Frame, CV_GRAY2BGR);

		    	  size_t lCOUNT_BINS 		= mLaneFilter->COUNT_BINS;
			  int    lBASE_LINE_ICS		= mLaneFilter->BASE_LINE_ICCS + mLaneFilter->O_ICCS_ICS.y;
			  int    lPURVIEW_LINE_ICS	= mLaneFilter->PURVIEW_LINE_ICCS + mLaneFilter->O_ICCS_ICS.y;
		    
		    	  // Draw Histogram-Bins at the Base
		    	  for (size_t i=0; i < lCOUNT_BINS; i++)
		    	  {
			    int x =  mLaneFilter->BASE_BINS.at<int32_t>(i,0) + mLaneFilter->O_ICCS_ICS.x ;

			    if ((x== lBASE_LB + mLaneFilter->O_ICCS_ICS.x) | (x== lBASE_RB + mLaneFilter->O_ICCS_ICS.x))
			    {
			    	cv::line(Frame, cv::Point(x, lBASE_LINE_ICS), cv::Point(x,lBASE_LINE_ICS - 60), 
				     cv::Scalar(0,0,255), 1);
			    }
			    else
			    {
			    	cv::line(Frame, cv::Point(x, lBASE_LINE_ICS), cv::Point(x,lBASE_LINE_ICS - 30),
				cv::Scalar(0,0,0), 1);
			    }
		    	  }

		    	  // Draw Histogram-Bins at the Purview
		    	  for (size_t i=0; i < lCOUNT_BINS; i++)
		    	  {
			    	int x; 

				x = mLaneFilter->PURVIEW_BINS.at<int32_t>(i,0) + mLaneFilter->O_ICCS_ICS.x;

			    	 cv::line(Frame, cv::Point(x, lPURVIEW_LINE_ICS), cv::Point(x, lPURVIEW_LINE_ICS - 30),
				 cvScalar(0,0,0), 1);

				 x= lPURV_LB + mLaneFilter->O_ICCS_ICS.x;

			    	 cv::line(Frame, cv::Point(x, lPURVIEW_LINE_ICS), cv::Point(x, lPURVIEW_LINE_ICS - 60),
				 cvScalar(0,0,255), 1);


				 x= lPURV_RB + mLaneFilter->O_ICCS_ICS.x;

			    	 cv::line(Frame, cv::Point(x, lPURVIEW_LINE_ICS), cv::Point(x, lPURVIEW_LINE_ICS - 60),
				 cvScalar(0,0,255), 1);
		    	  }


		    	  // Draw lines to Vanishing Point
		    	  for (size_t i=0; i < lCOUNT_BINS; i++)
		    	  {
			       int  x		=  lBASE_LB + mLaneFilter->O_ICCS_ICS.x ;
			       auto VPt_ICS	=  cv::Point(lBIN_H + mLaneFilter->O_ICCS_ICS.x, lBIN_V + mLaneFilter->O_ICCS_ICS.y);

			       cv::line(Frame, cv::Point(x, lBASE_LINE_ICS), VPt_ICS, cvScalar(0,255,0), 2);

			       x =  lBASE_RB + mLaneFilter->O_ICCS_ICS.x ;
			       cv::line(Frame, cv::Point(x, lBASE_LINE_ICS), VPt_ICS, cvScalar(0,255,0), 2);
		    	  }

     		    	  cv::imshow( "Test VanishgingPt", Frame);
     		    	  while(cv::waitKey(10) != 32);

		          cout<< "Width_cm at Base " <<mBaseHistModel.width_cm<<" 	Width_cm at Purview"<<lWidth_cm<<endl;
		   	}*/

		      if(lMIN_WIDTH< lWidth_cm && lWidth_cm < lMAX_WIDTH)
		      {

		   	 lIdx_NLB    = lIdx_LB   + lIDX_MARK;
		   	 lIdx_NRB    = lIdx_RB   - lCount_NRB;	

		         // VP Probability over lane width difference between base and purview line
		         mLikelihood_W     = mLaneMembership.WIDTH_DIFF_NORMA;
		         mLikelihood_W    *= exp(-pow(mBaseHistModel.width_cm - lWidth_cm, 2) / mLaneMembership.WIDTH_DIFF_NOMIN );
		
		         // VP Probability over left lane boundary
		         mLikelihood_LB       =  round(lPtrHistPurview[lIdx_LB-1]*0.25
					      + lPtrHistPurview[lIdx_LB]
				  	      + lPtrHistPurview[lIdx_LB+1]*0.25);
												  
		         // VP Probability over Right LaneBoundary 				 
		         mLikelihood_RB       = round(lPtrHistPurview[lIdx_RB-1]*0.25
					      + lPtrHistPurview[lIdx_RB]
					      + lPtrHistPurview[lIdx_RB+1]*0.25);
					
		     	 mConditionalProb     =  (mLikelihood_LB*mLikelihood_RB) /(float)SCALE_FILTER;
		
		      	 //VP Probability over Non-Boundary Region
		      	 mInvCorrelationNB   = 0;
		      	 lRange 	  = mHistPurview(cv::Range(lIdx_NLB, lIdx_NLB + lCount_NLB), cv::Range::all());
		      	 mInvCorrelationNB   +=sum(lRange)[0];
			
		         lRange 	  = mHistPurview(cv::Range(lIdx_NRB, lIdx_NRB + lCount_NRB), cv::Range::all());
		         mInvCorrelationNB   +=sum(lRange)[0];
		
		         mLikelihood_NB	   = mLaneMembership.NEG_BOUNDARY_NORMA;
		         mLikelihood_NB   *= exp(-pow( mInvCorrelationNB/(float)SCALE_FILTER , 2)/mLaneMembership.NEG_BOUNDARY_NOMIN);
											 				 
		         //Conditional Probability			 
		         mConditionalProb  = mConditionalProb* mLikelihood_NB * mLikelihood_W;		
		         mPosterior 	   = round(mConditionalProb * mTransitVpFilter.at<int32_t>(v ,h));
			
		         mVpFilter->filter.at<int32_t>(v, h) = mPosterior;

		         if(mPosterior > mMaxPosterior)
		         {
			    mMaxPosterior	= mPosterior;
			    mVanishPt.V 	= lBIN_V;
			    mVanishPt.H 	= lBIN_H;
  			    mIdxPurview_LB  	= lIdx_LB;
  			    mIdxPurview_RB  	= lIdx_RB;
		         }//end, if posterior is greater than existing Max

		      }//end, if Lane Width is in Range
		   } //end, if Intersections are not in the BIN-Range 
				
		 }//for-end			
	     }//for-end
		
	     if (mMaxPosterior == 0)
	     {
		mVanishPt.V = mVanishPt.V_prev;
		mVanishPt.H = mVanishPt.H_prev;
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

	   const int&  lIdxBase_LB 	= mBaseHistModel.binIdxBoundary_left;	
	   const int&  lIdxBase_RB 	= mBaseHistModel.binIdxBoundary_right;

	   const auto& lPURV_LB	  = mLaneFilter->PURVIEW_BINS.at<int32_t>(mIdxPurview_LB, 0);
	   const auto& lPURV_RB	  = mLaneFilter->PURVIEW_BINS.at<int32_t>(mIdxPurview_RB, 0);

	   float lLaneWidth	  = (lBINS_cm(mIdxPurview_RB) - lBINS_cm(mIdxPurview_LB));


	   //Set LaneModel
	   mPtrLaneModel->boundaryLeft[0] 	  = lBASE_LB;
	   mPtrLaneModel->boundaryLeft[1] 	  = lPURV_LB;

	   mPtrLaneModel->boundaryRight[0] 	  = lBASE_RB;
	   mPtrLaneModel->boundaryRight[1] 	  = lPURV_RB;

	   mPtrLaneModel->boundaryLeft_cm[0]  	  = lBINS_cm(lIdxBase_LB);
	   mPtrLaneModel->boundaryLeft_cm[1]  	  = lBINS_cm(mIdxPurview_LB);

	   mPtrLaneModel->boundaryRight_cm[0] 	  = lBINS_cm(lIdxBase_RB);
	   mPtrLaneModel->boundaryRight_cm[1] 	  = lBINS_cm(mIdxPurview_RB);

	   mPtrLaneModel->width_cm		  = lLaneWidth;
	   mPtrLaneModel->vanishingPt	  	  = mVanishPt;
	}
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
				<<"******************************"<<endl
				<<  "Assigning Lane Model."<<endl
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

