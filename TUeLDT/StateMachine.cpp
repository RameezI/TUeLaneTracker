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

#include "StateMachine.h"

#ifdef   S32V2XX
 #include <oal.h>
 #include <apex.h>
 #include "BufferingDAG_s32v.h"
 #include "TrackingLaneDAG_s32v.h"
#else
 #include "BufferingDAG_generic.h"
 #include "TrackingLaneDAG_generic.h"
#endif


StateMachine::StateMachine(unique_ptr<FrameFeeder> frameFeeder, const LaneTracker::Config& Config)
 : mQuitRequest(false),
   mRebootRequest(false),
   mCurrentState(States::BOOTING),
   mConfig(Config),
   mPtrFrameFeeder(std::move(frameFeeder)),
   mPtrFrameRenderer(nullptr),
   mPtrLaneFilter(nullptr),
   mPtrVanishingPtFilter(nullptr),
   mPtrTemplates(nullptr),
   mPtrLaneModel(nullptr)
{

	#ifdef S32V2XX
	  int  lSuccess = OAL_Initialize();
	  if(lSuccess!=0)
	    throw "Failed to initialize s32v OAL context ";
	  ACF_Init();
	#endif

	if (mPtrFrameFeeder == nullptr)
	  throw "State-Machine is missing an instance of FrameFeeder";


	#ifdef PROFILER_ENABLED
	  Logger::Init();
	  LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	  <<"******************************"<<endl
	  << "State-Machine successfully Created..."<<endl
	  <<"******************************"<<endl<<endl;
	#endif
}


