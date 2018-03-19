/******************************************************************************
* NXP Confidential Proprietary
* 
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
  mStartBufferShift(false),
  mStartFiltering(false),
  mFiltersReady(false),
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
  mSTEP_BASE(0),
  mSTEP_PURVIEW(0)
{	
	//Write Images to a video file
	//mOutputVideo.open("TUeLaneTracker.avi", CV_FOURCC('M','P','4','V'), 30, mFrameRGB.size());
}


int TrackingLaneDAG_generic::init_DAG()
{
	const size_t& lCOUNT 	 = mLaneFilter->COUNT_BINS;

	mX_ICCS_SCALED	 	 =  SCALE_INTSEC*mX_ICCS;
	mBASE_BINS_SCALED  	 =  SCALE_INTSEC*mLaneFilter->BASE_BINS;
	mPURVIEW_BINS_SCALED	 =  SCALE_INTSEC*mLaneFilter->PURVIEW_BINS;

	mLOWER_LIMIT_BASE	 =  mBASE_BINS_SCALED.at<int32_t>(0,0);
	mLOWER_LIMIT_PURVIEW  	 =  mPURVIEW_BINS_SCALED.at<int32_t>(0,0);

	mUPPER_LIMIT_BASE	 =  mBASE_BINS_SCALED.at<int32_t>(lCOUNT-1,0);
	mUPPER_LIMIT_PURVIEW  	 =  mPURVIEW_BINS_SCALED.at<int32_t>(lCOUNT-1,0);


	//Assuming a constant step
	mSTEP_BASE	 	 =  mBASE_BINS_SCALED.at<int32_t>(1,0)
				   -mBASE_BINS_SCALED.at<int32_t>(0,0) ;

	mSTEP_PURVIEW	 	 =  mPURVIEW_BINS_SCALED.at<int32_t>(1,0)
				   -mPURVIEW_BINS_SCALED.at<int32_t>(0,0) ;

        mHistBase      		 =  cv::Mat::zeros(lCOUNT,  1 ,  CV_32S);
        mHistPurview   		 =  cv::Mat::zeros(lCOUNT,  1 ,  CV_32S);

  	return 0;
}


void TrackingLaneDAG_generic::execute(cv::Mat FrameGRAY)
{

	WriteLock  wrtLock(_mutex, std::defer_lock);


	BufferingDAG_generic::execute(FrameGRAY);
       
	 //Start Filtering
	  wrtLock.lock();
	  this->mStartFiltering = true;
	  wrtLock.unlock();
	 _sateChange.notify_one();


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
		 lBaseBinIdx	= (*lPtrIntBase    - mLOWER_LIMIT_BASE    + (mSTEP_BASE/2))/mSTEP_BASE;

		 lPurviewBinIdx	= (*lPtrIntPurview - mLOWER_LIMIT_PURVIEW + (mSTEP_PURVIEW/2))/mSTEP_PURVIEW;
		
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
	wrtLock.lock();
	_sateChange.wait(wrtLock,[this]{return mFiltersReady;} );
	mFiltersReady = false; //reset the flag for next loop.
	this->mStartBufferShift = true;
	wrtLock.unlock();
	_sateChange.notify_one();
		
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
			
		int& lIdx_BL    	 = Models[i].binIdxBoundary_left;
		int& lIdx_BR		 = Models[i].binIdxBoundary_right;

		int& lIdx_NBL	 	 = Models[i].binIdxNonBoundary_left;
		int& lIdx_NBR    	 = Models[i].binIdxNonBoundary_right;

		int& lCount_NBL          = Models[i].nonBoundaryBinsCount_left;
		int& lCount_NBR		 = Models[i].nonBoundaryBinsCount_right;

		mLikelihood_LB  	=  round(lPtrHistBase[lIdx_BL-1]*0.25
					   +lPtrHistBase[lIdx_BL]
					   +lPtrHistBase[lIdx_BL+1]*0.25);
										 
		mLikelihood_RB 		=  round(lPtrHistBase[lIdx_BR-1]*0.25
					   +lPtrHistBase[lIdx_BR]
					   +lPtrHistBase[lIdx_BR+1]*0.25);
	
		mConditionalProb  	=  (mLikelihood_LB *mLikelihood_RB)/(float)SCALE_FILTER;



		//TODO:start=> Put this block on the side thread
		mCorrelationNB	= 0;
		lRange = mHistBase(cv::Range(lIdx_NBL,  lIdx_NBL  +  lCount_NBL), cv::Range::all());
		mCorrelationNB +=sum(lRange)[0];
		
	    	lRange = mHistBase(cv::Range(lIdx_NBR, lIdx_NBR + lCount_NBR), cv::Range::all());
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
	      return;
	   else
	      mBaseHistModel = Models[lBestModelIdx];

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
	   int32_t	lIdx_BL, lIdx_BR;

	   size_t  	lIdx_Mid, lIdx_NBL, lIdx_NBR, lCount_NBL, lCount_NBR;

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
		   const int&  binV = mVpFilter->BINS_V(v);
		   const int&  binH = mVpFilter->BINS_H(h);
		
		   lIntSecLeft  = SCALE_INTSEC*( ((binH - lBaseLB)/(float)(binV - lBaseLine))*(lPurviewLine - binV) +binH );
		   lIntSecRight = SCALE_INTSEC*( ((binH - lBaseRB)/(float)(binV - lBaseLine))*(lPurviewLine - binV) +binH );

		   lIdx_BL 	= ( lIntSecLeft  - mLOWER_LIMIT_PURVIEW + (mSTEP_PURVIEW/2.0))/mSTEP_PURVIEW;
		   lIdx_BR 	= ( lIntSecRight - mLOWER_LIMIT_PURVIEW + (mSTEP_PURVIEW/2.0))/mSTEP_PURVIEW;
		   lIdx_Mid  	= round((lIdx_BL+ lIdx_BR)/2.0 );


		   //^TODO:start=> Make non-boundary region dependent on the binwidth
		   lIdx_NBL   	= lIdx_BL   + 2;
		   lIdx_NBR 	= lIdx_Mid  + 4;	
		   lCount_NBL   = (lIdx_Mid - 3) - (lIdx_BL  + 2) ;        
		   lCount_NBR   = (lIdx_BR  - 2) - (lIdx_Mid + 3) ;
		   //(TODO:end)

					   
		   if (lIdx_BL > 0 && lIdx_BR < (int32_t) (mLaneFilter->COUNT_BINS -2) )
		   {			

		      lWidth_cm= mLaneFilter->BINS_cm(lIdx_BR) - mLaneFilter->BINS_cm(lIdx_BL);

		      // VP Probability over lane width difference between base and purview line
		      mLikelihood_W  	= mLaneMembership.WIDTH_DIFF_NORMA;
		      mLikelihood_W    *= exp(-pow(mBaseHistModel.width_cm - lWidth_cm, 2) / mLaneMembership.WIDTH_DIFF_NOMIN );
		
		      // VP Probability over left lane boundary
		      mLikelihood_LB 	=  round(lPtrHistPurview[lIdx_BL-1]*0.25
					      + lPtrHistPurview[lIdx_BL]
				  	      + lPtrHistPurview[lIdx_BL+1]*0.25);
												  
		      // VP Probability over Right LaneBoundary 				 
		      mLikelihood_RB 	= round(lPtrHistPurview[lIdx_BR-1]*0.25
					      + lPtrHistPurview[lIdx_BR]
					      + lPtrHistPurview[lIdx_BR+1]*0.25);
					
		      mConditionalProb  =  (mLikelihood_LB*mLikelihood_RB) /(float)SCALE_FILTER;
		
		      //VP Probability over Non-Boundary Region
		      mCorrelationNB = 0;
		      lRange 		= mHistPurview(cv::Range(lIdx_NBL, lIdx_NBL + lCount_NBL), cv::Range::all());
		      mCorrelationNB   +=sum(lRange)[0];
			
		      lRange 		= mHistPurview(cv::Range(lIdx_NBR, lIdx_NBR + lCount_NBR), cv::Range::all());
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
	mLaneModel.setModel(mBaseHistModel.boundary_left,
			    mBaseHistModel.boundary_right,
			    mBaseHistModel.boundary_left_cm,
			    mBaseHistModel.boundary_right_cm,
			    mVanishPt);

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
mProfiler.start("DISPLAY");
#endif

	#ifdef DISPLAY_GRAPHICS
	{

	//   mLaneModel.drawLane(FrameGRAY, mSPAN,  *mLaneFilter);
	   //write the processed frame to the video
	   //mOutputVideo<<mFrameRGB;
	}
	#endif // Display Graphics	
								
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
		  		<<"******************************"<<endl
		  		<<  "Screen Display." <<endl
				<<  "Max Time: " << mProfiler.getMaxTime("DISPLAY")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("DISPLAY")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("DISPLAY")<<endl
		  		<<"******************************"<<endl<<endl;	
		 		#endif

}//extractLanes




void TrackingLaneDAG_generic::runAuxillaryTasks()
{
	int  lRowIndex;
	cv::Rect lROI;
	WriteLock  wrtLock(_mutex, std::defer_lock);

	wrtLock.lock();
	 if (mBufferReady == false)

// MODE: A + C 
	{
		wrtLock.unlock();
		 // Extract Depth Template
		 lRowIndex = mCAMERA.RES_VH(0)- mSPAN; 
		 lROI = cv::Rect(0,lRowIndex,mCAMERA.RES_VH(1), mSPAN);

		wrtLock.lock();
		 mDEPTH_MAP_ROOT(lROI).copyTo(mDepthTemplate);
		wrtLock.unlock();

		 // Extract Focus Template
		 lRowIndex = mVP_RANGE_V - mVanishPt.V;
		 lROI = cv::Rect(0, lRowIndex, mCAMERA.RES_VH(1), mSPAN);
		
		wrtLock.lock();
		 mFOCUS_MASK_ROOT(lROI).copyTo(mFocusTemplate);	
		wrtLock.unlock();

		wrtLock.lock();
		  for ( std::size_t i = 0; i< mBufferPool->Probability.size()-1 ; i++ )
		  {
			 mBufferPool->Probability[i+1].copyTo(mBufferPool->Probability[i]);		
			 mBufferPool->GradientTangent[i+1].copyTo(mBufferPool->GradientTangent[i]);		
		  }
		
		  mTemplatesReady = true;
		  mBufferReady    = true;
		wrtLock.unlock();
	}

	else

// MODE: A ONLY 
	{
		wrtLock.unlock();
		 lRowIndex = mCAMERA.RES_VH(0)- mSPAN; 
		 lROI = cv::Rect(0,lRowIndex,mCAMERA.RES_VH(1), mSPAN);
		
		wrtLock.lock();
		 mDEPTH_MAP_ROOT(lROI).copyTo(mDepthTemplate);
		wrtLock.unlock();

		lRowIndex = mVP_RANGE_V-mVanishPt.V;
		lROI = cv::Rect(0, lRowIndex, mCAMERA.RES_VH(1), mSPAN);


		wrtLock.lock();
		 mFOCUS_MASK_ROOT(lROI).copyTo(mFocusTemplate);
		 mTemplatesReady= true;		
		wrtLock.unlock();
	}
	
	_sateChange.notify_one();
	
	
//MODE B:

	wrtLock.lock();
	_sateChange.wait(wrtLock,[this]{return mStartFiltering;} );

	int64_t lSUM;
	
	//Predict Lane States
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
			

	mFiltersReady   = true;
	mStartFiltering = false;
	
	wrtLock.unlock();
	_sateChange.notify_one();

// MODE: C  

	wrtLock.lock();
	_sateChange.wait(wrtLock,[this]{return mStartBufferShift;} );
	
	   for ( std::size_t i = 0; i< mBufferPool->Probability.size()-1 ; i++ )
	   {
	 	mBufferPool->Probability[i+1].copyTo(mBufferPool->Probability[i]);		
		mBufferPool->GradientTangent[i+1].copyTo(mBufferPool->GradientTangent[i]);
	   }	

	   mBufferReady    = true;
	   mStartBufferShift=false;

	wrtLock.unlock();
}

TrackingLaneDAG_generic::~TrackingLaneDAG_generic()
{
	

}
