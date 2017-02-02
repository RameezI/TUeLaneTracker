#ifndef BUFFERINGSTATE_H
#define BUFFERINGSTATE_H
#include "compileConfig.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ColorTransformer.h"
#include "ChannelsSplitter.h"
#include "ChannelsMerger.h"
#include "GradientsExtractor.h"
#include "State.h"

using namespace cv;
using namespace Eigen;
using namespace std;


class BufferingState : public State
{
	
	
private:
shared_ptr<Mat>         mFrameRGB;
shared_ptr<Mat> 	    mFrameHSV;
shared_ptr<Mat>			mFrameH;   //use split
shared_ptr<Mat>			mFrameS;
shared_ptr<Mat>			mFrameV;
shared_ptr<Mat>			mFrameHS; //use merge
shared_ptr<Mat>			mFrameGRAY;




shared_ptr<VanishingPt> mVanishingPt;
shared_ptr<Templates>	mTemplates;
shared_ptr<Likelihoods> mLikelihoods;
         
	
public:
	BufferingState();
	void run();
	void conclude();
	int  grabFrame();
	~BufferingState();
	
	#ifdef DIRECTORY_INPUT
	void setSource(const vector<cv::String>& files);
	vector<cv::String> mFiles;
	int mCount;
	#endif
};

#endif // BUFFERINGSTATE_H
