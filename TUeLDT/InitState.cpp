#include "InitState.h"
 

InitState::InitState(const CameraProperties& CAMERA, shared_ptr<LaneFilter> laneFilter, shared_ptr<VanishingPtFilter> vpFilter, const int NbBuffer)
: mLaneFilter(laneFilter),
  mVanishPtFilter(vpFilter),
  mNbBuffer(NbBuffer),
  mRES_VH(CAMERA.RES_VH)
  
{
  
	int size[2];
	
	
/*	Wrap Matlab_vpFilter.mFilter to point to respective member of mVanishPtFilter.  */
	size[0]= mVanishPtFilter->mFilter.rows();
	size[1]= mVanishPtFilter->mFilter.cols();
	Matlab_vpFilter.mFilter= emxCreateWrapperND_real_T(mVanishPtFilter->mFilter.data(), 2, size);
/*	Wrap Matlab_vpFilter.mPrior  to point to respective member of mVanishPtFilter.  */	
	size[0]= mVanishPtFilter->mPrior.rows();
	size[1]= mVanishPtFilter->mPrior.cols();
	Matlab_vpFilter.mPrior= emxCreateWrapperND_real_T(mVanishPtFilter->mPrior.data(), 2, size);
	
	
/*	Wrap Matlab_LaneFilter.mFilter to point to respective member of mLaneFilter.  */
	size[0]= mLaneFilter->mFilter.rows();
	size[1]= mLaneFilter->mFilter.cols();
	Matlab_LaneFilter.mFilter= emxCreateWrapperND_real_T(mLaneFilter->mFilter.data(), 2, size);
	
/*	Wrap Matlab_LaneFilter.mPrior to point to respective member of mLaneFilter.  */	
	size[0]= mLaneFilter->mPrior.rows();
	size[1]= mLaneFilter->mPrior.cols();
	Matlab_LaneFilter.mPrior= emxCreateWrapperND_real_T(mLaneFilter->mPrior.data(), 2, size);
	
	
	run();
}


void InitState::run()
{
// ^TODO: Initialised Memory allocation for outputs... Make sure to clean up in the destructor.	  
	run_Init_State_initialize();
	emxInit_MatlabStruct_likelihoods(&Matlab_Likelihoods);
	emxInit_MatlabStruct_templates(&Matlab_Templates);
	
	
// ^TODO: Temp Solution, convert unnecessary floats to int
	float RES_VH[2]= {mRES_VH[0], mRES_VH[1]}	;
	double  Status;

	run_Init_State(	RES_VH,
					mNbBuffer, 
					&Matlab_LaneFilter, 
					&Matlab_vpFilter, 
					&Status, 
					&Matlab_Likelihoods,
					&Matlab_Templates, 
					&Matlab_vanishingPt	);
					

	//Map<MatrixXf> mLikelihoods->TOT_P(&Matlab_Likelihoods.TOT_P.data,640,480);
				
	//mLikelihoods.				
	 				
					
//^TODO: Assign Likelihoods, Templates and VanishingPoint 					
					
}

InitState::~InitState()
{	
	emxDestroy_MatlabStruct_templates(Matlab_Templates);
	emxDestroy_MatlabStruct_likelihoods(Matlab_Likelihoods);
	run_Init_State_terminate();
}

