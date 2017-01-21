#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
class Histogram
{
	struct Lane  // IN VP Coordinate System
	{
		const int 	PX_STEP;
		const int 	PX_MAX;
		const int  	FILTER_OFFSET;
		const float	CONF_THRESH;
		float 		BIN_H[];
		cv::Mat 	PRIOR;
		cv::Mat 	TRANSITION;
		cv::Mat   	FILTER;
	};
	
	struct VanishingPoint  // IN Center-Image Coordinate System
	{
		const int 	STEP;
		const int 	RANGE_V;
		const int  	RANGE_H;
		const float	BINS_V;
		cv::Mat 	PRIOR[];
		cv::Mat 	TRANSITION;
		cv::Mat   	FILTER;
	};
	
	
public:
	
	Histogram();
	~Histogram();

};

#endif // HISTOGRAM_H
