#ifndef COLORTRANSFORM_H
#define COLORTRANSFORM_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"


class ColorTransformer
{
public:
	ColorTransformer(const cv::Mat& src, cv::Mat& dst, int code);
	~ColorTransformer();

};

#endif // COLORTRANSFORM_H
