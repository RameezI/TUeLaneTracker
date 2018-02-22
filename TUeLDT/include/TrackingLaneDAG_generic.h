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

	bool            mStartBufferShift;
	bool    	mStartFiltering;
	bool 		mFiltersReady;
	
	
	const int	mMAX_PIXELS_ROI;

	int  		mLOWER_LIMIT_IntBase;
	int 		mUPPER_LIMIT_IntBase;
	
	int  		mLOWER_LIMIT_IntPurview;
	int 		mUPPER_LIMIT_IntPurview;
	
	int		mSCALED_STEP_LANE_FILTER;
	int 		mSCALED_STEP_VP_FILTER;
	int 		mSCALED_START_LANE_FILTER;
	int 		mSCALED_START_VP_FILTER;
	
	
	int   		mLikelihoodLeftBoundary;
	int 		mLikelihoodRightBoundary;
	float        	mLikelihoodNegBoundary;
	int        	mPosteriorProbBase;
	
	
	int   		mLikelihoodVP_LBoundary;
	int   		mLikelihoodVP_RBoundary;
	float     	mLikelihoodVP_NegBoundary;
    	float		mLikelihoodVP_Width;
	int       	mPosteriorProbVP;
	
	LaneFilter* 		mLaneFilter;
	VanishingPtFilter*	mVpFilter;
	
	
	cv::Mat 		mX_VPRS_SCALED;	
	cv::Mat 		mIntBase;           
	cv::Mat			mIntPurview;
	cv::Mat 		mIntWeights;
	
	cv::Mat			mHistBase;
	cv::Mat			mHistPurview;
	
	cv::Mat			mProbMapFocussed;
	cv::Mat 		mGradTanFocussed;
	
	cv::Mat 		mTransitLaneFilter;
	cv::Mat 		mTransitVpFilter;

	LaneModel   	mLaneModel;
	

	// Only Enable in case of Video Recording
	//VideoWriter mOutputVideo;



	
public:

	TrackingLaneDAG_generic(BufferingDAG_generic&& bufferingGraph);
	int  init_DAG();
	void runAuxillaryTasks();
	void extractLanes();
	void extractControllerInputs();
	~TrackingLaneDAG_generic();

};

#endif // TRACKINGLANESDAG_GENERIC_H
