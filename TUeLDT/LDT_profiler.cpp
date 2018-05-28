
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

#include "LDT_profiler.h"
 
ProfilerLDT::ProfilerLDT()
: mStartTime(0),
  mEndTime(0),
  mStartTick(0),
  mEndTick(0)
{
	mBlockName.clear();
	mStartTick.clear();
	mBlockMaxTime.clear();
 	mBlockAvgTime.clear();
	mBlockMinTime.clear();
	mBlockMultiplicity.clear();
}

void ProfilerLDT::start(string BlkName)
{

     	LockGuard  lLock(mMutex); //lock gaurd to protect race conditions
	 mBlockName.push_back(BlkName);
	 mStartTick.push_back(FSL_Ticks());
}

void ProfilerLDT::end()
{
     	LockGuard  lLock(mMutex); //lock gaurd to protect race conditions

	mEndTick = FSL_Ticks();
	blockTime_ms(mStartTick.back(), mEndTick);
	
	mStartTick.pop_back();
	mBlockName.pop_back();
}


   
void ProfilerLDT::blockTime_ms(uint64_t t_start, uint64_t t_finish)
{

	mStartTime	= FSL_TicksToSeconds(t_start);
	mEndTime	= FSL_TicksToSeconds(t_finish);

	   
	if ( mBlockMultiplicity.find(mBlockName.back()) == mBlockMultiplicity.end() ) 
	{// not found

	mBlockMultiplicity[mBlockName.back()]	=  1;
	mBlockAvgTime[mBlockName.back()]	= (mEndTime-mStartTime)*1000;
	mBlockMinTime[mBlockName.back()]	= (mEndTime-mStartTime)*1000;
	mBlockMaxTime[mBlockName.back()]	= (mEndTime-mStartTime)*1000;

	} 
	else 

	{// found

		if ( (mBlockMultiplicity[mBlockName.back()] != 0) & (mBlockMultiplicity[mBlockName.back()] <UINTMAX_MAX) )
		{    

		   mBlockMultiplicity[mBlockName.back()] = mBlockMultiplicity[mBlockName.back()]+1;


		   mBlockAvgTime[mBlockName.back()] = 	
			   (1- (1.0/mBlockMultiplicity[mBlockName.back()]))*mBlockAvgTime[mBlockName.back()] 
		   +       (1.0/mBlockMultiplicity[mBlockName.back()])*(mEndTime-mStartTime)*1000;


		   if (mBlockMinTime[mBlockName.back()] > (mEndTime-mStartTime)*1000)
		   mBlockMinTime[mBlockName.back()] = (mEndTime-mStartTime)*1000;

		   else			
		   if (mBlockMaxTime[mBlockName.back()] < (mEndTime-mStartTime)*1000)
		   mBlockMaxTime[mBlockName.back()] = (mEndTime-mStartTime)*1000;							
		}
	}
}


uint64_t ProfilerLDT::getMultiplicity(std::string block)
{

   LockGuard  lLock(mMutex); //lock gaurd to protect race conditions

   if ( mBlockMultiplicity.find(block) == mBlockMultiplicity.end() ) 
    {// not found     
        return 0;
    } 
    else 
    {// found
        return mBlockMultiplicity[block];
    }
					
}


double ProfilerLDT::getAvgTime(std::string block)
{

   LockGuard  lLock(mMutex); //lock gaurd to protect race conditions

   if ( mBlockAvgTime.find(block) == mBlockAvgTime.end() ) 
   {// not found
        return -1;
   } 
   else 
   {// found 
        return mBlockAvgTime[block];
   }

}


double ProfilerLDT::getMinTime(std::string block)
{

   LockGuard  lLock(mMutex); //lock gaurd to protect race conditions

    if ( mBlockMinTime.find(block) == mBlockMinTime.end() ) 
    {// not found
        return -1;
  
    } 
    else 
    {// found
    
        return mBlockMinTime[block];
    }

}

double ProfilerLDT::getMaxTime(std::string block)
{

					
   LockGuard  lLock(mMutex); //lock gaurd to protect race conditions

    if ( mBlockMaxTime.find(block) == mBlockMaxTime.end() ) 
    {// not found    
        return -1;
    
    } 
    else 
    {// found
        return mBlockMaxTime[block];
    }

}

void ProfilerLDT::writeTimings()
{
    
}



ProfilerLDT::~ProfilerLDT()
{    
    
}

