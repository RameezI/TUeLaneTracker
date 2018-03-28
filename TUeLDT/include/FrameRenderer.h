#ifndef FRAME_RENDERER_H
#define FRAME_RENDERER_H

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

#include "LaneModel.h"		//implicit include of LaneFilter and VanishingPtFilter
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;

class FrameRenderer
{
   const int 		mHORIZON;
   const LaneFilter& 	mLANE_FLTR;

   private:

    vector<Point> lBoundaryPts_L;
    vector<Point> lBoundaryPts_R;
    vector<Point> lBoundaryPts_M;
    float lSlopeLeft;
    float lSlopeRight;

public:
   FrameRenderer(const int HORIZON, const LaneFilter& LANE_FLTR)
   : mHORIZON(HORIZON), mLANE_FLTR(LANE_FLTR)
   {

   }
   void drawLane(const cv::Mat& FRAME, const LaneModel& Lane);
   vector<float> getDirectionalParameters();
};



#endif