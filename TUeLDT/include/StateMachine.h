#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include "State.h"
#include <memory>
#include <atomic>
#include <thread>
#include "SigInit.h"
#include "Car.h"
#include "Camera.h"
#include "Lane.h"
#include "LaneFilter.h"
#include "VanishingPtFilter.h"
#include "InitState.h"
#include "BufferingState.h"
//#include "NcursesTUI.h"


using namespace std;


class StateMachine
{
private:
    
#ifdef DIRECTORY_INPUT
	const vector<cv::String>& mFiles;
#endif
	const string 	mStateStrings[4];
	static States   sCurrentState;
	
	
	const Camera mCamera;
	const Lane   mLane;
	const Car    mCar;

	
	//These Objects are shared between various States
	shared_ptr<LaneFilter> 			mLaneFilter;
	shared_ptr<VanishingPtFilter> 	mVanishingPtFilter;
	shared_ptr<Likelihoods> 		mLikelihoods;
	shared_ptr<Templates>   		mTemplates;

#ifdef PROFILER_ENABLED 
	std::thread                     mThreadProfiler;
	std::atomic<bool>				mDisposed;
	#ifndef s32v2xx
//	NcursesTUI* 					mUserInterface; ^TODO: Design a Textual User Interface to display high level Information only.
	#endif
#endif	
	
	
	
	
	

public:	
	// STATE MACHINE METHODS
#ifdef DIRECTORY_INPUT
	StateMachine(const vector<cv::String>&);
#else
	StateMachine();
#endif

	int run(shared_ptr<SigInit>);	
	void PrintMachineInfo();
	~StateMachine();

};

#endif // STATEMACHINE_H
