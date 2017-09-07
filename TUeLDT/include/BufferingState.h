#ifndef BUFFERINGSTATE_H
#define BUFFERINGSTATE_H
#include "State.h"
#include "opencv2/core/ocl.hpp"

#ifdef s32v2xx
#include "BufferingDAG_s32v.h"
#else
#include "BufferingDAG_generic.h"
#endif

using namespace cv;
using namespace Eigen;
using namespace std;

class BufferingState : public State
{
	
friend class TEST_BufferingState;
	
private:
	uint_fast8_t mRetryGrab;
	std::thread mSideExecutor;
	
public:		
	int setSource();
	void setupDAG(const Templates& templates);
	void run();
	
	#ifdef s32v2xx	
	   BufferingDAG_s32v    bufferingGraph;
	#else
	   BufferingDAG_generic bufferingGraph;
	#endif

	#ifdef PROFILER_ENABLED
	void getOpenClInfo();
	#endif
		
	
	 BufferingState();
	~BufferingState();

};

#endif // BUFFERINGSTATE_H
