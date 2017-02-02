#ifndef BUFFERINGSTATE_H
#define BUFFERINGSTATE_H
#include "compileConfig.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "States.h"

using namespace cv;
using namespace Eigen;
using namespace std;


class BufferingState : public State
{
	
	
private:
shared_ptr<VanishingPt> mVanishingPt;
shared_ptr<Templates>	mTemplates;
shared_ptr<Likelihoods> mLikelihoods;
shared_ptr<Mat>         mFrame;          
	
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
