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
#include "FrameFeeder.h"

ImgStoreFeeder::ImgStoreFeeder(string sourceStr)
: mQueuesSync(true),
  mMAX_BUFFER_SIZE(2),
  mMAX_RETRY(100), 		// Main thread sleeps for 1ms and then retry to grab.
  mSLEEP_ms(10),  		// Sleep time for the mAsyncGrabber
  mFolder(""),
  mSkipFrames(0),
  mFrameCount(0)
{
   parseSettings(sourceStr);
  
   mAsyncGrabber = std::thread([this]
   {
     WriteLock  lLock(mMutex, std::defer_lock);	
     mFrameCount = mSkipFrames;

     while(!Stopped.load())
    {
	  if(!Paused.load())
       {
          #ifdef PROFILER_ENABLED
	   mProfiler.start("FRAME_READING");
          #endif

          cv::UMat lFrame, lFrameGRAY;

          lLock.lock(); //protecting mFiles and mFrameCount shared variables.
           lFrame  		= imread(mFiles[mFrameCount]).getUMat(cv::ACCESS_READ);
           if (!lFrame.empty())
	   {
             cv::cvtColor(lFrame, lFrameGRAY, CV_RGB2GRAY);
	   }
          lLock.unlock();

	  //Put the frames in the queue for the stateMachine
          enqueue(lFrameGRAY, mProcessQueue); //Thread-safe method to enqueue processing Frames
          enqueue(lFrame,     mDisplayQueue); //Thread-safe method to enqueue display Frames

          #ifdef PROFILER_ENABLED
           LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
           <<"******************************"<<endl
           <<  "Frame enqueued:"<<endl
           << (std::string)(mFiles[mFrameCount])<<endl
	   << mFrameCount<<"/"<<mFiles.size()<<endl
           <<"******************************"<<endl<<endl;
	  #endif

          if(mFrameCount+1 < mFiles.size())
	  {
             mFrameCount ++;
          }

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

      if(mSLEEP_ms >0)
      std::this_thread::sleep_for(std::chrono::milliseconds(mSLEEP_ms));
   }

  });
}



void ImgStoreFeeder::parseSettings(string& srcStr)
{
   string lDelimiter = ",";
   size_t lPos	     =  0 ;

   vector<string>  lTokens;
   while ( (lPos = srcStr.find(lDelimiter) ) != string::npos )
   {
     lTokens.push_back(srcStr.substr(0, lPos));
     srcStr.erase(0, lPos + lDelimiter.length() );
   }
   lTokens.push_back(srcStr);  //push_back the last substring too.

   mFolder   = lTokens[0];

   if(lTokens.size() == 2)
   {
     mSkipFrames  = stoi(lTokens[1], nullptr);
     if (mSkipFrames < 0)
     throw "mSkipFrames  must be a positive integer";
   }


    	glob(mFolder, mFiles);

   if (mFiles.size() <= (uint32_t)mSkipFrames)
   {
      throw "No files found OR the total number of image files is less than mSkipFrames.";
   }
 
}

void ImgStoreFeeder::enqueue(cv::UMat& frame, vector<cv::UMat>& queue)
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

cv::UMat ImgStoreFeeder::dequeue()
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

cv::UMat ImgStoreFeeder::dequeueDisplay()
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

ImgStoreFeeder::~ImgStoreFeeder()
{
   Stopped.store(true);

   if(mAsyncGrabber.joinable())
   {
      mAsyncGrabber.join();

      #ifdef PROFILER_ENABLED
	LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	<<  "********************************"<<endl
	<<  "[ImgStoreFeeder is Joined]"<<endl
	<<"******************************"<<endl<<endl;
      #endif
   }

   #ifdef PROFILER_ENABLED
    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
    <<  "********************************"<<endl
    <<  "[ImgStoreFeeder is Destroyed]"<<endl
    <<"******************************"<<endl<<endl;
   #endif
}
