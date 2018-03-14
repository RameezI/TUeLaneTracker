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
  mStartBufferShift(false),
  mStartFiltering(false),
  mFiltersReady(false),
  mMAX_PIXELS_ROI(mFrameGRAY_ROI.rows * mFrameGRAY_ROI.cols)

{	
	//Write Images to a video file
	//mOutputVideo.open("TUeLaneTracker.avi", CV_FOURCC('M','P','4','V'), 30, mFrameRGB.size());
}


int TrackingLaneDAG_generic::init_DAG()
{
	mX_ICCS.convertTo(mX_ICCS_SCALED, CV_32S, SCALE_INTSEC );

        mHistBase      = cv::Mat::zeros(mLaneFilter->mNb_HISTOGRAM_BINS,  1 ,  CV_32S);
        mHistPurview   = cv::Mat::zeros(mLaneFilter->mNb_HISTOGRAM_BINS,  1 ,  CV_32S);

  	return 0;
}




void TrackingLaneDAG_generic::extractLanes()
{

	WriteLock  wrtLock(_mutex, std::defer_lock);

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
	subtract(-mLaneFilter->OFFSET_V, -mY_ICCS, mIntBase, cv::noArray(), CV_32S);
	divide(mIntBase, mGradTanFocussed, mIntBase, SCALE_INTSEC_TAN, CV_32S);
	add(mIntBase, mX_ICCS_SCALED, mIntBase);
	
	//Purview Intersections
	subtract(-mVpFilter->OFFSET_V, -mY_ICCS, mIntPurview, cv::noArray(), CV_32S);
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
	//Build Mask for Valid Intersections
	bitwise_and(mProbMapFocussed > 0, mGradTanFocussed !=0,    mMask);
	bitwise_and(mMask, mIntBase    > mLOWER_LIMIT_IntBase,     mMask);
	bitwise_and(mMask, mIntPurview > mLOWER_LIMIT_IntPurview,  mMask);
    	bitwise_and(mMask, mIntBase    < mUPPER_LIMIT_IntBase,     mMask);
    	bitwise_and(mMask, mIntPurview < mUPPER_LIMIT_IntPurview,  mMask);

        mHistBase      = cv::Mat::zeros(mLaneFilter->mNb_HISTOGRAM_BINS,  1 ,  CV_32S);
        mHistPurview   = cv::Mat::zeros(mLaneFilter->mNb_HISTOGRAM_BINS,  1 ,  CV_32S);

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
	   int32_t* 	IN_basePTR 	    	= mIntBase.ptr<int32_t>(0);
	   int32_t* 	IN_purviewPTR   	= mIntPurview.ptr<int32_t>(0);
	   int32_t* 	IN_weightsPTR   	= mIntWeights.ptr<int32_t>(0);
	   uint8_t* 	IN_maskPTR   		= mMask.ptr<uint8_t>(0);

	   int32_t* 	HistBase_pixelPTR    	=  mHistBase.ptr<int32_t>(0);
	   int32_t* 	HistPurview_pixelPTR 	=  mHistPurview.ptr<int32_t>(0);

	   uint16_t   	lBaseBinIdx;
	   uint16_t   	lPurviewBinIdx;
	   int32_t    	lWeightBin;

	   for (int i = 0; i < mMAX_PIXELS_ROI; i++,IN_basePTR++,IN_purviewPTR++, IN_weightsPTR++ , IN_maskPTR++)
	   {
			
		   if(!(*IN_maskPTR ==0) )
		   {		
			lBaseBinIdx=
				(*IN_basePTR    - mSCALED_START_LANE_FILTER + (mSCALED_STEP_LANE_FILTER/2) )
				/ mSCALED_STEP_LANE_FILTER;
			

			lPurviewBinIdx=
				(*IN_purviewPTR -  mSCALED_START_VP_FILTER   + (mSCALED_STEP_VP_FILTER/2)  )
				/ mSCALED_STEP_VP_FILTER ;
			
			lWeightBin = *IN_weightsPTR;
			
			
			*(HistBase_pixelPTR 	+ lBaseBinIdx	)  	+= lWeightBin;
			*(HistPurview_pixelPTR  + lPurviewBinIdx) 	+= lWeightBin;
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
mProfiler.start("HISTOGRAM_MATCHING");
#endif

	{
   	    int64_t SUM;

	    //Normalising Base Histogram
	    SUM = sum(mHistBase)[0];
	    mHistBase.convertTo(mHistBase, CV_64F, SCALE_FILTER);
	    mHistBase.convertTo(mHistBase, CV_32S, 1.0/SUM );
	 

	    //Normalising Purview Histogram
	    SUM = sum(mHistPurview)[0];
	    mHistPurview.convertTo(mHistPurview, CV_64F, SCALE_FILTER);
	    mHistPurview.convertTo(mHistPurview, CV_32S, 1.0/SUM );
		
		
	   int   BestModelIdx=-1;
	   int   NegLaneCorrelation;
	   float x;
	   int   bestPosteriorProb  = 0;
	   float conditionalProb;
		
	   int32_t* HistBasePTR    	=  mHistBase.ptr<int32_t>(0);
	   vector<BaseHistogramModel>& Models= mLaneFilter->baseHistogramModels;
	   cv::Mat    range;
	
	   for(size_t i=0; i< Models.size(); i++)
	   {    
			
		int& LeftIdx    	= Models[i].binID_leftBoundary;
		int& RightIdx		= Models[i].binID_rightBoundary;

		int& NegLeftIdx		= Models[i].binID_NegBoundaryLeft;
		int& NegRightIdx    	= Models[i].binID_NegBoundaryRight;

		int& Nleft          	= Models[i].nbNonBoundaryBinsLeft;
		int& Nright		= Models[i].nbNonBoundaryBinsRight;

cout<<endl<<LeftIdx<<"  ,  "<<RightIdx<<"   ,  "<< NegLeftIdx<<"  ,  "<<NegRightIdx<<"  ,  "<<Nleft<<"  ,  "<<Nright<<endl;
		

		mLikelihoodLeftBoundary  =  round(HistBasePTR[LeftIdx-1]*0.25
						+HistBasePTR[LeftIdx]
						+HistBasePTR[LeftIdx+1]*0.25);
										 
		mLikelihoodRightBoundary = round( HistBasePTR[RightIdx-1]*0.25
						+HistBasePTR[RightIdx]
						+HistBasePTR[RightIdx+1]*0.25);
	
		conditionalProb  	 =  (mLikelihoodLeftBoundary*mLikelihoodRightBoundary)/(float)SCALE_FILTER;


//TODO:	Put on the side thread.(start)
		NegLaneCorrelation=0;
		range = mHistBase(cv::Range(NegLeftIdx,  NegLeftIdx  +  Nleft), cv::Range::all());
		NegLaneCorrelation +=sum(range)[0];
		
	    	range = mHistBase(cv::Range(NegRightIdx, NegRightIdx + Nright), cv::Range::all());
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
	   mLaneModel.centerLane    	= round((mLaneModel.leftOffset + mLaneModel.rightOffset)/2.0);
		
	   mLaneModel.laneWidth     	= Models[BestModelIdx].width_cm;
	 
	   mLaneModel.confidenceLeft  	= round(100* mLaneModel.confidenceLeft/(float)SCALE_FILTER);
	   mLaneModel.confidenceRight  	= round(100* mLaneModel.confidenceRight/(float)SCALE_FILTER);

		cout <<"Index : "<<BestModelIdx;
		cout<< "Filter: "<<mPosteriorProbBase;
		
	}//Scope End


	   {
	     
	     cv::Mat lIntBase, lIntPurview, lIntWeights, lMask,  lHistBase, lHistPurview;
	     cv::FileStorage file("/home/s32v/compare/Mat_new", cv::FileStorage::READ);
	     file["mIntBase"]>> lIntBase;
	     file["mIntPurview"]>> lIntPurview;
	     file["mIntWeights"]>> lIntWeights;
	     file["mMask"]>> lMask;
	     file["mHistBase"]>>lHistBase;
	     file["mHistPurview"]>>lHistPurview;

	     // lHistBase.convertTo(lIntBase, CV_32S);
	     int d  = cv::norm(mIntBase, lIntBase, cv::NORM_INF);
		 d += cv::norm(mIntPurview, lIntPurview, cv::NORM_INF);
		 d += cv::norm(mIntWeights, lIntWeights, cv::NORM_INF);
		 d += cv::norm(mMask, lMask, cv::NORM_INF);
		 d += cv::norm(mHistBase,    lHistBase,    cv::NORM_INF);
		 d += cv::norm(mHistPurview, lHistPurview, cv::NORM_INF);

	      cout<<"dist: " << d <<endl;
	     
	     //cout<<"size of Histogram Base:" << lHistBase.size()<<"	vs	"<< mHistBase.size()<<endl;
	   }

	  exit(0);


		
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
	   int   		bestPosteriorProb  = 0;
	   int32_t* 		HistPurviewPTR     =  mHistPurview.ptr<int32_t>(0);

	
	   const float left_VP  = -mLaneModel.leftOffset ;												
	
	   const float right_VP  = mLaneModel.rightOffset ;
				
		
	   const int    FRAME_CENTER_V		= mCAMERA.FRAME_CENTER(0);
	   const float  PIXEL_TO_CM     	= 1.0/mCAMERA.CM_TO_PIXEL;

	   const int    VP_FILTER_OFFSET 	= mVpFilter->OFFSET_V;
	   const int    VP_HIST_STEP	 	= mVpFilter->STEP;
	   const int    VP_HIST_START	 	= mVpFilter->HISTOGRAM_BINS(0);
	   const int    VP_HIST_SIZE	 	= mVpFilter->HISTOGRAM_BINS.size();
	   const int    VP_HIST_END	 	= mVpFilter->HISTOGRAM_BINS(VP_HIST_SIZE-1);
	   const float  VP_HIST_RATIO  	 	= mVpFilter->mVP_LANE_RATIO;
	   const float  WIDTH_FACTOR   	 	= PIXEL_TO_CM* 1.0/VP_HIST_RATIO;
		

	   float  IntSecLeft, IntSecRight; 
	   int    LeftIdx, MidIdx, RightIdx;

	   int   NegLeftIdx, NegRightIdx;
	   int   NbNegLaneLeft, NbNegLaneRight, NegLaneCorrelation;
	   float x, conditionalProb;
	
	   cv::Mat   range;
	
	   float  width_cm;
	
	   // Store Previous values of VP 
	   mVanishPt.V_prev = mVanishPt.V;
	   mVanishPt.H_prev = mVanishPt.H;
		
	   for(int v=0; v < mVpFilter->mNb_VP_BINS_V;v++)
	   {	for(int h=0; h < mVpFilter->mNb_VP_BINS_H; h++)
		{
		   const	int&  binV = mVpFilter->VP_BINS_V(v);
		   const 	int&  binH = mVpFilter->VP_BINS_H(h);
		
		   IntSecLeft = ((binH - left_VP)/(float)(binV - FRAME_CENTER_V))
					 *(VP_FILTER_OFFSET - binV) +binH;


		   IntSecRight 	= ((binH - right_VP)/(float)(binV - FRAME_CENTER_V))
				  *(VP_FILTER_OFFSET - binV) +binH;

		   IntSecLeft 	= VP_HIST_STEP * round(IntSecLeft/VP_HIST_STEP);

	           LeftIdx 	= (IntSecLeft  - VP_HIST_START + (VP_HIST_STEP/2))/VP_HIST_STEP;
	           RightIdx 	= (IntSecRight - VP_HIST_START + (VP_HIST_STEP/2))/VP_HIST_STEP;
		
		   MidIdx  	= round( ( LeftIdx+ RightIdx ) /2.0);
		
		   NegLeftIdx  	   = LeftIdx+2;
		   NegRightIdx     = MidIdx+4;
		
		   NbNegLaneLeft   = (MidIdx-3) - (LeftIdx+ 2) ;        
		   NbNegLaneRight  = (RightIdx-2) - (MidIdx+3);
		
		   width_cm= (IntSecRight -IntSecLeft) * WIDTH_FACTOR;
					   
		   if (LeftIdx > 0 && RightIdx < VP_HIST_SIZE-2)
		   {	
			
		      // VP Probability over Lane Width at Offseted-VP 
		      mLikelihoodVP_Width = mLaneMembership.WIDTH_DIFF_NORMA 
		       * exp( - pow(mLaneModel.laneWidth - width_cm, 2) / mLaneMembership.WIDTH_DIFF_NOMIN );
				
				
		      // VP Probability over Left LaneBoundary
		      mLikelihoodVP_LBoundary =  round(HistPurviewPTR[LeftIdx-1]*0.25
					      + HistPurviewPTR[LeftIdx]
				  	      + HistPurviewPTR[LeftIdx+1]*0.25);
												  
		      // VP Probability over Right LaneBoundary 				 
		      mLikelihoodVP_RBoundary = round(HistPurviewPTR[RightIdx-1]*0.25
					      + HistPurviewPTR[RightIdx]
					      + HistPurviewPTR[RightIdx+1]*0.25);
					
		      conditionalProb  =  (mLikelihoodVP_LBoundary*mLikelihoodVP_RBoundary) /(float)SCALE_FILTER;	
		
		      //VP Probability over Negative(NO) Boundary
		      NegLaneCorrelation=0;
		      range = mHistPurview(cv::Range(NegLeftIdx,  NegLeftIdx  +  NbNegLaneLeft), cv::Range::all());
		      NegLaneCorrelation +=sum(range)[0];
			
		      range = mHistPurview(cv::Range(NegRightIdx, NegRightIdx + NbNegLaneRight), cv::Range::all());
		      NegLaneCorrelation +=sum(range)[0];
		
		      x= (float)NegLaneCorrelation / SCALE_FILTER ;
		      mLikelihoodVP_NegBoundary = mLaneMembership.NEG_BOUNDARY_NORMA 
		       *exp (-pow(x,2)/mLaneMembership.NEG_BOUNDARY_NOMIN);
											 				 
		      //Conditional Probability			 
		      conditionalProb  =   conditionalProb
					* mLikelihoodVP_NegBoundary
					* mLikelihoodVP_Width;
					
		      mPosteriorProbVP = round(conditionalProb * mTransitVpFilter.at<int32_t>(v ,h));
			
		      mVpFilter->filter.at<int32_t>(v, h) = mPosteriorProbVP;

		      if(mPosteriorProbVP > bestPosteriorProb)
		      {
			   bestPosteriorProb=mPosteriorProbVP;
			   mVanishPt.V =  binV;
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
				<<  "Histogram Matching MAP Estimate VanishingPt."<<endl
				<<  "Max Time: " << mProfiler.getMaxTime("VP_HISTOGRAM_MATCHING")<<endl
				<<  "Avg Time: " << mProfiler.getAvgTime("VP_HISTOGRAM_MATCHING")<<endl
				<<  "Min Time: " << mProfiler.getMinTime("VP_HISTOGRAM_MATCHING")<<endl
		  		<<"******************************"<<endl<<endl;	
				#endif







#ifdef PROFILER_ENABLED
mProfiler.start("DISPLAY");
#endif

	#ifdef DISPLAY_GRAPHICS
	{
	  using namespace cv;

	   const float lRatioLookAhead = 0.35;

	   //Transform VP to Image Coordianate System
	   int VP_V =  mVanishPt.V + mCAMERA.FRAME_CENTER(0);
	   int VP_H =  mVanishPt.H + mCAMERA.FRAME_CENTER(1);	

	   //Lane Bundaries
	   Point  Start_leftLaneInner( mCAMERA.FRAME_CENTER(1) 	-  
		((int)( mLaneModel.leftOffset)/mLaneFilter->STEP)*mLaneFilter->STEP,  mCAMERA.RES_VH(0) );

	   Point  Start_rightLaneInner( mCAMERA.FRAME_CENTER(1) +  
		((int)( mLaneModel.rightOffset)/mLaneFilter->STEP)*mLaneFilter->STEP,  mCAMERA.RES_VH(0) );
	
	   Point  StartMidLane( ((int)((Start_leftLaneInner.x + Start_rightLaneInner.x)/2.0)/ mLaneFilter->STEP)* 
		  mLaneFilter->STEP,  mCAMERA.RES_VH(0) );
 
	   float slopeLeft =  (float)( VP_V-mCAMERA.RES_VH(0) ) / (VP_H- Start_leftLaneInner.x);
	   float slopeRight = (float)( VP_V-mCAMERA.RES_VH(0) ) / (VP_H- Start_rightLaneInner.x);
	

	   Point End_leftLaneInner 	= Start_leftLaneInner;
	   End_leftLaneInner.x 		+= -round((mCAMERA.RES_VH(0)*lRatioLookAhead) / slopeLeft);
	   End_leftLaneInner.y 		+= -round( (mCAMERA.RES_VH(0)*lRatioLookAhead));
	
	   Point End_rightLaneInner 	= Start_rightLaneInner;
	   End_rightLaneInner.x 	+= -round ((mCAMERA.RES_VH(0)*lRatioLookAhead) / slopeRight);
	   End_rightLaneInner.y 	+= -round ((mCAMERA.RES_VH(0)*lRatioLookAhead));


	   // Draw Left Boundary Line
	   line(mFrameRGB,
		 Start_leftLaneInner,
		 End_leftLaneInner,
		 CvScalar(0,255,0),
		 3
	   	);

	   //Draw Right Boundary Line	
	   line(mFrameRGB,
		 Start_rightLaneInner,
		 End_rightLaneInner,
		 CvScalar(0,255,0),
		 3
	   	);
	   
	   //Draw Middle Line
	   line(mFrameRGB,
	     StartMidLane,
	     (End_rightLaneInner + End_leftLaneInner)/2.0,
	     CvScalar(255,0,0),
	     2
	    );


	   //Draw Purview Line
	   line(mFrameRGB,
	     cvPoint(0, mCAMERA.FRAME_CENTER(0) +  mVpFilter->OFFSET_V),
	     cvPoint(mCAMERA.RES_VH(1), mCAMERA.FRAME_CENTER(0) + mVpFilter->OFFSET_V),
	     CvScalar(0,0,0),
	     1
	    );
	
 
	  // Highlight ROI 
	   Rect lROI;
	   lROI = Rect(0, mCAMERA.RES_VH(0) - mSpan, mCAMERA.RES_VH(1), mSpan);
	   cv::Mat lYellow(mSpan, mCAMERA.RES_VH(1), CV_8UC3, Scalar(0,125,125));

    	   cv::Mat lFrameRGB_mSPAN = mFrameRGB(lROI);
	   cv::addWeighted(lYellow, 0.4, lFrameRGB_mSPAN, 0.6, 0, lFrameRGB_mSPAN);
	

	
	   // Draw Histogram-Bins at the Base
	   for (int i=0; i< mLaneFilter->HISTOGRAM_BINS.size(); i++)
	   {
		int x = mCAMERA.FRAME_CENTER(1)+ mLaneFilter->HISTOGRAM_BINS(i); 
		if(x !=  StartMidLane.x)
		line(mFrameRGB, cvPoint(x,mCAMERA.RES_VH(0)), cvPoint(x,mCAMERA.RES_VH(0) -30), cvScalar(0,0,0), 1);
		else
		line(mFrameRGB, cvPoint(x,mCAMERA.RES_VH(0)), cvPoint(x,mCAMERA.RES_VH(0) -40), cvScalar(0,0,255), 2);
	   }



	   // Draw Histogram-Bins at the Purview
	   for (int i=0; i< mVpFilter->HISTOGRAM_BINS.size(); i++)
	   {

		int x = mCAMERA.FRAME_CENTER(1)+ mVpFilter->HISTOGRAM_BINS(i);
		//if(x !=  StartMidLane.x)
		line(mFrameRGB, cvPoint(x,mCAMERA.FRAME_CENTER(0) + mVpFilter->OFFSET_V), 
				cvPoint(x,mCAMERA.FRAME_CENTER(0) + mVpFilter->OFFSET_V-30), cvScalar(0,0,0), 1);
	   }


	   #ifdef DISPLAY_GRAPHICS_DCU
	   	mDCU.PutFrame(mFrameRGB.data);
	   #else  

	   imshow( "Display window", mFrameRGB);
	   
	   if ( (char)32 == (char) waitKey(10) )
	   {
		cout << "Lane Histogram Bins :		"<< mLaneFilter->HISTOGRAM_BINS.transpose() <<endl << endl;

		cout << "VanishingPt Histogram Bins : 	"<< mVpFilter->HISTOGRAM_BINS.transpose() <<endl << endl;

		while ((char)32 != (char)waitKey(1));
	   }

	   #endif



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



void TrackingLaneDAG_generic::extractControllerInputs()
{
	
}



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
		 lRowIndex = mCAMERA.RES_VH(0)- mSpan; 
		 lROI = cv::Rect(0,lRowIndex,mCAMERA.RES_VH(1), mSpan);

		wrtLock.lock();
		 mDEPTH_MAP_ROOT(lROI).copyTo(mDepthTemplate);
		wrtLock.unlock();

		 // Extract Focus Template
		 lRowIndex = mVP_Range_V - mVanishPt.V;
		 lROI = cv::Rect(0, lRowIndex, mCAMERA.RES_VH(1), mSpan);
		
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
		 lRowIndex = mCAMERA.RES_VH(0)- mSpan; 
		 lROI = cv::Rect(0,lRowIndex,mCAMERA.RES_VH(1), mSpan);
		
		wrtLock.lock();
		 mDEPTH_MAP_ROOT(lROI).copyTo(mDepthTemplate);
		wrtLock.unlock();

		lRowIndex = mVP_Range_V-mVanishPt.V;
		lROI = cv::Rect(0, lRowIndex, mCAMERA.RES_VH(1), mSpan);


		wrtLock.lock();
		 mFOCUS_MASK_ROOT(lROI).copyTo(mFocusTemplate);
		 mTemplatesReady= true;		
		wrtLock.unlock();
	}
	
	_sateChange.notify_one();
	
	
//MODE B:

	wrtLock.lock();
	_sateChange.wait(wrtLock,[this]{return mStartFiltering;} );

	int64_t SUM;
	
	//Predict Lane States
	mLaneFilter->filter.convertTo(mLaneFilter->filter, CV_64F);
	boxFilter(mLaneFilter->filter, mTransitLaneFilter, -1, cv::Size(11,11), cv::Point(-1,-1), false, cv::BORDER_REPLICATE );
    	mLaneFilter->filter.convertTo(mLaneFilter->filter, CV_32S);

	SUM = sum(mTransitLaneFilter)[0];
	mTransitLaneFilter= mTransitLaneFilter*SCALE_FILTER;
	mTransitLaneFilter.convertTo(mTransitLaneFilter, CV_32S, 1.0/SUM);
	mTransitLaneFilter = 	mTransitLaneFilter + 0.1*mLaneFilter->prior;


	//Predict VP States
	mVpFilter->filter.convertTo(mVpFilter->filter, CV_64F);
	boxFilter(mVpFilter->filter, mTransitVpFilter, -1, cv::Size(3,3), cv::Point(-1,-1), false, cv::BORDER_REPLICATE );
    	mVpFilter->filter.convertTo(mVpFilter->filter, CV_32S);
	
	SUM = sum(mTransitVpFilter)[0];
	mTransitVpFilter= mTransitVpFilter*SCALE_FILTER;
	mTransitVpFilter.convertTo(mTransitVpFilter, CV_32S, 1.0/SUM);	
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
