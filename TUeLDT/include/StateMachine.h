#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include "SigInit.h"
#include "State.h"
#include "InitState.h"
#include "BufferingState.h"
#include "TrackingLaneState.h"


using namespace std;

class StateMachine
{
	
private:
	static States   sCurrentState;

public:	
	StateMachine();
	int spin(shared_ptr<SigInit>);	
	~StateMachine();
};

#endif
