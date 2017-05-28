#include "BufferingState.h"

BufferingState::BufferingState()
: bufferingGraph(unique_ptr<BufferingDAG_generic>(new BufferingDAG_generic()))
{
	#ifdef PROFILER_ENABLED
	getOpenClInfo();
	#endif	
}



/* Setting up Input Source*/   
#ifdef DIRECTORY_INPUT
	void BufferingState::setSource(const vector<cv::String>& files)
	{
		bufferingGraph->mFiles = files;
	}

#else

	void BufferingState::setSource()
	{

	}
	
#endif



/* Setting up Root Templates */
  
	void BufferingState::setTemplates (Templates* templates)
	{
			
		#ifdef PROFILER_ENABLED
			mProfiler.start("SetRootTemplates");
		#endif
			
			//if GRADIENT_TAN_ROOT is defined as UMAT than it should interpoerable
			// In that case we can assign a opencv::UMat to s32v::UMAT
			// Only right class has to be instantiated as bufferingGraph
			
			bufferingGraph->mGRADIENT_TAN_ROOT = templates->GRADIENT_TAN_ROOT;
			bufferingGraph->mDEPTH_MAP_ROOT    = templates->DEPTH_MAP_ROOT;
			bufferingGraph->mFOCUS_MASK_ROOT   = templates->FOCUS_MASK_ROOT;
			
			this->currentStatus= StateStatus::ACTIVE;
					
		 #ifdef PROFILER_ENABLED
		 mProfiler.end();
		 LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
								  <<"******************************"<<endl
								  <<  "Completing Buffering Setup." <<endl
								  <<  "ROOT Templates copy Time: "  << mProfiler.getAvgTime("SetRootTemplates")<<endl
								  <<"******************************"<<endl<<endl;	
								 #endif				
	}



/* Running Directed Acyclic Graph
 * 
*/  
	void BufferingState::run()
	{
		
		#ifdef PROFILER_ENABLED
			mProfiler.start("SingleRun");
		#endif	
			

		#ifdef DIRECTORY_INPUT

		if (bufferingGraph->mCountFrame < bufferingGraph->mFiles.size())
		{	
			if(this->StateCounter < sNbBuffer)
				this->StateCounter++;
			else
				this->currentStatus = StateStatus::DONE;
		}
		else
		{				
			this->currentStatus = StateStatus::DONE;	
			return;
		} 

		#else 
			//^TODO: Check for camera errors or signals to finalise this state			
			if(this->StateCounter < sNbBuffer)
				this->StateCounter++;
			else
				this->currentStatus = StateStatus::DONE;
			
		#endif


		if (0==bufferingGraph->grabFrame())
			bufferingGraph->executeDAG_buffering();
		else
			cerr << "Problem loading image!!!" << endl;
														

		 #ifdef PROFILER_ENABLED
		 mProfiler.end();
		 LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
								  <<"******************************"<<endl
								  <<  "Completing a run loop." <<endl
								  <<  "Single run-loop time: " << mProfiler.getAvgTime("SingleRun")<<endl
								  <<"******************************"<<endl<<endl;	
								 #endif
	}



/* ********************************************* /*
/*  Low level Class PRIVATE FUnction
 *  getOpenClInfo()
*/

#ifdef PROFILER_ENABLED

	void BufferingState::getOpenClInfo()
	{
		
		if (!cv::ocl::haveOpenCL())
		{
			 LOG_INFO_(LDTLog::BUFFERING_LOG) <<endl
							  <<"******************************"<<endl
							  <<  "OpenCL Info:" <<endl
							  <<  "Sorry, OpenCl is not available. " <<endl
							  <<"******************************"<<endl<<endl;	
			//return;
		}

		cv::ocl::Context context;
		if (!context.create(cv::ocl::Device::TYPE_GPU))
		{
			LOG_INFO_(LDTLog::BUFFERING_LOG) <<endl
							  <<"******************************"<<endl
							  <<  "OpenCL Info:" <<endl
							  <<  "Sorry, Failed creating the Context. " <<endl
							  <<"******************************"<<endl<<endl;	
			//return;
		}

		else
		{
		
		
			for (int i = 0; i < context.ndevices(); i++)
			{
				cv::ocl::Device device = context.device(i);
				std::string name= device.name();
				std::string OpenCL_C_Version= device.OpenCL_C_Version();
				
						LOG_INFO_(LDTLog::BUFFERING_LOG) <<endl
							  <<"******************************"<<endl
							  <<  "OpenCL Device Detected:" <<endl
							  <<  "name:  " << name<< endl
							  <<  "available:  " << device.available()<< endl
							  <<  "imageSupport:  " << device.imageSupport()<< endl
							  <<  "globalMemSize:  " << device.globalMemSize()<< endl
							  <<  "localMemSize:  " << device.localMemSize()<< endl
							  <<  "maxWorkGroup:  " << device.maxWorkGroupSize()<< endl
							  <<  "maxWorkItemDim:  " << device.maxWorkItemDims()<< endl
							  <<  "maxComputeUnits:  " << device.maxComputeUnits()<< endl
							  <<  "preferredVectorWidthChar:  " << device.preferredVectorWidthChar()<< endl
							  <<  "preferredVectorWidthDouble:  " << device.preferredVectorWidthDouble()<< endl
							  <<  "preferredVectorWidthFloat:  " << device.preferredVectorWidthFloat()<< endl
							  <<  "preferredVectorWidthHalf:  " << device.preferredVectorWidthHalf()<< endl
							  <<  "preferredVectorWidthLong:  " << device.preferredVectorWidthLong()<< endl
							  <<  "preferredVectorWidthShort:  " << device.preferredVectorWidthShort()<< endl
							  <<  "image2DMaxHeight:  " << device.image2DMaxHeight()<< endl
							  <<  "image2DMaxWidth:  " << device.image2DMaxWidth()<< endl
							  <<  "OpenCL_C_Version:  " << OpenCL_C_Version<< endl
							  <<"******************************"<<endl<<endl;
			}
		
		}

	}
	
#endif


BufferingState::~BufferingState()
{
	
	
}

