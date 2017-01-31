#include "States.h"


int State::mStateCounter =0;


State::State()
: mStatusStrings({"DONE" "ACTIVE" "INACTIVE" "ERROR" }),
  mStateStatus(StateStatus::INACTIVE)
{
	
	
}

State::~State()
{
	
	
	
}

