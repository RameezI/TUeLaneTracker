#include "compileConfig.h"

#include <iostream>
#include <string>
#include <memory>
#include <sys/stat.h>
#include <Eigen/Dense>
#include "opencv2/opencv.hpp"
#include <opencv2/core/eigen.hpp>
#include <math.h>
#include <fstream>

#include "LDT_profiler.h"
#include "LDT_logger.h"

#include "Camera.h"
#include "Lane.h"
#include "Car.h"
#include "LaneFilter.h"
#include "VanishingPtFilter.h"
#include "VanishingPtFilter.h"
#include "Templates.h"


#ifdef s32v2xx
#include <apex.h>
#include <oal.h>
#include <umat.hpp>
#include "frame_output_v234fb.h"
#endif

#ifndef STATE_H
#define STATE_H

using namespace std;
using namespace Eigen;
using namespace cv;


enum States{BOOTING, BUFFERING, DETECTING_LANES, DISPOSING };
enum StateStatus {DONE, ACTIVE, INACTIVE, ERROR };
enum FrameSource {DIRECTORY, RTSP, GMSL};




class State
{
	
protected:

	#ifdef PROFILER_ENABLED
	ProfilerLDT mProfiler;
	#endif
	
public:

	static const  int      	sNbBuffer      =  5;
	int64_t 		StateCounter   =  0;     	
	StateStatus 	       	currentStatus  =  StateStatus::INACTIVE;
	
	void dispose(){currentStatus = StateStatus::ERROR;}

	State(){}
	~State(){}
};
#endif
