#ifndef BUFFERING_STATE_H
#define BUFFERING_STATE_H

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

#include <thread>
#include "State.h"

#ifdef S32V2XX
 #include "BufferingDAG_s32v.h"
#else
 #include "BufferingDAG_generic.h"
#endif

template<typename GRAPH>
class BufferingState : public State
{
	
private:
	size_t       	mBufferSize;

public:
	BufferingState(const LaneTracker::Config& Config);

	GRAPH 	mGraph;
	void 	setupDAG(const Templates& TEMPLATES, const size_t& BUFFER_SIZE);
	void 	run(cv::UMat Frame);
};


//******************************************************************/*
// Class Definitions
//Templated class thus declarations and definitions in a single file
//******************************************************************/

template<typename GRAPH>
BufferingState<GRAPH>::BufferingState(const LaneTracker::Config& Config)
: mBufferSize(0), mGraph(Config)
{}


template<typename GRAPH>  
void BufferingState<GRAPH>::setupDAG(const Templates & TEMPLATES, const size_t& BUFFER_SIZE)
{
  if(BUFFER_SIZE <= 0)
  {
    currentStatus = StateStatus::ERROR;
  }
  else if ( 0 == mGraph.init_DAG(TEMPLATES, BUFFER_SIZE) )
  {
    mBufferSize = BUFFER_SIZE;
    currentStatus= StateStatus::ACTIVE;
  }
}

template<typename GRAPH>  
void BufferingState<GRAPH>::run(cv::UMat Frame)
{
  try
  {
    mGraph.execute(Frame);
    this->StateCounter++;
  }
  catch(const char* msg)
  {
	#ifdef PROFILER_ENABLED
	  LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	  <<"******************************"<<endl
	  << "Buffering Execution Failed" <<endl
	  << msg <<endl
	  <<"******************************"<<endl<<endl;
	 #endif
	currentStatus = StateStatus::ERROR;
  }
  catch(...)
  {
    currentStatus = StateStatus::ERROR;
  }

  if(StateCounter >= mBufferSize-1)
      currentStatus = StateStatus::DONE;
}

#endif // BUFFERING_STATE_H
