#ifndef BUFFERINGDAG_GENERIC_H
#define BUFFERINGDAG_GENERIC_H

/******************************************************************************
* NXP Confidential Proprietary
* 
* Copyright (c) 2017 NXP Semiconductor;
* All Rights Reserved
*
* AUTHOR : Rameez Ismail
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
* ****************************************************************************/ 

#include <thread>
#include <mutex>
#include <condition_variable>
#include "State.h"		//implicit include of profiling and logging headers
#include "LaneFilter.h" 	//implicit include of opencv headers
#include "VanishingPtFilter.h"
#include "gmock/gmock.h"

#ifdef DISPLAY_GRAPHICS_DCU
#include "frame_output_v234fb.h"
#endif


template<typename T>
struct BufferPool;

class BufferingDAG_generic
{

template<typename T>
friend class BufferingState;

FRIEND_TEST(BufferingTest, RGB_IMAGE);
FRIEND_TEST(BufferingTest, GRAY_IMAGE);
FRIEND_TEST(BufferingTest, PROB_MAP);
FRIEND_TEST(BufferingTest, GRAD_TAN);

public:
	BufferingDAG_generic ();



protected:


	using MutexType = std::mutex;
	using WriteLock = std::unique_lock<MutexType>;
	

	MutexType 				_mutex;
	condition_variable  	_sateChange;
	bool 					mTemplatesReady;
	bool 					mBufferReady;


	#ifdef PROFILER_ENABLED
    	 ProfilerLDT         mProfiler;
	#endif

	
	/*................................................
	 Set from outside, before buffering is activated   */ 

	int 					mSpan;
	int						mMargin;
	int 					mVP_Range_V;
	
	cv::Mat	 				mGRADIENT_TAN_ROOT;
    cv::Mat					mFOCUS_MASK_ROOT;
    cv::Mat					mDEPTH_MAP_ROOT;

	cv::Mat     			mX_ICS;
	cv::Mat     			mY_ICS;
	

	unique_ptr<BufferPool<BufferingDAG_generic>>	mBufferPool;
	/***************************************************/


	// Needed for graph computations
	const Camera 			mCAMERA;
	const LaneMembership    mLaneMembership;
	VanishingPt 			mVanishPt;


	//Image Frames
	cv::Mat     			mFrameRGB;
	cv::Mat					mFrameGRAY;
	cv::Mat					mFrameGRAY_ROI;

	
	//Binary Mask 
	cv::Mat 				mMask;
	
	
	//Image Gradients
	cv::Mat     			mGradX;
	cv::Mat     			mGradY;
	cv::Mat     			mGradX_abs;
	cv::Mat     			mGradY_abs;
	cv::Mat     			mFrameGradMag;


	// Extracted Templates
	cv::Mat 				mGradTanTemplate;
	cv::Mat 				mDepthTemplate;
	cv::Mat 				mFocusTemplate;
	cv::Mat 				mX_VPRS;
	cv::Mat 				mY_VPRS;


	// Temporary Probability Maps
	cv::Mat 				mTempProbMat;
	cv::Mat					mProbMap_Gray;
	cv::Mat					mProbMap_GradMag;
	cv::Mat 				mProbMap_GradDir;
	

    // Display Control Unit
    #ifdef DISPLAY_GRAPHICS_DCU
     io::FrameOutputV234Fb   mDCU;
	#endif


	uint64_t 					mFrameCount;


	FrameSource					mSource;
	vector<cv::String>			mFiles;
	cv::VideoCapture 			mRTSP_CAPTURE;

	
public:

	int  init_DAG();			// For initialising DAG ONE TIME EXECUTION
	int  grabFrame(); 			// Grab Frame from the Source
	void runAuxillaryTasks(); 	// Perform assitve tasks for buffering from seperate executor
	void buffer();   			// Perform tasks for buffering from main Thread
	
	
   	BufferingDAG_generic (BufferingDAG_generic && bufferingGraph)
	
