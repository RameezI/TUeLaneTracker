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
#include "opencv2/opencv.hpp"
#include <opencv2/core/eigen.hpp>

using namespace Eigen;


/*****************************************************//**
*  This structure contains following root templates for the TUeLaneTracker
*  /n /emp FOCUS_MASK_ROOT 
*  /n /emp GRADIENT_TAN_ROOT
*  /n /emp DEPTH_MAP_ROOT
*  /n /emp X_ICS
*  /n /emp Y_ICS 
*   ****************************************************/
struct Templates
{
	
public:

	const int MARGIN;  	/**< /brief Vertical margin between the image center and ROI [pixels]
				 	/n +ve Margin implies the ROI is below the center line.
					/n -ve Margin implies the ROI is above the center line. */ 


	const int VP_RANGE_V;   /**< Vertical range of the vanishing-point in each direction [pixels] */
	const int SPAN;		/**< Vertical size  of the ROI [pixels] 
				      /n Automatically calculated from  /em MARGIN and /em VP_RANGE_V */


	cv::Mat FOCUS_MASK_ROOT;    /**< /brief ROOT-TEMPLATE for extracting mask to compensate vehicle pitch movements.
				     /n The size of /em FOCUS ROOT is [SPAN+(2xVP_RANGE_V), RES_H]

				     /n Normal activation all elements in rowrange /em (SPAN-VP_RANGE) = 255
 				     /n Best   activation all elements in /em SPAN = 255
				     /n Worst  activation all elements in rowrange /em (SPAN-2*VP_RANGE) = 255
				*/


	cv::Mat GRADIENT_TAN_ROOT;	/**< ROOT-TEMPLATE for extracting gradient tangents refference.
				  /n The size of /em GRADIENT_TAN_ROOT is [2xRES_V +1 	, 2xRES_H +1]
				*/


	cv::Mat DEPTH_MAP_ROOT;	/**< ROOT-TEMPLATE for assigning perspective weights to the pixels. 
				     /n The size of /em DEPTH_MAP_ROOT is [RES_V, RES_H]
				*/


	cv::Mat X_ICS;		/**< ROOT-TEMPLATE containing the X pixel indices in Image Coordinate System. 
				   /n The size of /em X_ICS is [RES_V, RES_H]
				*/


	cv::Mat Y_ICS;		/**< ROOT-TEMPLATE containing the Y pixel indices in Image Coordinate System. 
				   /n The size of /em Y_ICS is [RES_V, RES_H]
				*/





	/*****************************************************//**
 	*  /brief  Creates root templates for the TUeLaneTracker
 	*  /param  RES_V is an integer argument representing vertical resolution of the camera. [pixels]
 	*  /param  RES_H is an integer argument representing horizental resolution of the camera. [pixels]
 	*  /param  FOV_V is an integer argument representing field of view of the camera. [degrees]
 	*  /parame VP_RANGE_ROWS is an integer argument representing vertical range of the vanishing-point. [pixels]
 	*   ****************************************************/
	Templates(const int RES_V, const int RES_H, const float FOV_V, const int VP_RANGE_ROWS)
	: MARGIN(80), 
	  VP_RANGE_V(VP_RANGE_ROWS), 
	  SPAN((RES_V/2)-MARGIN + VP_RANGE_ROWS) 

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
			cv::Mat Row = cv::Mat(1, RES_H, CV_16S);
			int16_t* ptr = Row.ptr<int16_t>(0);
			for (int i=0; i<RES_H; i++)
			{
				ptr[i]= i;			
			}
			repeat(Row,SPAN,1 ,X_ICS);
			X_ICS.convertTo(X_ICS, CV_16S);

		/* Create Y Template */
			cv::Mat Col = cv::Mat(SPAN, 1, CV_16S);
			ptr = Col.ptr<int16_t>(0);
			for (int i=0; i<SPAN; i++)
			{
				ptr[i]= i;			
			}
			repeat(Col,1, RES_H ,Y_ICS);

	 	/* Load Gradient Tangent Template */
			std::stringstream formattedString;
			string templateFile, prefix, format;
			prefix= "../../Templates/GradientTangent_";

			formattedString<<prefix<<std::to_string(RES_H)<<"x"<<std::to_string(RES_V);
			templateFile = formattedString.str();

			struct stat buf;
			int statResult = stat(templateFile.c_str(),&buf);
			if (statResult != 0) 
			{
				cout << "File not found: " << templateFile.c_str() << endl;
				exit(-2);
			}
			else
			{
				cv::FileStorage loadGradientTemplate( templateFile, cv::FileStorage::READ);
				loadGradientTemplate["ROOT_DIR_TEMPLATE"]>> GRADIENT_TAN_ROOT;
				GRADIENT_TAN_ROOT.convertTo(GRADIENT_TAN_ROOT,CV_16SC1);
			}		
	}	
}; 
#endif
