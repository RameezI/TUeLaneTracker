/** \file */ 
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "StateMachine.h"
using namespace std;
using namespace cv;

int main() /**  
		This is the entry point of the application.
		- Sets the path to the input images if flag DIRECTORY_INPUT is defined
		- Initialises the sigInit handler
		- Creates a stateMachine and spins it until user issues a quit signal through the sigInit handler. 		
	  */
{
	
#ifdef PROFILER_ENABLED
	  Logger::Init();
#endif	
	
	
#ifdef DIRECTORY_INPUT
	 
	 vector< cv::String> IMG_filenames;
	 cv::String folder = "/media/rameez/Linux-Extended/DataSet/eindhoven/PNG_imgs";
	 //cv::String folder = "/home/rameez/Desktop/subDataSet";
	 glob(folder, IMG_filenames);
	 glob(folder, IMG_filenames);
	 const uint skipFrames = 4000;	
	 
	if (IMG_filenames.size() <= skipFrames)
	{
	 cout<<endl;
	 cout<<"Total Number of Image Files to Process : " << 0;
	 cout<<endl;
	 return -1; //Nothing to do
	}

	 cout<<endl;
	 cout<<"Total Number of Image Files to Process : " << IMG_filenames.size() - skipFrames;
	 cout<<endl;



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


#ifdef DIRECTORY_INPUT	
	StateMachine stateMachine(IMG_filenames, skipFrames);
#else
	StateMachine stateMachine;
#endif


	return stateMachine.spin(sigInit);

}
