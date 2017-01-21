#include "StateMachine.h"
#include "Camera.h"

using namespace std;
using namespace cv;

StateMachine::StateMachine(const vector<cv::String>& filenames) 
: mFileNames(filenames),
  mLane(Lane())
{

	
}

int StateMachine::run(shared_ptr<SigInit> sigInit)
{

	
	// Loop through every Image unless interrupted by user or read error.
	for(size_t i = 0; i < mFileNames.size(); ++i)
    {
        Mat image = imread(mFileNames[i]);
		imshow( "Display window", image );
		waitKey(1);
	
        if(!image.data)
		{
            cerr << "Problem loading image!!!" << endl;
			return -1;
		}
			
		if(sigInit->sStatus==SigStatus::STOP)
		{
			 cout << "Exit on Ctrl+C" << endl;
			return 0;
		} // if Ctrl+C	
			
    }
	
	return 0;
}

StateMachine::~StateMachine()
{
	
}

