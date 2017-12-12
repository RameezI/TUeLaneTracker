#include "StateMachine.h"
using namespace std;
using namespace cv;


/********************************************//**
 * \brief sCurrentState initialisation. 
 *  The static variable is set initialy to States::Booting
 * ***********************************************/
States StateMachine::sCurrentState = States::BOOTING;


 
StateMachine::StateMachine(FrameSource lFrameSource, string lSourceStr)

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
	unique_ptr<TrackingLaneState>   pTrackingState;


	/* BOOTING PROCESS */
	{				
	   InitState		bootingState;

	   pLaneFilter 		= bootingState.createLaneFilter();
	   pVanishingPtFilter	= bootingState.createVanishingPtFilter();
	   pTemplates           = bootingState.createTemplates();
					
	   if (bootingState.currentStatus == StateStatus::DONE)						
		sCurrentState = States::BUFFERING;	
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
		sCurrentState = States::DISPOSING;
	   }							
	} //booting process block ends
			
			
	 /* BUFFERING PROCESS */
	if (sCurrentState == States::BUFFERING)
	{
	   BufferingState<BufferingDAG_generic>  lBufferingState;
			
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
	   	sCurrentState = States::DETECTING_LANES;
		pTrackingState.reset(new TrackingLaneState(move(lBufferingState.mGraph)));				
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
		sCurrentState = States::DISPOSING;
	   }	
		
	}// Buffering process block ends
		
		
	/* TRACKING LANE PROCESS*/
	if (sCurrentState==States::DETECTING_LANES)
	{

	   TrackingLaneState& trackingState = *pTrackingState;

	   if (trackingState.currentStatus == StateStatus::INACTIVE)
	   {			
		trackingState.setupDAG(pLaneFilter.get(), pVanishingPtFilter.get());
	   }
	
	   while (trackingState.currentStatus == StateStatus::ACTIVE)
	   {
		trackingState.run();
		
		if (sigInit->sStatus==SigStatus::STOP)
		{
		   #ifdef PROFILER_ENABLED
			LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   	<< "****************************************"<<endl
		   	<<  "[Tracking  Process Interrupted by User]"<<endl
			<<  "Shutting Down the State-Machine"	     <<endl
			<< "***************************************"<<endl;
		   #endif 
		   trackingState.dispose();
		}
	   }
			
	   if( trackingState.currentStatus == StateStatus::DONE)
	   {
		#ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   <<  "********************************"<<endl
		   <<  "[Tracking Finished]"<<endl
		   <<  "Shutting Down the State-Machine."<<endl
		   <<  "********************************"<<endl<<endl;
		#endif 
	   	sCurrentState = States::DISPOSING;
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
		sCurrentState = States::DISPOSING;
	   }

	} // TrackingState process block ends	
		
		
	/* Shutting Down */
	if(sCurrentState==States::DISPOSING)
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
