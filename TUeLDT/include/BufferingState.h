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
private:
#endif

//Private Variables needed for Computations

Mat                         mGradientDir;
Mat                         mDepth;

const Vector2i			   	mRES_VH;
const LaneMembership	   	mLaneMembership;
const MatrixXf				mOnes;
MatrixXf  				   	mFrameProbMap;
MatrixXf  				   	mKeyFrameProbMap;
MatrixXf  				   	mKeyFrameGradDir;

MatrixXf					mProbMap_Gray;
MatrixXf					mProbMap_GradMag;
MatrixXf					mProbMap_GradDir;


//Image Frames
shared_ptr<Mat>        		mFrameRGB;
shared_ptr<Mat>		   		mFrameGRAY;
shared_ptr<Mat>        		mFrameGRAY_float;

//Image Gradients
shared_ptr<Mat>         	mFrameGradMag;
shared_ptr<Mat>         	mFrameGradAng;

//State Dependencies
shared_ptr<VanishingPt> 	mVanishingPt;
shared_ptr<Templates>		mTemplates;
shared_ptr<Masks> 			mMasks;
shared_ptr<Likelihoods> 	mLikelihoods;





#ifdef DIRECTORY_INPUT
	uint mCountFrame;
	vector<cv::String> mFiles;
#endif



private:
	void executeDAG_buffering();
	void applyGaussian();
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
							shared_ptr<Masks>		,
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
