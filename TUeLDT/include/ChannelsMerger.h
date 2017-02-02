#ifndef CHANNELSMERGER_H
#define CHANNELSMERGER_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

class ChannelsMerger
{
public:

ChannelsMerger(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst);
	ChannelsMerger();
	~ChannelsMerger();

};

#endif // CHANNELSMERGER_H
