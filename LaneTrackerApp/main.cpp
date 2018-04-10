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

	FrameSource 	lFrameSource;
	std::string 	lSourceStr;


	// Prsing command line options 
	{
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

	} // End parsing command line options 
	




	unique_ptr<FrameFeeder>  lPtrFeeder = createFrameFeeder(lFrameSource, lSourceStr);
	if(lPtrFeeder == nullptr)
	{
	  lReturn = -1;
	}

	shared_ptr<SigInit> sigInit= make_shared<SigInit>();
	if(sigInit->sStatus == SigStatus::FAILURE)
	{
	  lReturn = -1;
	}


	if (lReturn==0)
	{
	  uint64_t lCyclesCount = 0;

	  std::cout<<endl<<endl;
	  std::cout<<"******************************"<<std::endl;
	  std::cout<<" Press Ctrl + c to terminate."<<std::endl;
	  std::cout<<"******************************"<<std::endl;

	  StateMachine stateMachine(move(lPtrFeeder));

		//vector<int> newLaneParams = {300,50,250,500,30};
		stateMachine.setLaneParameters({350,50,250,500,30});

	  cout<<stateMachine.getCurrentState();
	  States lPreviousState = stateMachine.getCurrentState();

	  while (stateMachine.getCurrentState() != States::DISPOSED)
	  {
		if (sigInit->sStatus == SigStatus::STOP)
		stateMachine.quit();

		lReturn = stateMachine.spin();
		if (lCyclesCount > 10)
			imshow("output",stateMachine.getCurrentFrame());
		lCyclesCount ++;
		
		if(lPreviousState != stateMachine.getCurrentState())
		{
		  cout<<endl<<stateMachine.getCurrentState();
		  std::cout.flush();
		  lPreviousState = stateMachine.getCurrentState();
		}
		else if (lCyclesCount%100==0)
		{
		  cout <<endl<<stateMachine.getCurrentState();
		  cout <<"state cycle-count = " << lCyclesCount;
		}
	   }// End spinning
	   
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

	}
	catch(const char* msg)
	{
	    cout<<"******************************"<<endl
	    <<"Failed to create frame feeder!"<<endl
	    << msg <<endl
	    <<"******************************"<<endl<<endl;
	    lPtrFeeder = nullptr; 
	}
	catch (...)
	{
	    cout<<"******************************"<<endl
	    <<"Failed to create frame feeder!"<<endl
	    << "unkown-excepton"<<endl
	    <<"******************************"<<endl<<endl;
	   lPtrFeeder = nullptr; 
	}

	return lPtrFeeder;

}
