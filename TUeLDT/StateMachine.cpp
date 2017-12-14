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
#include "InitState.h"
#include "BufferingState.h"
#include "TrackingLaneState.h"


#ifdef   S32V2XX
 #include "BufferingDAG_s32v.h"
 #include "TrackingLaneDAG_s32v.h"
#else
 #include "BufferingDAG_generic.h"
 #include "TrackingLaneDAG_generic.h"
#endif
 


StateMachine::StateMachine(FrameSource lFrameSource, std::string lSourceStr)

: mFrameSource(lFrameSource),
  mSourceStr(lSourceStr) 
{
	#ifdef PROFILER_ENABLED
		#ifdef DIRECTORY_IPUT
		LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		<<"******************************"<<endl
		<<  "DIRECTORY_INPUT DEFINED."	  <<endl
		<<"******************************"<<endl<<endl;
		#endif
	#endif		
}




int StateMachine::spin(shared_ptr<SigInit> sigInit)
{

	int lReturn = 0;
	
	// The memory allocated by these pointers have life until the span of this function
	// i.e. until the State Machine is spinning
	unique_ptr<LaneFilter>  	pLaneFilter;
	unique_ptr<VanishingPtFilter>   pVanishingPtFilter;
	unique_ptr<Templates> 		pTemplates;

	#ifdef S32V2XX
	 unique_ptr<TrackingLaneState<TrackingLaneDAG_s32v>>   	 pTrackingState;
	#else
	 unique_ptr<TrackingLaneState<TrackingLaneDAG_generic>>  pTrackingState;
	#endif



	// BOOTING PROCESS //
	{				
	   InitState		bootingState;

	   pLaneFilter 		= bootingState.createLaneFilter();
	   pVanishingPtFilter	= bootingState.createVanishingPtFilter();
	   pTemplates           = bootingState.createTemplates();
					
	   if (bootingState.currentStatus == StateStatus::DONE)						
		mCurrentState = States::BUFFERING;	
	   else 
	   {
		#ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   <<"****************************************"<<endl
		   <<  "[Failed to Complete the Booting Process]"<<endl
		   <<  "Shutting Down the State-Machine."<<endl
		   <<"******************************"<<endl<<endl;
		#endif
		lReturn =-1; 
		mCurrentState = States::DISPOSING;
	   }							
	} //booting process block ends
			



			
	// BUFFERING PROCESS //
	if (mCurrentState == States::BUFFERING)
	{

	   #ifdef S32V2XX
	    BufferingState<BufferingDAG_s32v>  	 lBufferingState;
	   #else
	    BufferingState<BufferingDAG_generic> lBufferingState;
	   #endif
	
	   if (lBufferingState.currentStatus == StateStatus::INACTIVE)
	   {			
	   	lReturn |= lBufferingState.setSource(mFrameSource, mSourceStr);
		
		if (lReturn != 0)
		{	
		   #ifdef PROFILER_ENABLED
		  	LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   	<<"*********************************"<<endl
		   	<<  "[Failed to Setup the Source]"<<endl
			<<  "Shutting Down the State-Machine"<<endl
			<<"******************************"<<endl;
		   #endif 
		
		   lBufferingState.dispose();
		}
		else
		{
		   lBufferingState.setupDAG(std::ref(*pTemplates));
		}
	   }
	
	   while (lBufferingState.currentStatus == StateStatus::ACTIVE)
	   {	
		lBufferingState.run();

		if (sigInit->sStatus==SigStatus::STOP)
		{
		   #ifdef PROFILER_ENABLED
			LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   	<<"***************************************"<<endl
		   	<<  "[Buffering Process Intrupted by the user]"<<endl
			<<  "Shutting Down the State-Machine"	   <<endl
			<<"***************************************"<<endl;
		   #endif 
	   	   
		   lBufferingState.dispose();
		}
	   }
			
	   if( lBufferingState.currentStatus == StateStatus::DONE)
	   {
	   	mCurrentState = States::DETECTING_LANES;

		pTrackingState.reset
		#ifdef S32V2XX
		 (new TrackingLaneState<TrackingLaneDAG_s32v>(move(lBufferingState.mGraph)));
		#else
		 (new TrackingLaneState<TrackingLaneDAG_generic>(move(lBufferingState.mGraph)));
		#endif
	   }
			
	   else
	   {
		#ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   <<"******************************"<<endl
		   << "[Buffering State Error]"<<endl
		   <<  "Shutting Down the State-Machine."<<endl
		   <<"******************************"<<endl<<endl;
		#endif
		lReturn	= -1; 
		mCurrentState = States::DISPOSING;
	   }	
		
	}// Buffering process block ends
		
		





	// TRACKING LANE PROCESS //
	if (mCurrentState==States::DETECTING_LANES)
	{

	   #ifdef S32V2XX
	    TrackingLaneState<TrackingLaneDAG_s32v>&    lTrackingState 	= *pTrackingState;
	   #else
	    TrackingLaneState<TrackingLaneDAG_generic>& lTrackingState 	= *pTrackingState;
	   #endif
	
	  
	   if (lTrackingState.currentStatus == StateStatus::INACTIVE)
	   {			
		lTrackingState.setupDAG(pLaneFilter.get(), pVanishingPtFilter.get());
	   }
	
	   while (lTrackingState.currentStatus == StateStatus::ACTIVE)
	   {
		lTrackingState.run();
		
		if (sigInit->sStatus==SigStatus::STOP)
		{
		   #ifdef PROFILER_ENABLED
			LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   	<< "****************************************"<<endl
		   	<<  "[Tracking  Process Interrupted by User]"<<endl
			<<  "Shutting Down the State-Machine"	     <<endl
			<< "***************************************"<<endl;
		   #endif 
		   lTrackingState.dispose();
		}
	   }
			
	   if( lTrackingState.currentStatus == StateStatus::DONE)
	   {
		#ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   <<  "********************************"<<endl
		   <<  "[Tracking Finished]"<<endl
		   <<  "Shutting Down the State-Machine."<<endl
		   <<  "********************************"<<endl<<endl;
		#endif 
	   	mCurrentState = States::DISPOSING;
	   }
	   else
	   {		
		#ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   <<  "********************************"<<endl
		   <<  "[Tracking State Error]"<<endl
		   <<  "Shutting Down the State-Machine."<<endl
		   <<"******************************"<<endl<<endl;
		#endif 
		lReturn = -1;
		mCurrentState = States::DISPOSING;
	   }
	
	} // TrackingState process block ends

		
		
	/* Shutting Down */
	if(mCurrentState==States::DISPOSING)
	{				
	   
	   #ifdef PROFILER_ENABLED
	   	cout<< "State Machine is Disposed"<< endl;
		cout<< "See the log to inquire what caused this shutdown"<<endl;
	   #endif 
	}
	
	return lReturn;
}

StateMachine::~StateMachine()
{
	
}
