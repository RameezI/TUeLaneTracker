#include "InitState.h"
 

InitState::InitState(const CameraProperties& CAMERA)

: mRES_VH(CAMERA.RES_VH),
  
  mLikelihoods(make_shared<Likelihoods>( mRES_VH(0), mRES_VH(1) ) ),  // Allocated on heap but as it is smart, no need for deallocation 
  
  mTemplates(make_shared<Templates> ( mRES_VH(0), mRES_VH(1) ) ),
  
  mMasks(make_shared<Masks>( mRES_VH(0), mRES_VH(1) ) ),
  
  mVanishingPt(make_shared<VanishingPt>())  
  
{
  
	mStateStatus = StateStatus::ACTIVE;

}


void InitState::run()
{

	
//^TODO: Error of 0.12 degrees in the template. Load Template directly from Matlab CSV file.
	
#ifdef PROFILER_ENABLED
mProfiler.start("SingleRun");
#endif	
	
createTemplate_initialize();

#ifdef PROFILER_ENABLED
mProfiler.start("WrapDirectionTemplate");
#endif	
	
emxArray_real32_T *TEMPLATE;
TEMPLATE = emxCreateWrapper_real32_T(mTemplates->GRADIENT_DIR_ROOT.data(), 2*mRES_VH(0)+1, 2*mRES_VH(1)+1);


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


createTemplate((float)mRES_VH(0), (float)mRES_VH(1), TEMPLATE);
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



#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BOOTING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Single-RUN of the Booting State." <<endl
										  <<  "Time: " << mProfiler.getAvgTime("SingleRun")<<endl
										  <<"******************************"<<endl<<endl;	
									   #endif

	sStateCounter++;
	mStateStatus = StateStatus::DONE;		
}

void InitState::conclude()
{
	sStateCounter=0;
	mStateStatus = StateStatus::INACTIVE;
}

InitState::~InitState()
{	
	//emxDestroy_MatlabStruct_templates(Matlab_Templates);
	//emxDestroy_MatlabStruct_likelihoods(Matlab_Likelihoods);
	//run_Init_State_terminate();
}

