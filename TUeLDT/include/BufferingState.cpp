#include "BufferingState.h"

BufferingState::BufferingState()
: mFrameRGB(make_shared<Mat>()),
#ifdef DIRECTORY_INPUT
  mCount(0)
#endif
{
	
	
	
}



int BufferingState::grabFrame()
{
	
#ifdef DIRECTORY_INPUT
 
	*mFrameRGB = imread(mFiles[mCount]);		
	
	if(!mFrameRGB->data)
			return -1;
			
	else	mCount ++;
			return 0;
			

#endif




}


void BufferingState::conclude()
{
	
	
	
}

void BufferingState::run()
{
	
#ifdef DIRECTORY_INPUT


	if (mCount < mFiles.size()) {
		
		if (0==grabFrame())
			{
				//example wrapper---> could be used to Map Color tranformation to a a ROS node or Apex Process
				ColorTransformer RGB2GRAY(*mFrameRGB, *mFrameGRAY, cv::COLOR_BGR2GRAY);
				//ColorTransformer RGB2HSV (*mFrameRGB, *mFrameHSV,  cv::COLOR_BGR2HSV);
				//ChannelsSplitter  HSVSPLIT(*mFrameHSV, *mFrameH, *mFrameS, *mFrameV);
				
				imshow( "Display window", *mFrameRGB );
				waitKey(1);
			}
		
		else
			
			cerr << "Problem loading image!!!" << endl;
												
		}
		
	else		
		{
		   mStateStatus = StateStatus::DONE;
			
		} 
	
	sStateCounter++;
	
#endif

}




#ifdef DIRECTORY_INPUT
void BufferingState::setSource(const vector<cv::String>& files)
{
	mFiles = files;
	mStateStatus= StateStatus::ACTIVE;
}
#endif

BufferingState::~BufferingState()
{
	
	
}

