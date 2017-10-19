#ifndef BUFFERINGDAG_GENERIC_H
#define BUFFERINGDAG_GENERIC_H
#include "State.h"
#include <thread>
#include <mutex>
#include <condition_variable>

struct BufferPool;

class BufferingDAG_generic
{
	
friend class BufferingState;

public:
BufferingDAG_generic ();


protected:

using MutexType = std::mutex;
using WriteLock = std::unique_lock<MutexType>;
	

	MutexType 			_mutex;
	condition_variable  		_sateChange;
	bool 				mTemplatesReady;
	bool 				mBufferReady;


	#ifdef PROFILER_ENABLED
    	 ProfilerLDT         mProfiler;
	#endif
	
	/*................................................
	 Set from outside, before buffering is activated   */ 
	int mSpan;
	int mMargin;
	int mVP_Range_V;
	
	Mat 	mGRADIENT_TAN_ROOT;
    	Mat	mFOCUS_MASK_ROOT;
    	Mat	mDEPTH_MAP_ROOT;

	Mat     mX_IRS;
	Mat     mY_IRS;
	
	unique_ptr<BufferPool>	mBufferPool;
	/***************************************************/


	// Needed for graph computations
	const Camera 		mCAMERA;
	const LaneMembership    mLaneMembership;
	VanishingPt 		mVanishPt;


	//Image Frames
	Mat     mFrameRGB;
	Mat	mFrameGRAY;
	Mat	mFrameGRAY_ROI;

	
	//Binary Mask 
	Mat 	mMask;
	
	
	//Image Gradients
	Mat     mGradX;
	Mat     mGradY;
	Mat     mGradX_abs;
	Mat     mGradY_abs;
	Mat     mFrameGradMag;


	// Extracted Templates
	Mat 	mGradTanTemplate; 
	Mat 	mDepthTemplate;
	Mat 	mFocusTemplate;
	Mat 	mX_VPRS;
	Mat 	mY_VPRS;


	// Temporary Probability Maps
	Mat 	mTempProbMat;
	Mat	mProbMap_Gray;
	Mat	mProbMap_GradMag;
	Mat 	mProbMap_GradDir;
	
	uint64_t 	mFrameCount;
	
	#ifdef DIRECTORY_INPUT	
	 vector<cv::String>	mFiles;
	#else
	   VideoCapture 	mRTSP_CAPTURE;
	#endif

	
public:

	int  init_DAG();		// For initialising DAG ONE TIME EXECUTION

	int  grabFrame(); 		// Grab Frame from the Source
	void runAuxillaryTasks();       // Perform assitve tasks for buffering from seperate executor
	void buffer();   		// Perform tasks for buffering from main Thread
	
	
	
   	BufferingDAG_generic (BufferingDAG_generic && bufferingGraph)

   	{
	

	WriteLock  wrtLock(_mutex);
	
	   mTemplatesReady      	= std::move(bufferingGraph.mTemplatesReady);
           mBufferReady              	= std::move(bufferingGraph.mBufferReady);

	   mSpan 			= std::move(bufferingGraph.mSpan);
	   mMargin			= std::move(bufferingGraph.mMargin);
	   mVP_Range_V			= std::move(bufferingGraph.mVP_Range_V);
		
	   mGRADIENT_TAN_ROOT 		= std::move(bufferingGraph.mGRADIENT_TAN_ROOT);
	   mFOCUS_MASK_ROOT   		= std::move(bufferingGraph.mFOCUS_MASK_ROOT);
    	   mDEPTH_MAP_ROOT    		= std::move(bufferingGraph.mDEPTH_MAP_ROOT);

           mX_IRS                       = std::move(bufferingGraph.mX_IRS); 
           mY_IRS                       = std::move(bufferingGraph.mY_IRS);
	
	   mBufferPool   		= std::move(bufferingGraph.mBufferPool);
	   mVanishPt			= std::move(bufferingGraph.mVanishPt);
	
	   mFrameRGB			= std::move(bufferingGraph.mFrameRGB);
	   mFrameGRAY			= std::move(bufferingGraph.mFrameGRAY);
	   mFrameGRAY_ROI		= std::move(bufferingGraph.mFrameGRAY_ROI);

	   mMask 			= std::move(bufferingGraph.mMask);
	
	   mGradX			= std::move(bufferingGraph.mGradX);
	   mGradY			= std::move(bufferingGraph.mGradY);
	   mFrameGradMag		= std::move(bufferingGraph.mFrameGradMag);
	
	   mGradTanTemplate		= std::move(bufferingGraph.mGradTanTemplate);
	   mDepthTemplate		= std::move(bufferingGraph.mDepthTemplate);
	   mFocusTemplate		= std::move(bufferingGraph.mFocusTemplate);
	
	   mTempProbMat			= std::move(bufferingGraph.mTempProbMat);
	   mProbMap_Gray		= std::move(bufferingGraph.mProbMap_Gray);
	   mProbMap_GradMag		= std::move(bufferingGraph.mProbMap_GradMag);
	   mProbMap_GradDir		= std::move(bufferingGraph.mProbMap_GradDir);
	
	   mFrameCount			= std::move(bufferingGraph.mFrameCount);
	   mX_VPRS			= std::move(bufferingGraph.mX_VPRS);
	   mY_VPRS			= std::move(bufferingGraph.mY_VPRS);
		
	   #ifdef DIRECTORY_INPUT	
	      mFiles			= std::move(bufferingGraph.mFiles);
	   #else
	      mRTSP_CAPTURE 		= std::move(bufferingGraph.mRTSP_CAPTURE);
	   #endif

	   wrtLock.unlock();
   	}


};

struct BufferPool
{
        vector<Mat> Probability;
        vector<Mat> GradientTangent;

        BufferPool(const int RES_V, const int RES_H, std::size_t bufferSize)
        {
          for (std::size_t i=0; i< bufferSize; i++)
          {
             Mat PROB_MAP( RES_V, RES_H, CV_8UC1  );
             Mat GRAD_TAN( RES_V, RES_H, CV_16SC1 );

             Probability.push_back(PROB_MAP);
             GradientTangent.push_back(GRAD_TAN);

          }
        }
};


#endif // BUFFERINGDAG_GENERIC_H
