/** \file */ 
#include "SigInit.h"
#include "StateMachine.h"
#include "boost/program_options.hpp"

using namespace std;
namespace po = boost::program_options;





int main(int argc, char* argv[]) /**  
	This is the entry point of the application.
	- Initialises the sigInit handler
	- Creates a stateMachine and spins it until user issues a quit signal through the sigInit handler. 		
	*/

{
	int lReturn 	= 0;

	FrameSource 	lFrameSource;
	string 		lSourceStr;	
	
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
	  return -1;
	}

	shared_ptr<SigInit> sigInit= make_shared<SigInit>();

	if(sigInit->sStatus == SigStatus::FAILURE)
	  lReturn|= -1;

	if (lReturn==0)
	{
		std::cout<<endl<<endl;
		std::cout<<"******************************"<<std::endl;
		std::cout<<" Press Ctrl + c to terminate."<<std::endl;
		std::cout<<"******************************"<<std::endl;

		uint64_t nbCycles = 0;

		StateMachine stateMachine(lFrameSource, lSourceStr);
	

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

	cout<<"Completed!"<<endl<<"program ended with exit code " <<lReturn<<endl;
	return(lReturn);
	
}
