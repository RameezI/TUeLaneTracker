#ifndef LANE_H
#define LANE_H
#include <stdio.h>
#include <math.h>
#include "ScalingFactors.h"
	
struct LaneParameters
{

public:
	const float AVG_WIDTH;
	const float STD_WIDTH;
	const float MIN_WIDTH;
	const float MAX_WIDTH;
	
	LaneParameters() //Lane Widths in cm
	: AVG_WIDTH(300),STD_WIDTH(50),MIN_WIDTH(250),MAX_WIDTH(500) 
	{
		//Default Constructor
	}
};

struct LaneModel
{
public:
	    int 	leftOffset;
		int 	rightOffset;
		
		int 	confidenceLeft;
		int     confidenceRight;
		
		float   laneWidth;

		
		LaneModel()
		: leftOffset(0),rightOffset(0),confidenceLeft(0), confidenceRight(0), laneWidth(0)
		{
			
		}
};

struct LaneMembership 
	{
		
		public:
		const uint8_t   TIPPING_POINT_GRAY; 
		const uint8_t   TIPPING_POINT_GRAD_Mag;
		
		private:
		const float WIDTH_STD;
		
		public:
		const float WIDTH_DIFF_NORMA;
		const float WIDTH_DIFF_NOMIN;
		
		private:
		const float NEG_BOUNDARY_STD;
		
		public:
		const float NEG_BOUNDARY_NORMA;
		const float NEG_BOUNDARY_NOMIN;
		
		LaneMembership() //Initialisation of  members through initialisation list
		:TIPPING_POINT_GRAY(100),
		 TIPPING_POINT_GRAD_Mag(40),
		 
		 WIDTH_STD(15),
		 WIDTH_DIFF_NORMA( (1/sqrt( 2*M_PI*pow(WIDTH_STD,2) ))*SCALE_FILTER ),
		 WIDTH_DIFF_NOMIN( 2*pow(WIDTH_STD,2) ),
		 
		 NEG_BOUNDARY_STD(0.2),
		 NEG_BOUNDARY_NORMA( (2/sqrt(2*M_PI*pow(NEG_BOUNDARY_STD,2)))*SCALE_FILTER  ),
		 NEG_BOUNDARY_NOMIN(2*pow(NEG_BOUNDARY_STD,2))	 
		{
			//Default Constructor
		}
	};

#endif // LANE_H
