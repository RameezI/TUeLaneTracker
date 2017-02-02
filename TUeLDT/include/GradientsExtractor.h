#ifndef GRADIENTEXTRACTOR_H
#define GRADIENTEXTRACTOR_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
class GradientsExtractor
{
public:
	GradientsExtractor(const cv::Mat& src, cv::Mat& dst1, cv::Mat& dst2);
	~GradientsExtractor();

};

#endif // GRADIENTEXTRACTOR_H
