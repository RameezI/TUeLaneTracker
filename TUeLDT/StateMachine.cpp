#include "StateMachine.h"
#include "States.h"
using namespace std;
using namespace cv;



/* Initialise all static variables */
const int StateMachine::sNbBuffer =3;


/*Constructor*/
StateMachine::StateMachine(const vector<cv::String>& filenames) 
: mFileNames(filenames),
  mSource(Source::DIRECTORY),
  mStateStrings({ "BOOTING", "BUFFERING", "DETECTING_LANES", "RESETING" })


{
	//^TODO: Constructor Overloading in case of Camera
	//^TODO: STATE machine should run staes indefinitely unless interruted by user signal
	//^TODO: No for Loop over a set of images inside StateMachine::run function. ^Single Responsibility
	
	
	
	mLaneFilter			=	make_shared<LaneFilter>(mLane.mPROPERTIES, mCamera.mCM_TO_PIXEL);
    mVanishingPtFilter 	= 	make_shared<VanishingPtFilter>(mLaneFilter->mBINS_HISTOGRAM, mCamera.mPROPERTIES);
	

	
}


 int StateMachine::run(shared_ptr<SigInit> sigInit)
{
		//Creates States
		InitState 			booting(mCamera.mPROPERTIES, mLaneFilter, mVanishingPtFilter, sNbBuffer);
		BufferingState		buffering();
	
	
		mCurrentState= States::BOOTING;
	
	
	while (sigInit->sStatus==SigStatus::STOP)
	{
		
		switch (mCurrentState)
		{
		
		case  States::BOOTING : { 
									 if (booting.mStateStatus == StateStatus::INACTIVE) {
											booting.run(); 
											booting.mStateCounter ++; 
										}
											
	/*Transition*/				else if (booting.mStateStatus == StateStatus::DONE) {  
											
											//^TODO: get all allocated memory Maps
											booting.mStateCounter=0; 
											mCurrentState =	States::BUFFERING;
										}
										
									else {
										
	/*Transition*/							 mCurrentState = States::DISPOSING;
										
										 }
		
								  }
		break;
		
		case States::BUFFERING : {
			
			
			
			
		 						 }
		break;
		  
		}
		
	}
	
	
/*	
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
*/	
	
	
	
	return 0;
}
StateMachine::~StateMachine()
{
	
}

