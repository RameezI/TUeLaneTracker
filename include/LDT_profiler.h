/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/


#ifndef _LDT_PROFILE_H
#define _LDT_PROFILE_H
#include "common_time_measure.h"
#include <iostream>
#include <map>
#include <vector>
#include <mutex>

using  namespace std;
using  LockGuard = std::lock_guard<std::mutex>;

    class ProfilerLDT
    {

private:
            
	std::mutex 			mMutex;
	double mStartTime;
	double mEndTime;

	vector<uint64_t> mStartTick;
	uint64_t mEndTick;

	vector<string> mBlockName;

	std::map<string, double> 	mBlockMaxTime;
	std::map<string, double> 	mBlockAvgTime;
	std::map<string, double> 	mBlockMinTime;
	std::map<string, uint64_t> 	mBlockMultiplicity;
	   
	void blockTime_ms(uint64_t , uint64_t );
            
public:
	
	ProfilerLDT();		
	   
	void    	start(std::string);
 
	void 		writeTimings(); 	//write Timings to a file

	double 		getAvgTime(std::string);  	//get Timings for a specific block
	double 		getMaxTime(std::string);  	//get Timings for a specific block
	double 		getMinTime(std::string);  	//get Timings for a specific block
		

	uint64_t 	getMultiplicity(std::string);
	
	void        end(); 

	~ProfilerLDT();
    };
	
#endif
