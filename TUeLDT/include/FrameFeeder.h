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

#include "Config.h"
#include "LDT_logger.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

using namespace std;
using WriteLock = std::unique_lock<std::mutex>;


class FrameFeeder
{

public:
	   std::atomic<bool> 	Stopped;
	   std::atomic<bool> 	Paused;

protected:
   vector<cv::UMat>		mDisplayQueue;
   vector<cv::UMat>		mProcessQueue;

   FrameFeeder(): Stopped(false),Paused(true){}

   virtual  void parseSettings(string& srcStr)  = 0;
   virtual  void enqueue(cv::UMat& frame, vector<cv::UMat>& queue) = 0;

public:
   virtual cv::UMat dequeue() = 0 ;
   virtual cv::UMat dequeueDisplay() = 0;
   virtual  ~FrameFeeder(){}

};

class ImgStoreFeeder: public FrameFeeder
{

private:
	const std::size_t		mMAX_BUFFER_SIZE;
	const std::size_t		mMAX_RETRY;
	const std::size_t		mSLEEP_ms;

	string					mFolder;
	int 	        		mSkipFrames;
	uint32_t 				mFrameCount;
	vector< cv::String> 	mFiles;
	std::thread				mAsyncGrabber;
	std::mutex 				mMutex;

	void enqueue(cv::UMat& frame, vector<cv::UMat>& queue) override;
	void parseSettings(string& srcStr) override;

public:
	ImgStoreFeeder(string sourceStr);
	cv::UMat dequeue() override;
	cv::UMat dequeueDisplay() override;
	~ImgStoreFeeder();
};

#endif
