#ifndef CHANNELSPLITTER_H
#define CHANNELSPLITTER_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
class ChannelsSplitter
{
public:
	ChannelsSplitter(const cv::Mat& src, cv::Mat& dst1, cv::Mat& dst2, cv::Mat& dst3);
   ~ChannelsSplitter();
};

#endif // CHANNELSPLITTER_H
