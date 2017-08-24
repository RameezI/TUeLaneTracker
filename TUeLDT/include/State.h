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
#include  "Car.h"
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


enum States{BOOTING, BUFFERING, DETECTING_LANES, RESETING, DISPOSING }; //^TODO: Add PAUSED state as well
enum StateStatus {DONE, ACTIVE, INACTIVE, ERROR };


class State
{

	
protected:
	#ifdef PROFILER_ENABLED
	ProfilerLDT mProfiler;
	#endif
	
public:

	static const  int      	sNbBuffer =5;
	int64_t 		StateCounter;     	
	StateStatus 	       	currentStatus;
	
	void conclude(){currentStatus = StateStatus::DONE;}
	
	State();
	~State();
};


struct BufferPool
{
		#ifdef s32v2xx

		  std::array<vsdk::UMat, State::sNbBuffer> Probability;
		  std::array<vsdk::UMat, State::sNbBuffer> GradientTangent;
		  
		   BufferPool(const int RES_V, const int RES_H)
		   {
			for (int i=0; i< State::sNbBuffer; i++)
			{   
			   Probability[i]	= vsdk::UMat(RES_V, RES_H,  VSDK_CV_8UC1);
			   GradientTangent[i]	= vsdk::UMat(RES_V, RES_H,  VSDK_CV_16SC1);
			}
		   }

		#else

		  std::array<Mat, State::sNbBuffer> Probability;
		  std::array<Mat, State::sNbBuffer> GradientTangent;

		   BufferPool(const int RES_V, const int RES_H)
		   {	
			for (int i=0; i< State::sNbBuffer; i++)
			{   
			   Probability[i]	= Mat::zeros(RES_V, RES_H,  CV_8UC1);
			   GradientTangent[i]	= Mat::zeros(RES_V, RES_H, CV_16SC1);
			}
		   }

	 	#endif		
};

#endif
