#include "Lane.h"

/* Default Unit used in this code is cm, no need to convert back to meters.
double operator "" _cm( long double cm)
{
	return cm/100;
}*/

Lane::Lane()
: AVG_WIDTH(300),STD_WIDTH(50),MIN_WIDTH(250),MAX_WIDTH(500) 
{
	
	
}

Lane::~Lane()
{	
	
}

