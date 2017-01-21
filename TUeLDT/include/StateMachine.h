#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include <memory>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Lane.h"
#include "sigInit.h"
#include "Camera.h"
using namespace std;
class StateMachine
{
	private:
	const vector<cv::String> mFileNames;
	static int mStateCounter;
	Lane mLane;
	
	public:
	StateMachine(const vector<cv::String>&);
	int run(shared_ptr<SigInit>);
	~StateMachine();

};

#endif // STATEMACHINE_H
