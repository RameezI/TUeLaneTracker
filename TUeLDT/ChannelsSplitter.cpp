#include "ChannelsSplitter.h"

ChannelsSplitter::ChannelsSplitter(const cv::Mat& src, cv::Mat& dst1, cv::Mat& dst2, cv::Mat& dst3)
{
	std::vector<Mat> HSV;
	cv::split(src,HSV);
	 dst1 = HSV[0];
	 dst2 = HSV[1];
	 dst3 = HSV[2];
}

ChannelsSplitter::~ChannelsSplitter()
{
}

