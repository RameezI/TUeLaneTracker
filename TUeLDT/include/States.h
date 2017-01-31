#include <iostream>
#include <string>
#ifndef STATE_H
#define STATE_H

using namespace std;

enum StateStatus {DONE, ACTIVE, INACTIVE, ERROR };


class State
{
	


public:

	static int 		mStateCounter;     	//Number Of Itterations in a state
	const string 	mStatusStrings[4];
	StateStatus 	mStateStatus;
	
	void printStatus();
	
	State();
	~State();
};

#endif // STATE_H
