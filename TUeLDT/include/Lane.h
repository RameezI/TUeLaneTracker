#ifndef LANE_H
#define LANE_H
#include <Eigen/Dense>

//double operator "" _cm( long double );
using namespace Eigen;


	struct LaneProperties
	{
		const float AVG_WIDTH;
		const float STD_WIDTH;
		const float MIN_WIDTH;
		const float MAX_WIDTH;
		
		LaneProperties()
		: AVG_WIDTH(300),STD_WIDTH(50),MIN_WIDTH(250),MAX_WIDTH(500) 
		{
			
		}
		
	};
	
		struct LaneMembership 
	{
		/* The folllowing vectors define Sigmoid Membership of the Pixels */
		
		const Vector2f GRAY;  // First element is the decay rate of a sigmoid function wile second represent the shift.
		const Vector2f  MAG;
		const Vector2f  DIR;
		
		LaneMembership() //Initialisation of  members through initialisation list
		:GRAY(Vector2f(25, 0.6)),  // The gray value of 0.6 , in range of [0 1] , will have membership 0.5. 
								   // Membership will grow exponnetialy with an exponential constatnt 25.
		 MAG(Vector2f(50, 0.15)),
		 
		 DIR(Vector2f(-0.25, 15))
		 
		{
			//Default Constructor
		}
	};

class Lane
{

public:
	const LaneProperties 		mPROPERTIES;
	const LaneMembership		mMembership;
	Lane();
	~Lane();

};

#endif // LANE_H
