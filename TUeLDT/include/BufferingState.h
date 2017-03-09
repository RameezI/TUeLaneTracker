#ifndef BUFFERINGSTATE_H
#define BUFFERINGSTATE_H
#include "compileConfig.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "GaussianPreProcessor.h"
#include "ColorTransformer.h"
#include "ChannelsSplitter.h"
#include "GradientsExtractor.h"
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
protected:
#endif

//Private Variables needed for Computations

	const Vector2i			   	mRES_VH;
	const LaneMembership	   	mLaneMembership;
	
	Mat mOientationTemplate; 
	Mat mDEPTHTemplate;

	//State Dependencies
	shared_ptr<VanishingPt> 	mVanishingPt;
	shared_ptr<Templates>		mTemplates;
	shared_ptr<Likelihoods> 	mLikelihoods;


	//Extracted Templates
	Mat     mGradientDirTemplate;
	Mat     mDepthMapTemplate;

	//Image Frames
	Mat     mFrameRGB;
	Mat		mFrameGRAY;

	//Image Gradients
	Mat     mFrameGradMag;
	Mat     mFrameGradAng;

	//Matrices computed on the GPU
	UMat	mProbMap_Gray;
	UMat	mProbMap_GradMag;
	UMat    mProbMap_GradDir;



	#ifdef DIRECTORY_INPUT
		uint mCountFrame;
		vector<cv::String> mFiles;
	#endif



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
	
	BufferingState(const CameraProperties& CAMERA, const LaneMembership& MEMBERSHIP);
	void injectDependencies(shared_ptr<VanishingPt>	,
							shared_ptr<Templates>	,	
							shared_ptr<Likelihoods>);	
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
