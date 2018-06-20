/******************************************************************************
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2017 NXP Semiconductor;
 * All Rights Reserved
 *
 * AUTHOR : Ruud Siebierski
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
#include "FrameFeeder.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <regex>

StreamFeeder::StreamFeeder(string sourceUriStr) 
: mQueuesSync(true),
  mMAX_BUFFER_SIZE(2),
  mMAX_RETRY(100), 	// Main thread sleeps for 1ms and then retry to grab.
  mSLEEP_ms(20),  	// Sleep time for the mAsyncGrabber
  mUri("")
{
  parseSettings (sourceUriStr);
  mAsyncGrabber = std::thread(&StreamFeeder::captureThread,this);
}


// Expect a camera-index (e.g. USB cam) or
// a URL e.g. rtsp://admin:12345@192.168.140.14/streams/channel/1 or
// a video file location e.g. home/bluebox/C0Recording.mp4

void StreamFeeder::parseSettings(string& srcStr)
{
  if (srcStr.length() > 1)
  {
    std::regex url_regex( R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",std::regex::extended);
    std::smatch url_match_result;
    if (!std::regex_match(srcStr, url_match_result, url_regex))
    {
      throw "Invalid URI used: " + srcStr;
    }
  }
  mUri = srcStr;
}




void StreamFeeder::captureThread()
{

  WriteLock frameLock(mMutex, std::defer_lock);

  cv::VideoCapture inputCapture;
  
  inputCapture.open(mUri);

  if (!inputCapture.isOpened())
  throw "Unable to open video input on: " + mUri ;

 
  while(!Stopped)
  {
    if(!Paused)
    {
	#ifdef PROFILER_ENABLED
	 mProfiler.start("FRAME_READING");
      	#endif

     	cv::UMat lFrame, lFrameGRAY;

	try
	{
          inputCapture >> lFrame;
          if (!lFrame.empty())
          {
           cv::cvtColor(lFrame, lFrameGRAY, CV_RGB2GRAY);
          }

	  //Put the frames in the queue for the stateMachine
          enqueue(lFrameGRAY, mProcessQueue); //Thread-safe method to enqueue processing Frames
          enqueue(lFrame,     mDisplayQueue); //Thread-safe method to enqueue display Frames

  	  #ifdef PROFILER_ENABLED
  	    mProfiler.end();
  	    LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
	    <<"******************************"<<endl
	    <<  "Frame Reading Time." <<endl
	    <<  "Max Time: " << mProfiler.getMaxTime("FRAME_READING")<<endl
	    <<  "Avg Time: " << mProfiler.getAvgTime("FRAME_READING")<<endl
	    <<  "Min Time: " << mProfiler.getMinTime("FRAME_READING")<<endl
	    <<"******************************"<<endl<<endl;
	  #endif
	}
	catch(const std::exception & ex)
	{
	  Stopped.store(true);
	  cerr << ex.what() << endl;
	}

    }

    if(mSLEEP_ms >0)
    std::this_thread::sleep_for(std::chrono::milliseconds(mSLEEP_ms));

  }//while ends

}


void StreamFeeder::enqueue(cv::UMat& frame, vector<cv::UMat>& queue)
{

   WriteLock  lLock(mMutex, std::defer_lock);

   if(!frame.empty())
   {
      lLock.lock(); //Protect queue from race-condition

      queue.push_back(frame);

      if(queue.size() >  mMAX_BUFFER_SIZE)
      {
         queue.erase(queue.begin());

         #ifdef PROFILER_ENABLED
          LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
          <<"******************************"<<endl
          <<  "WARNING!! "<<endl
          <<  "Dropping frames in the queue, cannot keep-up with the frame production rate"<<endl
          <<"******************************"<<endl<<endl;
        #endif
     }

     lLock.unlock();
  }

}


cv::UMat StreamFeeder::dequeue()
{

  #ifdef PROFILER_ENABLED
  mProfiler.start("FRAME_DEQUEUE");
  #endif

   WriteLock  lLock(mMutex, std::defer_lock);

   size_t lTryGrab = 0;

   lLock.lock(); //Protect queue from any race-condition

   while (mProcessQueue.empty() && lTryGrab < mMAX_RETRY)
   {
      lLock.unlock();
      lTryGrab ++ ;
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      lLock.lock();
   }

   if(mProcessQueue.empty())
   {
      throw "No images in the process queue, the producer is too slow or down! [Empty Frame Queue] ";
   }

   cv::UMat lFrame = mProcessQueue[0];
   mProcessQueue.erase(mProcessQueue.begin());

   mQueuesSync = false;
   lLock.unlock();

   #ifdef PROFILER_ENABLED
    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
    <<"******************************"<<endl
    <<  "Frame dequeued for processing:"<<endl
    <<  "[Attempt count: "<<lTryGrab<<"/"<<mMAX_RETRY<<"]"<<endl
    <<"******************************"<<endl<<endl;
   #endif

   if(lFrame.empty())
    throw "Failed to get the frame from the process queue! [Empty Frame Exception] ";


    #ifdef PROFILER_ENABLED
    mProfiler.end();
    LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
                                    <<"******************************"<<endl
                                    <<  "Frame Dequeue Time." <<endl
                                    <<  "Max Time: " << mProfiler.getMaxTime("FRAME_DEQUEUE")<<endl
                                    <<  "Avg Time: " << mProfiler.getAvgTime("FRAME_DEQUEUE")<<endl
                                    <<  "Min Time: " << mProfiler.getMinTime("FRAME_DEQUEUE")<<endl
                                    <<"******************************"<<endl<<endl;
                                    #endif
   return lFrame;
}


cv::UMat StreamFeeder::dequeueDisplay()
{
   WriteLock  lLock(mMutex, std::defer_lock);
   size_t lTryGrab = 0;

   lLock.lock(); //Protect queue from race-condition

    while (mDisplayQueue.empty() && lTryGrab < mMAX_RETRY)
    {
      lLock.unlock();
      lTryGrab ++ ;
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      lLock.lock();
    }
    if(mDisplayQueue.empty())
    {
      cout<<"No images in the display queue, the producer is too slow or down! [Empty Frame Queue] "<<endl;
      throw "No images in the display queue, the producer is too slow or down! [Empty Frame Queue] ";
    }

    cv::UMat lFrame = mDisplayQueue[0];
    mDisplayQueue.erase(mDisplayQueue.begin());
    mQueuesSync = true;
   lLock.unlock();

   #ifdef PROFILER_ENABLED
    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
    <<"******************************"<<endl
    <<  "Frame dequeued for display:"<<endl
    <<  "[Attempt count: "<<lTryGrab<<"/"<<mMAX_RETRY<<"]"<<endl
    <<"******************************"<<endl<<endl;
   #endif

   if(lFrame.empty())
   throw "Failed to get the frame from the display Queue! [Empty Frame Exception] ";

   return lFrame;
}

StreamFeeder::~StreamFeeder()
{
  Stopped = true;

  if (mAsyncGrabber.joinable())
  {
    mAsyncGrabber.join();
    #ifdef PROFILER_ENABLED
     LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
     <<  "********************************"<<endl
     <<  "[StreamFeeder is Joined]"<<endl
     <<"******************************"<<endl<<endl;
    #endif
  }

  #ifdef PROFILER_ENABLED
      LOG_INFO_(LDTLog::STATE_MACHINE_LOG) << endl
      <<"********************************"<< endl
      << "[StreamFeeder is Destroyed]"<< endl
      << "******************************"<< endl << endl;
  #endif
}
