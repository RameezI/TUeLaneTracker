#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "StateMachine.h"
using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	//namedWindow( "Lane Tracker TU/e", WINDOW_AUTOSIZE ); // Create a window for display.
	vector< cv::String> IMG_filenames;
	cv::String folder = "/media/rameez/Linux-Extended/DataSet/eindhoven/PNG_imgs";
	glob(folder, IMG_filenames);
	
	//Object that handles Signal interrupts from the user i.e. ctrl+c
	shared_ptr<SigInit> sigInit= make_shared<SigInit>();
	
	 //Check if Siginit is properly initialised
	//if(sigInit->sStatus == SigStatus::FAILURE) 
	//{
	//	return -1;
	//}
  
	std::cout<<"******************************"<<std::endl;
	std::cout<<" Press Ctrl+C to terminate."<<std::endl;
	std::cout<<"******************************"<<std::endl;
	
	StateMachine stateMachine(IMG_filenames);
	return stateMachine.run(sigInit);

}
 