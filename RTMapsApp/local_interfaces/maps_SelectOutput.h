////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_SelectOutput_H
#define _Maps_SelectOutput_H

#include "/opt/rtmaps/sdk/ubuntu1604_x86_64/include/maps.hpp"
#include "/opt/rtmaps/sdk/ubuntu1604_x86_64/include/maps.h"

using namespace std;

//Fucntion definitions

// Declares a new MAPSComponent child 
class MAPSSelectOutput : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSSelectOutput)
private :
	// Place here your specific methods and attributes

		// This member variable will be used for allocating once the output buffers at first time.
	bool m_firstTime;

public:
};

#endif
