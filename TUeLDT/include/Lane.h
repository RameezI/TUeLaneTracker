#ifndef LANE_H
#define LANE_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <Eigen/Dense>

double operator "" _cm( long double );
using namespace Eigen;


class Lane
{
	struct Properties
	{
		const float AVG_WIDTH;
		const float STD_WIDTH;
		const float MIN_WIDTH;
		const float MAX_WIDTH;
		const int   CM_STEP;
		
		Properties()
		: AVG_WIDTH(200.0_cm),STD_WIDTH(0),MIN_WIDTH(0),MAX_WIDTH(0),CM_STEP(0) 
		{
			
		}
		
	};
	
	struct Distributions 
	{
		
		const Vector2f GRAY;
		const Vector4f YELL;
		const Vector2f  SAT;
		const Vector2f  MAG;
		const Vector2f  DIR;
		
		Distributions() //Initialisation of  members through initialisation list
		:GRAY((Vector2f()<< 0, 0).finished()),
		
		 YELL((Vector4f()<< 0, 0, 0, 0).finished()),
		 
		 SAT((Vector2f()<< 0, 0).finished()),
		 
		 MAG((Vector2f()<< 0, 0).finished()),
		 
		 DIR((Vector2f()<< 0, 0).finished())
		{
			//Default Constructor
		}
	};
	
public:
	Properties 		mProperties;
	Distributions		mDists;
	Lane();
	~Lane();

};

#endif // LANE_H
