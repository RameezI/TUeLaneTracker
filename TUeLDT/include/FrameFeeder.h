#ifndef FRAME_FEEDER_H
#define FRAME_FEEDER_H

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

#include "opencv2/opencv.hpp"
#include "LDT_logger.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

using namespace std;
using WriteLock = std::unique_lock<std::mutex>;

struct cvFrames
{
  cv::Mat	Frame;
  cv::Mat	FrameGRAY;
  cv::string	FrameInfo;

 cvFrames(cv::Mat& frame, cv::Mat& frameGRAY, const string&  frameInfo)
 : Frame(frame),
   FrameGRAY(frameGRAY),
   FrameInfo(frameInfo){}

}

/* ^TODO: Hint make a copy of this file with specialisation, in case of vsdkFrames */
template<typename T>
class FrameFeeder
{

protected:

   const std::size_t	mMAX_BUFFER_SIZE;
   const std::size_t	mMAX_RETRY;
   std::mutex 		mMutex;
   vector<cv::Mat>	mQueueFrame;
   vector<cv::Mat>	mQueueFrameGRAY;
	

   FrameFeeder(): mMAX_BUFFER_SIZE(3), mMAX_RETRY(10), Stopped(false), Paused(true)
   {

   }

   void enqueue(cv::Mat& frame, vector<cv::Mat>& queue)
   {
      WriteLock  lLock(mMutex, std::defer_lock);	

      lLock.lock(); //Protect queue from race-condition

        queue.push_back(frame);

	if(queue.size() >  mMAX_BUFFER_SIZE)
	{
          queue.erase(queue.begin());
      	  #ifdef PROFILER_ENABLED
	  LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	   <<"******************************"<<endl
	   <<  "WARNING!! "<<endl
	   <<  "Droping frames in the queue, cannot keep-up with the frame production rate"<<endl
	   <<"******************************"<<endl<<endl;
          #endif
	}
      lLock.unlock();
   }
 
   virtual void    parseSettings(string& srcStr)  = 0;	

public:	

   std::atomic<bool> Stopped;
   std::atomic<bool> Paused; 


   cv::Mat dequeueFrame()
   {
      WriteLock  lLock(mMutex, std::defer_lock);	
      size_t lTryGrab = 0;

      lLock.lock(); //Protect queue from race-condition
      while (mQueueFrame.empty() && lTryGrab < mMAX_RETRY)
      { 
	lLock.unlock();
	lTryGrab ++ ;
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	lLock.lock();
      }
      if(mQueueFrame.empty())
      {
     	throw "No images in the queue, the producer is too slow or down! [Empty Frame Queue] ";
      }

      cv::Mat lFrame = mQueueFrame[0];
      mQueueFrame.erase(mQueueFrame.begin());
      lLock.unlock();

      #ifdef PROFILER_ENABLED
       LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
       <<"******************************"<<endl
       <<  "Frame dequeued:"<<endl
       <<  "[Attempt count: "<<lTryGrab<<"/"<<mMAX_RETRY<<"]"<<endl
       <<"******************************"<<endl<<endl;
      #endif


      if(lFrame.empty())
        throw "Failed to get the frame from mQueueFrame! [Empty Frame Exception] ";

      return lFrame;
   }
 
   cv::Mat dequeueFrameGRAY()
   {
      WriteLock  lLock(mMutex, std::defer_lock);	
      size_t lTryGrab = 0;

      lLock.lock(); //Protect queue from race-condition
      while (mQueueFrameGRAY.empty() && lTryGrab < mMAX_RETRY)
      { 
	lLock.unlock();
	lTryGrab ++ ;
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	lLock.lock();
      }

      if(mQueueFrameGRAY.empty())
      {
     	throw "No images in the queue, the producer is too slow or down! [Empty Frame Queue] ";
      }

      cv::Mat lFrame = mQueueFrameGRAY[0];
      mQueueFrameGRAY.erase(mQueueFrameGRAY.begin());
      lLock.unlock();

      #ifdef PROFILER_ENABLED
       LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
       <<"******************************"<<endl
       <<  "Frame dequeued:"<<endl
       <<  "[Attempt count: "<<lTryGrab<<"/"<<mMAX_RETRY<<"]"<<endl
       <<"******************************"<<endl<<endl;
      #endif

      if(lFrame.empty())
        throw "Failed to get the frame from mQueueFrame! [Empty Frame Exception] ";

      return lFrame;
   }

};



class ImgStoreFeeder: public FrameFeeder
{

private:
	string			mFolder;
	int 	        	mSkipFrames;
	uint32_t 		mFrameCount;
	vector< cv::String> 	mFiles;
	std::thread		mAsyncGrabber;

public:
	
	ImgStoreFeeder(string sourceStr);
	~ImgStoreFeeder();
	void parseSettings(string& srcStr) override;
};

#endif
