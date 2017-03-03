#include "LDT_logger.h" 
ProfilerStatus Logger::mProfilerStatus= ProfilerStatus::NOT_INITIALISED;


void Logger::Init()
	{
		if (mProfilerStatus==ProfilerStatus::NOT_INITIALISED)
			{		
				plog::init<LDTLog::STATE_MACHINE_LOG>(plog::info, "Log_StateMachine"); //Initialize the logger.
				plog::init<LDTLog::BUFFERING_LOG>(plog::info, "Log_Buffering"); //Initialize the logger.
				plog::init<LDTLog::BUFFERING_PROFILE>(plog::debug, "Profile_Buffering"); 		   
				plog::init<LDTLog::BOOTING_PROFILE>(plog::debug,   "Profile_Booting"); 		  
				mProfilerStatus = ProfilerStatus::INITIALISED;
			}
		
	}