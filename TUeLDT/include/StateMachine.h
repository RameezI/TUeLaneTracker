#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include "compileConfig.h"
#include <memory>
#include "SigInit.h"
#include "Car.h"
#include "Camera.h"
#include "Lane.h"
#include "LaneFilter.h"
#include "VanishingPtFilter.h"
#include "State.h"
#include "InitState.h"
#include "BufferingState.h"


using namespace std;
enum States{BOOTING, BUFFERING, DETECTING_LANES, RESETING, DISPOSING }; //^TODO: Add PAUSED state as well

class StateMachine
{
	private:
	static States   sCurrentState;
	const string 	mStateStrings[4];
	
	const vector<cv::String>& mFiles;
	

	const Camera mCamera;
	const Lane   mLane;
	const Car    mCar;
	
	
	
	
	
	//These Objects are shared between various States
	shared_ptr<LaneFilter> 			mLaneFilter;
	shared_ptr<VanishingPtFilter> 	mVanishingPtFilter;
	shared_ptr<Likelihoods> 		mLikelihoods;
	shared_ptr<Templates>   		mTemplates;
	
	

public:	
	// STATE MACHINE METHODS
#ifdef DIRECTORY_INPUT
	StateMachine(const vector<cv::String>&);
#endif
	int run(shared_ptr<SigInit>);	
	void PrintMachineInfo();
	//STAE MACHINE 

	
	
	~StateMachine();

};

#endif // STATEMACHINE_H
