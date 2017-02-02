#include "States.h"


int State::sStateCounter =0;



State::State()
: mStatusStrings({"DONE" "ACTIVE" "INACTIVE" "ERROR" }),
  mStateStatus(StateStatus::INACTIVE)
{
	
	
}

State::~State()
{
	
	
	
}

