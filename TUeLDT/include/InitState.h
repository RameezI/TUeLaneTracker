#ifndef INITSTATE_H
#define INITSTATE_H
#include <memory>
#include <Eigen/Dense>
#include <math.h>
#include "State.h"
#include  "LaneFilter.h"
#include  "VanishingPtFilter.h"

//Code Generation Includes
#include "createTemplate.h"
#include "createTemplate_types.h"
#include "createTemplate_emxAPI.h"
#include "createTemplate_initialize.h"
#include "createTemplate_terminate.h"

		

class InitState: public State
{	

// For testing purposes, make private when testing completed for private members.
public :

		Vector2i				 	  mRES_VH;	
		shared_ptr<VanishingPt> 	  mVanishingPt;
		shared_ptr<Templates>		  mTemplates;
		shared_ptr<Likelihoods> 	  mLikelihoods;
		shared_ptr<Masks> 			  mMasks;

public:
		void run();
		void conclude();
		InitState(const CameraProperties&);
		~InitState();

	};
	
#endif // INITSTATE_H
