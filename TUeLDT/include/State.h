#include "compileConfig.h"
#include <iostream>
#include <string>
#include <memory>
#include <sys/stat.h>
#include <Eigen/Dense>
#include "opencv2/opencv.hpp"
#include <opencv2/core/eigen.hpp>
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

	static const  int      sNbBuffer =3;
	int 			       StateCounter;     	
	StateStatus 	       currentStatus;
	
	void printStatus();
	
	State();
	~State();
};





/******************************************************************************************* */
								/*  TYPE_DEFINITIONS */
/******************************************************************************************* */

struct Templates
{
	
	public:
		const int MARGIN;
		const int VP_RANGE_V;
		const int SPAN;
		
		Mat GRADIENT_TAN_ROOT;
		Mat FOCUS_MASK_ROOT;
		Mat DEPTH_MAP_ROOT;

  
  
	Templates(const int RES_V, const int RES_H, const int VP_RANGE_ROWS)
	: MARGIN(80), 
	  VP_RANGE_V(VP_RANGE_ROWS), 
	  SPAN((RES_V/2)-MARGIN + VP_RANGE_ROWS) 
	{
		   
		/* Create Focus Template */			
			MatrixXi FOCUS_ROOT     = MatrixXi::Zero(SPAN + 2*VP_RANGE_V, RES_H);
			FOCUS_ROOT.block(2*VP_RANGE_V, 0, SPAN, RES_H) = MatrixXi::Constant(SPAN, RES_H, 255);			
			eigen2cv(FOCUS_ROOT, FOCUS_MASK_ROOT);
			FOCUS_MASK_ROOT.convertTo(FOCUS_MASK_ROOT, CV_8U);

			
		/* Create Depth Template */

			MatrixXf DEPTH_ROOT  = MatrixXf::Zero(2*RES_V +1,RES_H);			
			const float step 	= 45.0/RES_V;
			float angle = 90- step;
			
			for(int n = RES_V; n < 2*RES_V +1; n++)
			{
			  float x= 1.75 * tan(angle * M_PI/180.0 );
			   
			   if( x > 100) x=100;
			  DEPTH_ROOT.row(n) = ArrayXf::Constant(RES_H, x);
			  angle = angle - step;
			}
			
			eigen2cv(DEPTH_ROOT, DEPTH_MAP_ROOT);
			DEPTH_MAP_ROOT.convertTo(DEPTH_MAP_ROOT, CV_8U);



	  /* Load Gradient Tangent Template */
	    std::stringstream formattedString;
		string templateFile, prefix, format;
		prefix= "Templates/GradientTangent_";
		formattedString<<prefix<<std::to_string(RES_H)<<"x"<<std::to_string(RES_V);
		templateFile = formattedString.str();
		struct stat buf;
		int statResult = stat(templateFile.c_str(),&buf);
		if (statResult || buf.st_ino < 0) 
		{
			cout << "File not found: " << templateFile.c_str() << endl;
			exit(-2);
		}
		else
		{
			FileStorage loadGradientTemplate( templateFile, FileStorage::READ);
			loadGradientTemplate["ROOT_DIR_TEMPLATE"]>> GRADIENT_TAN_ROOT;
			GRADIENT_TAN_ROOT.convertTo(GRADIENT_TAN_ROOT,CV_16SC1);
		}		
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



struct BufferPool
{
		std::array<Mat, State::sNbBuffer> Probability;
		std::array<Mat, State::sNbBuffer> GradientTangent;
		
		BufferPool(const int RES_V, const int RES_H)
		{
			
				for (int i=0; i< State::sNbBuffer; i++)
				{
					Probability[i]= Mat::zeros(RES_V, RES_H,  CV_8UC1);
					GradientTangent[i]= Mat::zeros(RES_V,RES_H, CV_16SC1);
				}
		}		
};

#endif // STATE_H
