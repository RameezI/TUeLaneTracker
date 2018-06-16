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
*
* ****************************************************************************/ 

#include "InitState.h"

// Class Implementation
unique_ptr<LaneFilter> InitState::createLaneFilter()
{
	
	unique_ptr<LaneFilter> 	lLaneFilter = nullptr;
	
	try
	{
	  Camera 			lCamera(mConfig);
	  LaneProperties		lLane(mConfig);

	  lLaneFilter =	unique_ptr<LaneFilter>(new LaneFilter(lLane, lCamera, mConfig));

	  mLaneFilterCreated 	= true;

	  if (checkCreationStatus())
	   currentStatus= StateStatus::DONE;

	}
	catch(const char* msg)
	{
	   #ifdef PROFILER_ENABLED
	    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	    <<"******************************"<<endl
	    <<  "Lane-Filter Instantiation Failed: "<<endl
	    << msg <<endl
	    <<"******************************"<<endl<<endl;
	   #endif
	   lLaneFilter.reset(nullptr);				
	   mLaneFilterCreated = false;
	   currentStatus= StateStatus::ERROR;
	}
	return lLaneFilter;	
}


unique_ptr<VanishingPtFilter> InitState::createVanishingPtFilter()
{

	unique_ptr<VanishingPtFilter> lVanishingPtFilter;

	try
	{
	  Camera 		lCamera(mConfig);
	  LaneProperties   	lLane(mConfig);
	  LaneFilter 		lLaneFilter(lLane, lCamera, mConfig);
	
	  lVanishingPtFilter = unique_ptr<VanishingPtFilter>
	  (new VanishingPtFilter(lCamera.HORIZON_VH(1), lCamera.HORIZON_VH(0), mConfig)); 
	   
	  mVpFilterCreated 	= true;
	
	  if (checkCreationStatus())
	   currentStatus	= StateStatus::DONE;

	 }
	catch(const char* msg)
	{
	   #ifdef PROFILER_ENABLED
	    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	    <<"******************************"<<endl
	    <<  "Vanishing-Point-Filter Instantiation Failed: "<<endl
	    << msg <<endl
	    <<"******************************"<<endl<<endl;
	   #endif

	  lVanishingPtFilter.reset(nullptr);
	  mVpFilterCreated	= false;
	  currentStatus		= StateStatus::ERROR;
	}

	return lVanishingPtFilter; 	
}


unique_ptr<Templates> InitState::createTemplates()
{
	unique_ptr<Templates> lTemplates;

	try
	{
	  Camera 		lCamera(mConfig);
	  LaneProperties    	lLane(mConfig);
	  LaneFilter 		lLaneFilter(lLane, lCamera, mConfig);
	  VanishingPtFilter 	lVanishingPtFilter(lCamera.HORIZON_VH(1), lCamera.HORIZON_VH(0), mConfig);

	
	  lTemplates	 = unique_ptr<Templates>
	  ( new  Templates (lCamera.RES_VH(0), lCamera.RES_VH(1), lCamera.FOV_VH(0),
		 lCamera.HORIZON_VH(1), lCamera.HORIZON_VH(0), lVanishingPtFilter.RANGE_V) );
	
	   mTemplatesCreated	= true;
	
	  if (checkCreationStatus())
	  currentStatus		= StateStatus::DONE;		
	}
	catch(const char* msg)
	{
	   #ifdef PROFILER_ENABLED
	    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	    <<"******************************"<<endl
	    <<  "Templates Instantiation Failed: "<<endl
	    << msg <<endl
	    <<"******************************"<<endl<<endl;
	   #endif

	   lTemplates.reset(nullptr);
	   mTemplatesCreated	= false;
	   currentStatus	= StateStatus::ERROR;
	}

	return lTemplates;
}


bool InitState::checkCreationStatus()
{
	if (mLaneFilterCreated && mVpFilterCreated && mTemplatesCreated)
	 return true;
	else
	return false;
}
