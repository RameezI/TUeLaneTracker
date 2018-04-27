////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "../local_interfaces/maps_SelectTracker.h"	// Includes the header of this component


// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSSelectTracker)
    MAPS_INPUT("ConfidenceVector",MAPS::FilterFloat32,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSSelectTracker)
    MAPS_OUTPUT("indexHighestConf",MAPS::Integer32,NULL,NULL,1)
    MAPS_OUTPUT("valueHighestConf",MAPS::Float32,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSSelectTracker)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSSelectTracker)
    //MAPS_ACTION("aName",MAPSSelectTracker::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LaneTracker) behaviour
MAPS_COMPONENT_DEFINITION(MAPSSelectTracker,"SelectTracker","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  -1, // Nb of inputs. Leave -1 to use the number of declared input definitions
			  -1, // Nb of outputs. Leave -1 to use the number of declared output definitions
			  -1, // Nb of properties. Leave -1 to use the number of declared property definitions
			  -1) // Nb of actions. Leave -1 to use the number of declared action definitions

//Initialization: Birth() will be called once at diagram execution startup.			  
void MAPSSelectTracker::Birth()
{
    m_firstTime = true;
}

void MAPSSelectTracker::Core() 
{
   ReportInfo("Passing through Core() method");

   MAPSIOElt* ioEltIn = StartReading(Input(0));
	if (ioEltIn == NULL)
		return;

    int inputvector = ioEltIn->VectorSize();


    int indexMax = 0;
    float maxConf = ioEltIn->Float32(0);

    for(int i = 1;i<inputvector;i++)
    {
        if (ioEltIn->Float32(i) > maxConf)
        {
            indexMax = i;
            maxConf = ioEltIn->Float32(i);
        }
    }


    MAPSIOElt* ioOutIndex = StartWriting(Output(0));
    ioOutIndex->Integer32(0) = indexMax;
    ioOutIndex->Timestamp() = ioEltIn->Timestamp();
    StopWriting(ioOutIndex);

    MAPSIOElt* ioOutConf = StartWriting(Output(1));
    ioOutConf->Float32(0) = maxConf;
    ioOutConf->Timestamp() = ioEltIn->Timestamp();
    StopWriting(ioOutConf);
}
//De-initialization: Death() will be called once at diagram execution shutdown.
void MAPSSelectTracker::Death()
{
    // Reports this information to the RTMaps console. You can remove this line if you know when Death() is called in the component lifecycle.
    ReportInfo("Passing through Death() method");
}