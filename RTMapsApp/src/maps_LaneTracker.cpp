////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "../local_interfaces/maps_LaneTracker.h"	// Includes the header of this component

//#define IMAGEOUTPUT

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLaneTracker)
    MAPS_INPUT("matSrcIn",MAPS::FilterIplImage,MAPS::FifoReader)
    //MAPS_INPUT("iName",MAPS::FilterInteger32,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLaneTracker)
    MAPS_OUTPUT("cycleCount",MAPS::Integer32,NULL,NULL,1)
	#ifdef IMAGEOUTPUT

    MAPS_OUTPUT("iplimage",MAPS::IplImage,NULL,NULL,0)
	#endif
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLaneTracker)
    //MAPS_PROPERTY("pName",128,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLaneTracker)
    //MAPS_ACTION("aName",MAPSLaneTracker::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LaneTracker) behaviour
MAPS_COMPONENT_DEFINITION(MAPSLaneTracker,"LaneTracker","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  -1, // Nb of inputs. Leave -1 to use the number of declared input definitions
			  -1, // Nb of outputs. Leave -1 to use the number of declared output definitions
			  0, // Nb of properties. Leave -1 to use the number of declared property definitions
			  0) // Nb of actions. Leave -1 to use the number of declared action definitions

//Initialization: Birth() will be called once at diagram execution startup.			  
void MAPSLaneTracker::Birth()
{
        // Reports this information to the RTMaps console. You can remove this line if you know when Birth() is called in the component lifecycle.
    ReportInfo("Passing through Birth() method");

    	//Alloctation of the _firstTime member variable (see the .h file)
	m_firstTime = true;


    lFrameSource = RTMAPS;
    lSourceStr = "";
    nbCycles = 0;
    lReturn = 0;


	lPtrFeeder = createFrameFeeder(lFrameSource, lSourceStr);
	if(lPtrFeeder == nullptr)
	  lReturn = -1;

	if (lReturn==0)
	{

		std::cout<<endl<<endl;
		std::cout<<"******************************"<<std::endl;
		std::cout<<" Press Ctrl + c to terminate."<<std::endl;
		std::cout<<"******************************"<<std::endl;
		
		//^TODO: Replace by State.h member variable.
		nbCycles = 0;

		try
		{
            lPtrStateMachine = new StateMachine(move(lPtrFeeder));
		    cout<<lPtrStateMachine->getCurrentState();
	 	    lPreviousState = lPtrStateMachine->getCurrentState();
		}

		catch(const char* msg)
		{	
	   	   #ifdef PROFILER_ENABLED
		    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
		    <<"******************************"<<endl
		    << "State-Machine Failure."
		    << msg <<endl
		    <<"******************************"<<endl<<endl;
		   #endif

		   lReturn = -1;
		}
		catch(...)
		{
	   	   #ifdef PROFILER_ENABLED
	    	    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	    	    <<"******************************"<<endl
	    	    <<  "State-Machine Failure. "<<endl
	    	    <<  "Unknown Exception!"<<endl
	    	    <<"******************************"<<endl<<endl;
	  	    #endif
	   	    lReturn = -1; 
		}

	
	}
}

