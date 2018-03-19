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
}

void ImgStoreFeeder::parseSettings(string& srcStr)
{
   cout << srcStr<<endl;
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

   glob(mFolder, mFiles);

   if (mFiles.size() <= (uint32_t)mSkipFrames)
   {
      throw "Total number of image files is less than mSkipFrames.";
   }
 
}

void ImgStoreFeeder::produceFrames()
{
   cv::Mat lMatGRAY;
   mFrames.push_back( imread(mFiles[mFrameCount]) );
   cv::cvtColor(mFrames.back(),lMatGRAY, cv::COLOR_RGB2GRAY );
   mFramesGRAY.push_back(lMatGRAY);

  if (mFrameCount+1 < mFiles.size());
     mFrameCount ++;
}


cv::Mat ImgStoreFeeder::getFrame()
{	
  assert(!mFrames.empty());
  cv::Mat lFrame = mFrames[0];
  mFrames.erase(mFrames.begin());

  if (lFrame.empty())
     throw "Failed to get the frame! [Empty Frame Exception] ";

  return lFrame;
}


cv::Mat ImgStoreFeeder::getFrameGRAY()
{
  assert(!mFramesGRAY.empty());
  cv::Mat lFrame = mFramesGRAY[0];
  mFramesGRAY.erase(mFramesGRAY.begin());

  if (lFrame.empty())
     throw "Failed to get the frame! [Empty Frame Exception] ";

  return lFrame;
}
