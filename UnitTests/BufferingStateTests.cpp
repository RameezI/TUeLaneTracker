#include "BufferingStateTests.h"

TEST_BufferingState testBufferingState;
	
TEST(BufferingStateTests)
	
{
	CHECK_EQUAL(185,testBufferingState.Span);
	CHECK_EQUAL(0,testBufferingState.testTypes);
	CHECK_EQUAL(0,testBufferingState.testResult_GradX);
	CHECK_EQUAL(0,testBufferingState.testResult_GradY);
	CHECK_EQUAL(0,testBufferingState.testResult_Mag);
	CHECK_EQUAL(0,testBufferingState.testResult_GradTan);
//	CHECK_EQUAL(0,testBufferingState.testResult_ProbFrame);
	
	
/*		
	//Mapping OpenCv Data to Eigen Maps
	Eigen::Map< Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > 
	FrameGray_Mapped ((float*) mFrameGRAY->data, mRES_VH(0),mRES_VH(1));
		
	Eigen::Map< Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > 
	mFrameGradMag_Mapped ((float*) mFrameGradMag->data, mRES_VH(0),mRES_VH(1));

	Eigen::Map< Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > 
	mFrameGradDir_Mapped ((float*) mFrameGradAng->data, mRES_VH(0),mRES_VH(1));	
*/

}