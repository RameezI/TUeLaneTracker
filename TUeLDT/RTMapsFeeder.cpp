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

RTMapsFeeder::RTMapsFeeder(string sourceStr)
: mFolder(""),
  mSkipFrames(0),
  mFrameCount(0)
{
    parseSettings(sourceStr);
}


void RTMapsFeeder::parseSettings(string& srcStr)
{
    if (srcStr != "")
        std::cout << "RTMaps does not require parseSettings" << endl;
}

RTMapsFeeder::~RTMapsFeeder()
{

}

void RTMapsFeeder::setImageLinker(cv::Mat imgLink)
{
    mImgCurrent = imgLink;
}

cv::Mat RTMapsFeeder::dequeue()
{
    cv::Mat gray;
    cv::cvtColor(mImgCurrent,gray, cv::COLOR_RGB2GRAY);
    return gray;
}

cv::Mat RTMapsFeeder::dequeueDisplay()
{
    return  mImgCurrent;
}

void RTMapsFeeder::enqueue(cv::Mat& frame, vector<cv::Mat>& queue)
{
    if (false)
        queue.push_back(frame);
}