#ifndef INITSTATE_H
#define INITSTATE_H
#include <memory>
#include <Eigen/Dense>
#include <math.h>
#include "State.h"
#include "Camera.h"
#include "LaneFilter.h"
#include "VanishingPtFilter.h"

//Code Generation Includes
#include "createTemplate.h"
#include "createTemplate_types.h"
#include "createTemplate_emxAPI.h"
#include "createTemplate_initialize.h"
#include "createTemplate_terminate.h"

		

class InitState: public State
{	

private :
       
		const Camera mCamera;
		const Lane   mLane;

public:		
		unique_ptr<LaneFilter> 			laneFilter;
		unique_ptr<VanishingPtFilter> 	vanishingPtFilter;
		unique_ptr<Templates>		  	templates;

public:
		void run();
		void conclude();
		InitState();
	   ~InitState();

};
	
#endif // INITSTATE_H
