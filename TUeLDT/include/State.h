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

enum States{BOOTING, BUFFERING, DETECTING_LANES, DISPOSED };
enum StateStatus {DONE, ACTIVE, INACTIVE, ERROR };
enum FrameSource {DIRECTORY, STREAM, GMSL};

inline std::ostream& operator<<(std::ostream& out, const States& state)
{
	if(state == States::BOOTING)
	  out<<"[BOOTING]	";
	else if (state == States::BUFFERING)
	  out<<"[BUFFERING]	";
	else if (state == States::DETECTING_LANES)
	  out<<"[TRACKING]	";
	else if (state == States::DISPOSED)
	  out<<"[DISPOSED]	";
	else
	  out << "[UNKNOWN] 	";
	return out;	
}

inline std::ostream& operator<<(std::ostream& out, const StateStatus& stateStatus)
{
	if(stateStatus == StateStatus::DONE)
	  out<<"DONE!";
	else if (stateStatus == StateStatus::ACTIVE)
	  out<<"ACTIVE";
	else if (stateStatus == StateStatus::INACTIVE)
	  out<<"INACTIVE";
	else if (stateStatus == StateStatus::ERROR)
	  out<<"ERROR!";
	else
	  out <<"UNKNOWN!";
	return out;	
}


inline std::istream& operator>>(std::istream& in, FrameSource& frameSource)
{
	std::string token;
	in >> token;
	
	if (token.compare("imgstore")==0)
	  frameSource = FrameSource::DIRECTORY;

	else if (token.compare("stream")==0)
	  frameSource = FrameSource::STREAM;

	else if (token.compare("gmsl")==0)
	  frameSource = FrameSource::GMSL;

	else
	  in.setstate(std::ios_base::failbit);
	
	return in;
}
 
inline std::ostream& operator<<(std::ostream& out, const FrameSource& frameSource)
{
	if(frameSource == FrameSource::DIRECTORY)
	  out<<"imgstore";
	else if (frameSource == FrameSource::STREAM)
	  out<<"stream";
	else if (frameSource == FrameSource::GMSL)
	  out<<"gmsl";
	else
	  out << "unknown-source";
	
	return out;	
}


class State
{
	
protected:

	#ifdef PROFILER_ENABLED
	ProfilerLDT mProfiler;
	#endif
	
public:
	uint64_t 		StateCounter   =  0;
	StateStatus 	       	currentStatus  =  StateStatus::INACTIVE;
	
	void preDispose()	{currentStatus = StateStatus::ERROR;}

	State(){}
	~State(){}
};
#endif
