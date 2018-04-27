////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "../local_interfaces/maps_ParamViewer.h"	// Includes the header of this component
#include <iostream>


// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSParamViewer)
    MAPS_INPUT("DirectionalParameters",MAPS::FilterFloat32,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSParamViewer)
    	MAPS_OUTPUT("topDown",MAPS::IplImage,NULL,NULL,0)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSParamViewer)
    MAPS_PROPERTY("ResH",960,false,false)
    MAPS_PROPERTY("ResV",600,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSParamViewer)
    //MAPS_ACTION("aName",MAPSParamViewer::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LaneTracker) behaviour
MAPS_COMPONENT_DEFINITION(MAPSParamViewer,"ParamViewer","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  -1, // Nb of inputs. Leave -1 to use the number of declared input definitions
			  -1, // Nb of outputs. Leave -1 to use the number of declared output definitions
			  -1, // Nb of properties. Leave -1 to use the number of declared property definitions
			  -1) // Nb of actions. Leave -1 to use the number of declared action definitions

//Initialization: Birth() will be called once at diagram execution startup.			  
void MAPSParamViewer::Birth()
{
    m_firstTime = true;
    ResH = (int)GetIntegerProperty("ResH");
    ResV = (int)GetIntegerProperty("ResV");

}

void MAPSParamViewer::Core() 
{
   //ReportInfo("Passing through Core() method");

   MAPSIOElt* paramsIn = StartReading(Input(0));
	if (paramsIn == NULL)
    {   
        ReportInfo("No Parameter input!");
		return;
    }

    if (m_firstTime)
    {
        m_firstTime = false;
        Output(0).AllocOutputBuffer(ResH*ResV);
    }

    std::vector<float> dirParams;
    dirParams.push_back(paramsIn->Float32(0));
    dirParams.push_back(paramsIn->Float32(1));
    dirParams.push_back(paramsIn->Float32(2));

    std::cout<<"Params: \t" << dirParams[0] << "\t"<< dirParams[1] << "\t"<< dirParams[2] << endl;

    //Top Down View
		MAPSIOElt* frameOut = StartWriting(Output(0));
		frameOut->IplImage() = getLaneFrame(dirParams);
		frameOut->Timestamp() = paramsIn->Timestamp();
		StopWriting(frameOut);
   

}
//De-initialization: Death() will be called once at diagram execution shutdown.
void MAPSParamViewer::Death()
{
    // Reports this information to the RTMaps console. You can remove this line if you know when Death() is called in the component lifecycle.
    ReportInfo("Passing through Death() method");
}



cv::Mat MAPSParamViewer::getLaneFrame(vector<float> dirParams)
{
  cv::Mat mLaneFrame(ResV,ResH,CV_8UC3,cv::Scalar(0,0,0));

  float widthR = 600;
  float cstart, cend;

  cstart = ResH / 2;
  cend = cstart + ResV * tan(dirParams[0]*M_PI/180);

  int pxl = cstart + dirParams[1]*ResH/widthR;
  int pxr = cstart - dirParams[2]*ResH/widthR;

	line(mLaneFrame, cvPoint(pxl, 0), cvPoint(pxl,ResV), cvScalar(0,255,0), 3);
  line(mLaneFrame, cvPoint(pxr, 0), cvPoint(pxr, ResV), cvScalar(0,255,0), 3);
  line(mLaneFrame, cvPoint((pxr+pxl)/2, 0), cvPoint((pxr+pxl)/2, ResV), cvScalar(255,0,0), 3);
  line(mLaneFrame, cvPoint(cstart,ResV), cvPoint(cend, 0), cvScalar(0,0,255), 5);

  //std::cout<<"Lane Frame"<<endl;
  return mLaneFrame;
}