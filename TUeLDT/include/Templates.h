#ifndef __TEMPLATES_TUe_H
#define __TEMPLATES_TUe_H

/******************************************************************************
* NXP Confidential Proprietary
* 
* Copyright (c) 2017 NXP Semiconductor;
* All Rights Reserved
*
* AUTHOR : Rameez Ismail
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
* ****************************************************************************/ 

#include <sys/stat.h>
#include <Eigen/Dense>
#include "State.h"
#include "opencv2/opencv.hpp"
#include <opencv2/core/eigen.hpp>

using namespace Eigen;


/*************************************************************************//**
*  This structure contains following root templates for the TUeLaneTracker
*  /n /emp FOCUS_MASK_ROOT 
*  /n /emp GRADIENT_TAN_ROOT
*  /n /emp DEPTH_MAP_ROOT
*  /n /emp X_ICS
*  /n /emp Y_ICS 
*   ************************************************************************/

struct Templates
{
	
public:

        const int HORIZON_ICCS_H;  	/**< /brief Position of Horizon in the Image-Center-CS [pixels] */


	const int HORIZON_ICCS_V;  	/**< /brief Position of Horizon in the Image-Center-CS [pixels]
				 	  *  /n +ve value implies that the ROI is below the center line.
					  *  /n -ve value implies that the ROI is above the center line. */


	const int VP_RANGE_V;   	/**< Vertical range of the vanishing-point in either direction [pixels] */

	const int SPAN;			/**< Vertical size  of the ROI [pixels]
					/n Automatically calculated from  #HORIZON_ICCS_V and #VP_RANGE_V */

	cv::Mat FOCUS_MASK_ROOT;  	/**< /brief ROOT-TEMPLATE for extracting mask to compensate vehicle pitch movements.
					/n The size of /em FOCUS ROOT is [#SPAN + (2x#VP_RANGE_V), #RES_H]
				     	/n Normal activation all elements in rowrange (#SPAN - #VP_RANGE) = 255
 				     	/n Best   activation all elements in #SPAN = 255
				     	/n Worst  activation all elements in rowrange (#SPAN - 2x#VP_RANGE) = 255 */


	cv::Mat GRADIENT_TAN_ROOT;	/**< ROOT-TEMPLATE for extracting gradient tangents reference.
				  	/n The size of /em GRADIENT_TAN_ROOT is [2x#RES_V +1 , 2x#RES_H +1] */


	cv::Mat DEPTH_MAP_ROOT;		/**< ROOT-TEMPLATE for assigning perspective weights to the pixels.
				     	/n The size of /em DEPTH_MAP_ROOT is [#RES_V, #RES_H] */


	cv::Mat X_ICS;			/**< X-Coordinates of the ROI in the Image-Coordinate-System.
				   	/n The size of /em X_ICS is [#SPAN, #RES_H] */


	cv::Mat Y_ICS;			/**< Y-Coordinates of the ROI in the Image-Coordinate-System.
				   	/n The size of /em Y_ICS is [#SPAN, #RES_H] */



	/*****************************************************//**
 	*  /brief  Creates root templates for the TUeLaneTracker
 	*  /param  RES_V is an integer argument representing vertical resolution of the camera. [pixels]
 	*  /param  RES_H is an integer argument representing horizental resolution of the camera. [pixels]
 	*  /param  FOV_V is an integer argument representing field of view of the camera. [degrees]
 	*  /parame VP_RANGE_ROWS is an integer argument representing vertical range of the vanishing-point. [pixels]
 	*   ****************************************************/
	Templates(const int RES_V, const int RES_H, const float FOV_V, const int HORIZON_H, const int HORIZON_V, const int VP_RANGE_ROWS)
	: HORIZON_ICCS_H(HORIZON_H),
	  HORIZON_ICCS_V(HORIZON_V),
	  VP_RANGE_V(VP_RANGE_ROWS), 
	  SPAN((RES_V/2)- HORIZON_ICCS_V + VP_RANGE_ROWS) 

