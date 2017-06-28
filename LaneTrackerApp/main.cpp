#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "StateMachine.h"
using namespace std;
using namespace cv;

int main()
{
	
#ifdef PROFILER_ENABLED
	  Logger::Init();
#endif	
	
	
#ifdef DIRECTORY_INPUT
	 
	 vector< cv::String> IMG_filenames;
	 cv::String folder = "/media/rameez/Linux-Extended/DataSet/eindhoven/PNG_imgs";
	 glob(folder, IMG_filenames);
	 const int skipFrames = 4000;	
	 cout<<endl;
	 cout<<"Total Number of Image Files : " << IMG_filenames.size() - skipFrames;
	 cout<<endl;
	 
	 if (IMG_filenames.size()- skipFrames < 1)
		 return -1;
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
