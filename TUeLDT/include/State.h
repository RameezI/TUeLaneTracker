#ifndef STATE_H
#define STATE_H

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
*
* ****************************************************************************/ 

#include <Config.h>
#include "LDT_profiler.h"
#include "LDT_logger.h"

enum States{BOOTING, BUFFERING, DETECTING_LANES, DISPOSING };
enum StateStatus {DONE, ACTIVE, INACTIVE, ERROR };
enum FrameSource {DIRECTORY, RTSP, GMSL};

class State
{
	
protected:

	#ifdef PROFILER_ENABLED
	ProfilerLDT mProfiler;
	#endif
	
public:
	int64_t 				StateCounter   =  0;
	StateStatus 	       	currentStatus  =  StateStatus::INACTIVE;
	
	void dispose()			{currentStatus = StateStatus::ERROR;}

	State(){}
	~State(){}
};
#endif
