#ifndef LANE_H
#define LANE_H
#include <Eigen/Dense>

//double operator "" _cm( long double );
using namespace Eigen;


class Lane
{
	struct Properties
	{
		const float AVG_WIDTH;
		const float STD_WIDTH;
		const float MIN_WIDTH;
		const float MAX_WIDTH;
		
		Properties()
		: AVG_WIDTH(300),STD_WIDTH(50),MIN_WIDTH(250),MAX_WIDTH(500) 
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
		:GRAY(Vector2f(25, 0.6)),
		
		 YELL(Vector4f(100, 0.16-0.05, 100, 0.33+0.05)),
		 
		 SAT(Vector2f(10, 0.4)),
		 
		 MAG(Vector2f(50, 0.15)),
		 
		 DIR(Vector2f(-0.25, 15))
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
