#include "StateMachine.h"
#include "State.h"
using namespace std;
using namespace cv;


States StateMachine::sCurrentState = States::BOOTING;


/*Constructor*/
#ifdef DIRECTORY_INPUT    // ^TODO: Define Seprate Constructor for camera input which takes SDI object.
StateMachine::StateMachine(const vector<cv::String>& filenames) 
: 
  mFiles(filenames),
  
  mBootingState(new InitState()),
  
  mBufferingState(new BufferingState())

{
	#ifdef PROFILER_ENABLED
	const std::string str = filenames[0];
	LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
										 <<"******************************"<<endl
										 <<  "DIRECTORY_INPUT DEFINED." <<endl <<str<<endl
										 <<"******************************"<<endl<<endl;	
										 #endif
	
	
	#ifdef PROFILER_ENABLED
	LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
										 <<"******************************"<<endl
										 <<  "StateMachine Constructed" <<endl
										 <<"******************************"<<endl<<endl;	
										#endif						
}


#else

StateMachine::StateMachine()
: 
	mStateStrings({ "BOOTING", "BUFFERING", "DETECTING_LANES", "RESETING" })

{

	#ifdef PROFILER_ENABLED
	LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
										 <<"******************************"<<endl
										 <<  "StateMachine Constructed."   <<endl
										 <<"******************************"<<endl<<endl		
										 #endif										
}

#endif


int StateMachine::spin(shared_ptr<SigInit> sigInit)
{
	sCurrentState = States::BOOTING; //remove when confirmed
	
	while (sigInit->sStatus!=SigStatus::STOP)
	{
		
	
		
/**************************************************************************************************/
										// INITIALISING //
		
		switch (sCurrentState)
		{
		
		case  States::BOOTING : { 
									 if (mBootingState->currentStatus == StateStatus::ACTIVE) {
											mBootingState->run(); 
										}
											
/*Transition*/					else if (mBootingState->currentStatus == StateStatus::DONE) {  
											
											//^TODO: get all allocated memory Maps
											mBootingState->conclude();
											sCurrentState =	States::BUFFERING;
										}
										
									else {
										
/*Transition*/							    sCurrentState = States::DISPOSING;
										
										 }
		
								  }
		break;
		
		
/***************************************************************************************************/
											// BUFFERING //
		
		case States::BUFFERING : {
			
			
							if (mBufferingState->currentStatus == StateStatus::INACTIVE) {
								  
								  /*Inject Dependencies */
								  
									#ifdef DIRECTORY_INPUT 
										mBufferingState->setSource(mFiles);
									#else
									    mBufferingState->setSource();
									#endif
									
									mBufferingState->setTemplates(std::move(mBootingState->templates));										
							 }
										
											
							else  if (mBufferingState->currentStatus  == StateStatus::ACTIVE) {  
											
											mBufferingState->run();
										}
										
/*Transition*/				else  if(mBufferingState->currentStatus  == StateStatus::DONE )  {
	
	
										    mBufferingState->conclude();										
										    sCurrentState = States::DETECTING_LANES;
										 
										 }
								
/*Transition*/						else {
									
											sCurrentState = States::DISPOSING;
		
										 }

		 						 }
		break;
/***********************************************************************************************************/		  
		}
		
	}
	
	
	return 0;
}

StateMachine::~StateMachine()
{
	
}

