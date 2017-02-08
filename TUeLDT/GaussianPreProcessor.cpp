#include "GaussianPreProcessor.h"

GaussianPreProcessor::GaussianPreProcessor(const cv::Mat& src, cv::Mat& dst)
{
	
	GaussianBlur( src, dst, Size( 11, 11 ), 1.5, 1.5, BORDER_REPLICATE);
	
	
	
}

GaussianPreProcessor::~GaussianPreProcessor()
{
}

