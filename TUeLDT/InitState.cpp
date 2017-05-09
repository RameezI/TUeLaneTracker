#include "InitState.h"
 

InitState::InitState()
:  
	laneFilter(	unique_ptr<LaneFilter>(new LaneFilter (mLane, mCamera) ) ),

	vanishingPtFilter( unique_ptr<VanishingPtFilter>( new VanishingPtFilter (laneFilter->HISTOGRAM_BINS,
																			 laneFilter->OFFSET_V,
																			  mCamera)) ),
													  
    templates( unique_ptr<Templates> ( new  Templates (mCamera.RES_VH(0), mCamera.RES_VH(1) ) ))
   
{
  
	this->currentStatus = StateStatus::ACTIVE;

}


void InitState::run()
{
	
#ifdef PROFILER_ENABLED
mProfiler.start("SingleRun");
#endif	
	
	createTemplate_initialize();





#ifdef PROFILER_ENABLED
mProfiler.start("WrapDirectionTemplate");
#endif	
	
	emxArray_real32_T *TEMPLATE;
	TEMPLATE = emxCreateWrapper_real32_T(templates->GRADIENT_DIR_ROOT.data(), 2*mCamera.RES_VH(0)+1, 2*mCamera.RES_VH(1)+1);

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BOOTING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Wrapping Direction Template." <<endl
										  <<  "Time: " << mProfiler.getAvgTime("WrapDirectionTemplate")<<endl
										  <<"******************************"<<endl<<endl;	
									   #endif	


#ifdef PROFILER_ENABLED
mProfiler.start("CreateTemplate");
#endif	

	createTemplate((float)mCamera.RES_VH(0), (float)mCamera.RES_VH(1), TEMPLATE);
	emxDestroyArray_real32_T(TEMPLATE);  

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BOOTING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Creating Gradient Direction Template." <<endl
										  <<  "Time: " << mProfiler.getAvgTime("CreateTemplate")<<endl
										  <<"******************************"<<endl<<endl;	
									   #endif	

createTemplate_terminate();		




	this->StateCounter++;
	this->currentStatus = StateStatus::DONE;

#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BOOTING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Single-RUN of the Booting State." <<endl
										  <<  "Time: " << mProfiler.getAvgTime("SingleRun")<<endl
										  <<"******************************"<<endl<<endl;	
									   #endif		
}

void InitState::conclude()
{
	this->StateCounter=0;
	this->currentStatus = StateStatus::INACTIVE;
}

InitState::~InitState()
{	
	//emxDestroy_MatlabStruct_templates(Matlab_Templates);
	//emxDestroy_MatlabStruct_likelihoods(Matlab_Likelihoods);
	//run_Init_State_terminate();
}

