/** \file */ 
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "StateMachine.h"
using namespace std;
using namespace cv;

int main() /**  
		This is the entry point of the application.
		- Initialises the sigInit handler
		- Creates a stateMachine and spins it until user issues a quit signal through the sigInit handler. 		
	  */
{
	
#ifdef PROFILER_ENABLED
	  Logger::Init();
#endif	
	
	shared_ptr<SigInit> sigInit= make_shared<SigInit>();
	
	 //Check if Siginit is properly initialised
	if(sigInit->sStatus == SigStatus::FAILURE) 
	{
		return -1;
	}
  
	std::cout<<"******************************"<<std::endl;
	std::cout<<" Press Ctrl+C to terminate."<<std::endl;
	std::cout<<"******************************"<<std::endl;

	StateMachine stateMachine;

	return stateMachine.spin(sigInit);
}
