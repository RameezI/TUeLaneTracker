#include "GradientsExtractor.h"
using namespace cv;
GradientsExtractor::GradientsExtractor(const cv::Mat& src, cv::Mat& dst1, cv::Mat& dst2)
{
	
	int scale = 1;
	int delta = 0;
	int ddepth = CV_64F;
	
	Mat grad_x, grad_y;
	
	Sobel( src, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_REPLICATE );
	Sobel( src, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_REPLICATE );
	cv::magnitude(grad_x,grad_y, dst1);
	cv::phase(grad_x, grad_y, dst2);
	
}

GradientsExtractor::~GradientsExtractor()
{
}

