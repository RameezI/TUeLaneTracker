#include "StateMachine.h"
#include "State.h"
using namespace std;
using namespace cv;
States StateMachine::sCurrentState = States::BOOTING;


// ^TODO: Define Seprate Constructor for camera input which takes SDI object.


/*Constructor*/
#ifdef DIRECTORY_INPUT  
 
	StateMachine::StateMachine(const vector<cv::String>& filenames) 
	: mFiles(filenames)

	{
		#ifdef PROFILER_ENABLED
		const std::string str = filenames[0];
		LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
			 <<"******************************"<<endl
			 <<  "DIRECTORY_INPUT DEFINED."	   <<endl
			 <<"******************************"<<endl<<endl;	
			 #endif
		
		
		#ifdef PROFILER_ENABLED
		LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
			 <<"******************************" <<endl
			 <<  "StateMachine Constructed"     <<endl
			 <<"******************************" <<endl<<endl;
			#endif						
	}

#else

	StateMachine::StateMachine()
	{
		#ifdef PROFILER_ENABLED
		LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
			 <<"******************************"<<endl
			 <<  "StateMachine Constructed."   <<endl
			 <<"******************************"<<endl<<endl;	
			 #endif										
	}

#endif


	int StateMachine::spin(shared_ptr<SigInit> sigInit)
	{	
		// These pointers have life until the Span of this function
		unique_ptr<LaneFilter>  		pLaneFilter;
		unique_ptr<VanishingPtFilter>   pVanishingPtFilter;
		unique_ptr<Templates> 			pTemplates;
		unique_ptr<TrackingLaneState>   pTrackingState = NULL;


		/* BOOTING PROCESS */
		{				
			InitState		bootingState;

			pLaneFilter 		= bootingState.createLaneFilter();
			pVanishingPtFilter	= bootingState.createVanishingPtFilter();
			pTemplates           	= bootingState.createTemplates();
					
			if (bootingState.currentStatus == StateStatus::DONE)						
				sCurrentState = States::BUFFERING;	
			else 
				sCurrentState = States::DISPOSING;							
		} 
			
			
		/* BUFFERING PROCESS */
		if (sCurrentState == States::BUFFERING)
		{
			BufferingState  bufferingState; 
			
			if (bufferingState.currentStatus == StateStatus::INACTIVE)
			{			
				/*Inject Dependencies for Buffering State */								  
				#ifdef DIRECTORY_INPUT 
					bufferingState.setSource(mFiles);
				#else
					bufferingState.setSource();
				#endif
						
				bufferingState.setupDAG(std::ref(*pTemplates));
			}
	
			while (bufferingState.currentStatus == StateStatus::ACTIVE)
				bufferingState.run();
				
			
			if( bufferingState.currentStatus == StateStatus::DONE)
			{
			    sCurrentState = States::DETECTING_LANES;
			    pTrackingState.reset(new TrackingLaneState(move(bufferingState.bufferingGraph)));				
			}
			
			else
			    sCurrentState = States::DISPOSING;
				
		} 
		
		
		/* Tracking Lanes */
		if(sCurrentState==States::DETECTING_LANES)
		{
			TrackingLaneState& trackingState = *pTrackingState;

			if (trackingState.currentStatus == StateStatus::INACTIVE)			
			trackingState.setupDAG(pLaneFilter.get(), pVanishingPtFilter.get());
		
		
			while (trackingState.currentStatus == StateStatus::ACTIVE)
			{
				trackingState.run();
				if (sigInit->sStatus==SigStatus::STOP)
				trackingState.conclude();
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
				return 0;				
			 }
			 else
				sCurrentState = States::DISPOSING;
		}	
		
		
		/* Shutting Down */
		if(sCurrentState==States::DISPOSING)
		{
				#ifdef PROFILER_ENABLED
				 LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
				 <<"******************************"<<endl
				 <<  "Shutting Down the State-Machine."   <<endl
				 <<"******************************"<<endl<<endl;
				#endif 
				return -1;			
		}

	}

StateMachine::~StateMachine()
{
	
}

