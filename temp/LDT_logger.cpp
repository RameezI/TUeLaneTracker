#include "LDT_logger.h" 
ProfilerStatus Logger::mProfilerStatus= ProfilerStatus::NOT_INITIALISED;


void Logger::Init()
	{
	  if (mProfilerStatus==ProfilerStatus::NOT_INITIALISED)
	  {		
		plog::init<LDTLog::STATE_MACHINE_LOG>(plog::info,   "Log_StateMachine"); //Initialize the logger.
		plog::init<LDTLog::BOOTING_STATE_LOG>(plog::info,   "Log_Booting"); //Initialize th
		plog::init<LDTLog::BUFFERING_STATE_LOG>(plog::info, "Log_Buffering"); //Initialize the logger.
		plog::init<LDTLog::TRACKING_STATE_LOG>(plog::debug, "Log_Tracking");
		plog::init<LDTLog::TIMING_PROFILE>(plog::debug,     "Profile_ExecutionTime"); 		   				 		  
		mProfilerStatus = ProfilerStatus::INITIALISED;
	  }
		
	}
