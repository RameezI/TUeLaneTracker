#ifndef VANSIHINGPOINFILTER_H
#define VANSIHINGPOINFILTER_H
#include  "LaneFilter.h"
#include <memory>
#include <Eigen/Dense>
#include <math.h>
#include "Lane.h"
#include "Camera.h"

using namespace std;
using namespace Eigen;
class VanishingPtFilter
{

	
public:

	template <typename Derived>
	VanishingPtFilter(const MatrixBase<Derived>&  , const CameraProperties& );
	~VanishingPtFilter();
	
	

private: 
		//const VectorXi  mBINS_LANE_HISTOGRAM;
		const int 		mLANE_FILTER_OFFSET_V;   //CONSTANT: might be need outside ----> make public
		const int  	 	mVP_FILTER_OFFSET_V;       //CONSTANT: might be needed outside ---> make public
		const int    	mFRAME_CENTER_V;
		const int    	mFRAME_CENTER_H;
		const float     mVP_LANE_RATIO;
		void  createPrior();
	

public:

	const int 	 	mRANGE_H;
	const int 	 	mRANGE_V;
	const int    	mSTEP;
	const int    	mNb_BINS_H;    	// number of bins in the horizental direction.
	const int    	mNb_BINS_V;    	// number of bins in the  vertical   direction.
    const VectorXi 	mBINS_H;     	// Histogram Bins in Pixels.
	const VectorXi 	mBINS_V;	    //Histogram Bins in Pixels.
	const VectorXi 	mBINS_HISTOGRAM;	    //Histogram Bins in Pixels.
    

	
	MatrixXd     mFilter;
	//Matrix7d	 mTransition;  
	

	//const Ref<const MatrixXd>& getFilter();
	//shared_ptr<MatrixXd> getPrior();
	//shared_ptr<MatrixXd> getTransition(); //^TODO: Make it Private Memeber if only used by LaneFilter class.
	
	
	//LaneFilter(const shared_ptr<const Lane>, const shared_ptr<const Camera>);
	 

};

#endif // VANSIHINGPOINFILTER_H
