/** \file */ 
#include "StateMachine.h"
#include "boost/program_options.hpp"

using namespace std;
namespace po = boost::program_options;
/*
//custom validator for sourceMode
void validate (boost::any& v, const std::vector<std::string>& values, FrameSource* target_type, int)
{
	po::validators::check_first_occurrence(v);
		
	const string& str= po::validators::get_single_string(values);
		
	if(str.compare("imgstore")==0)
		v= boost::any(FrameSource::DIRECTORY);
	else
		throw po::validation_error(po::validation_error::invalid_option_value);


}
*/

std::istream& operator>>(std::istream& in, FrameSource& frameSource)
{
	std::string token;
	in >> token;
	
	if (token.compare("imgstore")==0)
	  frameSource = FrameSource::DIRECTORY;
	else
	  in.setstate(std::ios_base::failbit);
	
	return in;
}
 
std::ostream& operator<<(std::ostream& out, const FrameSource& frameSource)
{
	if(frameSource == FrameSource::DIRECTORY)
	  out<<"imgstore";
	else if (frameSource == FrameSource::RTSP)
	  out<<"rtsp";
	else if (frameSource == FrameSource::GMSL)
	  out<<"gmsl";
	else
	  out << "unknown-source";
	
	return out;	
}


int main(int argc, char* argv[]) /**  
	This is the entry point of the application.
	- Initialises the sigInit handler
	- Creates a stateMachine and spins it until user issues a quit signal through the sigInit handler. 		
	*/

{
	int lReturn = 0;

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

	{
	  po::variables_map vMap;
	  po::store(po::parse_command_line(argc, argv, lDesc), vMap);
	  po::notify(vMap);

	  if ( vMap.count("help") )
	  {
	    cout << lDesc <<endl;
	    cout << "	Valid arguments for 'Mode': ";
	    cout << "["<<FrameSource::DIRECTORY<<" ";
	    cout <<FrameSource::RTSP<<" ";
	    cout <<FrameSource::GMSL<<"]";

	    cout <<endl<<endl<< "	Examples:"<<endl;
	    cout<< "	./TUeLaneTracker -m " << FrameSource::DIRECTORY << " -s " << "/home/DataSet" <<endl;
	    cout<<endl<<endl;
	    return -1;
	  }

	}

	






	//string lSourceStr = "/mnt/data0/DataSet/eindhoven/PNG_imgs";
        //string lSourceStr = "/media/s32v/LinuxExtended/DataSet/eindhoven/PNG_imgs";
        //string lSourceStr = "/home/root/data/Eindhoven";
        
	
	#ifdef PROFILER_ENABLED
	  Logger::Init();
	#endif	
	
	shared_ptr<SigInit> sigInit= make_shared<SigInit>();

	if(sigInit->sStatus == SigStatus::FAILURE) 
	   lReturn|= -1;
  

	std::cout<<"******************************"<<std::endl;
	std::cout<<" Press Ctrl + c to terminate."<<std::endl;
	std::cout<<"******************************"<<std::endl;


	StateMachine stateMachine(lFrameSource, lSourceStr);
	lReturn = stateMachine.spin(sigInit);

	return(lReturn);
}
