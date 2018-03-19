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

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;

class FrameFeeder
{

public:
   virtual cv::Mat	getFrameGRAY()  	= 0;
   virtual cv::Mat	getFrame()  		= 0;
   virtual void  	produceFrames()		= 0;

protected:
   virtual void parseSettings(string& srcStr)  = 0;	
   vector<cv::Mat>	mFramesGRAY;
   vector<cv::Mat>	mFrames;
};

class ImgStoreFeeder: public FrameFeeder
{

private:
	string			mFolder;
	int 	        	mSkipFrames;
	uint32_t 		mFrameCount;
	vector< cv::String> 	mFiles;

public:
	
	ImgStoreFeeder(string sourceStr);
	void parseSettings(string& srcStr) override;
	void produceFrames() override;
	cv::Mat getFrame() override;
	cv::Mat getFrameGRAY() override;
};
#endif

/*

	else if(lSource == FrameSource::STREAM)
	{
		#ifdef PROFILER_ENABLED
		 LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		 <<"******************************"<<endl
		 << "Setting up the Frame Source" <<endl
		 << "MODE: "<< FrameSource::STREAM<<endl
		 <<"******************************"<<endl<<endl;
		#endif

		mGraph.mSource = lSource;
	   	mGraph.mFrameCount = 0;
		
		try
		{
		   if(!mGraph.mCAPTURE.open(lSourceStr))
		   lReturn = -1;
		}

		catch(...)
		{
		    lReturn = -1;
		}
	}

	else if (lSource == FrameSource::GMSL)
	{

		#ifdef PROFILER_ENABLED
		 LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		 <<"******************************"<<endl
		 << "Setting up the Frame Source" <<endl
		 << "MODE: "<< FrameSource::GMSL<<endl
		 <<"******************************"<<endl<<endl;
		#endif

		mGraph.mSource = lSource;
	   	mGraph.mFrameCount = 0;
	
		throw "GMSL function not implemented";
		lReturn =-1;
	}

	else
	{

		#ifdef PROFILER_ENABLED
		 LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		 <<"******************************"<<endl
		 << "Setting up the Frame Source" <<endl
		 << "MODE: NOT RECOGNIZED"<<endl
		 << "Exiting"<<endl
		 <<"******************************"<<endl<<endl;
		#endif

		lReturn =-1;
	}

	return lReturn;

}
uint_fast8_t 	mRetryGrab;
int  	setSource(FrameSource, string);

//^TODO: Move to BuffeingState.h -> Localise Frame Input Code
int BufferingDAG_generic::grabFrame()	
{
	int lReturn = 0;
		
	#ifdef PROFILER_ENABLED
	mProfiler.start("IMAGE_READ");
	#endif 


	   if(mSource == FrameSource::DIRECTORY)
	   {
		 mFrameRGB = imread(mFiles[mFrameCount]);
		
		 #ifdef PROFILER_ENABLED
		  LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
		  <<"Processing Frame: "<<mFrameCount<<endl;
		 #endif

		 if (mFrameCount+1 < mFiles.size())
	   	  mFrameCount ++;
	   }

	   else if (mSource == FrameSource::STREAM)
	   {
		 mCAPTURE >> mFrameRGB;

		 #ifdef PROFILER_ENABLED
		  LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
		  <<"Processing Frame: "<<mFrameCount<<endl;
		 #endif

	   	 mFrameCount ++;
	   }

	   else if (mSource == FrameSource::GMSL)
	   {
		 #ifdef PROFILER_ENABLED
		  LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
		  <<"Undefined Input Mode: "<<endl;
		 #endif

		lReturn = -1;
	   }

	   else
	   {
		 #ifdef PROFILER_ENABLED
		  LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
		  <<"Undefined Input Mode: "<<endl;

*/





