#include "BufferingState.h"

BufferingState::BufferingState()
:mRetryGrab(0)
{

}

  
int BufferingState::setSource()
{

	int lReturn=0;

	#ifdef DIRECTORY_INPUT
	{
	   vector< cv::String> lFiles;
           cv::String lFolder = "/media/rameez/Linux-Extended/DataSet/eindhoven/PNG_imgs";
           //cv::String lFolder = "/home/root/data/Eindhoven";
           
	   try
	   {
		glob(lFolder, lFiles);
	   }
	   catch(...)
	   {
		lReturn|= -1;
	   }

           const uint lSkipFrames = 4000;  
         
      	   if (lFiles.size() <= lSkipFrames)
           {
         	cout<<endl;
         	cout<<"Total Number of Image Files to Process : " << 0;
         	cout<<endl;
         	lReturn |= -1;
           }
	   else
	   {
     	   	cout<<endl;
           	cout<<"Total Number of Image Files to Process : " << lFiles.size() - lSkipFrames;
           	cout<<endl;	
	   }

	   bufferingGraph.mFiles = lFiles;
	   bufferingGraph.mFrameCount =lSkipFrames;
	   return lReturn;
	}
	#else
	{
		cout<< "RTSP MODE"<<endl;
		Mat lFrame;
		try
		{
		  
	   	   bufferingGraph.mFrameCount = 0;
		   if(!bufferingGraph.mRTSP_CAPTURE.open("rtsp://192.168.8.1:8554/test"))
		    lReturn |=-1;
		}
		catch(...)
		{
		    lReturn=-1;
		}
		
		return lReturn;

	}
	#endif
}




void BufferingState::setupDAG(const Templates& templates)
{
	
#ifdef PROFILER_ENABLED
mProfiler.start("SET_UP_BUFFERING_DAG");
#endif

	const int RES_H 	 	=  bufferingGraph.mCAMERA.RES_VH(1);

	bufferingGraph.mVP_Range_V   	= templates.VP_RANGE_V;
	bufferingGraph.mSpan		= templates.SPAN;
	bufferingGraph.mMargin		= templates.MARGIN;
	

	//Allocate Buffers
	bufferingGraph.mBufferPool.reset(new BufferPool(templates.SPAN, RES_H, sNbBuffer)); 


	// Assign Templates
	bufferingGraph.mGRADIENT_TAN_ROOT = templates.GRADIENT_TAN_ROOT;
	bufferingGraph.mDEPTH_MAP_ROOT    = templates.DEPTH_MAP_ROOT;
	bufferingGraph.mFOCUS_MASK_ROOT   = templates.FOCUS_MASK_ROOT;

	
	// Define X and Y coordinates in VP reffernce system
	bufferingGraph.mY_IRS	 =   templates.Y_IRS;
	bufferingGraph.mX_IRS   =    templates.X_IRS;


	if ( 0 == bufferingGraph.init_DAG() )
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



void BufferingState::run()
{
	
#ifdef PROFILER_ENABLED
	mProfiler.start("RUN_BUFFERING_DAG");
#endif	
	
	if (mSideExecutor.joinable())
		mSideExecutor.join();
	

	if (0==bufferingGraph.grabFrame())
	{
		mSideExecutor =
		#ifndef S32V2XX
			std::thread(&BufferingDAG_generic::runAuxillaryTasks, std::ref(bufferingGraph));
		#else
			std::thread(&BufferingDAG_s32v::runAuxillaryTasks,    std::ref(bufferingGraph));
		#endif
		
		bufferingGraph.buffer();
		
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

BufferingState::~BufferingState()
{
	if (mSideExecutor.joinable())
		mSideExecutor.join();	
}