	{
		/* Create Focus Template */
			MatrixXi FOCUS_ROOT   = MatrixXi::Zero(SPAN + 2*VP_RANGE_V, RES_H);
			FOCUS_ROOT.block(2*VP_RANGE_V, 0, SPAN, RES_H) = MatrixXi::Constant(SPAN, RES_H, 255);
			eigen2cv(FOCUS_ROOT, FOCUS_MASK_ROOT);
			FOCUS_MASK_ROOT.convertTo(FOCUS_MASK_ROOT, CV_8U);	
	
		/* Create Depth Template */
			MatrixXf DEPTH_ROOT  = MatrixXf::Zero(RES_V, RES_H);			
			const float step     = FOV_V/RES_V;
			float angle = 90- step;
			
			for(int n = 0; n < RES_V ; n++)
			{
			  float x= 1.75 * tan(angle * M_PI/180.0 );
			  x = x*x;
			  if( x > 100) x=100;

			  DEPTH_ROOT.row(n) = ArrayXf::Constant(RES_H, x);
			  angle = angle - step;
			}
			
			eigen2cv(DEPTH_ROOT, DEPTH_MAP_ROOT);
			DEPTH_MAP_ROOT.convertTo(DEPTH_MAP_ROOT, CV_16U);

		/* Create X Template */
			cv::Mat Row = cv::Mat(1, RES_H, CV_32S);
			int32_t* ptr = Row.ptr<int32_t>(0);
			for (int i=0; i<RES_H; i++)
			{
				ptr[i]= i;			
			}
			repeat(Row,SPAN,1 ,X_ICS);

		/* Create Y Template */
			cv::Mat Col = cv::Mat(SPAN, 1, CV_32S);
			ptr = Col.ptr<int32_t>(0);
			for (int i=0; i<SPAN; i++)
			{
				ptr[i]= (RES_V-SPAN) + i;			
			}
			repeat(Col,1, RES_H ,Y_ICS);

	 	/* Load Gradient Tangent Template */
			std::stringstream formattedString;
			string templateFile, path, prefix, format;

			char lBuff[65536] = {"\0"};
			ssize_t len = ::readlink("/proc/self/exe", lBuff, sizeof(lBuff)-1);
			if (len!=-1)
			{
				path = std::string(lBuff);
				std::string::size_type Idx = path.find_last_of("/");
				path =	path.substr(0,Idx);
			}
			else
			{
				#ifdef PROFILER_ENABLED
				 LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
				 <<"Unable to find the path to binary"<<endl
				 <<"[searching for Template]"<<endl;
				#endif
			}

			prefix= "GradientTangent_";

			formattedString<<path<<"/ConfigFiles/Templates/"<<prefix
			<<std::to_string(RES_H)<<"x"<<std::to_string(RES_V)<<".yaml";

			templateFile = formattedString.str();

			struct stat buf;
			int statResult = stat(templateFile.c_str(),&buf);
			if (statResult != 0) 
			{
			  #ifdef PROFILER_ENABLED
			   LOG_INFO_(LDTLog::STATE_MACHINE_LOG)
			   <<"File not found: "<<templateFile.c_str()<<endl;
		 	  #endif
			}
			else
			{
			  cv::FileStorage loadGradientTemplate( templateFile, cv::FileStorage::READ);
			  loadGradientTemplate["ROOT_DIR_TEMPLATE"]>> GRADIENT_TAN_ROOT;
			  GRADIENT_TAN_ROOT.convertTo(GRADIENT_TAN_ROOT,CV_16SC1);
			}

			//Check if every template is non-empty and throw an exception if not.
			if (GRADIENT_TAN_ROOT.empty() ) throw "Gradient_TAN_ROOT is empty" ; 
			if (FOCUS_MASK_ROOT.empty()   ) throw "FOCUS_MASK_ROOT is empty" ;
                        if (DEPTH_MAP_ROOT.empty() )    throw "DEPTH_MAP_ROOT is empty";
                        if (X_ICS.empty() ) 		throw "X_ICS is Empty";
                        if (Y_ICS.empty() ) 		throw "Y_ICS is Empty";
					
	}	
}; 
#endif
