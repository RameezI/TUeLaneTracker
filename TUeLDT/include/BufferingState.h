#ifndef BUFFERINGSTATE_H
#define BUFFERINGSTATE_H
#include "State.h"
#include "BufferingDAG_generic.h"


using namespace cv;
using namespace Eigen;
using namespace std;

class BufferingState : public State
{
	
public:		
	BufferingState();
		
	unique_ptr<BufferingDAG_generic> bufferingGraph;
	
	void setTemplates(Templates* templates);
	void run();
	
	#ifdef PROFILER_ENABLED
	void getOpenClInfo();
	#endif
		
	#ifdef DIRECTORY_INPUT
	void setSource(const vector<cv::String>& files);
	#else
	void setSource();
	#endif

	~BufferingState();

};

#endif // BUFFERINGSTATE_H
