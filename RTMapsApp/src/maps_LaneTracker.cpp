////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "../local_interfaces/maps_LaneTracker.h"	// Includes the header of this component

#define IMAGEOUTPUT

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLaneTracker)
    MAPS_INPUT("matSrcIn",MAPS::FilterIplImage,MAPS::FifoReader)
	//MAPS_INPUT("cameraconfigfile",MAPS::FilterTextUTF16, MAPS::FifoReader)
    //MAPS_INPUT("iName",MAPS::FilterInteger32,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLaneTracker)
    MAPS_OUTPUT("cycleCount",MAPS::Integer32,NULL,NULL,1)
	#ifdef IMAGEOUTPUT
    	MAPS_OUTPUT("videoOverlay",MAPS::IplImage,NULL,NULL,0)
    	MAPS_OUTPUT("topDown",MAPS::IplImage,NULL,NULL,0)
		MAPS_OUTPUT("directionalParams",MAPS::Float32,NULL,NULL,3)
	#endif
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLaneTracker)
    MAPS_PROPERTY("AVG_WIDTH",300,false,false)
    MAPS_PROPERTY("STD_WIDTH",50,false,false)
    MAPS_PROPERTY("MIN_WIDTH",250,false,false)
    MAPS_PROPERTY("MAX_WIDTH",500,false,false)
    MAPS_PROPERTY("AVG_WIDTH_LM",30,false,false)
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
			  -1, // Nb of properties. Leave -1 to use the number of declared property definitions
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


	lPtrFeeder = createFrameFeeder();
	if(lPtrFeeder == nullptr)
	  lReturn = -1;

	if (lReturn==0)
	{
		//lPtrFeeder->setImageLinker(lImgPtr);
		std::cout<<endl<<endl;
		std::cout<<"******************************"<<std::endl;
		std::cout<<" Press Ctrl + c to terminate."<<std::endl;
		std::cout<<"******************************"<<std::endl;
		
		//^TODO: Replace by State.h member variable.
		nbCycles = 0;

		try
		{
            lPtrStateMachine = new StateMachine(move(lPtrFeeder));

			int AVG_WIDTH = (int)GetIntegerProperty("AVG_WIDTH");
			int STD_WIDTH = (int)GetIntegerProperty("STD_WIDTH");
			int MIN_WIDTH = (int)GetIntegerProperty("MIN_WIDTH");
			int MAX_WIDTH = (int)GetIntegerProperty("MAX_WIDTH");
			int AVG_WIDTH_LM = (int)GetIntegerProperty("AVG_WIDTH_LM");

			lPtrStateMachine->setLaneParameters({AVG_WIDTH,STD_WIDTH,MIN_WIDTH,MAX_WIDTH,AVG_WIDTH_LM});
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
	else
	{
		std::cout << "Failed creating the FrameFeeder" << endl;
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

	lPtrStateMachine->forwardImage(tempImg);

	nbCycles++;

    //ReportInfo("Passing through Core() method");
	//Same as multiplier3 (manual allocation of the output buffers).
	if (m_firstTime) {
		std::cout<<"Setting first time settings"<<endl;
		m_firstTime = false;
		//Example of how to test the incoming images color format,
		//and abort if the input images color format is not supported.
		#ifdef IMAGEOUTPUT
		if (MAPS_FC_STR(imgIn.channelSeq) != MAPS_CHANNELSEQ_GRAY &&
			MAPS_FC_STR(imgIn.channelSeq) != MAPS_CHANNELSEQ_BGR &&
			MAPS_FC_STR(imgIn.channelSeq) != MAPS_CHANNELSEQ_RGB)
			Error("Unsupported image format.");

		Output(1).AllocOutputBufferIplImage(imgIn);
		Output(2).AllocOutputBufferIplImage(imgIn);
		#endif
	}


// --------------------------------------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------------------------------------
	//std::cout<<"Going for a spin"<<endl;

	if (lPtrStateMachine->getCurrentState() != States::DISPOSED)
	{
		lReturn = lPtrStateMachine->spin();
		//nbCycles ++;
		
		if(lPreviousState != lPtrStateMachine->getCurrentState())
		{
			cout<<endl;	
			cout<<lPtrStateMachine->getCurrentState();
			lPreviousState = lPtrStateMachine->getCurrentState();

			std::cout.flush();
		}
		else if (nbCycles%100==0)
		{
			cout <<endl;
			cout <<lPtrStateMachine->getCurrentState();
			cout <<"state cycle-count = " << nbCycles;
		}
	}

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
	if (lPtrStateMachine->getCurrentState() == DETECTING_LANES && nbCycles > 6)
	{
		//Video overlay
		MAPSIOElt* ioEltOut2 = StartWriting(Output(1));
		ioEltOut2->IplImage() = lPtrStateMachine->getCurrentFrame();
		ioEltOut2->Timestamp() = ioEltIn->Timestamp();
		StopWriting(ioEltOut2);

		//Top Down View
		MAPSIOElt* ioEltOut3 = StartWriting(Output(2));
		ioEltOut3->IplImage() = lPtrStateMachine->getTopDownFrame();
		ioEltOut3->Timestamp() = ioEltIn->Timestamp();
		StopWriting(ioEltOut3);

		//Dir Params
		MAPSIOElt* ioEltOut4 = StartWriting(Output(3));
		vector<float> dirParams = lPtrStateMachine->getDirectionalParams();
		int vectorsize = dirParams.size();
		for (int i=0; i<vectorsize; i++) {
			ioEltOut4->Float32(i) = dirParams[i];
		}
		ioEltOut4->VectorSize() = vectorsize;
		ioEltOut4->Timestamp() = ioEltIn->Timestamp();
		StopWriting(ioEltOut4);
	}
#endif

}

//De-initialization: Death() will be called once at diagram execution shutdown.
void MAPSLaneTracker::Death()
{
    // Reports this information to the RTMaps console. You can remove this line if you know when Death() is called in the component lifecycle.
    ReportInfo("Passing through Death() method");
}


unique_ptr<FrameFeeder> MAPSLaneTracker::createFrameFeeder()
{

	unique_ptr<FrameFeeder>	lPtrFeeder;

	/** Create Image Feeder */
	try
	{
		std::cout << "Try creating FrameFeeder" << endl;
	   lPtrFeeder=  unique_ptr<FrameFeeder>( new RTMapsFeeder("") );
	   //lPtrFeeder->setImageLinker(lImgPtr);

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
		std::cout << "Creating frame feeder failed" << endl;
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
		std::cout << "Creating frame feeder failed" << endl;
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