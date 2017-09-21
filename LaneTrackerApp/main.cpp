/** \file */ 
#include "StateMachine.h"
using namespace std;
using namespace cv;

int main() /**  
		This is the entry point of the application.
		- Initialises the sigInit handler
		- Creates a stateMachine and spins it until user issues a quit signal through the sigInit handler. 		
	  */
{

	int lReturn =0;
	
	#ifdef PROFILER_ENABLED
	  Logger::Init();
	#endif	
	
	shared_ptr<SigInit> sigInit= make_shared<SigInit>();
	
	if(sigInit->sStatus == SigStatus::FAILURE) 
	   lReturn|= -1;
  
	std::cout<<"******************************"<<std::endl;
	std::cout<<" Press Ctrl+C to terminate."<<std::endl;
	std::cout<<"******************************"<<std::endl;

	StateMachine stateMachine;

	lReturn = stateMachine.spin(sigInit);
	exit(lReturn);
}
