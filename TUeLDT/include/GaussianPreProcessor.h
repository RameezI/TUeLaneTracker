#ifndef GAUSSIANPREPROCESSOR_H
#define GAUSSIANPREPROCESSOR_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
class GaussianPreProcessor
{
public:
	GaussianPreProcessor(const cv::Mat& src, cv::Mat& dst);
	~GaussianPreProcessor();

};

#endif // GAUSSIANPREPROCESSOR_H
