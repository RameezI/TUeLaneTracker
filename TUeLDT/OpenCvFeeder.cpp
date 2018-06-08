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


const int CAM_WIDTH_PIX = 640;
const int CAM_HEIGHT_PIX = 480;

OpenCvFeeder::OpenCvFeeder(string sourceUriStr) :
  mLatestFrameRGB(CAM_HEIGHT_PIX,CAM_WIDTH_PIX,CV_8UC3),
  mLatestFrameGray(CAM_HEIGHT_PIX,CAM_WIDTH_PIX,CV_8UC1)
{
  parseSettings (sourceUriStr);
  mAsyncGrabber = std::thread(&OpenCvFeeder::captureThread,this);
}

OpenCvFeeder::~OpenCvFeeder()
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

// Expect either a camera-index (e.g. USB cam) or
// an URL e.g. rtsp://admin:12345@192.168.140.14/streams/channel/1
void OpenCvFeeder::parseSettings(string& srcStr)
{
  if (srcStr.length() > 1)
  {
    // Check uri
    // TODO : add GStreamer pipeline support, e.g. to support:
    // rtsp://admin:12345@192.168.140.14/streams/channel/1 latency=0 ! rtpjitterbuffer ! rtpsession ! rtpssrcdemux ! rtpptdemux ! rtpjitterbuffer !rtph264depay ! h264parse ! video/x-h264 ! avdec_h264 ! appsink name=appsink0 sync=0
    std::regex url_regex(
        R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",
        std::regex::extended);
    std::smatch url_match_result;
    if (!std::regex_match(srcStr, url_match_result, url_regex))
    {
      throw "Invalid URI used: " + srcStr;
    }
  }
  mUri = srcStr;
}

void OpenCvFeeder::captureThread()
{
  WriteLock frameLock(mMutex, std::defer_lock);

  cv::VideoCapture inputCapture;
  inputCapture.open(mUri);

  try
  {

    if (!inputCapture.isOpened())
    {
      cout << "Unable to open video input on: "+mUri;
      return;
    }
    cout << "Opened video stream: " << mUri << endl;

    while(!Stopped)
    {

      if(!Paused)
      {
#ifdef PROFILER_ENABLED
        mProfiler.start("FRAME_READING");
#endif
        frameLock.lock();

        inputCapture >> mLatestFrameRGB;

        if (!mLatestFrameRGB.empty())
        {
          cv::cvtColor(mLatestFrameRGB, mLatestFrameGray, CV_RGB2GRAY);
        }
        else
        {
          mLatestFrameGray = cv::UMat(0,0,CV_8UC1);
          mLatestFrameRGB = cv::UMat(0,0,CV_8UC3);
          Stopped = true;
        }
        frameLock.unlock();
	
	
#ifdef PROFILER_ENABLED
        mProfiler.end();
        LOG_INFO_(LDTLog::TIMING_PROFILE)<<endl
        <<"******************************"<<endl
        << "Frame Reading Time." <<endl
        << "Max Time: " << mProfiler.getMaxTime("FRAME_READING")<<endl
        << "Avg Time: " << mProfiler.getAvgTime("FRAME_READING")<<endl
        << "Min Time: " << mProfiler.getMinTime("FRAME_READING")<<endl
        <<"******************************"<<endl<<endl;
#endif
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(30));

    }
  }
  catch(exception& ex)
  {
    cerr << "Exception in frame grabber thread : " << ex.what() << endl;
    throw;
  }
}


cv::UMat OpenCvFeeder::dequeue()
{
  std::unique_lock<std::mutex> lLock(mMutex);

  if(mLatestFrameGray.empty())
  {
    throw "Gray frame is empty! [Empty Frame Exception] ";
  }
  return mLatestFrameGray;

}

cv::UMat OpenCvFeeder::dequeueDisplay()
{
  std::unique_lock<std::mutex> lLock(mMutex);

  if (mLatestFrameRGB.empty())
  {
    throw "RGB frame is empty (end of stream?) ! [Empty Frame Exception] ";
  }

  return mLatestFrameRGB;
}

