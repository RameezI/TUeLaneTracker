#include "sigInit.h"


SigStatus SigInit::sStatus= SigStatus::INIT;

SigInit::SigInit() 
{
	 // prepare internal signal handler
  struct sigaction lSa;
  memset(&lSa, 0, sizeof(lSa));
  lSa.sa_handler = handler;
  
  if( sigaction(SIGINT, &lSa, NULL) != 0)
  {
	#ifdef PROFILER_ENABLED   
    VDB_LOG_ERROR("Failed to register signal handler.\n");
	#endif
	sStatus= SigStatus::FAILURE;
  } // if signal not registered
	
}
SigInit::~SigInit()
{
	sStatus = SigStatus::INIT;
	
}

void SigInit::handler(int aSigNo)
{
  sStatus = SigStatus::STOP;
} 
