#include "StateMachine.h"
#include "States.h"
using namespace std;
using namespace cv;


States StateMachine::sCurrentState = States::BOOTING;


/*Constructor*/
#ifdef DIRECTORY_INPUT    // ^TODO: Define Seprate Constructor for camera input which takes SDI object.
StateMachine::StateMachine(const vector<cv::String>& filenames) 
: mFiles(filenames),
  mStateStrings({ "BOOTING", "BUFFERING", "DETECTING_LANES", "RESETING" })


{
	//^TODO: Constructor Overloading in case of Camera
	//^TODO: STATE machine should run staes indefinitely unless interruted by user signal
	//^TODO: No for Loop over a set of images inside StateMachine::run function. ^Single Responsibility
	
	
	
	mLaneFilter			=	make_shared<LaneFilter>(mLane.mPROPERTIES, mCamera.mCM_TO_PIXEL);
    mVanishingPtFilter 	= 	make_shared<VanishingPtFilter>(mLaneFilter->mBINS_HISTOGRAM, mCamera.mPROPERTIES);
	

	
}
#endif

 int StateMachine::run(shared_ptr<SigInit> sigInit)
{
		//Creates States
		InitState 			booting(mCamera.mPROPERTIES, mLaneFilter, mVanishingPtFilter);
		BufferingState		buffering;
		
		//return 0;
	
	while (sigInit->sStatus!=SigStatus::STOP)
	{
		
	
		
/**************************************************************************************************/
										// INITIALISING //
		
		switch (sCurrentState)
		{
		
		case  States::BOOTING : { 
									 if (booting.mStateStatus == StateStatus::ACTIVE) {
											booting.run(); 
											booting.sStateCounter ++; 
										}
											
/*Transition*/					else if (booting.mStateStatus == StateStatus::DONE) {  
											
											//^TODO: get all allocated memory Maps
											booting.conclude();
											sCurrentState =	States::BUFFERING;
										}
										
									else {
										
/*Transition*/							   sCurrentState = States::DISPOSING;
										
										 }
		
								  }
		break;
		
		
/***************************************************************************************************/
											// BUFFERING //
		
		case States::BUFFERING : {
			
			
									 if (buffering.mStateStatus == StateStatus::INACTIVE) {
										 #ifdef DIRECTORY_INPUT 
											buffering.setSource(mFiles);
										 #endif
										}
											
							  else  if (buffering.mStateStatus  == StateStatus::ACTIVE) {  
											
											buffering.run();
										}
										
/*Transition*/				 else	if(buffering.mStateStatus  == StateStatus::DONE )  {
	
	
										    buffering.conclude();										
										    sCurrentState = States::DETECTING_LANES;
										 
										 }
								
/*Transition*/						else {
									
											sCurrentState = States::DISPOSING;
		
										 }

		 						 }
		break;
/***********************************************************************************************************/		  
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

