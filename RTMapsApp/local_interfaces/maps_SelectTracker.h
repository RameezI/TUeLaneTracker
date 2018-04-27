////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_SelectTracker_H
#define _Maps_SelectTracker_H

#include "/opt/rtmaps/sdk/ubuntu1604_x86_64/include/maps.hpp"
#include "/opt/rtmaps/sdk/ubuntu1604_x86_64/include/maps.h"

using namespace std;

//Fucntion definitions

// Declares a new MAPSComponent child class
class MAPSSelectTracker : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSSelectTracker)
private :
	// Place here your specific methods and attributes

		// This member variable will be used for allocating once the output buffers at first time.
	bool m_firstTime;

public:
};

#endif
