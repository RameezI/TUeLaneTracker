#include "StateMachine.h"
#include "State.h"
using namespace std;
using namespace cv;
States StateMachine::sCurrentState = States::BOOTING;

 
StateMachine::StateMachine() 
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
	// These pointers have life until the Span of this function
	unique_ptr<LaneFilter>  	pLaneFilter;
	unique_ptr<VanishingPtFilter>   pVanishingPtFilter;
	unique_ptr<Templates> 		pTemplates;
	unique_ptr<TrackingLaneState>   pTrackingState;


	/* BOOTING PROCESS */
	{				
	   InitState	bootingState;

	   pLaneFilter 		= bootingState.createLaneFilter();
	   pVanishingPtFilter	= bootingState.createVanishingPtFilter();
	   pTemplates           = bootingState.createTemplates();
					
	   if (bootingState.currentStatus == StateStatus::DONE)						
		sCurrentState = States::BUFFERING;	
	   else 
	   {
		#ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   <<"******************************"<<endl
		   <<  "Failed to Complete the Booting Process"<<endl
		   <<  "Shutting Down the State-Machine."<<endl
		   <<"******************************"<<endl<<endl;
		#endif 
		sCurrentState = States::DISPOSING;
	   }							
	} 
			
			
	/* BUFFERING PROCESS */
	if (sCurrentState == States::BUFFERING)
	{
	   BufferingState  bufferingState;
			
	   if (bufferingState.currentStatus == StateStatus::INACTIVE)
	   {			
	   	int lReturn = bufferingState.setSource();
		
		if (lReturn != 0)
		{	
		   #ifdef PROFILER_ENABLED
		  	LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   	<<"******************************"<<endl
		   	<<" Failed to Setup the Source..."<<endl
			<<"******************************"<<endl;
		   #endif 

		   bufferingState.dispose();
		}
		else
		{
		   bufferingState.setupDAG(std::ref(*pTemplates));		
		}
	   }
	
	   if (bufferingState.currentStatus == StateStatus::ACTIVE)
	   {	
		bufferingState.run();

		if (sigInit->sStatus==SigStatus::STOP)
		{
		   #ifdef PROFILER_ENABLED
			LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   	<<"***************************************"<<endl
		   	<<" Buffering Process Intrupred by User   "<<endl
			<<"*************************************"<<endl;
		   #endif 
	   	   bufferingState.dispose();
		}
	   }
			
	   if( bufferingState.currentStatus == StateStatus::DONE)
	   {
	   	sCurrentState = States::DETECTING_LANES;
		pTrackingState.reset(new TrackingLaneState(move(bufferingState.bufferingGraph)));				
	   }
			
	   else
	   {
		#ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   <<"******************************"<<endl
		   <<  "Shutting Down the State-Machine."<<endl
		   <<"******************************"<<endl<<endl;
		#endif 
		sCurrentState = States::DISPOSING;
	   }	
		
	} 
		
		
	/* Tracking Lanes */
	if (sCurrentState==States::DETECTING_LANES)
	{

	   TrackingLaneState& trackingState = *pTrackingState;

	   if (trackingState.currentStatus == StateStatus::INACTIVE)
	   {			
		trackingState.setupDAG(pLaneFilter.get(), pVanishingPtFilter.get());
	   }
	
	   if (trackingState.currentStatus == StateStatus::ACTIVE)
	   {
		trackingState.run();
		
		if (sigInit->sStatus==SigStatus::STOP)
		{
		   #ifdef PROFILER_ENABLED
			LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   	<<"******************************"<<endl
		   	<<  "Tracking  Process Interrupted by User.."<<endl;
		   #endif 
		   trackingState.dispose();
		}
	   }
			
	   if( trackingState.currentStatus == StateStatus::DONE)
	   {
		#ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   <<"******************************"<<endl
		   <<  "Tracking Finished"           <<endl
		   <<  "Shutting Down the State-Machine."<<endl
		   <<"******************************"<<endl<<endl;
		#endif 
	   	sCurrentState = States::DISPOSING;
	   }
	   else
	   {
			
		#ifdef PROFILER_ENABLED
		   LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		   <<"******************************"<<endl
		   <<  "Shutting Down the State-Machine."<<endl
		   <<"******************************"<<endl<<endl;
		#endif 
		sCurrentState = States::DISPOSING;
	   }

	}	
		
		
	/* Shutting Down */
	if(sCurrentState==States::DISPOSING)
	{				
	   cout<< "State Machine is in Disposed State"<< endl;
		
	   #ifdef PROFILER_ENABLED
		cout<< "See the log to inquire what caused this shutdown"<<endl;
	   #endif 

	   return 0;
	}
	
	else
	   return 0;
}

StateMachine::~StateMachine()
{
	
}
