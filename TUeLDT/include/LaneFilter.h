#ifndef LANEFILTER_H
#define LANEFILTER_H
#include <Eigen/Dense>
#include <math.h>
#include "Lane.h"
#include "Camera.h"

using namespace std;
using namespace Eigen;
typedef Matrix<float, 7, 7> Matrix7f;

	
	class LaneFilter
	{
		
	private:  //make public for testing
			//shared_ptr<Lane>  mLane;
			const LaneProperties  mLANE;
			const double mCM_TO_PX;
			void  createPrior();
			void  createHistograms();
		

	public:
		const int 	 	mSTEP_CM;
		const int 	 	mSTEP_PX;
		const int 	 	mPX_MAX;
		const int	 	mCONF_THRESH;
		const int    	mNb_BINS;         	// number of bins in Observation Histogram.
		const int   	mNb_OFFSETS;    		// number of bins in the mFilter.
		const VectorXi  mBINS_HISTOGRAM;     // -PX_MAX:STEP: PX_MAX
		const VectorXi  mBINS_FILTER;        //  0 : STEP : PX_MAX
		
		
		
		MatrixXf     	mPrior;
		Matrix7f	 	mTransition; 
		MatrixXf 	 	mFilter;           //^TODO: Rename it as mStatesFilter or something more intutive --> probabilities of states
		 
		

		//const Ref<const MatrixXd>& getFilter();
		//shared_ptr<MatrixXd> getPrior();
		//shared_ptr<MatrixXd> getTransition(); //^TODO: Make it Private Memeber if only used by LaneFilter class.
		
		
		LaneFilter(const LaneProperties& LANE,  const double& CM_TO_PIXEL);
	   ~LaneFilter();
	};
#endif // LANEFILTER_H