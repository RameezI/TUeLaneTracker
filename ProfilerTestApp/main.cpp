#include <stdio.h>
#include <iostream>
#include <chrono>
#include "LDT_profiler.h"
#include <plog/Log.h>

using namespace std::chrono;
using namespace std;


void NestedDelay();
static ProfilerLDT  BufferingProfiler;


int main(int argc, char **argv)
{

	



plog::init(plog::info, "Log_ProfilerTest"); //Initialize the logger.


double resolution= (double) std::chrono::high_resolution_clock::period::num/ std::chrono::high_resolution_clock::period::den;


LOG_INFO<<endl
					  <<"******************************"<<endl
					  <<  "Determining Resolution for the chrono::high_resolution_clock."<<endl
					  <<  "Resolution: " <<resolution<<endl
					  <<  "-is steady: " << boolalpha << high_resolution_clock::is_steady<< endl
					  <<"******************************"<<endl<<endl;
	
BufferingProfiler.start("TopLavel");
NestedDelay();
BufferingProfiler.end();
 
LOG_INFO<<endl
					  <<"******************************"<<endl
					  <<  "Completing Toplevel loop." <<endl
					  <<  "Top Level time: " << BufferingProfiler.getTiming("TopLavel")<<endl
					  <<"******************************"<<endl<<endl;
 
 
return 0;

}


void NestedDelay()
{
	
	
	
	
bool sleep = true;
;	


for (int i=0; i<2; i ++)
 {
	
	auto start = std::chrono::high_resolution_clock::now();
	sleep =true;



		while(sleep)
		{
			auto now = std::chrono::high_resolution_clock::now();
			
			auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
			if ( elapsed.count() > 1000 )
				sleep = false;
		}
		
BufferingProfiler.start("MidLavel");
		for (int i=0; i<1000; i++)
		{
			auto start = std::chrono::high_resolution_clock::now();
			sleep =true;

		BufferingProfiler.start("DeepestLavel");		
			while(sleep)
			{
				auto now = std::chrono::high_resolution_clock::now();
				
				auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
				if ( elapsed.count() > 1000 )
					sleep = false;
			}
       BufferingProfiler.end();
/*	   LOG_INFO<<endl
					  <<"******************************"<<endl
					  <<  "Completing Deepestlevel loop." <<endl
					  <<  "Deepest Level time: " << BufferingProfiler.getTiming("DeepestLavel")<<endl
					  <<"******************************"<<endl<<endl;
*/	   
			
		}
BufferingProfiler.end();
/*LOG_INFO<<endl
					  <<"******************************"<<endl
					  <<  "Completing Midlevel loop." <<endl
					  <<  "Mid Level time: " << BufferingProfiler.getTiming("MidLavel")<<endl
					  <<"******************************"<<endl<<endl;
*/
 }
	
	
}