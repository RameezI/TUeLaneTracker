#ifndef TRACKINGLANESDAG_GENERIC_H
#define TRACKINGLANESDAG_GENERIC_H
#include "State.h"
#include "LaneFilter.h"
#include "VanishingPtFilter.h"
#include "BufferingDAG_generic.h"
#include <thread>

const int SCALE_INTSEC= 1024;
const int SCALE_GRAD_TAN = 128;
const int SCALE_INTSEC_TAN = 128*1024;

class TrackingLanesDAG_generic: public BufferingDAG_generic
{
	
friend class TrackingLaneState;
friend class TEST_TrackingState;
	
private:

	const int				mMAX_PIXELS_ROI;

	int  					LOWER_LIMIT_IntBase;
	int 					UPPER_LIMIT_IntBase;
	
	int  					LOWER_LIMIT_IntPurview;
	int 					UPPER_LIMIT_IntPurview;
	
	int						SCALED_STEP_LANE_FILTER;
	int 					SCALED_STEP_VP_FILTER;
	int 					SCALED_START_LANE_FILTER;
	int 					SCALED_START_VP_FILTER;
	
	LaneFilter* 			mLaneFilter;
	VanishingPtFilter*		mVpFilter;
	
	Mat 					mX_VPRS_SCALED;	
	Mat 					mIntBase;           
	Mat						mIntPurview;
	Mat 					mIntWeights;
	
	vector<uint16_t>		mBaseBinIdx; 	  //Only valid list of Intersections IDs
	vector<uint16_t>		mPurviewBinIdx;  //Only valid list of Intersections IDs
	vector<uint16_t>	    mWeightBin;     //Only valid list of Weights IDs
	
	Mat						mHistBase;
	Mat						mHistPurview;
	
	Mat 					mMask;
	Mat						mProbMapFocussed;
	Mat 					mGradTanFocussed;

	
	
	
public:
	TrackingLanesDAG_generic(BufferingDAG_generic&& bufferingGraph);
	void auxillaryTasks();
	void extractLanes();
	void extractControllerInputs();
	~TrackingLanesDAG_generic();

};






#endif // TRACKINGLANESDAG_GENERIC_H
