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
#include "InitState.h"
#include "BufferingState.h"


using namespace std;


class StateMachine
{
	
	
private:
    
#ifdef DIRECTORY_INPUT
	const vector<cv::String>& mFiles;
#endif


	static States   sCurrentState;


public:	
	// STATE MACHINE METHODS
#ifdef DIRECTORY_INPUT
	StateMachine(const vector<cv::String>&);
#else
	StateMachine();
#endif

	int spin(shared_ptr<SigInit>);	
	void PrintMachineInfo();
	~StateMachine();

};

#endif // STATEMACHINE_H
