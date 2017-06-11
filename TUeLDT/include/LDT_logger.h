#ifndef LDTLOGGER_H
#define LDTLOGGER_H
#include <plog/Log.h>


enum LDTLog {STATE_MACHINE_LOG=0,BOOTING_STATE_LOG, BUFFERING_STATE_LOG, TRACKING_STATE_LOG, TIMING_PROFILE};
enum ProfilerStatus{NOT_INITIALISED=-1, INITIALISED};
class Logger
{
	
private:
	static ProfilerStatus mProfilerStatus;
	
public:		
	static void Init();

};
#endif

