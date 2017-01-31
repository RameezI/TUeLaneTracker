#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include <memory>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "sigInit.h"
#include "Car.h"
#include "Camera.h"
#include "Lane.h"
#include "LaneFilter.h"
#include "VanishingPtFilter.h"
#include "States.h"
#include "InitState.h"
#include "BufferingState.h"
using namespace std;

enum Source{CAMERA, DIRECTORY};
enum States{BOOTING, BUFFERING, DETECTING_LANES, RESETING, DISPOSING }; //^TODO: Add PAUSED state as well

class StateMachine
{
	private:
	const Source 	mSource;
	static States   mCurrentState;
	const string 	mStateStrings[4];
	
	const vector<cv::String> mFileNames;
	const static int sNbBuffer;
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
	StateMachine(const vector<cv::String>&);
	int run(shared_ptr<SigInit>);	
	void PrintMachineInfo();
	
	//STAE MACHINE 

	
	
	~StateMachine();

};

#endif // STATEMACHINE_H
