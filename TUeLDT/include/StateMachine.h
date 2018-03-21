#ifndef STATEMACHINE_H
#define STATEMACHINE_H

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
#include  <memory>
#include "State.h"
#include "InitState.h"
#include "BufferingState.h"
#include "TrackingLaneState.h"
#include "FrameFeeder.h"
#include "FrameRenderer.h"

using namespace std;

class StateMachine
{
	
private:

	bool	mQuitRequest;
	States 	mCurrentState;

	unique_ptr<FrameFeeder>			mPtrFrameFeeder;
	unique_ptr<FrameRenderer>		mPtrFrameRenderer;

	unique_ptr<LaneFilter>  		mPtrLaneFilter;
	unique_ptr<VanishingPtFilter>  		mPtrVanishingPtFilter;
	unique_ptr<Templates> 			mPtrTemplates;
	LaneModel				mLaneModel;


	#ifdef S32V2XX
	 unique_ptr<InitState>						mPtrBootingState;
	 unique_ptr<BufferingState<BufferingDAG_s32v>>  	 	mPtrBufferingState;
	 unique_ptr<TrackingLaneState<TrackingLaneDAG_s32v>>  	 	mPtrTrackingState;
	#else
	 unique_ptr<InitState>						mPtrBootingState;
	 unique_ptr<BufferingState<BufferingDAG_generic>>  	 	mPtrBufferingState;
	 unique_ptr<TrackingLaneState<TrackingLaneDAG_generic>>   	mPtrTrackingState;
	#endif

public:

	bool	isInitialized();
	States	getCurrentState();
	void 	quit();

	StateMachine(unique_ptr<FrameFeeder> frameFeeder);
	int spin();
	~StateMachine();
};

#endif
