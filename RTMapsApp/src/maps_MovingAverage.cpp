////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "../local_interfaces/maps_MovingAverage.h"	// Includes the header of this component
#include <iostream>

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSMovingAverage)
    MAPS_INPUT("Confidence",MAPS::FilterFloat32,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSMovingAverage)
    MAPS_OUTPUT("ConfidenceAverage",MAPS::Float32,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSMovingAverage)
    MAPS_PROPERTY("NrAveragePoints",2,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSMovingAverage)
    //MAPS_ACTION("aName",MAPSMovingAverage::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LaneTracker) behaviour
MAPS_COMPONENT_DEFINITION(MAPSMovingAverage,"MovingAverage","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  -1, // Nb of inputs. Leave -1 to use the number of declared input definitions
			  -1, // Nb of outputs. Leave -1 to use the number of declared output definitions
			  -1, // Nb of properties. Leave -1 to use the number of declared property definitions
			  -1) // Nb of actions. Leave -1 to use the number of declared action definitions

//Initialization: Birth() will be called once at diagram execution startup.			  
void MAPSMovingAverage::Birth()
{
    m_firstTime = true;
    pSMA = new SMA((int)GetIntegerProperty("NrAveragePoints"));
    average = 0;
}

void MAPSMovingAverage::Core() 
{
    MAPSIOElt* confIn = StartReading(Input(0));
	if (confIn == NULL)
    {   
        ReportInfo("No confidence input!");
		return;
    }

    pSMA->add(confIn->Float32());

    MAPSIOElt* confOut = StartWriting(Output(0));
    confOut->Float32() = pSMA->avg();
    confOut->VectorSize() = 1;
    confOut->Timestamp() = confIn->Timestamp();
    StopWriting(confOut);
}
//De-initialization: Death() will be called once at diagram execution shutdown.
void MAPSMovingAverage::Death()
{
    // Reports this information to the RTMaps console. You can remove this line if you know when Death() is called in the component lifecycle.
    ReportInfo("Passing through Death() method");
}
