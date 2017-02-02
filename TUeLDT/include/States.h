#include <iostream>
#include <string>
#include <memory>
#include <Eigen/Dense>
#include <math.h>
#include "StateMachine.h"
#ifndef STATE_H
#define STATE_H

using namespace std;
using namespace Eigen;

enum StateStatus {DONE, ACTIVE, INACTIVE, ERROR };


class State
{
	


public:

	static int 			   sStateCounter;     	//Number Of Itterations in a state
	static const  int      sNbBuffer =3;
	const string 		   mStatusStrings[4];
	StateStatus 		   mStateStatus;
	void printStatus();
	
	State();
	~State();
};



struct Templates
{
  MatrixXd GRADIENT_DIR_ROOT;
  MatrixXd PROB_ROOT;
  MatrixXd DEPTH_ROOT;
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
		std::array<MatrixXd, State::sNbBuffer> TOT_ALL;
		std::array<MatrixXd, State::sNbBuffer> GRADIENT_DIR_ALL;
		
		MatrixXd  TOT_FOCUSED;
		MatrixXd  TOT_MAX;
		MatrixXd  GRADIENT_DIR_AVG;
		MatrixXd  TOT_ALL_BACK_UP;
		MatrixXd  GRADIENT_DIR_ALL_BACK_UP;
};

struct Masks
{		
		MatrixXd  FOCUS;
		
};


#endif // STATE_H
