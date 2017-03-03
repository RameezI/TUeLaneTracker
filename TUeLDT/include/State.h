#include "compileConfig.h"
#include <iostream>
#include <string>
#include <memory>
#include <Eigen/Dense>
#include "opencv2/opencv.hpp"
#include <math.h>
#include "LDT_profiler.h"
#include "LDT_logger.h"

#ifdef s32v2xx
#include "frame_output_v234fb.h"
#endif

#ifndef STATE_H
#define STATE_H

using namespace std;
using namespace Eigen;
using namespace cv;


enum States{BOOTING, BUFFERING, DETECTING_LANES, RESETING, DISPOSING }; //^TODO: Add PAUSED state as well
enum StateStatus {DONE, ACTIVE, INACTIVE, ERROR };


class State
{

protected:
	
	
	
	#ifdef PROFILER_ENABLED
	ProfilerLDT mProfiler;
	#endif

    
	
	
public:

	static int 			   sStateCounter;     	//Number Of Itterations in a state
	static const  int      sNbBuffer =3;
	const string 		   mStatusStrings[4];
	
	StateStatus 		   mStateStatus;
	void printStatus();
	
	State();
	~State();
};

/******************************************************************************************* */
								/*  TYPE_DEFINITIONS */
/******************************************************************************************* */

struct Templates
{
  MatrixXf GRADIENT_DIR_ROOT;
  MatrixXf FOCUS_ROOT;
  MatrixXf DEPTH_ROOT;
  
  MatrixXf GRADIENT_DIR;
  MatrixXf DEPTH;
  
	Templates(const int RES_V, const int RES_H)
	{
	
		GRADIENT_DIR =  MatrixXf::Zero(RES_V, RES_H); 
		DEPTH 		 =  MatrixXf::Zero(RES_V, RES_H);
			
		/* Create Focus Template */
			const int Margin  = 100;
			FOCUS_ROOT     = MatrixXf::Zero(2*RES_V+1,RES_H);
			FOCUS_ROOT.block(RES_V+Margin, 0, RES_V+1-Margin, RES_H) = MatrixXf::Constant(RES_V +1 -Margin, RES_H, 1);
			
			for(int n = RES_V; n < RES_V + Margin; n++)
			{				
			  float x= pow(( (n+1)-RES_V )/float(Margin), 3);
			  FOCUS_ROOT.row(n) = ArrayXf::Constant(RES_H, x);			  
			}
			
		/* Create Depth Template */
			DEPTH_ROOT     	= MatrixXf::Zero(2*RES_V+1,RES_H);			
			const float step 	= 45.0/RES_V;
			float angle = 90- step;
			
			for(int n = RES_V; n < 2*RES_V + 1; n++)
			{
			  float x= 1.75 * tan(angle * M_PI/180.0 );
			   
			   if( x > 100) x=100;
			  DEPTH_ROOT.row(n) = ArrayXf::Constant(RES_H, x);
			  angle = angle - step;
			}			
			
			
	     /* Create Gradient Direction Template */
			GRADIENT_DIR_ROOT  = MatrixXf::Zero(2*RES_V+1,2*RES_H+1);
		
	}	
  
}; 



struct VanishingPt
{
  int  V;
  int  H;
  int  V_prev;
  int  H_prev;
  VanishingPt()
  : V(0),H(0),V_prev(0),H_prev(0){}
};



struct Likelihoods
{
		std::array<MatrixXf, State::sNbBuffer> TOT_ALL;
		std::array<MatrixXf, State::sNbBuffer> GRADIENT_DIR_ALL;
		std::array<MatrixXf, State::sNbBuffer> TOT_ALL_BACKUP;
		std::array<MatrixXf, State::sNbBuffer> GRADIENT_DIR_ALL_BACKUP;
		
		MatrixXf  TOT_MAX;
		MatrixXf  TOT_MAX_FOCUSED;
		MatrixXf  GRADIENT_DIR_TOT_MAX;
		
		
		Likelihoods(const int RES_V, const int RES_H)
		{
			
				TOT_MAX =  MatrixXf::Zero(RES_V, RES_H);
				GRADIENT_DIR_TOT_MAX = MatrixXf::Zero(RES_V, RES_H);
				TOT_MAX_FOCUSED = MatrixXf::Zero(RES_V, RES_H);
   
				for (int i=0; i< State::sNbBuffer; i++)
				{
					TOT_ALL[i]= MatrixXf::Zero(RES_V,RES_H);
					GRADIENT_DIR_ALL[i]= MatrixXf::Zero(RES_V,RES_H);
				}
	
				TOT_ALL_BACKUP = TOT_ALL;
				GRADIENT_DIR_ALL_BACKUP = GRADIENT_DIR_ALL;		
		}		
};

struct Masks
{		
		MatrixXf  FOCUS;  //^Todo: Convert to UMat
		
		Masks(const int RES_V, const int RES_H)
		{
			FOCUS =  MatrixXf::Zero(RES_V, RES_H);
		}
};


#endif // STATE_H
