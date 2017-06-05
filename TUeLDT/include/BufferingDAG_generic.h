#ifndef BUFFERINGDAG_GENERIC_H
#define BUFFERINGDAG_GENERIC_H
#include "State.h"
#include "Camera.h"
#include "Lane.h"
#include <opencv2/core/eigen.hpp>
#include "opencv2/core/ocl.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>
#include  <fstream>



class BufferingDAG_generic
{

using MutexType = std::mutex;
using WriteLock = std::unique_lock<MutexType>;
	
friend class BufferingState;
friend class TEST_BufferingState;

BufferingDAG_generic ()
{

}

public:
//Note: An explicit move constructor of following format might be required

/*  BufferingDAG_generic(BufferingDAG_generic&& graph)
 * : mBufferPool(std::move(graph.mBufferPool))
 * {
 * 
 * 
 * }
 * */
	
protected:

	MutexType 			_mutex;
	condition_variable  _sateChange;
	bool 				mTemplatesReady= false;

#ifdef PROFILER_ENABLED
    ProfilerLDT                 mProfiler;
#endif
	
	//Set from outside, before buffering is activated  
	int mSpan;
	int mMargin;
	int mVP_Range_V;
	
	Mat mGRADIENT_TAN_ROOT;
    Mat	mFOCUS_MASK_ROOT;
    Mat	mDEPTH_MAP_ROOT;
	
	unique_ptr<BufferPool>	mBufferPool;
	
	// Needed for graph computations
	const Camera 		    mCAMERA;
	const LaneMembership    mLaneMembership;
	VanishingPt 			mVanishPt;

	//Image Frames
	Mat     mFrameRGB;
	Mat		mFrameGRAY;
	Mat		mFrameGRAY_ROI;
	
	//Image Gradients
	Mat     mGradX;
	Mat     mGradY;
	Mat     mGradX_abs;
	Mat     mGradY_abs;
	Mat     mFrameGradMag;

	// Extracted Templates
	Mat mGradTanTemplate; 
	Mat mDepthTemplate;
	Mat mFocusTemplate;

	// Temporary Probability Maps
	Mat mTempProbMat;
	Mat	mProbMap_Gray;
	Mat	mProbMap_GradMag;
	Mat mProbMap_GradDir;
	
	
#ifdef DIRECTORY_INPUT
	uint 			   mCountFrame;
	vector<cv::String> mFiles;
#endif

private:
	void computeGradients();
	
public:
	int  grabFrame(); 		 		// Grab Frame from the Source
	void auxillaryTasks();  		// Perform tasks from seperate executor
	void executeDAG_buffering();   //  Perform tasks from main Thread
	
	
	
	BufferingDAG_generic (BufferingDAG_generic && bufferingGraph)
{
	

	WriteLock  wrtLock(_mutex);
	
	mSpan 				= std::move(bufferingGraph.mSpan);
	mMargin				= std::move(bufferingGraph.mMargin);
	mVP_Range_V			= std::move(bufferingGraph.mVP_Range_V);
		
	mGRADIENT_TAN_ROOT 	= std::move(bufferingGraph.mGRADIENT_TAN_ROOT);
    mFOCUS_MASK_ROOT   	= std::move(bufferingGraph.mFOCUS_MASK_ROOT);
    mDEPTH_MAP_ROOT    	= std::move(bufferingGraph.mDEPTH_MAP_ROOT);
	
	mBufferPool    		= std::move(bufferingGraph.mBufferPool);
	mVanishPt			= std::move(bufferingGraph.mVanishPt);
	
	
	mFrameRGB			= std::move(bufferingGraph.mFrameRGB);
	mFrameGRAY			= std::move(bufferingGraph.mFrameGRAY);
	mFrameGRAY_ROI		= std::move(bufferingGraph.mFrameGRAY_ROI);
	
	mGradX				= std::move(bufferingGraph.mGradX);
	mGradY				= std::move(bufferingGraph.mGradY);
	mFrameGradMag		= std::move(bufferingGraph.mFrameGradMag);
	
	mGradTanTemplate	= std::move(bufferingGraph.mGradTanTemplate);
	mDepthTemplate		= std::move(bufferingGraph.mDepthTemplate);
	mFocusTemplate		= std::move(bufferingGraph.mFocusTemplate);
	
	
	mTempProbMat		= std::move(bufferingGraph.mTempProbMat);
	mProbMap_Gray		= std::move(bufferingGraph.mProbMap_Gray);
	mProbMap_GradMag	= std::move(bufferingGraph.mProbMap_GradMag);
	mProbMap_GradDir	= std::move(bufferingGraph.mProbMap_GradDir);
	
	
	
	wrtLock.unlock();
}




};

#endif // BUFFERINGDAG_GENERIC_H
