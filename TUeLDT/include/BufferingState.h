#ifndef BUFFERINGSTATE_H
#define BUFFERINGSTATE_H
#include "State.h"
#include "BufferingDAG_generic.h"
#include "opencv2/core/ocl.hpp"

using namespace cv;
using namespace Eigen;
using namespace std;

class BufferingState : public State
{
	
friend class TEST_BufferingState;
	
private:

	std::thread mSideExecutor;
	
public:		
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
	
	 BufferingState();
	~BufferingState();

};

#endif // BUFFERINGSTATE_H
