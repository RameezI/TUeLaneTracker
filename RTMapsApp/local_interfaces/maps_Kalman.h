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
	const int N_STATES = 3;
	float timestamp_prev = 0;

	// System Model
		//Process
		Matrix3f A;
		MatrixXf B;
		Matrix3f Q;
		//Observer
		Matrix3f C;
		Matrix3f R;
		Matrix3f R0;
		//States
		MatrixXf x;
		MatrixXf x_hat;
		MatrixXf y;
		Matrix3f P;
		Matrix3f P_hat;
		//Output
		MatrixXf u;
		//Kalman
		MatrixXf y_tilde;
		Matrix3f S;
		Matrix3f K;
		//Misc
		Matrix3f I;
	
	void Adjust(float vx, float c, float dt);
	void Predict();
	void Update();
public:
};

#endif
