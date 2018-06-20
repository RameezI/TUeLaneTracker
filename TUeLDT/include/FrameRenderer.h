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
#include "Config.h"
#include "LaneModel.h"		//implicit include of LaneFilter and VanishingPtFilter
#include "opencv2/opencv.hpp"
#include "FrameFeeder.h"

#ifdef DISPLAY_GRAPHICS_DCU
#include "frame_output_v234fb.h"
#endif


using namespace cv;

class FrameRenderer
{

private:

   const cv::Mat        mBASE_BINS;
   const cv::Mat	mPURVIEW_BINS;
   const cv::Point	mO_ICCS_ICS;
   const int 		mBASE_LINE_ICS;
   const int 		mPURVIEW_LINE_ICS;
   const size_t		mCOUNT_BINS;
   const int      	mHORIZON_V;

   FrameFeeder& 	mFrameFeeder;

   #ifdef DISPLAY_GRAPHICS_DCU
    io::FrameOutputV234Fb  mDCU;
   #endif

   #ifdef WRITE_FRAMES_TO_FILE
    cv::VideoWriter  mOutputVideo;
   #endif

public:
   FrameRenderer(const LaneFilter& LANE_FLTR, FrameFeeder& FRAME_FEEDER)
   : 
     mBASE_BINS(LANE_FLTR.BASE_BINS),
     mPURVIEW_BINS(LANE_FLTR.PURVIEW_BINS),
     mO_ICCS_ICS(LANE_FLTR.O_ICCS_ICS),
     mBASE_LINE_ICS(LANE_FLTR.BASE_LINE_ICCS + mO_ICCS_ICS.y),
     mPURVIEW_LINE_ICS(LANE_FLTR.PURVIEW_LINE_ICCS + mO_ICCS_ICS.y),
     mCOUNT_BINS(mBASE_BINS.rows),
     mHORIZON_V(LANE_FLTR.CAMERA.HORIZON_VH(0)),
     mFrameFeeder(FRAME_FEEDER)

     #ifdef DISPLAY_GRAPHICS_DCU
      , mDCU(io::FrameOutputV234Fb(LANE_FLTR.CAMERA.RES_VH(1), LANE_FLTR.CAMERA.RES_VH(0), io::IO_DATA_DEPTH_08, io::IO_DATA_CH3))
     #endif

   {
      #ifdef WRITE_FRAMES_TO_FILE
      Size lSize = cv::Size( LANE_FLTR.CAMERA.RES_VH(1), LANE_FLTR.CAMERA.RES_VH(0));
       mOutputVideo.open("TUeLaneTracker.avi", CV_FOURCC('F','M','P','4'), 25,  lSize);
      #endif
   }
   void drawLane(const cv::UMat& FRAME, const LaneModel& Lane);
};

#endif
