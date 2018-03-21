/** \file */ 
#include "SigInit.h"
#include "FrameFeeder.h"
#include "StateMachine.h"
#include "boost/program_options.hpp"

using namespace std;
namespace po = boost::program_options;


//Fucntion definitions
unique_ptr<FrameFeeder> createFrameFeeder(FrameSource srcMode, string srcString);


int main(int argc, char* argv[]) /**  
	This is the entry point of the application.
	- Initialises the sigInit handler
	- Creates a stateMachine and spins it until user issues a quit signal through the sigInit handler. 		
	*/
{
	int lReturn 	= 0;

	FrameSource 		 lFrameSource;
	std::string 		 lSourceStr;
	
	po::options_description	lDesc("Options");

	lDesc.add_options()
	("help,h", 
			"\t produces help message")

	("Mode,m", 
			po::value<FrameSource>(&lFrameSource)->default_value(FrameSource::DIRECTORY),
			"\t selects frame input mode")

	("Source,s",
			po::value<string>(&lSourceStr)->default_value("../DataSet"),
			"\t provides source configuration");

	po::variables_map vMap;
	po::store(po::parse_command_line(argc, argv, lDesc), vMap);
	po::notify(vMap);

	if ( vMap.count("help") )
	{
 	  cout << lDesc <<endl;
	  cout << "	Valid arguments for 'Mode': ";
	  cout << "["<<FrameSource::DIRECTORY<<" ";
	  cout <<FrameSource::STREAM<<" ";
	  cout <<FrameSource::GMSL<<"]";

	  cout <<endl<<endl<< "	Examples:"<<endl;
	  cout<< "	./TUeLaneTracker -m " << FrameSource::DIRECTORY << " -s " << "/home/DataSet" <<endl;
	  cout<<endl<<endl;
	  return 0;
	}
	
	#ifdef PROFILER_ENABLED
	  Logger::Init();
	  if(lFrameSource == FrameSource::DIRECTORY)
	  {
	    LOG_INFO_(LDTLog::STATE_MACHINE_LOG) <<endl
	    <<"******************************"<<endl
	    << "State-Machine Log Created..."<<endl
	    <<"******************************"<<endl<<endl;
	  }
	#endif


	unique_ptr<FrameFeeder>  lPtrFeeder = createFrameFeeder(lFrameSource, lSourceStr);
	if(lPtrFeeder == nullptr)
	  lReturn = -1;

	shared_ptr<SigInit> sigInit= make_shared<SigInit>();
	if(sigInit->sStatus == SigStatus::FAILURE)
	  lReturn = -1;


	if (lReturn==0)
	{
		std::cout<<endl<<endl;
		std::cout<<"******************************"<<std::endl;
		std::cout<<" Press Ctrl + c to terminate."<<std::endl;
		std::cout<<"******************************"<<std::endl;
		
		//^TODO: Replace by State.h member variable.
		uint64_t nbCycles = 0;
		
		try
		{
		   StateMachine stateMachine(move(lPtrFeeder));

		   cout<<stateMachine.getCurrentState();
	 	   States lPreviousState = stateMachine.getCurrentState();

		   while (stateMachine.getCurrentState() != States::DISPOSED)
		   {
			if (sigInit->sStatus == SigStatus::STOP)
			stateMachine.quit();

			lReturn = stateMachine.spin();
			nbCycles ++;
			
			if(lPreviousState != stateMachine.getCurrentState())
			{
			  cout<<endl;	
			  cout<<stateMachine.getCurrentState();
			  lPreviousState = stateMachine.getCurrentState();

			  std::cout.flush();
			}
			else if (nbCycles%100==0)
			{
			  cout <<endl;
			  cout <<stateMachine.getCurrentState();
			  cout <<"state cycle-count = " << nbCycles;
			}

		    }
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

	cout<<endl<<"The program ended with exit code " <<lReturn<<endl;
	return(lReturn);
	
}



unique_ptr<FrameFeeder> createFrameFeeder(FrameSource srcMode, string srcString)
{

	unique_ptr<FrameFeeder>	lPtrFeeder;

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
