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
#include <mutex>
#include <future>
#include "State.h"		//implicit include of profiling and logging headers
#include "Templates.h"
#include "LaneModel.h"		//implicit include of LaneFilter and VanishingPtFilter
#include "gtest/gtest.h"	//google-testing framework includes

using WriteLock = std::unique_lock<std::mutex>;

struct BufferPool;

class BufferingDAG_generic
{

template<typename T>

friend class BufferingState;
friend class TrackingTest;
FRIEND_TEST(BufferingTest, GRAY_IMAGE);
FRIEND_TEST(BufferingTest, PROB_MAP);
FRIEND_TEST(BufferingTest, GRAD_TAN);
FRIEND_TEST(TrackingTest, MAX_POOLING);

public:
	BufferingDAG_generic (const LaneTracker::Config& Config);

protected:
	std::mutex 			_mutex;
	std::future<void>		mFuture;


	#ifdef PROFILER_ENABLED
    	 ProfilerLDT         		mProfiler;
	#endif

	size_t          mBufferPos;		/**< Active Buffer Position */


	int		mHORIZON_ICCS_H; 	/**< /brief Position of Horizon in the Image-Center-CS [pixels] */

	int		mHORIZON_ICCS_V; 	/**< /brief Position of Horizon in the Image-Center-CS [pixels]
				 	  	*  /n +ve value implies that the ROI is below the center line.
					  	*  /n -ve value implies that the ROI is above the center line. */

	int 		mVP_RANGE_V;      	/**< Vertical range of the vanishing-point in either direction [pixels] */

	int 		mSPAN;			/**< Vertical size  of the ROI [pixels]
						* /n Automatically calculated from  #HORIZON_ICCS and #VP_RANGE_V */
	
	cv::Mat	 	mGRADIENT_TAN_ROOT;	/**< ROOT-TEMPLATE for extracting gradient tangents reference.
				  		* /n The size of /em GRADIENT_TAN_ROOT is [2x#RES_V +1 , 2x#RES_H +1] */

	cv::Mat		mFOCUS_MASK_ROOT; 	/**< /brief ROOT-TEMPLATE for extracting mask to compensate pitch movements.
						* /n The size of /em FOCUS ROOT is [#SPAN + (2x#VP_RANGE_V), #RES_H]
						* /n Normal activation all elements in rowrange (#SPAN - #VP_RANGE) = 255
						* /n Best   activation all elements in #SPAN = 255
						* /n Worst  activation all elements in rowrange (#SPAN - 2x#VP_RANGE) = 255 */

    	cv::Mat		mDEPTH_MAP_ROOT;	/**< ROOT-TEMPLATE for assigning perspective weights to the pixels.
				     		* /n The size of /em DEPTH_MAP_ROOT is [#RES_V, #RES_H] */

	cv::Mat     	mX_ICS;	 		/**< X-Coordinates of the ROI in the Image-Coordinate-System.
				   		* /n The size of /em X_ICS is [#SPAN, #RES_H] */

	cv::Mat     	mY_ICS;  		/**< X-Coordinates of the ROI in the Image-Coordinate-System.
				   		* /n The size of /em X_ICS is [#SPAN, #RES_H] */

	
	unique_ptr<BufferPool>	mBufferPool; 	/**< A buffer, of pre-specified number, for temporal pooling */


	// Miscellaneous; needed for graph computations
	const Camera 			mCAMERA;
	const LaneMembership    	mLaneMembership;
	VanishingPt 			mVanishPt;

	//ROI Frame
	cv::Mat				mFrameGRAY_ROI;
	
	//Binary Mask 
	cv::Mat 			mMask;
	
	//Image Gradients
	cv::Mat     			mGradX;
	cv::Mat     			mGradY;
	cv::Mat     			mGradX_abs;
	cv::Mat     			mGradY_abs;
	cv::Mat     			mFrameGradMag;

	// Extracted Templates
	cv::Mat 			mGradTanTemplate;
	cv::Mat 			mDepthTemplate;
	cv::Mat 			mFocusTemplate;

	// Temporary Probability Maps
	cv::Mat 			mTempProbMat;
	cv::Mat				mProbMap_Gray;
	cv::Mat				mProbMap_GradMag;
	cv::Mat 			mProbMap_GradDir;
	
	
public:	
	/** For initialising of the DAG [ONE TIME EXECUTION]  */
	int  init_DAG(const Templates & TEMPLATES, const size_t & BUFFER_SIZE);

	/** executes the Directed Acyclic Graph*/
	void execute(cv::UMat& FrameGRAY);  
	
   	BufferingDAG_generic (BufferingDAG_generic && bufferingGraph)
	: mCAMERA(bufferingGraph.mCAMERA)
   	{
	
	   WriteLock  lLock(_mutex, std::defer_lock);
	   lLock.lock();
	
	     mBufferPos			= std::move(bufferingGraph.mBufferPos);
	

	     mHORIZON_ICCS_H		= std::move(bufferingGraph.mHORIZON_ICCS_H);
	     mHORIZON_ICCS_V		= std::move(bufferingGraph.mHORIZON_ICCS_V);

	     mVP_RANGE_V		= std::move(bufferingGraph.mVP_RANGE_V);
	     mSPAN 			= std::move(bufferingGraph.mSPAN);
		
	     mGRADIENT_TAN_ROOT 	= std::move(bufferingGraph.mGRADIENT_TAN_ROOT);
	     mFOCUS_MASK_ROOT   	= std::move(bufferingGraph.mFOCUS_MASK_ROOT);
	     mDEPTH_MAP_ROOT    	= std::move(bufferingGraph.mDEPTH_MAP_ROOT);

       	     mX_ICS                   	= std::move(bufferingGraph.mX_ICS);
             mY_ICS                   	= std::move(bufferingGraph.mY_ICS);
	
	     mBufferPool   		= std::move(bufferingGraph.mBufferPool);
	     mVanishPt			= std::move(bufferingGraph.mVanishPt);

	     mFrameGRAY_ROI		= std::move(bufferingGraph.mFrameGRAY_ROI);

	     mMask 			= std::move(bufferingGraph.mMask);
	
	     mGradX			= std::move(bufferingGraph.mGradX);
	     mGradY			= std::move(bufferingGraph.mGradY);
	     mFrameGradMag		= std::move(bufferingGraph.mFrameGradMag);
	
	     mGradTanTemplate		= std::move(bufferingGraph.mGradTanTemplate);
	     mDepthTemplate		= std::move(bufferingGraph.mDepthTemplate);
	     mFocusTemplate		= std::move(bufferingGraph.mFocusTemplate);
	
	     mTempProbMat		= std::move(bufferingGraph.mTempProbMat);
	     mProbMap_Gray		= std::move(bufferingGraph.mProbMap_Gray);
	     mProbMap_GradMag		= std::move(bufferingGraph.mProbMap_GradMag);
	     mProbMap_GradDir		= std::move(bufferingGraph.mProbMap_GradDir);
	

	   lLock.unlock();
   	}
};


struct BufferPool
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
