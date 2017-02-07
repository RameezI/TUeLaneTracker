#include "ColorTransformer.h"

ColorTransformer::ColorTransformer(const cv::Mat& src, cv::Mat& dst, int code)
{
	
	
	cvtColor(src, dst, code);
	
	
	
}
ColorTransformer::~ColorTransformer()
{
}

