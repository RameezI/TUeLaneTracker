////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_LaneTracker_H
#define _Maps_LaneTracker_H

// Includes maps sdk library header
#include <opencv2/opencv.hpp>
#define __IPL_H__
#include "/opt/rtmaps/sdk/ubuntu1604_x86_64/include/maps.hpp"
#include "/opt/rtmaps/sdk/ubuntu1604_x86_64/include/maps.h"

#include "FrameFeeder.h"
#include "StateMachine.h"
#include "boost/program_options.hpp"

using namespace std;
namespace po = boost::program_options;


//Fucntion definitions

// Declares a new MAPSComponent child class
class MAPSLaneTracker : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLaneTracker)
private :
	// Place here your specific methods and attributes

		// This member variable will be used for allocating once the output buffers at first time.
	bool m_firstTime;

	unique_ptr<FrameFeeder>  lPtrFeeder;
	StateMachine* lPtrStateMachine;
	States lPreviousState;
	int nbCycles;
	int lReturn;

	cv::Mat* lImgPtr;
public:
	unique_ptr<FrameFeeder> createFrameFeeder();
};

#endif
