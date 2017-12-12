#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include "SigInit.h"
#include "State.h"
#include "InitState.h"
#include "BufferingState.h"
#include "TrackingLaneState.h"
#include "BufferingDAG_generic.h"

#ifdef	 S32V2XX
#include "BufferingDAG_s32v.h"
#endif

using namespace std;

class StateMachine
{
	
private:

	static States   	sCurrentState;
	const  FrameSource	mFrameSource;
	const  string		mSourceStr;

public:	

	StateMachine(FrameSource lFrameSource, string lSourceString);
	int spin(shared_ptr<SigInit>);	
	~StateMachine();
};

#endif
