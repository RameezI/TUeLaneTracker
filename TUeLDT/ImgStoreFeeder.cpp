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
: mFolder(""),
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
           cv::UMat lFrame, lFrameGRAY;

	   lLock.lock(); //protecting mFiles and mFrameCount shared variables.
	   lFrame  	= imread(mFiles[mFrameCount]).getUMat(cv::ACCESS_READ);
	   lFrameGRAY  	= imread(mFiles[mFrameCount], cv::IMREAD_GRAYSCALE).getUMat(cv::ACCESS_READ);
	   lLock.unlock();

	   //Put the frames in the queue for the stateMachine
           enqueue(lFrameGRAY, mProcessQueue); //Thread-safe method for enqueuing processing Frames
           enqueue(lFrame,     mDisplayQueue); //Thread-safe method for enqueuing display Frmaes

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

 	 }
	 std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
     throw "mSkipFrames  must be a postive integer";
   }

   try
   {
    	glob(mFolder, mFiles);
   }
   catch(...)
   {
	throw "DataSet not Found";
   }

   if (mFiles.size() <= (uint32_t)mSkipFrames)
   {
      throw "Total number of image files is less than mSkipFrames.";
   }
 
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
