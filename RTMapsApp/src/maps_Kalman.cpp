////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "../local_interfaces/maps_Kalman.h"	// Includes the header of this component
#include <iostream>

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSKalman)
    MAPS_INPUT("Confidence",MAPS::FilterFloat32,MAPS::FifoReader)
    MAPS_INPUT("DirectionalParams",MAPS::FilterFloat32,MAPS::FifoReader)
    MAPS_INPUT("Velocity",MAPS::FilterFloat64,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSKalman)
    MAPS_OUTPUT("FilteredParams",MAPS::Float32,NULL,NULL,4)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSKalman)
    MAPS_PROPERTY("R_Gain",500,false,false)
    MAPS_PROPERTY("C_ExpGain",2.00,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSKalman)
    //MAPS_ACTION("aName",MAPSKalman::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LaneTracker) behaviour
MAPS_COMPONENT_DEFINITION(MAPSKalman,"ParKalman","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  -1, // Nb of inputs. Leave -1 to use the number of declared input definitions
			  -1, // Nb of outputs. Leave -1 to use the number of declared output definitions
			  -1, // Nb of properties. Leave -1 to use the number of declared property definitions
			  -1) // Nb of actions. Leave -1 to use the number of declared action definitions

//Initialization: Birth() will be called once at diagram execution startup.			  
void MAPSKalman::Birth()
{
    m_firstTime = true;

    A <<    1,     0,      0,            
            0,     1,     -0,           
           -0,    -0,      1;

    B.resize(N_STATES,2);
    B <<    1,  0,
            0,  0,
            0,  0;

    I = Matrix3f::Identity();

    P = I;
    Q <<    1,        -0.01,     0.01,
            -0.01,      2,      -1,
            0.01,      -1,       2;

    R0 <<   1,         0,         0,
            0,         2,        -1,
            0,        -1,         2;

    C = I;

    x.resize(N_STATES,1);
    x_hat.resize(N_STATES,1);
    y.resize(N_STATES,1);
    y_tilde.resize(N_STATES,1);
    u.resize(N_STATES,1);
    S.resize(N_STATES,1);
}

void MAPSKalman::Core() 
{
   MAPSIOElt* confIn = StartReading(Input(0));
	if (confIn == NULL)
    {   
        ReportInfo("No confidence input!");
		return;
    }

   MAPSIOElt* paramsIn = StartReading(Input(1));
	if (paramsIn == NULL)
    {   
        ReportInfo("No parameters input!");
		return;
    }

    //If the size is not as expected, something is wrong.
    int inputvector = paramsIn->VectorSize();
    if (inputvector != 3)
    {   
        ReportInfo("Not the correct parameters input!");
		return;
    }


   MAPSIOElt* velIn = StartReading(Input(2));
    if (velIn == NULL)
    {   
        ReportInfo("No velocity input!");
		return;
    }


    //Set measurement
    y(0) = paramsIn->Float32(0);
    y(1) = paramsIn->Float32(1);
    y(2) = paramsIn->Float32(2);

    if (m_firstTime)
    {
        m_firstTime = false;
        x = y;
    }
    
    float timestamp_cur = paramsIn->Timestamp();
    float dt = (timestamp_cur-timestamp_prev)/1e6;
    timestamp_prev = timestamp_cur;

    Adjust(velIn->Float64(0),confIn->Float32(0),dt);
    Predict();
    Update();

    //Write output
		MAPSIOElt* paramsOut = StartWriting(Output(0));
        
		for (int i=0; i<inputvector; i++) {
			paramsOut->Float32(i) = x(i);
		}
		paramsOut->VectorSize() = inputvector;
		paramsOut->Timestamp() = paramsIn->Timestamp();
		StopWriting(paramsOut);

}
//De-initialization: Death() will be called once at diagram execution shutdown.
void MAPSKalman::Death()
{
    // Reports this information to the RTMaps console. You can remove this line if you know when Death() is called in the component lifecycle.
    ReportInfo("Passing through Death() method");
}

void MAPSKalman::Adjust(float vx, float c, float dt){
    float alphaToRad = (float)1 / (float)360 * 2 * M_PI; //small angle approximation: sin(alpha) ~= alpha, >1% error at >14deg
    float horDisp = vx * dt * alphaToRad; // [M]
    A(1,0) = -horDisp * 100;// [cm] 
    A(2,0) = horDisp * 100; // [cm] 

    float c_expgain = (float)GetIntegerProperty("C_ExpGain");
    R = R0 * (int)GetIntegerProperty("R_Gain") / pow(c,c_expgain) * pow(50,c_expgain);
}

void MAPSKalman::Predict(){
    x_hat = A * x;// + B * u;
    P_hat = A * P * A.transpose() + Q;
}


void MAPSKalman::Update(){
    y_tilde = y - C * x_hat;
    S = R + C * P_hat * C.transpose();
    K = P_hat * C.transpose() * S.inverse();

    x = x_hat + K * y_tilde;

    Matrix3f L = I - K * C;
    P = L * P_hat * L.transpose() + K * R * K.transpose();
}