#include "TrackingLanesDAG_generic.h"
#include <vector>


class FillBins_Parallel: public cv::ParallelLoopBody
{
	
private:

Mat& 	mBaseIntrsec;
Mat&    mPurviewIntrsec;
Mat& 	mWeightIntrsec;
Mat&    mMask;

const int mSTEP_LANE_HIST;
const int mSTEP_VP_HIST;

const int mSTART_LANE_HIST;
const int mSTART_VP_HIST;

uint16_t*	mBaseBinIdx;
uint16_t*   mPurviewBinIdx;
uint16_t*	mWeightBin;


public:
	FillBins_Parallel(
						  Mat& baseIntrsec,
						  Mat& purviewIntrsec,
						  Mat& weightIntrsec,
						  Mat& mask,
						  int stepLaneHist,
						  int stepVPHist,
						  int startLaneHist,
						  int startVPHist,
						  uint16_t* baseBinIdx,
						  uint16_t* purviewBinIdx,
						  uint16_t* weightBin
					  )
					  
		//Initialisation List 
		:  mBaseIntrsec(baseIntrsec),
		   mPurviewIntrsec(purviewIntrsec),
		   mWeightIntrsec(weightIntrsec),
		   mMask(mask),
		   mSTEP_LANE_HIST(stepLaneHist),
		   mSTEP_VP_HIST(stepVPHist),
		   mSTART_LANE_HIST(startLaneHist),
		   mSTART_VP_HIST(startVPHist),
		   mBaseBinIdx(baseBinIdx),
		   mPurviewBinIdx(purviewBinIdx),
		   mWeightBin(weightBin){}


	virtual void operator()( const cv::Range &range ) const 
	{	
		register int32_t* 	IN_basePTR 	    = mBaseIntrsec.ptr<int32_t>(0)      	+ range.start;
		register int32_t* 	IN_purviewPTR   = mPurviewIntrsec.ptr<int32_t>(0)   	+ range.start;
		register uint16_t* 	IN_weightsPTR   = mWeightIntrsec.ptr<uint16_t>(0) 		+ range.start;
		register uint8_t* 	IN_maskPTR   	= mMask.ptr<uint8_t>(0) 				+ range.start;
		
		register uint16_t*     	OUT_basePTR		= mBaseBinIdx 		+ range.start;
		register uint16_t*     	OUT_purviewPTR	= mPurviewBinIdx    + range.start;
		register uint16_t*     	OUT_weights		= mWeightBin		+ range.start;	
		

		
		for (int i = range.start; i < range.end; i++, IN_basePTR++, IN_purviewPTR++, IN_weightsPTR++, IN_maskPTR++)
		{
		
			if(!(*IN_maskPTR ==0) )
			{				
				*OUT_basePTR=
				( *IN_basePTR    - mSTART_LANE_HIST + (mSTEP_LANE_HIST/2) ) / mSTEP_LANE_HIST;
				
				*OUT_purviewPTR=
				( *IN_purviewPTR -  mSTART_VP_HIST   + (mSTEP_VP_HIST/2)  ) / mSTEP_VP_HIST ;
				
				*OUT_weights = *IN_weightsPTR;
				
				//OUT_basePTR++;
				//OUT_purviewPTR++;
				//OUT_weights++;
			}	
			
		}
	}
};


