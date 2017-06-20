#ifndef INITSTATE_H
#define INITSTATE_H
#include "State.h"

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
		 
		unique_ptr<LaneFilter> 		createLaneFilter();
		unique_ptr<VanishingPtFilter> 	createVanishingPtFilter();
		unique_ptr<Templates> 		createTemplates();
		InitState();
	   	~InitState();

};
	
#endif // INITSTATE_H
