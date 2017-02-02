#ifndef INITSTATE_H
#define INITSTATE_H
#include <memory>
#include <Eigen/Dense>
#include <math.h>
#include "State.h"
#include  "LaneFilter.h"
#include  "VanishingPtFilter.h"
#include "rt_nonfinite.h"
#include "run_Init_State.h"
#include "run_Init_State_terminate.h"
#include "run_Init_State_emxAPI.h"
#include "run_Init_State_initialize.h"
		

class InitState: public State
{	

// For testing purposes, make private when testing completed for private members.
public :

		//const int mNbBuffer; 
		
	    shared_ptr<LaneFilter>		  mLaneFilter;
		shared_ptr<VanishingPtFilter> mVanishPtFilter;
		Vector2i				 	  mRES_VH;
		
		MatlabStruct_likelihoods Matlab_Likelihoods;
		MatlabStruct_templates   Matlab_Templates;
		MatlabStruct_laneFilter  Matlab_LaneFilter;
		MatlabStruct_focusMask   Matlab_Masks;
		
		MatlabStruct_vpFilter    Matlab_vpFilter;
		MatlabStruct_vanishingPt Matlab_VanishingPt;
		
		
		shared_ptr<VanishingPt> mVanishingPt;
		shared_ptr<Templates>	mTemplates;
		shared_ptr<Likelihoods> mLikelihoods;
		 

public:
		void run();
		void conclude();
		InitState(const CameraProperties& , shared_ptr<LaneFilter> , shared_ptr<VanishingPtFilter>);
		~InitState();

	};
	
#endif // INITSTATE_H
