#ifndef INITSTATE_H
#define INITSTATE_H
#include <memory>
#include <Eigen/Dense>
#include <math.h>
#include "State.h"
#include "Camera.h"
#include "LaneFilter.h"
#include "VanishingPtFilter.h"





class InitState: public State
{	

private :


		bool  mLaneFilterCreated;
		bool  mVpFilterCreated;
		bool  mTemplatesCreated;
        bool  checkCreationStatus();
		
		

public	:

		/* These methods create instances and also keep record of the already created objects
		 * Make sure all objects are created before indicating "StateStatus::DONE". */
		 
		unique_ptr<LaneFilter> 		  createLaneFilter();
		unique_ptr<VanishingPtFilter> createVanishingPtFilter();
		unique_ptr<Templates> 		  createTemplates();
		InitState();
	   ~InitState();

};
	
#endif // INITSTATE_H
