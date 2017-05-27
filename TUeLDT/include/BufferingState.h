#ifndef BUFFERINGSTATE_H
#define BUFFERINGSTATE_H
#include "compileConfig.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "State.h"
#include "Camera.h"
#include "Lane.h"
#include <opencv2/core/eigen.hpp>
#include "opencv2/core/ocl.hpp"

using namespace cv;
using namespace Eigen;
using namespace std;

class BufferingState : public State


{
	
// Make public for testing	

#ifdef UNIT_TESTING
public:
#else
private:
#endif

	const Camera 				mCAMERA;
	const LaneMembership	   	mLaneMembership;
	Likelihoods	    			mLikelihoods;
	
	
	Mat mGRADIENT_DIR_ROOT;
    Mat	mFOCUS_MASK_ROOT;
    Mat	mDEPTH_MAP_ROOT;
				
	Mat mGradDirTemplate; 
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


	#ifdef DIRECTORY_INPUT
		uint mCountFrame;
		vector<cv::String> mFiles;
	#endif
	
	
protected:
	// List variable that are required by the derived states
	Mat mProbabiltyMap_Focussed;
	



protected:
	void executeDAG_buffering();
	void extractTemplates();
	void computeProbabilities();
	void computeOrientedGradients();
	int  grabFrame();
	
	#ifdef PROFILER_ENABLED
	void getOpenClInfo();
	#endif
	

public:
	
	BufferingState();
	void setTemplates(shared_ptr<Templates>);	
	void run();
	void conclude();
	
		
	#ifdef DIRECTORY_INPUT
	void setSource(const vector<cv::String>& files);
	#else
	void setSource();
	#endif

	~BufferingState();

};

#endif // BUFFERINGSTATE_H
