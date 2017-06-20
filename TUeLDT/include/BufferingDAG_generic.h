#ifndef BUFFERINGDAG_GENERIC_H
#define BUFFERINGDAG_GENERIC_H
#include "State.h"
#include <thread>
#include <mutex>
#include <condition_variable>



class BufferingDAG_generic
{
	
friend class BufferingState;
friend class TEST_BufferingState;
friend class TEST_TrackingState;

public:
BufferingDAG_generic ();


protected:

using MutexType = std::mutex;
using WriteLock = std::unique_lock<MutexType>;
	

	
protected:

	MutexType 			_mutex;
	condition_variable  _sateChange;
	bool 				mBufferReady= false;

#ifdef PROFILER_ENABLED
    ProfilerLDT         mProfiler;
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
	
	//Binary Mask 
	Mat 	mMask;
	
	
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
	Mat mX_VPRS;
	Mat mY_VPRS;

	// Temporary Probability Maps
	Mat mTempProbMat;
	Mat	mProbMap_Gray;
	Mat	mProbMap_GradMag;
	Mat mProbMap_GradDir;
	
	uint64_t 			mFrameCount;
	
#ifdef DIRECTORY_INPUT	
	vector<cv::String>	mFiles;
#endif

	
public:
	int  grabFrame(); 		 		// Grab Frame from the Source
	void auxillaryTasks();  		// Perform tasks from seperate executor
	void buffer();   //  Perform tasks from main Thread
	
	
	
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
	
	mFrameCount			= std::move(bufferingGraph.mFrameCount);
	mX_VPRS				= std::move(bufferingGraph.mX_VPRS);
	mY_VPRS				= std::move(bufferingGraph.mY_VPRS);
		
	#ifdef DIRECTORY_INPUT	
	mFiles				= std::move(bufferingGraph.mFiles);
	#endif
	
	
	
	wrtLock.unlock();
}




};

#endif // BUFFERINGDAG_GENERIC_H
