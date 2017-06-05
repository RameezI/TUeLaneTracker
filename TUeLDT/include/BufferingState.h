#ifndef BUFFERINGSTATE_H
#define BUFFERINGSTATE_H
#include "State.h"
#include "BufferingDAG_generic.h"
#include <thread>

using namespace cv;
using namespace Eigen;
using namespace std;

class BufferingState : public State
{
	
public:

	std::thread mSideExecutor;
	
public:		
	BufferingState();
		
	BufferingDAG_generic bufferingGraph;
	
	void setupDAG(const Templates& templates);
	void run();
	
	#ifdef PROFILER_ENABLED
	void getOpenClInfo();
	#endif
		
	#ifdef DIRECTORY_INPUT
	void setSource(const vector<cv::String>& files);
	#else
	void setSource();
	#endif

	~BufferingState();

};

#endif // BUFFERINGSTATE_H
