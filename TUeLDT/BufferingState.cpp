#include "BufferingState.h"

BufferingState::BufferingState()
:mRetryGrab(0)
{

 #ifdef PROFILER_ENABLED
  	//getOpenClInfo(); 
 #endif	

}

  
int BufferingState::setSource()
{

	#ifdef DIRECTORY_INPUT
	{
	   vector< cv::String> lFiles;
           cv::String lFolder = "/media/rameez/Linux-Extended/DataSet/eindhoven/PNG_imgs";
           
	   glob(lFolder, lFiles);
           const uint lSkipFrames = 0;  
         
      	   if (lFiles.size() <= lSkipFrames)
           {
         	cout<<endl;
         	cout<<"Total Number of Image Files to Process : " << 0;
         	cout<<endl;
         	return -1;
           }

     	   cout<<endl;
           cout<<"Total Number of Image Files to Process : " << lFiles.size() - lSkipFrames;
           cout<<endl;

	   bufferingGraph.mFiles = lFiles;
	   bufferingGraph.mFrameCount =lSkipFrames;
	   return 0;
	}
	#else
	{





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
	mProfiler.start("SingleRun_BUFFER");
#endif	
		
	if (mSideExecutor.joinable())
		mSideExecutor.join();
	

	if (0==bufferingGraph.grabFrame())
	{
		mSideExecutor =
		#ifndef s32v2xx
			std::thread(&BufferingDAG_generic::auxillaryTasks, std::ref(bufferingGraph));
		#else
			std::thread(&BufferingDAG_s32v::auxillaryTasks,    std::ref(bufferingGraph));
		#endif
		

		bufferingGraph.buffer();

		
		if(this->StateCounter < sNbBuffer-2)
		  this->StateCounter++;
		
		else
		{
		  this->StateCounter++;
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
				<<  "Completing a Buffering run." <<endl
				<<  "Total Graph time : " << mProfiler.getAvgTime("SingleRun_BUFFER")<<endl
				<<"******************************"<<endl<<endl;	
 				#endif

}

BufferingState::~BufferingState()
{
	if (mSideExecutor.joinable())
		mSideExecutor.join();	
}

/* ********************************************* */
/*  Low level Class PRIVATE FUnction for _Profiling/ Logging
 *  getOpenClInfo()
*/

#ifdef PROFILER_ENABLED

void BufferingState::getOpenClInfo()
{

	if (!cv::ocl::haveOpenCL())
	{
		 LOG_INFO_(LDTLog::BUFFERING_STATE_LOG) <<endl
		  <<"******************************"<<endl
		  <<  "OpenCL Info:" <<endl
		  <<  "Sorry, OpenCl is not available. " <<endl
		  <<"******************************"<<endl<<endl;	
	}

	cv::ocl::Context context;

	if (!context.create(cv::ocl::Device::TYPE_GPU))
	{
		LOG_INFO_(LDTLog::BUFFERING_STATE_LOG) <<endl
		  <<"******************************"<<endl
		  <<  "OpenCL Info:" <<endl
		  <<  "Sorry, Failed creating the Context. " <<endl
		  <<"******************************"<<endl<<endl;	
	}

	else
	{


		for (std::size_t i = 0; i < context.ndevices(); i++)
		{
			cv::ocl::Device device = context.device(i);
			std::string name= device.name();
			std::string OpenCL_C_Version= device.OpenCL_C_Version();
		
			LOG_INFO_(LDTLog::BUFFERING_STATE_LOG) <<endl
			  <<"***********************************"<<endl
			  <<  "OpenCL Device Detected:		| " <<endl
			  <<  "name:  				| "<< name<< endl
			  <<  "available:			| "<< device.available()<< endl
			  <<  "imageSupport:			| "<< device.imageSupport()<< endl
			  <<  "globalMemSize:			| "<< device.globalMemSize()<< endl
			  <<  "localMemSize:			| "<< device.localMemSize()<< endl
			  <<  "maxWorkGroup:			| "<< device.maxWorkGroupSize()<< endl
			  <<  "maxWorkItemDim:			| "<< device.maxWorkItemDims()<< endl
			  <<  "maxComputeUnits:			| "<< device.maxComputeUnits()<< endl
			  <<  "preferredVectorWidthChar:	| "<< device.preferredVectorWidthChar()<< endl
			  <<  "preferredVectorWidthDouble:	| "<< device.preferredVectorWidthDouble()<< endl
			  <<  "preferredVectorWidthFloat:	| "<< device.preferredVectorWidthFloat()<< endl
			  <<  "preferredVectorWidthHalf:	| "<< device.preferredVectorWidthHalf()<< endl
			  <<  "preferredVectorWidthLong:	| "<< device.preferredVectorWidthLong()<< endl
			  <<  "preferredVectorWidthShort:	| "<< device.preferredVectorWidthShort()<< endl
			  <<  "image2DMaxHeight:		| "<< device.image2DMaxHeight()<< endl
			  <<  "image2DMaxWidth:			| "<< device.image2DMaxWidth()<< endl
			  <<  "OpenCL_C_Version:		| "<< OpenCL_C_Version<< endl
			  <<"***********************************"<<endl<<endl;
		}

	}

}
	
#endif




