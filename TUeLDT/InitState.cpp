#include "InitState.h"
 

InitState::InitState(const CameraProperties& CAMERA, shared_ptr<LaneFilter> laneFilter, shared_ptr<VanishingPtFilter> vpFilter)
: mLaneFilter(laneFilter),
  mVanishPtFilter(vpFilter),
  mRES_VH(CAMERA.RES_VH),
  mLikelihoods(make_shared<Likelihoods>())  // Allocated on heap but as it is smart;) shared pointer no need for deallocation 
											//^TODO: initiate all pointer ahead.
  
{
  
	int size[2];
	
	
/*	Wrap Matlab_vpFilter.mFilter to point to respective member of mVanishPtFilter.  */
	size[0]= mVanishPtFilter->mFilter.rows();
	size[1]= mVanishPtFilter->mFilter.cols();
	Matlab_vpFilter.mFilter= emxCreateWrapperND_real32_T(mVanishPtFilter->mFilter.data(), 2, size);
	

	
/*	Wrap Matlab_vpFilter.mPrior  to point to respective member of mVanishPtFilter.  */	
	size[0]= mVanishPtFilter->mPrior.rows();
	size[1]= mVanishPtFilter->mPrior.cols();
	Matlab_vpFilter.mPrior= emxCreateWrapperND_real32_T(mVanishPtFilter->mPrior.data(), 2, size);
	
	
/*	Wrap Matlab_LaneFilter.mFilter to point to respective member of mLaneFilter.  */
	size[0]= mLaneFilter->mFilter.rows();
	size[1]= mLaneFilter->mFilter.cols();
	Matlab_LaneFilter.mFilter= emxCreateWrapperND_real32_T(mLaneFilter->mFilter.data(), 2, size);
	
/*	Wrap Matlab_LaneFilter.mPrior to point to respective member of mLaneFilter.  */	
	size[0]= mLaneFilter->mPrior.rows();
	size[1]= mLaneFilter->mPrior.cols();
	Matlab_LaneFilter.mPrior= emxCreateWrapperND_real32_T(mLaneFilter->mPrior.data(), 2, size);
	
	mStateStatus = StateStatus::ACTIVE;
	
	run();
}


void InitState::run()
{
	
// ^TODO: Initialised Memory allocation for outputs... Make sure to clean up in the destructor.	  
	run_Init_State_initialize();
	emxInit_MatlabStruct_likelihoods(&Matlab_Likelihoods);
	emxInit_MatlabStruct_templates(&Matlab_Templates);
	emxInit_MatlabStruct_focusMask(&Matlab_Masks);
	

	run_Init_State(	mRES_VH.data(),
					sNbBuffer, 
					&Matlab_LaneFilter, 
					&Matlab_vpFilter, 
					&Matlab_Likelihoods,
					&Matlab_Templates, 
					&Matlab_VanishingPt,
					&Matlab_Masks);
	
	Map<MatrixXd> TOT_MAX((Matlab_Likelihoods.TOT_MAX->data),
						Matlab_Likelihoods.TOT_MAX->size[0], 
						Matlab_Likelihoods.TOT_MAX->size[1]);
	


  // MatrixXd TOT_P_M =TOT_P; 
	mLikelihoods->TOT_MAX= TOT_MAX;  //^TODO: Copy Opperation Time it! One Option is to just directly use Map rather Matrix.

	for (int i=0; i<sNbBuffer; i++)
	 {
		Map<MatrixXd> TOT_MAX (Matlab_Likelihoods.TOT_ALL->data + i*Matlab_Likelihoods.TOT_MAX->size[0]*Matlab_Likelihoods.TOT_MAX->size[1], 
							 Matlab_Likelihoods.TOT_MAX->size[0], 
							 Matlab_Likelihoods.TOT_MAX->size[1]);
		mLikelihoods->TOT_ALL[i]= TOT_MAX;
	 } 
	
	
	//mLikelihoods->TOT_P_ALL[]
	//MatrixXd M = TOT_P;
	//mLikelihoods.				
	 				

//^TODO: Assign Likelihoods, Templates and VanishingPoint 		

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
	emxDestroy_MatlabStruct_templates(Matlab_Templates);
	emxDestroy_MatlabStruct_likelihoods(Matlab_Likelihoods);
	run_Init_State_terminate();
}

