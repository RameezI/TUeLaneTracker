

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
	mBlockName.push_back(BlkName);
	mStartTick.push_back(FSL_Ticks()); 
}

void ProfilerLDT::end()
{
	mEndTick = FSL_Ticks();
	blockTime_ms(mStartTick.back(), mEndTick);
	
	mStartTick.pop_back();
	mBlockName.pop_back();
}


   
void ProfilerLDT::blockTime_ms(uint64_t t_start, uint64_t t_finish)
{
           mStartTime	= FSL_TicksToSeconds(t_start);
           mEndTime		= FSL_TicksToSeconds(t_finish);
           
    if ( mBlockMultiplicity.find(mBlockName.back()) == mBlockMultiplicity.end() ) 
    {// not found
        
        mBlockMultiplicity[mBlockName.back()]	=	1;
        mBlockAvgTime[mBlockName.back()]		= (mEndTime-mStartTime)*1000;
    
    } else 

    {// found

        if ((mBlockMultiplicity[mBlockName.back()] != 0) & (mBlockMultiplicity[mBlockName.back()] <UINTMAX_MAX))
        {    
        
	mBlockMultiplicity[mBlockName.back()]	=	mBlockMultiplicity[mBlockName.back()]+1;
        mBlockAvgTime[mBlockName.back()]		= 	(1- (1.0/mBlockMultiplicity[mBlockName.back()]))*mBlockAvgTime[mBlockName.back()] 
											+
							 (1.0/mBlockMultiplicity[mBlockName.back()])*(mEndTime-mStartTime)*1000;
        
	}
   
    }
 
   
}

/*
void ProfilerLDT::printTiming()
{
    for (std::map<string,double>::iterator it=m_block_avgtime.begin(); it!=m_block_avgtime.end(); ++it)
					std::cout << it->first << " : " << it->second << '\n';
}
*/


uint64_t ProfilerLDT::getMultiplicity(std::string block)
{
   if ( mBlockMultiplicity.find(block) == mBlockMultiplicity.end() ) 
    {// not found
        
        return 0;
    } else 
    {// found
    
        return mBlockMultiplicity[block];
    }
					
}


double ProfilerLDT::getTiming(std::string block)
{
    if ( mBlockAvgTime.find(block) == mBlockAvgTime.end() ) 
    {// not found
        
        return -1;
    
    } else 
    {// found
    
        return mBlockAvgTime[block];
    }
					
}


void ProfilerLDT::writeTimings()
{
    


}



ProfilerLDT::~ProfilerLDT()
{
        
    
}

