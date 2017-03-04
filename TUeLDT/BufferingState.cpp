#include "BufferingState.h"

BufferingState::BufferingState(const CameraProperties& CAMERA, const LaneMembership& MEMBERSHIP)
: mRES_VH(CAMERA.RES_VH),

  mLaneMembership(MEMBERSHIP),
  
  mGradientDirTemplate(Mat::zeros(mRES_VH(0), mRES_VH(1), CV_16SC1)),

  mDepthMapTemplate(mRES_VH(0), mRES_VH(1), CV_8UC1),

  mFrameRGB(mRES_VH(0), mRES_VH(1), CV_8UC1),
  
  mFrameGRAY(mRES_VH(0), mRES_VH(1), CV_8UC1),
    
  mFrameGradMag(mRES_VH(0), mRES_VH(1), CV_8UC1),
  
  mFrameGradAng(mRES_VH(0), mRES_VH(1), CV_16SC1)
  
#ifdef DIRECTORY_INPUT
  , mCountFrame(0)
#endif

{
	#ifdef PROFILER_ENABLED
	getOpenClInfo();
	#endif	
}


#ifdef DIRECTORY_INPUT
void BufferingState::setSource(const vector<cv::String>& files)
{
	mFiles = files;
}
#else

void BufferingState::setSource()
{



	
}
	
#endif


void BufferingState::injectDependencies (shared_ptr<VanishingPt>  vanishingPt,
										 shared_ptr<Templates>	  templates,	
										 shared_ptr<Masks>	  	  masks,
										 shared_ptr<Likelihoods>  likelihoods)
{
	mVanishingPt = vanishingPt;
	mTemplates   = templates;
    mMasks       = masks;
	mLikelihoods = likelihoods;
	
	mStateStatus= StateStatus::ACTIVE;
}



void BufferingState::run()
{
	
#ifdef PROFILER_ENABLED
	mProfiler.start("SingleRun");
#endif	
	

#ifdef DIRECTORY_INPUT

	if (mCountFrame < mFiles.size())
		sStateCounter++;
	else
	{
		mStateStatus = StateStatus::DONE;	
		return;
	} 

#else 
	//^TODO: Check for camera errors or signals to finalise this state
	mStateStatus = StateStatus::DONE;
	sStateCounter++;
	
#endif





if (0==grabFrame())
	executeDAG_buffering();
else
	cerr << "Problem loading image!!!" << endl;
												

 #ifdef PROFILER_ENABLED
 mProfiler.end();
 LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
						  <<"******************************"<<endl
						  <<  "Completing a run loop." <<endl
						  <<  "Single run-loop time: " << mProfiler.getTiming("SingleRun")<<endl
						  <<"******************************"<<endl<<endl;	
						 #endif
}




void BufferingState::conclude()
{
	
	
	
}


/*  Low level Class PRIVATE FUnctions */

