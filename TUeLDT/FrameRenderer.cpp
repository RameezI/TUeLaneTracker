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

#include "FrameRenderer.h"

using namespace cv;

void FrameRenderer::drawLane(const cv::Mat& FRAME, const LaneModel& Lane)
{

   const int 	lBASE_LINE_ICS 		= mLANE_FLTR.BASE_LINE_ICCS 	+ mLANE_FLTR.O_ICCS_ICS.y;
   const int 	lPURVIEW_LINE_ICS 	= mLANE_FLTR.PURVIEW_LINE_ICCS	+ mLANE_FLTR.O_ICCS_ICS.y;

   const float 	lRatioLookAhead 	= 0.1;

   vector<Point> lBoundaryPts_L;
   vector<Point> lBoundaryPts_R;
   vector<Point> lBoundaryPts_M;

   //Transform VP to Image Coordianate System
   int VP_V =  Lane.vanishingPt.V + mLANE_FLTR.O_ICCS_ICS.y;
   int VP_H =  Lane.vanishingPt.H + mLANE_FLTR.O_ICCS_ICS.x;

   //Lane Bundaries
   lBoundaryPts_L.push_back( Point( Lane.boundaryLeft  + mLANE_FLTR.O_ICCS_ICS.x, lBASE_LINE_ICS) );
   lBoundaryPts_R.push_back( Point( Lane.boundaryRight + mLANE_FLTR.O_ICCS_ICS.x, lBASE_LINE_ICS) );
   lBoundaryPts_M.push_back( (lBoundaryPts_L[0] + lBoundaryPts_R[0])/2.0 );

   float lSlopeLeft =  (float)( VP_V - 	lBASE_LINE_ICS ) /(VP_H - lBoundaryPts_L[0].x);
   float lSlopeRight = (float)( VP_V -	lBASE_LINE_ICS ) /(VP_H - lBoundaryPts_R[0].x);

   lBoundaryPts_L.push_back(lBoundaryPts_L[0]);
   lBoundaryPts_L[1].x  += 	-round((lBASE_LINE_ICS * lRatioLookAhead) / lSlopeLeft);
   lBoundaryPts_L[1].y 	+= 	-round((lBASE_LINE_ICS * lRatioLookAhead));

   lBoundaryPts_R.push_back(lBoundaryPts_R[0]);
   lBoundaryPts_R[1].x  += 	-round((lBASE_LINE_ICS*lRatioLookAhead) / lSlopeRight);
   lBoundaryPts_R[1].y 	+= 	-round((lBASE_LINE_ICS*lRatioLookAhead));

   lBoundaryPts_M.push_back( (lBoundaryPts_L[1] + lBoundaryPts_R[1])/2.0);

   // Draw Left Boundary Line
   line(FRAME, lBoundaryPts_L[0], lBoundaryPts_L[1], CvScalar(0,255,0),3);

   //Draw Right Boundary Line	
   line(FRAME, lBoundaryPts_R[0], lBoundaryPts_R[1], CvScalar(0,255,0),3);
   
   //Draw Middle Line
   line(FRAME, lBoundaryPts_M[0], lBoundaryPts_M[1], CvScalar(255,0,0), 2 );


   //Draw Base Line
   line(FRAME, Point(0,lBASE_LINE_ICS),    Point(FRAME.cols,lBASE_LINE_ICS),	CvScalar(0,0,0),1);

   //Draw Purview Line
   line(FRAME, Point(0,lPURVIEW_LINE_ICS), Point(FRAME.cols,lPURVIEW_LINE_ICS),	CvScalar(0,0,0),1);


   // Highlight region below Horizon
   {
     Rect lROI;	
     int lX = 0;
     int lY = mLANE_FLTR.O_ICCS_ICS.y + mHORIZON;
     lROI = Rect(lX,lY, FRAME.cols, FRAME.rows-lY);
     cv::Mat lYellow(FRAME.rows -lY, FRAME.cols, CV_8UC3, Scalar(0,125,125));
     cv::Mat lFrameRGB_SPAN = FRAME(lROI);
     cv::addWeighted(lYellow, 0.4, lFrameRGB_SPAN, 0.6, 0, lFrameRGB_SPAN);
   }


   // Draw Histogram-Bins at the Base
   for (size_t i=0; i< mLANE_FLTR.COUNT_BINS; i++)
   {
	int x =  mLANE_FLTR.BASE_BINS.at<int32_t>(i,0) + mLANE_FLTR.O_ICCS_ICS.x ;

	if (x !=  lBoundaryPts_M[0].x)
	line(FRAME, cvPoint(x,lBASE_LINE_ICS), cvPoint(x,lBASE_LINE_ICS - 30), cvScalar(0,0,0), 1);
	else
	line(FRAME, cvPoint(x,lBASE_LINE_ICS), cvPoint(x,lBASE_LINE_ICS - 40), cvScalar(0,0,255), 2);
   }


   // Draw Histogram-Bins at the Purview
   for (size_t i=0; i< mLANE_FLTR.COUNT_BINS; i++)
   {
	int x = mLANE_FLTR.PURVIEW_BINS.at<int32_t>(i,0) + mLANE_FLTR.O_ICCS_ICS.x;
	line(FRAME, cvPoint(x, lPURVIEW_LINE_ICS), cvPoint(x, lPURVIEW_LINE_ICS - 30), cvScalar(0,0,0), 1);
   }

   imshow( "Display window", FRAME);
   
   if ( (char)32 == (char) waitKey(10) )
   {
	//cout << "Base Histogram Bins :		"<< mLANE_FLTR.BASE_BINS.transpose()<<endl<<endl;
	//cout << "Purview Histogram Bins : 		"<< mLANE_FLTR.PURVIEW_BINS.transpose()<<endl<<endl;

	while ((char)32 != (char)waitKey(1));
   }
}
