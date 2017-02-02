#include "BufferingState.h"

BufferingState::BufferingState()
: mFrame(make_shared<Mat>()),
#ifdef DIRECTORY_INPUT
  mCount(0)
#endif
{
	
	
	
}



int BufferingState::grabFrame()
{
	
#ifdef DIRECTORY_INPUT
 
	*mFrame = imread(mFiles[mCount]);		
	
	if(!mFrame->data)
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
				
				imshow( "Display window", *mFrame );
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