	#ifdef DISPLAY_GRAPHICS_DCU 
	: mDCU(io::FrameOutputV234Fb(mCAMERA.RES_VH(1), mCAMERA.RES_VH(0), io::IO_DATA_DEPTH_08, io::IO_DATA_CH3))
	#endif
	
   	{
	
	WriteLock  wrtLock(_mutex);
	
	   mTemplatesReady      	= std::move(bufferingGraph.mTemplatesReady);
       mBufferReady             = std::move(bufferingGraph.mBufferReady);

	   mSpan 					= std::move(bufferingGraph.mSpan);
	   mMargin					= std::move(bufferingGraph.mMargin);
	   mVP_Range_V				= std::move(bufferingGraph.mVP_Range_V);
		
	   mGRADIENT_TAN_ROOT 		= std::move(bufferingGraph.mGRADIENT_TAN_ROOT);
	   mFOCUS_MASK_ROOT   		= std::move(bufferingGraph.mFOCUS_MASK_ROOT);
	   mDEPTH_MAP_ROOT    		= std::move(bufferingGraph.mDEPTH_MAP_ROOT);

       mX_ICS                   = std::move(bufferingGraph.mX_ICS);
       mY_ICS                   = std::move(bufferingGraph.mY_ICS);
	
	   mBufferPool   			= std::move(bufferingGraph.mBufferPool);
	   mVanishPt				= std::move(bufferingGraph.mVanishPt);
	
	   mFrameRGB				= std::move(bufferingGraph.mFrameRGB);
	   mFrameGRAY				= std::move(bufferingGraph.mFrameGRAY);
	   mFrameGRAY_ROI			= std::move(bufferingGraph.mFrameGRAY_ROI);

	   mMask 					= std::move(bufferingGraph.mMask);
	
	   mGradX					= std::move(bufferingGraph.mGradX);
	   mGradY					= std::move(bufferingGraph.mGradY);
	   mFrameGradMag			= std::move(bufferingGraph.mFrameGradMag);
	
	   mGradTanTemplate			= std::move(bufferingGraph.mGradTanTemplate);
	   mDepthTemplate			= std::move(bufferingGraph.mDepthTemplate);
	   mFocusTemplate			= std::move(bufferingGraph.mFocusTemplate);
	
	   mTempProbMat				= std::move(bufferingGraph.mTempProbMat);
	   mProbMap_Gray			= std::move(bufferingGraph.mProbMap_Gray);
	   mProbMap_GradMag			= std::move(bufferingGraph.mProbMap_GradMag);
	   mProbMap_GradDir			= std::move(bufferingGraph.mProbMap_GradDir);
	
	   mFrameCount				= std::move(bufferingGraph.mFrameCount);
	   mX_VPRS					= std::move(bufferingGraph.mX_VPRS);
	   mY_VPRS					= std::move(bufferingGraph.mY_VPRS);
		
	   mSource					= std::move(bufferingGraph.mSource);
	   mFiles					= std::move(bufferingGraph.mFiles);
	   mRTSP_CAPTURE 			= std::move(bufferingGraph.mRTSP_CAPTURE);

	wrtLock.unlock();

   	}
};


template <>
struct BufferPool<BufferingDAG_generic>
{
    vector<cv::Mat> Probability;
    vector<cv::Mat> GradientTangent;

    BufferPool(const int RES_V, const int RES_H, std::size_t bufferSize)
    {
      for (std::size_t i=0; i< bufferSize; i++)
      {
         cv::Mat PROB_MAP( RES_V, RES_H, CV_8UC1  );
         cv::Mat GRAD_TAN( RES_V, RES_H, CV_16SC1 );

         Probability.push_back(PROB_MAP);
         GradientTangent.push_back(GRAD_TAN);

      }
    }
};
#endif // BUFFERINGDAG_GENERIC_H
