#ifndef TRACKINGLANESDAG_GENERIC_H
#define TRACKINGLANESDAG_GENERIC_H
#include "State.h"
#include "BufferingDAG_generic.h"


using namespace Eigen;

class TrackingLanesDAG_generic: public BufferingDAG_generic
{
	
friend class TrackingLaneState;
friend class TEST_TrackingState;
	
private:

	bool 				    mFiltersReady;
    bool                    mStartFiltering;
	
	const int				mMAX_PIXELS_ROI;

	int  					mLOWER_LIMIT_IntBase;
	int 					mUPPER_LIMIT_IntBase;
	
	int  					mLOWER_LIMIT_IntPurview;
	int 					mUPPER_LIMIT_IntPurview;
	
	int						mSCALED_STEP_LANE_FILTER;
	int 					mSCALED_STEP_VP_FILTER;
	int 					mSCALED_START_LANE_FILTER;
	int 					mSCALED_START_VP_FILTER;
	
	
	int   					mLikelihoodLeftBoundary;
	int 					mLikelihoodRightBoundary;
	float                   mLikelihoodNegBoundary;
	int                     mPosteriorProbBase;
	
	
	int   					mLikelihoodVP_LBoundary;
	int   					mLikelihoodVP_RBoundary;
	float                   mLikelihoodVP_NegBoundary;
    float					mLikelihoodVP_Width;
	int                     mPosteriorProbVP;
	
	//Caclutaions of Weighted Center Lane
	//int 					mCarWidth_pixels;
	//float					mCenterOffset;
	
	
	LaneFilter* 			mLaneFilter;
	VanishingPtFilter*		mVpFilter;
	
	
	Mat 					mX_VPRS_SCALED;	
	Mat 					mIntBase;           
	Mat						mIntPurview;
	Mat 					mIntWeights;
	
	vector<uint16_t>		mBaseBinIdx; 	  //Only valid list of Intersections IDs
	vector<uint16_t>		mPurviewBinIdx;  //Only valid list of Intersections IDs
	vector<int32_t>	        mWeightBin;     //Only valid list of Weights IDs
	
	Mat						mHistBase;
	Mat						mHistPurview;
	
	Mat						mProbMapFocussed;
	Mat 					mGradTanFocussed;
	
	Mat 					mTransitLaneFilter;
	Mat 					mTransitVpFilter;

	LaneModel               mLaneModel;
	
	
public:
	TrackingLanesDAG_generic(BufferingDAG_generic&& bufferingGraph);
	void auxillaryTasks();
	void extractLanes();
	void extractControllerInputs();
	~TrackingLanesDAG_generic();

};






#endif // TRACKINGLANESDAG_GENERIC_H
