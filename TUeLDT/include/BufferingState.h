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

using namespace cv;
using namespace Eigen;
using namespace std;


class BufferingState : public State
{
	
// Make public for testing	

public:
shared_ptr<Mat>        mFrameRGB;
shared_ptr<Mat>		   mFrameGRAY;
shared_ptr<Mat>        mFrameGRAY_float;

shared_ptr<Mat>         mFrameGradMag_Gray;
shared_ptr<Mat>         mFrameGradAng_Gray;

shared_ptr<VanishingPt> mVanishingPt;
shared_ptr<Templates>	mTemplates;
shared_ptr<Likelihoods> mLikelihoods;
         
	
public:
	uint mCountFrame;
	BufferingState();
	void run();
	void conclude();
	int  grabFrame();
	~BufferingState();
	
	#ifdef DIRECTORY_INPUT
	void setSource(const vector<cv::String>& files);
	vector<cv::String> mFiles;
	#else
	void setSource();
	#endif
};

#endif // BUFFERINGSTATE_H
