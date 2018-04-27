////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_ParamViewer_H
#define _Maps_ParamViewer_H

// Includes maps sdk library header
#include <opencv2/opencv.hpp>
#define __IPL_H__
#include "/opt/rtmaps/sdk/ubuntu1604_x86_64/include/maps.hpp"
#include "/opt/rtmaps/sdk/ubuntu1604_x86_64/include/maps.h"

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

//Fucntion definitions

// Declares a new MAPSComponent child class
class MAPSParamViewer : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSParamViewer)
private :
	// Place here your specific methods and attributes

		// This member variable will be used for allocating once the output buffers at first time.
	bool m_firstTime;
	int ResH, ResV;

	cv::Mat getLaneFrame(vector<float> dirParams);

public:
};

#endif