int StateMachine::spin()
{
	int lReturn = 0;

	// Process Quit Request
	if (mQuitRequest)
	{
	   #ifdef PROFILER_ENABLED
	    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	    <<"******************************"<<endl
	    << "[User Requested to Quit the State-Machine]"<<endl
	    <<  "Shutting Down the State-Machine."<<endl
	    <<"******************************"<<endl<<endl;
	   #endif
	   mCurrentState = States::DISPOSED;
	}
	else
	if (mRebootRequest)
	{
	   #ifdef PROFILER_ENABLED
	    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	    <<"******************************"<<endl
	    << "[User Requested to reboot the State-Machine]"<<endl
	    <<  "Rebooting the State-Machine."<<endl
	    <<"******************************"<<endl<<endl;
	   #endif

   	   mPtrBootingState.reset(nullptr);
   	   mPtrBufferingState.reset(nullptr);
   	   mPtrTrackingState.reset(nullptr);
	   mPtrLaneModel = nullptr;

	   mPtrFrameFeeder->Paused.store(true);
	   mCurrentState = States::BOOTING;
	   mRebootRequest = false;
	}
	else

	switch(mCurrentState) {

	case States::BOOTING :
	{
		if (mPtrBootingState == nullptr)
	  	{
		   mPtrBootingState   = unique_ptr<InitState>(new InitState(mConfig));
		}
	   	if (mPtrBootingState->currentStatus  != StateStatus::ERROR)
		{
	   	    mPtrLaneFilter.reset(nullptr);
	   	    mPtrVanishingPtFilter.reset(nullptr);
	   	    mPtrTemplates.reset(nullptr);

	   	    mPtrLaneFilter 	        = mPtrBootingState->createLaneFilter();
	   	    mPtrVanishingPtFilter       = mPtrBootingState->createVanishingPtFilter();
	   	    mPtrTemplates 	        = mPtrBootingState->createTemplates();
		}
		if (mPtrBootingState->currentStatus == StateStatus::DONE)
		{				

		   #ifdef PROFILER_ENABLED
		     LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		     <<"****************************************"<<endl
		     << "Booting State Completed"
		     << "Printing Booting Configuration"<<endl
		     <<  *(mPtrLaneFilter)<<endl
		     <<  *(mPtrVanishingPtFilter)<<endl
		     <<"****************************************"<<endl<<endl;
		   #endif
	
		   cout<< "Completed!"<<endl;
		   mCurrentState 	= States::BUFFERING;	   
		}
		else
		{
	      	   #ifdef PROFILER_ENABLED
	       	     LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	       	     <<"****************************************"<<endl
	       	     <<  "[Failed to Complete the Booting Process]"<<endl
	       	     <<  "Shutting Down the State-Machine."<<endl
	       	     <<"****************************************"<<endl<<endl;
	      	   #endif
			
		      mCurrentState 	= States::DISPOSED;
		      lReturn 		= -1;
		}
	} break; //BOOTING PROCESS SCOPE ENDS


	case States::BUFFERING :
	{
		if (mPtrBufferingState == nullptr)
		{
		   #ifdef S32V2XX
		    mPtrBufferingState.reset(new BufferingState<BufferingDAG_s32v>(mConfig));
		   #else
		    mPtrBufferingState.reset(new BufferingState<BufferingDAG_generic>(mConfig));
		   #endif
		   mPtrBootingState = nullptr;
		}
		if (mPtrBufferingState->currentStatus == StateStatus::INACTIVE)
		{
		      mPtrBufferingState->setupDAG(std::ref(*mPtrTemplates), mConfig.buffer_count);
		      mPtrFrameFeeder->Paused.store(false);
		}
		if (mPtrBufferingState->currentStatus == StateStatus::ACTIVE)
		{
		   try
		   {
		     mPtrBufferingState->run(mPtrFrameFeeder->dequeue());
		   }
		   catch(const char* msg)
		   {
	   	     #ifdef PROFILER_ENABLED
	              LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	              <<"******************************"<<endl
	              << "Exception while Buffering: "<<endl
	              << msg <<endl
		      << "Shutting Down the State-Machine."<<endl
	              <<"******************************"<<endl<<endl;
	             #endif
		      mCurrentState 	= States::DISPOSED;
		      lReturn 		= -1;
		   }
		}
		if( mPtrBufferingState->currentStatus == StateStatus::DONE)
		{
		   #ifdef PROFILER_ENABLED
		    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		    <<"******************************"<<endl
		    << "[Buffering State Completed]"<<endl
		    <<"******************************"<<endl<<endl;
		   #endif

		   mCurrentState = States::DETECTING_LANES; 
		   cout<<"Completed!"<<endl;
		}
		if( mPtrBufferingState->currentStatus == StateStatus::ERROR)
		{
		   #ifdef PROFILER_ENABLED
		    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		    <<"******************************"<<endl
		    << "[Buffering State Error]"<<endl
		    <<  "Shutting Down the State-Machine."<<endl
		    <<"******************************"<<endl<<endl;
		   #endif

		   mCurrentState 	= States::DISPOSED;
		   lReturn 	        = -1;
		}
	} break; // BUFFERING PROCESS SCOPE ENDS


	case States::DETECTING_LANES :
	{
		if (mPtrTrackingState == nullptr)
		{
		   #ifdef S32V2XX
		   mPtrTrackingState.reset(new TrackingLaneState<TrackingLaneDAG_s32v>( move(mPtrBufferingState->mGraph) ));
		   #else
		   mPtrTrackingState.reset(new TrackingLaneState<TrackingLaneDAG_generic>( move(mPtrBufferingState->mGraph) ));
		   #endif
		   mPtrBufferingState 	= nullptr; //BufferingState does not contain graph anymore, so make it unusable.
		}
		if (mPtrTrackingState->currentStatus == StateStatus::INACTIVE)
		{
		   mPtrTrackingState->setupDAG(mPtrLaneFilter.get(), mPtrVanishingPtFilter.get());
		   mPtrFrameRenderer.reset(new FrameRenderer(*mPtrLaneFilter, *mPtrFrameFeeder.get() ));
		}
		if (mPtrTrackingState->currentStatus == StateStatus::ACTIVE)
		{
		  try
		  {
		       mPtrLaneModel = mPtrTrackingState->run(mPtrFrameFeeder->dequeue());

		       if(mConfig.display_graphics)
		        mPtrFrameRenderer->drawLane(mPtrFrameFeeder->dequeueDisplay(), *mPtrLaneModel);
		  }
		  catch(const char* msg)
		  {
	   	     #ifdef PROFILER_ENABLED
	              LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	              <<"******************************"<<endl
	              << "Exception while Tracking: "<<endl
	              << msg <<endl
		      << "Shutting Down the State-Machine."<<endl
	              <<"******************************"<<endl<<endl;
	             #endif
		      mCurrentState 	= States::DISPOSED;
		      lReturn 		= -1;
		  }
		}
		if( (mPtrTrackingState->currentStatus == StateStatus::DONE) )
		{
		   mCurrentState 	= States::DISPOSED;
		}
		if(mPtrTrackingState->currentStatus == StateStatus::ERROR)
		{
		   #ifdef PROFILER_ENABLED
		    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		    <<  "********************************"<<endl
		    <<  "[Tracking State Error]"<<endl
		    <<  "Shutting Down the State-Machine."<<endl
		    <<"******************************"<<endl<<endl;
		   #endif

		   mCurrentState 	= States::DISPOSED;
		   lReturn 		= -1;
		}

	} break; // TRACKING STATE SCOPE ENDS 


	case States::DISPOSED :
	{
	        #ifdef PROFILER_ENABLED
	    	 LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	    	 <<  "********************************"<<endl
	     	 <<  "[STATE MACHINE IS IN DISPOSED STATE]"<<endl
	    	 <<  "Waiting for destruction..."<<endl
	    	 <<"******************************"<<endl<<endl;
	        #endif

	} break; } // END SWITCH

	return lReturn;
}

bool StateMachine::laneModel()
{
   if(mPtrLaneModel == nullptr)
     return false;
   else
     return true;
}

LaneModel StateMachine::getLaneModel()
{
    return *mPtrLaneModel;
}


void StateMachine::quit()
{
	mQuitRequest = true;
}


void StateMachine::reboot()
{
	mRebootRequest = true;
}


States StateMachine::getCurrentState()
{
	return mCurrentState;
}


StateMachine::~StateMachine()
{

   mPtrLaneModel = nullptr;
   mPtrFrameFeeder.reset(nullptr);
   mPtrBootingState.reset(nullptr);
   mPtrBufferingState.reset(nullptr);
   mPtrTrackingState.reset(nullptr);

   #ifdef S32V2XX
    OAL_Deinitialize();
   #endif

   #ifdef PROFILER_ENABLED
     LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
     <<  "********************************"<<endl
     <<  "[State-Machine Destroyed]"<<endl
     <<"******************************"<<endl<<endl;
   #endif

}
