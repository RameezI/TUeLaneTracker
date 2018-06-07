////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_MovingAverage_H
#define _Maps_MovingAverage_H

#include "/opt/rtmaps/sdk/ubuntu1604_x86_64/include/maps.hpp"
#include "/opt/rtmaps/sdk/ubuntu1604_x86_64/include/maps.h"
#include "SMA.h"

using namespace std;

//Fucntion definitions

// Declares a new MAPSComponent child class
class MAPSMovingAverage : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSMovingAverage)
private :
	// Place here your specific methods and attributes
    float average;
    SMA * pSMA;
		// This member variable will be used for allocating once the output buffers at first time.
	bool m_firstTime;

	
public:
};

#endif
