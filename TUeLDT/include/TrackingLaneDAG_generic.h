#ifndef TRACKINGLANESDAG_GENERIC_H
#define TRACKINGLANESDAG_GENERIC_H

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

#include "BufferingDAG_generic.h"

class TrackingLaneDAG_generic: public BufferingDAG_generic
{

template<typename T>
friend class TrackingLaneState;
	
private:
	int32_t		mMAX_PIXELS_ROI;
	
	float   	mLikelihood_LB;
	float 		mLikelihood_RB;
	float   	mLikelihood_NB;
    	float		mLikelihood_W;

	float 		mConditionalProb;
	float		mPosterior;
	float		mMaxPosterior;
	int32_t		mCorrelationNB;

	int32_t 	mLOWER_LIMIT_BASE;
	int32_t 	mLOWER_LIMIT_PURVIEW;

	int32_t 	mUPPER_LIMIT_BASE;
	int32_t 	mUPPER_LIMIT_PURVIEW;

	int32_t 	mSTEP_BASE_SCALED;
	int32_t 	mSTEP_PURVIEW_SCALED;
	
	size_t		mIdxPurview_LB;
	size_t		mIdxPurview_RB;
	


	LaneFilter* 		mLaneFilter;
	VanishingPtFilter*	mVpFilter;

	cv::Mat		mBASE_BINS_SCALED;
	cv::Mat		mPURVIEW_BINS_SCALED;
	cv::Mat 	mX_ICCS_SCALED;	

	cv::Mat		mProbMapFocussed;
	cv::Mat 	mGradTanFocussed;

	cv::Mat 	mIntBase;           
	cv::Mat		mIntPurview;
	cv::Mat 	mIntWeights;
	
	cv::Mat		mHistBase;
	cv::Mat		mHistPurview;

	cv::Mat		mHistBase_CV64F;
	cv::Mat		mHistPurview_CV64F;
	
	cv::Mat 	mTransitLaneFilter;
	cv::Mat 	mTransitVpFilter;

	BaseHistogramModel	mBaseHistModel;  	/**< Selected Base-Histogram Model */
	LaneModel   		mLaneModel;		/**< The detected Lane-Model */

	// Only Enable in case of Video Recording
	//cv::VideoWriter mOutputVideo;

	
public:
	TrackingLaneDAG_generic(BufferingDAG_generic&& bufferingGraph);
	int  init_DAG(LaneFilter* laneFilter, VanishingPtFilter* vpFilter);
	void execute(cv::UMat& FrameGRAY);
};

#endif // TRACKINGLANESDAG_GENERIC_H
