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
	std::thread  	mSideExecutor;

public:
	BufferingState();

	GRAPH 	mGraph;
	void 	setupDAG(const Templates& TEMPLATES, const size_t & BUFFER_SIZE);
	void 	run(cv::Mat Frame);

	~BufferingState();
};


//******************************************************************/*
// Class Definitions
//Templated class thus declarations and definitions in a single file
//******************************************************************/

template<typename GRAPH>
BufferingState<GRAPH>::BufferingState()
: mBufferSize(0){

}


template<typename GRAPH>  
void BufferingState<GRAPH>::setupDAG(const Templates & TEMPLATES, const size_t & BUFFER_SIZE)
{

#ifdef PROFILER_ENABLED
mProfiler.start("SET_UP_BUFFERING_DAG");
#endif
	if(BUFFER_SIZE <= 0)
	{
	   this->currentStatus = StateStatus::ERROR;
	}
	else if ( 0 == mGraph.init_DAG(TEMPLATES, BUFFER_SIZE) )
	{
	   mBufferSize = BUFFER_SIZE;
	   this->currentStatus= StateStatus::ACTIVE;
	}

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
				 <<"******************************"<<endl
				 <<  "Setting up Buffering Graph." <<endl
				 <<  "Setup Time: "  << mProfiler.getAvgTime("SET_UP_BUFFERING_DAG")<<endl
				 <<"******************************"<<endl<<endl;	
				 #endif
}



template<typename GRAPH>  
void BufferingState<GRAPH>::run(cv::Mat Frame)
{
   if (mSideExecutor.joinable())
      mSideExecutor.join();

   mSideExecutor = std::thread(&GRAPH::runAuxillaryTasks, std::ref(mGraph));
   mGraph.execute(Frame);

   this->StateCounter++;

   if(this->StateCounter >= mBufferSize-1)
   {
      this->currentStatus = StateStatus::DONE;
   }	
}


template<typename GRAPH>  
BufferingState<GRAPH>::~BufferingState()
{
   if (mSideExecutor.joinable())
      mSideExecutor.join();	
}

#endif // BUFFERING_STATE_H