#ifdef PROFILER_ENABLED
void BufferingState::getOpenClInfo()
{
	
	if (!cv::ocl::haveOpenCL())
	{
		 LOG_INFO_(LDTLog::BUFFERING_LOG) <<endl
						  <<"******************************"<<endl
						  <<  "OpenCL Info:" <<endl
						  <<  "Sorry, OpenCl is not available. " <<endl
						  <<"******************************"<<endl<<endl;	
		//return;
	}

	cv::ocl::Context context;
	if (!context.create(cv::ocl::Device::TYPE_GPU))
	{
		LOG_INFO_(LDTLog::BUFFERING_LOG) <<endl
						  <<"******************************"<<endl
						  <<  "OpenCL Info:" <<endl
						  <<  "Sorry, Failed creating the Context. " <<endl
						  <<"******************************"<<endl<<endl;	
		//return;
	}

	else
	{
	
	
		for (int i = 0; i < context.ndevices(); i++)
		{
			cv::ocl::Device device = context.device(i);

					std::string name= device.name();
					std::string OpenCL_C_Version= device.OpenCL_C_Version();

			
					LOG_INFO_(LDTLog::BUFFERING_LOG) <<endl
						  <<"******************************"<<endl
						  <<  "OpenCL Device Detected:" <<endl
						  <<  "name:  " << name<< endl
						  <<  "available:  " << device.available()<< endl
						  <<  "imageSupport:  " << device.imageSupport()<< endl
						  <<  "globalMemSize:  " << device.globalMemSize()<< endl
						  <<  "localMemSize:  " << device.localMemSize()<< endl
						  <<  "maxWorkGroup:  " << device.maxWorkGroupSize()<< endl
						  <<  "maxWorkItemDim:  " << device.maxWorkItemDims()<< endl
						  <<  "maxComputeUnits:  " << device.maxComputeUnits()<< endl
						  <<  "preferredVectorWidthChar:  " << device.preferredVectorWidthChar()<< endl
						  <<  "preferredVectorWidthDouble:  " << device.preferredVectorWidthDouble()<< endl
						  <<  "preferredVectorWidthFloat:  " << device.preferredVectorWidthFloat()<< endl
						  <<  "preferredVectorWidthHalf:  " << device.preferredVectorWidthHalf()<< endl
						  <<  "preferredVectorWidthLong:  " << device.preferredVectorWidthLong()<< endl
						  <<  "preferredVectorWidthShort:  " << device.preferredVectorWidthShort()<< endl
						  <<  "image2DMaxHeight:  " << device.image2DMaxHeight()<< endl
						  <<  "image2DMaxWidth:  " << device.image2DMaxWidth()<< endl
						  <<  "OpenCL_C_Version:  " << OpenCL_C_Version<< endl
						  <<"******************************"<<endl<<endl;
		}
	
	}

}
#endif





int BufferingState::grabFrame()
{
	
#ifdef DIRECTORY_INPUT 
	#ifdef PROFILER_ENABLED
	mProfiler.start("ImageRead");
	#endif 

		mFrameRGB = imread(mFiles[mCountFrame]);	
	 
	#ifdef PROFILER_ENABLED
	 mProfiler.end();
		const std::string str = mFiles[mCountFrame];	
		LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
											  <<"******************************"<<endl
											  <<  "Reading frame from directory." <<endl <<str<<endl
											  <<  "Read time: " << mProfiler.getTiming("ImageRead")<<endl
											  <<"******************************"<<endl<<endl;
											  #endif
		if(!mFrameRGB.data)
				return -1;
				
		else	mCountFrame ++;
				return 0;

#else

	
			
#endif

}


void BufferingState::computeProbabilities()
{

#ifdef PROFILER_ENABLED
mProfiler.start("MapCVtoEigen");
#endif 	
/*
	//Mapping OpenCv Data to Eigen Maps
	Eigen::Map< Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > 
	FrameGray_Mapped ((float*) mFrameGRAY->data, mRES_VH(0),mRES_VH(1));
		
	Eigen::Map< Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > 
	mFrameGradMag_Mapped ((float*) mFrameGradMag->data, mRES_VH(0),mRES_VH(1));

	Eigen::Map< Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > 
	mFrameGradDir_Mapped ((float*) mFrameGradAng->data, mRES_VH(0),mRES_VH(1));	
*/	
	
#ifdef PROFILER_ENABLED
mProfiler.end();
LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
										  <<"******************************"<<endl
										  <<  "Mapping OpenCv Data to Eigen using Eigen Map" <<endl
										  <<  "Time: " << mProfiler.getTiming("MapCVtoEigen")<<endl
										  <<"******************************"<<endl<<endl;	
									 #endif		
	
	
	//mProbMap_Gray = mOnes.cast<float>() * 0.1;
	
	//mProbMap_GradMag
	//mProbMap_GradDir
	
	
	//VectorXf bins_cm = mBINS_FILTER.cast<float>()*(1/mCM_TO_PX);
	
	
    //GRAY_P = 1./(1 + exp(-NGRAY(1)*(I-NGRAY(2))));
    
    
    //MAG_P = 1./(1 + exp(-NMAG(1)*(MAG-NMAG(2))));
     
     
    //DIR_P = 1./(1 + exp(-NDIR(1)*(abs(DIR-DIR_TEMPLATE)-NDIR(2))));
  
    
    //PROB         = GRAY_P .* MAG_P .* DIR_P;         %% prob with dir	
	
	
	
//FrameGray_Mapped.block(200,0, 200,200)= MatrixXf::Constant(200,200,0);
	
	
}



