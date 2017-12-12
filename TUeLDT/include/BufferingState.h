#ifndef BUFFERINGSTATE_H
#define BUFFERINGSTATE_H

#include "State.h"
#include <thread>

#ifdef S32V2XX
#include "BufferingDAG_s32v.h"
#else
#include "BufferingDAG_generic.h"
#endif



using namespace cv;
using namespace Eigen;
using namespace std;


template<typename GRAPH>
class BufferingState : public State
{
	
private:
	uint_fast8_t mRetryGrab;
	std::thread mSideExecutor;

public:
	GRAPH 	mGraph;

public:		
	BufferingState();
	int  setSource(FrameSource, string);
	void setupDAG(const Templates& templates);
	void run();
	~BufferingState();
};


//******************************************************************/*
// Class Definitions
//Templated class thus declarations and definitions in a single file
//******************************************************************/

template<typename GRAPH>
BufferingState<GRAPH>::BufferingState()
:mRetryGrab(0)
{

}

template<typename GRAPH>  
int BufferingState<GRAPH>::setSource(FrameSource lSource, string lSourceStr)
{
	int lReturn=0;

	if (lSource == FrameSource::DIRECTORY)
	{
           cv::String lFolder = lSourceStr;
	   vector< cv::String> lFiles;
           
	   try
	   {
		glob(lFolder, lFiles);
	   }

	   catch(...)
	   {
		lReturn = -1;
	   }

           const uint lSkipFrames = 4000;  
         
      	   if (lFiles.size() <= lSkipFrames)
           {
         	cout<<endl;
         	cout<<"Total Number of Image Files to Process : " << 0;
         	cout<<endl;
         	lReturn = -1;
           }

	   else
	   {
     	   	cout<<endl;
           	cout<<"Total Number of Image Files to Process : " << lFiles.size() - lSkipFrames;
           	cout<<endl;	
	   }

	   mGraph.mSource = lSource;
	   mGraph.mFiles = lFiles;
	   mGraph.mFrameCount =lSkipFrames;
	}

	else if(lSource == FrameSource::RTSP)
	{

		cout<< "RTSP MODE"<<endl;
		mGraph.mSource = lSource;
	   	mGraph.mFrameCount = 0;
		
		try
		{
		   //if(!mGraph.mRTSP_CAPTURE.open("rtsp://192.168.8.1:8554/test"))
		   if(!mGraph.mRTSP_CAPTURE.open(lSourceStr))
		   lReturn = -1;
		}

		catch(...)
		{
		    lReturn = -1;
		}
	}

	else if (lSource == FrameSource::GMSL)
	{
		cout<< "GMSL MODE" << endl;

	
		cout<< "This Mode is not yet implemented"<< endl;
		cout<< "Exitting"<<endl;
		lReturn =-1;
	}

	else
	{
		cout << "The input mode is not recognised" << endl;
		cout << "Exitting"<<endl;
		lReturn =-1;
	}

	return lReturn;

}




template<typename GRAPH>  
void BufferingState<GRAPH>::setupDAG(const Templates& templates)
{
	
#ifdef PROFILER_ENABLED
mProfiler.start("SET_UP_BUFFERING_DAG");
#endif

	const int RES_H 	= mGraph.mCAMERA.RES_VH(1);
	
	mGraph.mVP_Range_V   	= templates.VP_RANGE_V;
	mGraph.mSpan		= templates.SPAN;
	mGraph.mMargin		= templates.MARGIN;
	
	//allocate buffers
	mGraph.mBufferPool.reset(new BufferPool(templates.SPAN, RES_H, sNbBuffer)); 

	// assign templates
	mGraph.mGRADIENT_TAN_ROOT = templates.GRADIENT_TAN_ROOT;
	mGraph.mDEPTH_MAP_ROOT    = templates.DEPTH_MAP_ROOT;
	mGraph.mFOCUS_MASK_ROOT   = templates.FOCUS_MASK_ROOT;
	mGraph.mY_ICS	 	  = templates.Y_ICS;
	mGraph.mX_ICS    	  = templates.X_ICS;

	if ( 0 == mGraph.init_DAG() )
	this->currentStatus= StateStatus::ACTIVE;
		
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
void BufferingState<GRAPH>::run()
{
	
#ifdef PROFILER_ENABLED
	mProfiler.start("RUN_BUFFERING_DAG");
#endif	
	
	if (0==mGraph.grabFrame())
	{
		if (mSideExecutor.joinable())
		mSideExecutor.join();

		mSideExecutor = std::thread(&GRAPH::runAuxillaryTasks, std::ref(mGraph));
		mGraph.buffer();
		
		this->StateCounter++;
		
		if(this->StateCounter >= sNbBuffer-1)
		{
		  this->currentStatus = StateStatus::DONE;
		}
	}
		
	else
	{ 	
		mRetryGrab ++;
		if(mRetryGrab > 3)
		 currentStatus = StateStatus::ERROR;
	}

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::TIMING_PROFILE) <<endl
				<<"******************************"<<endl
				<<  "Total Time for Buffering Graph." <<endl
				<<  "Total Graph time : " << mProfiler.getAvgTime("RUN_BUFFERING_DAG")<<endl
				<<"******************************"<<endl<<endl;	
 				#endif
}


template<typename GRAPH>  
BufferingState<GRAPH>::~BufferingState()
{
	if (mSideExecutor.joinable())
	   mSideExecutor.join();	
}

#endif // BUFFERINGSTATE_H
