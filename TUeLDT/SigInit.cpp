
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


#include "SigInit.h"


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
