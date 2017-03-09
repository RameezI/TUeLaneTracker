#include "StateMachine.h"
#include "State.h"
using namespace std;
using namespace cv;


States StateMachine::sCurrentState = States::BOOTING;


/*Constructor*/
#ifdef DIRECTORY_INPUT    // ^TODO: Define Seprate Constructor for camera input which takes SDI object.
StateMachine::StateMachine(const vector<cv::String>& filenames) 
: mFiles(filenames),
  mStateStrings({ "BOOTING", "BUFFERING", "DETECTING_LANES", "RESETING" })
  #ifdef PROFILER_ENABLED
  , mDisposed(false)  
//  #ifndef s32v2xx
//  , mUserInterface(NcursesTUI::instance())
//  #endif	
  #endif


{
	#ifdef PROFILER_ENABLED
	const std::string str = filenames[0];
	LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
										 <<"******************************"<<endl
										 <<  "DIRECTORY_INPUT DEFINED." <<endl <<str<<endl
										 <<"******************************"<<endl<<endl;	
										 #endif
	
	
	//^NOTE: STATE machine should run staes indefinitely unless interruted by user signal
	mLaneFilter			=	make_shared<LaneFilter>(mLane.mPROPERTIES, mCamera.mCM_TO_PIXEL);
    mVanishingPtFilter 	= 	make_shared<VanishingPtFilter>(mLaneFilter->mBINS_HISTOGRAM, mCamera.mPROPERTIES);
	
	
	
	#ifdef PROFILER_ENABLED
	mThreadProfiler = std::thread(std::bind(&StateMachine::PrintMachineInfo,  this));	
	LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
										 <<"******************************"<<endl
										 <<  "StateMachine Constructed" <<endl
										 <<"******************************"<<endl<<endl;	
										#endif
}



#else

StateMachine::StateMachine()
: mStateStrings({ "BOOTING", "BUFFERING", "DETECTING_LANES", "RESETING" })

{
	
	mLaneFilter			=	make_shared<LaneFilter>(mLane.mPROPERTIES, mCamera.mCM_TO_PIXEL);
    mVanishingPtFilter 	= 	make_shared<VanishingPtFilter>(mLaneFilter->mBINS_HISTOGRAM, mCamera.mPROPERTIES);

	
	#ifdef PROFILER_ENABLED
	mThreadProfiler = std::thread(std::bind(&StateMachine::PrintMachineInfo,  this));
	LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
										 <<"******************************"<<endl
										 <<  "StateMachine Constructed."   <<endl
										 <<"******************************"<<endl<<endl		
										 #endif
}

#endif


int StateMachine::run(shared_ptr<SigInit> sigInit)
{

		InitState 			booting(mCamera.mPROPERTIES);
		BufferingState		buffering(mCamera.mPROPERTIES, mLane.mMembership);
		
		
		
		//return 0;
	
	while (sigInit->sStatus!=SigStatus::STOP)
	{
		
	
		
/**************************************************************************************************/
										// INITIALISING //
		
		switch (sCurrentState)
		{
		
		case  States::BOOTING : { 
									 if (booting.mStateStatus == StateStatus::ACTIVE) {
											booting.run(); 
										}
											
/*Transition*/					else if (booting.mStateStatus == StateStatus::DONE) {  
											
											//^TODO: get all allocated memory Maps
											booting.conclude();
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
			
			
							if (buffering.mStateStatus == StateStatus::INACTIVE) {
								  
								  /*Inject Dependencies */
								  
									#ifdef DIRECTORY_INPUT 
										buffering.setSource(mFiles);
									#endif
									
									
									buffering.injectDependencies(booting.mVanishingPt,
																 booting.mTemplates,  
																 booting.mLikelihoods
																 );										
							 }
										
											
							else  if (buffering.mStateStatus  == StateStatus::ACTIVE) {  
											
											buffering.run();
										}
										
/*Transition*/				else  if(buffering.mStateStatus  == StateStatus::DONE )  {
	
	
										    buffering.conclude();										
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
	
	
	
#ifdef PROFILER_ENABLED
      mDisposed.store(true);
	if(mThreadProfiler.joinable()) {
		//^TODO: Write to log File, and check if thread is joined.
            mThreadProfiler.join();
	}
#endif

  
	
	return 0;
}



void StateMachine::PrintMachineInfo()
{
	#ifdef PROFILER_ENABLED
	LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
										 <<"******************************"<<endl
										 <<  "Log written from sepetate thread."   <<endl
										 <<"******************************"<<endl<<endl;	
	#endif
	
	//^NOTE: This is read-only function of the class. Memebers must only be read here no write operations!
	//^TODO: Make the "this" pointer constant so that compiler can raise errors if one try to modify members from the printing thread
	//^TODO: Design a tabular output, for better viewing and online viewing of results.
	//^TODO: Use NCURSES to update the table periodically
/*	
	while (!mDisposed.load())
	{
	  
		cout<<endl<<endl<<endl;
		std::cout<<"******************************"<<std::endl;
		std::cout<<"         Status Update"<<std::endl;
		std::cout<<"******************************"<<std::endl;
		cout<<"Current State :	" << mStateStrings[sCurrentState] ;
	  
	}
*/
	
}

StateMachine::~StateMachine()
{
	
}

