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

#ifdef DIRECTORY_INPUT
	const vector<cv::String>& mFiles;
	const int mSkipFrames;
#endif
	static States   sCurrentState;


public:	
	// STATE MACHINE METHODS
#ifdef DIRECTORY_INPUT
	StateMachine(const vector<cv::String>&, const int& skipFrames);
#else
	StateMachine();
#endif

	int spin(shared_ptr<SigInit>);	
	~StateMachine();

};

#endif // STATEMACHINE_H
