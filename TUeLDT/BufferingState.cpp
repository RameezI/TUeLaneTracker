#include "BufferingState.h"

BufferingState::BufferingState()
:   
  mLikelihoods(Likelihoods(mCAMERA.RES_VH(0), mCAMERA.RES_VH(1)))
  
#ifdef DIRECTORY_INPUT
  , mCountFrame(0)
#endif

{
	#ifdef PROFILER_ENABLED
	getOpenClInfo();
	#endif	
}



/* Setting up Input Source
 * 
*/   
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



/* Setting up Root Templates
 * 
*/  
	void BufferingState::setTemplates (shared_ptr<Templates>	templates)
	{
			
		#ifdef PROFILER_ENABLED
			mProfiler.start("SetRootTemplates");
		#endif

			eigen2cv(templates->GRADIENT_DIR_ROOT, mGRADIENT_DIR_ROOT);   //^TODO: COPYING OPERATION CHANGE TO WRAPPING OR NATIVE OPENCV
			eigen2cv(templates->DEPTH_ROOT,        mDEPTH_MAP_ROOT);     //^TODO: COPYING OPERATION CHANGE TO WRAPPING OR NATIVE OPENCV
			eigen2cv(templates->FOCUS_ROOT,        mFOCUS_MASK_ROOT);   //^TODO: COPYING OPERATION CHANGE TO WRAPPING OR NATIVE OPENCV
			
			this->currentStatus= StateStatus::ACTIVE;
			
			
		 #ifdef PROFILER_ENABLED
		 mProfiler.end();
		 LOG_INFO_(LDTLog::BUFFERING_PROFILE) <<endl
								  <<"******************************"<<endl
								  <<  "Completing Buffering Setup." <<endl
								  <<  "ROOT Templates copy Time: "  << mProfiler.getAvgTime("SetRootTemplates")<<endl
								  <<"******************************"<<endl<<endl;	
								 #endif				
	}



/* Running Directed Acyclic Graph
 * 
*/  
	void BufferingState::run()
	{
		
		#ifdef PROFILER_ENABLED
			mProfiler.start("SingleRun");
		#endif	
			

		#ifdef DIRECTORY_INPUT

			if (mCountFrame < mFiles.size())
				this->StateCounter++;
			else
			{
				
				this->currentStatus = StateStatus::DONE;	
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
								  <<  "Single run-loop time: " << mProfiler.getAvgTime("SingleRun")<<endl
								  <<"******************************"<<endl<<endl;	
								 #endif
	}


	void BufferingState::conclude()
	{
		
		
		
	}

/* ********************************************* /*
/*  Low level Class PRIVATE FUnctions 
 *  getOpenClInfo()
 *  grabFrame()
 *  computeProbabilities()
 *  extractTemplates() 
 *  computeOrientedGradients()
*/

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




	 /*^TODO: Define Grabing mechanism in case of camera */
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
													  <<  "Read time: " << mProfiler.getAvgTime("ImageRead")<<endl
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
			//cv::subtract(GRAY_double_ARM, constantT, Processing_ARM);			
			//cv::exp(Processing_ARM, Processing_ARM );
			//cv::add(1, Processing_ARM, Processing_ARM);
			//cv::divide(1, Processing_ARM, Result_ARM);
	
	}



	void BufferingState::extractTemplates()
	{
		
		 int rowIndex= (mCAMERA.RES_VH(0)-mVanishingPt.V) - mCAMERA.RES_VH(0)/2;
		 int colIndex= (mCAMERA.RES_VH(1)-mVanishingPt.H) - mCAMERA.RES_VH(1)/2;

	   Rect ROI;

		ROI = Rect(colIndex, rowIndex, mCAMERA.RES_VH(1), mCAMERA.RES_VH(0) );	
		mGradDirTemplate = mGRADIENT_DIR_ROOT(ROI);

		ROI = Rect(0, rowIndex, mCAMERA.RES_VH(1), mCAMERA.RES_VH(0));
		mDepthTemplate   = mDEPTH_MAP_ROOT(ROI);
		
		
		ROI = Rect(0, rowIndex, mCAMERA.RES_VH(1), mCAMERA.RES_VH(0));	
		mFocusTemplate = mFOCUS_MASK_ROOT(ROI);


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

