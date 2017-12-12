/** \file */ 
#include "StateMachine.h"
#include "State.h"
using namespace std;
using namespace cv;

int main() /**  
		This is the entry point of the application.
		- Initialises the sigInit handler
		- Creates a stateMachine and spins it until user issues a quit signal through the sigInit handler. 		
	  */
{
	FrameSource lFrameSource = FrameSource::DIRECTORY;
	string	    lSourceStr 	 = "";
        //string lSourceStr = "/media/rameez/Linux-Extended/DataSet/eindhoven/PNG_imgs";
        //string lSourceStr = "/home/root/data/Eindhoven";
        

	int lReturn = 0;
	
	#ifdef PROFILER_ENABLED
	  Logger::Init();
	#endif	
	
	shared_ptr<SigInit> sigInit= make_shared<SigInit>();

	if(sigInit->sStatus == SigStatus::FAILURE) 
	   lReturn|= -1;
  

	std::cout<<"******************************"<<std::endl;
	std::cout<<" Press Ctrl + c to terminate."<<std::endl;
	std::cout<<"******************************"<<std::endl;


	StateMachine stateMachine(lFrameSource, lSourceStr);
	lReturn = stateMachine.spin(sigInit);

	return(lReturn);
}
