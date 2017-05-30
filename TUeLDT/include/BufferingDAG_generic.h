#ifndef BUFFERINGDAG_GENERIC_H
#define BUFFERINGDAG_GENERIC_H
#include "State.h"
#include "Camera.h"
#include "Lane.h"
#include <opencv2/core/eigen.hpp>
#include "opencv2/core/ocl.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"


class BufferingDAG_generic
{

friend class BufferingState;
	
protected:

    ProfilerLDT                 mProfiler;
	
	const Camera 				mCAMERA;
	const LaneMembership	   	mLaneMembership;
	
	int mSpan;
	int mMargin;
	int mVP_Range_V;
		
	Mat mGRADIENT_TAN_ROOT;
    Mat	mFOCUS_MASK_ROOT;
    Mat	mDEPTH_MAP_ROOT;
				
	Mat mGradTanTemplate; 
	Mat mDepthTemplate;
	Mat mFocusTemplate;

	BufferPool	    			mBufferPool;
	
	//Image Frames
	Mat     mFrameRGB;
	Mat		mFrameGRAY;
	Mat		mFrameGRAY_ROI;

	//Image Gradients
	Mat     mFrameGradMag;
	Mat     mFrameGradTan;

	// Temporary Probability Maps
	Mat	mProbMap_Gray;
	Mat	mProbMap_GradMag;
	Mat mProbMap_GradDir;
	
	Mat mProbabiltyMap_Focussed;

	VanishingPt 	mVanishPt;
	
#ifdef DIRECTORY_INPUT
	uint 			   mCountFrame;
	vector<cv::String> mFiles;
#endif

private:
		int 			mCurrentBufferPos;
		
		
public:

	int  grabFrame();
	void executeDAG_buffering();
	
	

public:
	BufferingDAG_generic();
   ~BufferingDAG_generic();

};

#endif // BUFFERINGDAG_GENERIC_H
