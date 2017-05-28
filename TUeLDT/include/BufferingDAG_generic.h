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
	
	BufferPool	    			mBufferPool;
	
	Mat mGRADIENT_TAN_ROOT;
    Mat	mFOCUS_MASK_ROOT;
    Mat	mDEPTH_MAP_ROOT;
				
	Mat mGradTanTemplate; 
	Mat mDepthTemplate;
	Mat mFocusTemplate;


	//Image Frames
	Mat     mFrameRGB;
	Mat		mFrameGRAY;
	Mat		mFrameGRAY_float;

	//Image Gradients
	Mat     mFrameGradMag;
	Mat     mFrameGradAng;

	// Temporary Probability Maps
	Mat	mProbMap_Gray;
	Mat	mProbMap_GradMag;
	Mat mProbMap_GradDir;
	
	Mat mProbabiltyMap_Focussed;

	
#ifdef DIRECTORY_INPUT
	uint 			   mCountFrame;
	vector<cv::String> mFiles;
#endif

private:
		int 			mCurrentBufferPos;
		VanishingPt 	mVanishPt;
		
protected:
	void executeDAG_buffering();
	void extractTemplates();
	void computeProbabilities();
	void computeOrientedGradients();
	int  grabFrame();
	

public:
	BufferingDAG_generic();
   ~BufferingDAG_generic();

};

#endif // BUFFERINGDAG_GENERIC_H
