////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_Kalman_H
#define _Maps_Kalman_H

#include "/opt/rtmaps/sdk/ubuntu1604_x86_64/include/maps.hpp"
#include "/opt/rtmaps/sdk/ubuntu1604_x86_64/include/maps.h"
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

//Fucntion definitions

// Declares a new MAPSComponent child class
class MAPSKalman : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSKalman)
private :
	// Place here your specific methods and attributes

		// This member variable will be used for allocating once the output buffers at first time.
	bool m_firstTime;

	// System Model
		//Process
		Matrix4f A;
		//Matrix4f AT;
		MatrixXf B;
		Matrix4f Q;
		//Observer
		Matrix4f C;
		//Matrix4f CT;
		Matrix4f R;
		Matrix4f R0;
		//States
		MatrixXf x;
		MatrixXf x_hat;
		MatrixXf y;
		Matrix4f P;
		Matrix4f P_hat;
		//Output
		MatrixXf u;
		//Kalman
		MatrixXf y_tilde;
		Matrix4f S;
		Matrix4f K;
		//Misc
		Matrix4f I;
	
	void Adjust(float vx, float c);
	void Predict();
	void Update();
public:
};

#endif
