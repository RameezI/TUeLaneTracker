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

InitState::InitState()
: mLaneFilterCreated(false),
  mVpFilterCreated  (false),
  mTemplatesCreated (false)
{	
	this->currentStatus = StateStatus::ACTIVE;
}


unique_ptr<LaneFilter> InitState::createLaneFilter()
{
	
	Camera 			camera;
	LaneParameters		lane;

	unique_ptr<LaneFilter> 	laneFilter( new LaneFilter (lane, camera) );
	mLaneFilterCreated 	= true;

	if (checkCreationStatus())
	currentStatus= StateStatus::DONE;

	return laneFilter;	
}


unique_ptr<VanishingPtFilter> InitState::createVanishingPtFilter()
{
	Camera 			camera;
	LaneParameters   	lane;
	LaneFilter 		laneFilter(lane, camera);
	
	unique_ptr<VanishingPtFilter> 
	vanishingPtFilter(new VanishingPtFilter (laneFilter.HISTOGRAM_BINS, laneFilter.OFFSET_V) ); 
	   
	mVpFilterCreated 	=true;
		
	if (checkCreationStatus())
	currentStatus= StateStatus::DONE;
	
	return vanishingPtFilter; 	
}


unique_ptr<Templates> InitState::createTemplates()
{
	Camera 			camera;
	LaneParameters    	lane;
	LaneFilter 		laneFilter(lane, camera);
	VanishingPtFilter 	vanishingPtFilter(laneFilter.HISTOGRAM_BINS, laneFilter.OFFSET_V);
	
	unique_ptr<Templates>
	templates ( new  Templates (camera.RES_VH(0), camera.RES_VH(1), camera.FOV(0), vanishingPtFilter.VP_RANGE_V) );
	
	mTemplatesCreated	= true;
	
	if (checkCreationStatus())
	currentStatus= StateStatus::DONE;		
	
	return templates;
}


bool InitState::checkCreationStatus()
{
	if (mLaneFilterCreated && mVpFilterCreated && mTemplatesCreated)
	return true;
	
	else
	return false;
}

InitState::~InitState()
{	

	
}