void MAPSLaneTracker::Core() 
{
    MAPSIOElt* ioEltIn = StartReading(Input(0));
	if (ioEltIn == NULL)
		return;

	//ReportInfo("Read Input Image...");
    IplImage& imgIn = ioEltIn->IplImage();
	cv::Mat tempImg = cv::cvarrToMat(&imgIn);

	imshow("input",tempImg);
	//std::cout<<"Setting image"<<endl;
	lImgPtr = &tempImg;

	if (lImgPtr == NULL){
		std::cout<<"NULL POINTER!"<<endl;
		return;
	}

	if (lImgPtr->empty()){
		std::cout << "Empty input!" << endl;
		ReportInfo("Empty!");
	}


    //ReportInfo("Passing through Core() method");
	//Same as multiplier3 (manual allocation of the output buffers).
	if (m_firstTime) {
		m_firstTime = false;
		//Example of how to test the incoming images color format,
		//and abort if the input images color format is not supported.
		if (MAPS_FC_STR(imgIn.channelSeq) != MAPS_CHANNELSEQ_GRAY &&
			MAPS_FC_STR(imgIn.channelSeq) != MAPS_CHANNELSEQ_BGR &&
			MAPS_FC_STR(imgIn.channelSeq) != MAPS_CHANNELSEQ_RGB)
			//The Error method will throw an exception, which will be caught by the RTMaps
			//engine outside of the Core() function, so that execution of this component
			//will stop and the Death function will be called immediately.
			//The error message is displayed in red in the console as it is with the ReportError method.
			//(All the other components in the diagram keep-on working).
			Error("Unsupported image format.");

		Output(1).AllocOutputBufferIplImage(imgIn);
	}

nbCycles++;
std::cout << nbCycles << endl;
// --------------------------------------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------------------------------------
	
	
	//std::cout<<"Start Writing number..."<<endl;

	MAPSIOElt* ioEltOut = StartWriting(Output(0));
	MAPSInt32* dstPtrCnt = (MAPSInt32*) ioEltOut->Data();
	*dstPtrCnt = nbCycles;
	//std::cout<<"Set Timestamp..."<<endl;
	ioEltOut->Timestamp() = ioEltIn->Timestamp();
	StopWriting(ioEltOut);



#ifdef IMAGEOUTPUT
	// IplImage * imgSrc = &imgIn;
	// std::cout<<"Create empty image..."<<endl;
	// IplImage* imgScribble = cvCreateImage(cvSize(320, 240), 8, 3);

	

	//std::cout<<"Start Writing..."<<endl;
	MAPSIOElt* ioEltOut2 = StartWriting(Output(1));

	//std::cout<<"Linking To Output..."<<endl;
	IplImage& imageOut = ioEltOut2->IplImage();

	//std::cout<<"Setting output image value..."<<endl;


	IplImage * imgDst = &imageOut;

	// int depthdiff = imgSrc->depth - imgDst->depth;
	// int widthdiff = imgSrc->width - imgDst->width;
	// int heightdiff = imgSrc->height - imgSrc->height;

	//cv::Mat frame = lPtrStateMachine->getFrame();
	//IplImage imgTmp = frame;
	//imgDst = cvCloneImage(&imgTmp);

	//std::cout<<"Src Depth: " << imgSrc->depth << " Width: " << imgSrc->width << " Height: " << imgSrc->height << endl;
	//std::cout<<"Dst Depth: " << imgDst->depth << " Width: " << imgDst->width << " Height: " << imgDst->height << endl;
	cvCopy(imgSrc,imgDst);
	//imageOut = cvCloneImage(&imgIn);

	//imageOut = *imgScribble;

	// std::cout<<"Setting Dst Ptr..."<<endl;
	// IplImage* ptrDstIm = &imageOut;

	// std::cout<<"Setting Dst Value..."<<endl;
	// *ptrDstIm = *imgScribble; //This is the error


	//IplImage* iplImg = new IplImage(lPtrStateMachine->getFrame());
	//IplImage* iplImg = new IplImage(*lImgPtr);

	//dstPtrIm = *imgScribble;
	//ioEltOut2->Timestamp() = ioEltIn->Timestamp();
	//std::cout<<"Stop Writing..."<<endl;
	StopWriting(ioEltOut2);
#endif
}

//De-initialization: Death() will be called once at diagram execution shutdown.
void MAPSLaneTracker::Death()
{
    // Reports this information to the RTMaps console. You can remove this line if you know when Death() is called in the component lifecycle.
    ReportInfo("Passing through Death() method");
}


unique_ptr<FrameFeeder> MAPSLaneTracker::createFrameFeeder(FrameSource srcMode, string srcString)
{

	unique_ptr<FrameFeeder>	lPtrFeeder;

	cout<<srcString<<endl;

	/** Create Image Feeder */
	try
	{
	   lPtrFeeder=  unique_ptr<FrameFeeder>( new ImgStoreFeeder(srcString) );

	   #ifdef PROFILER_ENABLED
	      LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	      <<"******************************"<<endl
	      << "Frame Input Mode: "<<srcMode<<endl
	      << "Source String: "<<srcString<<endl
	      <<"******************************"<<endl<<endl;
	   #endif
	}
	catch(const char* msg)
	{
	   #ifdef PROFILER_ENABLED
	    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	    <<"******************************"<<endl
	    << msg <<endl
	    <<"******************************"<<endl<<endl;
	   #endif
	   lPtrFeeder = nullptr; 
	}
	catch (...)
	{
	   #ifdef PROFILER_ENABLED
	    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	    <<"******************************"<<endl
	    <<  "FrameFeeder Instantiation Failed. "<<endl
	    <<  "Unknown Exception!"<<endl
	    <<"******************************"<<endl<<endl;
	   #endif
	   lPtrFeeder = nullptr; 
	}

	return lPtrFeeder;

}