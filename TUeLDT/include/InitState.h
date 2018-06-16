#ifndef INITSTATE_H
#define INITSTATE_H

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

#include <memory>
#include "State.h"
#include "LaneModel.h"
#include "Templates.h"

using namespace std;

class InitState: public State
{	

private :
		const LaneTracker::Config& mConfig;
		bool  mLaneFilterCreated;
		bool  mVpFilterCreated;
		bool  mTemplatesCreated;
        	bool  checkCreationStatus();
		
public	:
		/* These methods create instances and also keep record of the already created objects
		 * Make sure all objects are created before indicating "StateStatus::DONE". */
		 
		unique_ptr<LaneFilter> 			createLaneFilter();
		unique_ptr<VanishingPtFilter> 		createVanishingPtFilter();
		unique_ptr<Templates> 			createTemplates();
		
		InitState(const LaneTracker::Config& Config):
			    mConfig(Config),
			    mLaneFilterCreated(false),
  		  	    mVpFilterCreated  (false),
  		  	    mTemplatesCreated (false)
		{	
		  this->currentStatus = StateStatus::ACTIVE;
		}

	   	~InitState() {}
};



#endif // INITSTATE_H
