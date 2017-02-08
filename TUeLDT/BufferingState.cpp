#include "BufferingState.h"

BufferingState::BufferingState()
: mFrameRGB_double(make_shared<Mat>()),
  mFrameRGB(make_shared<Mat>()),
  mFrameGRAY(make_shared<Mat>()),
  mFrameHSV(make_shared<Mat>()),
  mFrameH(make_shared<Mat>()),   
  mFrameS(make_shared<Mat>()),
  mFrameV(make_shared<Mat>()),
  mFrameHS(make_shared<Mat>()), 
  
  
  mFrameGradMag_Gray(make_shared<Mat>()),
  mFrameGradMag_S(make_shared<Mat>()),
  mFrameGradMag_V(make_shared<Mat>()),
  mFrameGradMag_HS(make_shared<Mat>()),
  
  mFrameGradAng_Gray(make_shared<Mat>()),
  mFrameGradAng_V(make_shared<Mat>()),
  mFrameGradAng_S(make_shared<Mat>()),
  mFrameGradAng_HS(make_shared<Mat>()),

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
				mFrameRGB->convertTo(*mFrameRGB_double, CV_32FC3, 1/255.0); // Rescaling image to the range [0 1]
				GaussianPreProcessor filterGauss(*mFrameRGB_double, *mFrameRGB_double);
				
				

				ColorTransformer 	 rgb2gray(*mFrameRGB_double, *mFrameGRAY, cv::COLOR_BGR2GRAY); 
				ColorTransformer 	 rgb2hsv (*mFrameRGB_double, *mFrameHSV,  cv::COLOR_BGR2HSV);
				
				ChannelsSplitter 	 hsvSplit(*mFrameHSV, *mFrameH, *mFrameS, *mFrameV);
				
				mFrameH->convertTo(*mFrameH, CV_32FC1, 1/360.0); //Rescalling H channel to a range of [0 1]
				*mFrameHS = mFrameH->mul(*mFrameS);

				GradientsExtractor   GradientGray(*mFrameGRAY, *mFrameGradMag_Gray , *mFrameGradAng_Gray);
				GradientsExtractor	 GradientS   (*mFrameS   ,  *mFrameGradMag_S   , *mFrameGradAng_S);
				GradientsExtractor	 GradientSH  (*mFrameHS  ,  *mFrameGradMag_HS  , *mFrameGradAng_HS);
				GradientsExtractor	 GradientV   (*mFrameV   ,  *mFrameGradMag_V   , *mFrameGradAng_V);
				
				//GaussianPreProcessor filterGauss(*mFrameGradMag_HS, *mFrameRGB);
				
				
				//mFrameHS->convertTo(*mFrameHS, CV_32FC1, 360);
				imshow( "Display window", *mFrameGradMag_V);
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

