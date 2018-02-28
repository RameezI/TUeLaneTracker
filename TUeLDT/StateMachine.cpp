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

 

StateMachine::StateMachine(FrameSource lFrameSource, std::string lSourceStr)

: mInitialized(false),
  mQuitRequest(false),
  mFrameSource(lFrameSource),
  mSourceStr(lSourceStr),
  mCurrentState(States::BOOTING),
  mPtrLaneFilter(nullptr),
  mPtrVanishingPtFilter(nullptr),
  mPtrTemplates(nullptr)

{
	int lReturn =0;

	#ifdef PROFILER_ENABLED
		Logger::Init();
		if(lFrameSource == FrameSource::DIRECTORY)
		{
			LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
			<<"******************************"<<endl
			<<  "DIRECTORY_INPUT DEFINED."	  <<endl
			<<"******************************"<<endl<<endl;
		}
	#endif


	#ifdef S32V2XX
		int  lReturn |= OAL_Initialize();
		ACF_Init();
	#endif


	if(lReturn!=0)
	{
		#ifdef PROFILER_ENABLED
		 LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
 		 <<  "********************************"<<endl
		 <<  "[Failed to Start the Booting Process]"<<endl
		 <<  "Shutting Down the State-Machine."<<endl
		 <<"******************************"<<endl<<endl;
		#endif
	}

	mInitialized = (lReturn==0) ? true : false ;
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


	switch(mCurrentState) {

	case States::BOOTING :
	{
		mPtrBootingState 	= unique_ptr<InitState>(new InitState());

		if (mPtrBootingState->currentStatus  != StateStatus::ERROR)
		 mPtrLaneFilter 	= mPtrBootingState->createLaneFilter();

		if (mPtrBootingState->currentStatus != StateStatus::ERROR)
		 mPtrVanishingPtFilter	= mPtrBootingState->createVanishingPtFilter();

		if (mPtrBootingState->currentStatus != StateStatus::ERROR)
		 mPtrTemplates		= mPtrBootingState->createTemplates();

		if (mPtrBootingState->currentStatus == StateStatus::DONE)
		{
		
		  mPtrBufferingState.reset
		  #ifdef S32V2XX
		   (new BufferingState<BufferingDAG_s32>());
		  #else
		   (new BufferingState<BufferingDAG_generic>());
		  #endif
		
		  mCurrentState 	= States::BUFFERING;
		  mPtrBootingState 	= nullptr;
		  cout<< "Completed!" <<endl;
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
		  mPtrBootingState 	= nullptr;
		}
	}
	break; //BOOTING PROCESS SCOPE ENDS



	case States::BUFFERING :
	{
		if (mPtrBufferingState->currentStatus == StateStatus::INACTIVE)
		{
		   lReturn |= mPtrBufferingState->setSource(mFrameSource, mSourceStr);

		   if (lReturn != 0)
		   {

		      #ifdef PROFILER_ENABLED
		       LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		       <<"*********************************"<<endl
		       <<  "[Failed to Setup the Frame Source]"<<endl
		       <<  "Shutting Down the State-Machine"<<endl
		       <<"******************************"<<endl;
		      #endif
		      mPtrBufferingState->preDispose();
		   }
		   else
		   {
		      mPtrBufferingState->setupDAG(std::ref(*mPtrTemplates));
		   }
		}

		if (mPtrBufferingState->currentStatus == StateStatus::ACTIVE)
		{
		   mPtrBufferingState->run();
		}

		if( mPtrBufferingState->currentStatus == StateStatus::DONE)
		{
		   mPtrTrackingState.reset
		   #ifdef S32V2XX
		    (new TrackingLaneState<TrackingLaneDAG_s32v>( move(mPtrBufferingState->mGraph) ) );
		   #else
		    (new TrackingLaneState<TrackingLaneDAG_generic>( move(mPtrBufferingState->mGraph) ) );
		   #endif
		
		   mCurrentState 	= States::DETECTING_LANES;
		   mPtrBufferingState 	= nullptr;

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
		   mPtrBufferingState 	= nullptr;
		   lReturn 	        = -1;
		}

	}
	break; // BUFFERING PROCESS SCOPE ENDS


	case States::DETECTING_LANES :
	{
		if (mPtrTrackingState->currentStatus == StateStatus::INACTIVE)
		{
		   mPtrTrackingState->setupDAG(mPtrLaneFilter.get(), mPtrVanishingPtFilter.get());
		}

		if (mPtrTrackingState->currentStatus == StateStatus::ACTIVE)
		{
		   mPtrTrackingState->run();
		}

		if( (mPtrTrackingState->currentStatus == StateStatus::DONE) )
		{
		   #ifdef PROFILER_ENABLED
		    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		    <<  "********************************"<<endl
		    <<  "[Tracking Finished]"<<endl
		    <<  "Shutting Down the State-Machine."<<endl
		    <<  "********************************"<<endl<<endl;
		   #endif

		   mCurrentState 	= States::DISPOSED;
		   mPtrTrackingState 	= nullptr;
		}
		if (mPtrTrackingState->currentStatus == StateStatus::ERROR)
		{
		   #ifdef PROFILER_ENABLED
		    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		    <<  "********************************"<<endl
		    <<  "[Tracking State Error]"<<endl
		    <<  "Shutting Down the State-Machine."<<endl
		    <<"******************************"<<endl<<endl;
		   #endif

		   lReturn 		= -1;
		   mCurrentState 	= States::DISPOSED;
		   mPtrTrackingState 	= nullptr;
		}


	}
	break; // TRACKING STATE SCOPE ENDS
	
	case States::DISPOSED:
	{
		#ifdef S32V2XX
		 OAL_Deinitialize();
		#endif

		#ifdef PROFILER_ENABLED
		 LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		 << "State Machine is Disposed"<< endl;
		#endif

		cout<<endl<<"Ended..." <<endl;
	}
	break; } // END SWITCH

	return lReturn;

}

bool StateMachine::isInitialized()
{
	return mInitialized;
}

void StateMachine::quit()
{
	mQuitRequest = true;
}

States StateMachine::getCurrentState()
{
	return mCurrentState;
}

StateMachine::~StateMachine()
{
	
}