void BufferingState::extractTemplates()
{
	
	/*Block Opertaions Explaination: block(rowIndex, colIndex, NbRows, NbCols)	*/

	 int rowIndex= (mRES_VH(0)-mVanishingPt->V) - mRES_VH(0)/2;
	 int colIndex= (mRES_VH(1)-mVanishingPt->H) - mRES_VH(1)/2;
	
	mMasks->FOCUS			 	= mTemplates->FOCUS_ROOT.block(rowIndex, 0 , mRES_VH(0) , mRES_VH(1)); 

	mTemplates->DEPTH 		 	= mTemplates->DEPTH_ROOT.block(rowIndex, 0 , mRES_VH(0) , mRES_VH(1));

	mTemplates->GRADIENT_DIR 	= mTemplates->GRADIENT_DIR_ROOT.block(rowIndex, colIndex, mRES_VH(0) , mRES_VH(1));
	
	
	
	/* Using OpenCv ROI to achieve the same. ^TODO: Time and keep one that has lower latency /*
	 * Latency of Eigen Operation on Intelx86: 10ms
	 * Latency of OpenCv ROI on Intelx86: 
	 * Note: When transferring Image to GPU complete Image is tranffered not Just ROI
	 * 
	*/
	//Mat GradientDirRoot, FocusMaskRoot, DepthMapRoot;
  
	//eigen2cv(mTemplates->GRADIENT_DIR_ROOT, GradientDirRoot); //^TODO: COPYING OPERATION CHANGE TO WRAPPING OR NATIVE OPENCV
	//eigen2cv(mTemplates->DEPTH_ROOT,        DepthMapRoot);       //^TODO: COPYING OPERATION CHANGE TO WRAPPING OR NATIVE OPENCV
	//eigen2cv(mTemplates->FOCUS_ROOT,        FocusMaskRoot);   //^TODO: COPYING OPERATION CHANGE TO WRAPPING OR NATIVE OPENCV


/*Rectangle Opertaions Explaination: Rect(x, y, width, height)	*/

	 //Rect ROI = Rect(0, rowIndex, mRES_VH(1) , mRES_VH(0));
	 //mDepthMapTemplate = DepthMapRoot(ROI);
	
	//ROI = Rect(colIndex, rowIndex, mRES_VH(1), mRES_VH(0) );	
    //mGradientDirTemplate = GradientDirRoot(ROI);
	
	//ROI = Rect(colIndex, rowIndex, mRES_VH(1), mRES_VH(0) );	
    //mGradientDirTemplate = GradientDirRoot(ROI);


    //UMat a,b;
   	//a= mGradientDirTemplate.getUMat( ACCESS_READ);
	//b= mGradientDirTemplate.getUMat( ACCESS_READ);
}


void BufferingState::computeOrientedGradients()
{	
	int scale = 1;
	int delta = 0;
	int ddepth = CV_64F;
	
	Mat grad_x, grad_y;
	
	Sobel( mFrameGRAY, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_REPLICATE );
	Sobel( mFrameGRAY, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_REPLICATE );
	cv::magnitude(grad_x,grad_y, mFrameGradMag);
	cv::phase(grad_x, grad_y, mFrameGradAng);
	
}

BufferingState::~BufferingState()
{
	
	
}

