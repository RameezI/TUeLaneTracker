#ifndef INITSTATE_H
#define INITSTATE_H
#include <memory>
#include <Eigen/Dense>
#include <math.h>
#include  "LaneFilter.h"
#include  "VanishingPtFilter.h"
#include "rt_nonfinite.h"
#include "run_Init_State.h"
#include "run_Init_State_terminate.h"
#include "run_Init_State_emxAPI.h"
#include "run_Init_State_initialize.h"
		



struct Likelihoods
{
		MatrixXd  TOT_P_ALL;
		MatrixXd  DIR_ALL;
		MatrixXd  MASK_FOC_TOT_P;
		MatrixXd  TOT_P;
		MatrixXd  FOC_TOT_P;
		MatrixXd  AVG_DIR_TOT_P;
		MatrixXd  TOT_P_ALL_BACK_UP;
		MatrixXd  DIR_ALL_BACK_UP;
};

struct Templates
{
  MatrixXd ROOT_DIR_TEMPLATE;
  MatrixXd ROOT_PROB_TEMPLATE;
  MatrixXd ROOT_DEPTH_TEMPLATE;
  MatrixXd SEGMENT;
}; 

struct VanishingPt
{
  int  V;
  int  H;
  int  V_prev;
  int  H_prev;
  VanishingPt()
  : V(0),H(0),V_prev(0),H_prev(0){}
};


class InitState
{	

// For testing purposes, make private when testing completed for private members.
public :
	    shared_ptr<LaneFilter>		  mLaneFilter;
		shared_ptr<VanishingPtFilter> mVanishPtFilter;
		Vector2i				 	  mRES_VH;
		
		MatlabStruct_likelihoods Matlab_Likelihoods;
		MatlabStruct_templates   Matlab_Templates;
		MatlabStruct_laneFilter  Matlab_LaneFilter;
		MatlabStruct_vpFilter    Matlab_vpFilter;
		MatlabStruct_vanishingPt Matlab_vanishingPt;
		
		
		


public:
		VanishingPt mVanishingPt;
		Templates	mTemplates;
		Likelihoods mLikelihoods;
		
		static float  sStatus;
		static float  sNbBuffer;
	
		void run();
		InitState(const CameraProperties& , shared_ptr<LaneFilter> , shared_ptr<VanishingPtFilter>);
		~InitState();

	};
	
#endif // INITSTATE_H
