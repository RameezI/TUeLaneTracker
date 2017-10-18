#ifndef __TEMPLATES_TUe_H
#define __TEMPLATES_TUe_H
#include <Eigen/Dense>
#include "opencv2/opencv.hpp"
#include <opencv2/core/eigen.hpp>

using namespace cv;
using namespace Eigen;

struct Templates
{
	
public:
	const int MARGIN;
	const int VP_RANGE_V;
	const int SPAN;

	Mat GRADIENT_TAN_ROOT;
	Mat FOCUS_MASK_ROOT;
	Mat DEPTH_MAP_ROOT;
	Mat X_IRS;
	Mat Y_IRS;
	
	Templates(const int RES_V, const int RES_H, const int VP_RANGE_ROWS)
	: MARGIN(80), 
	  VP_RANGE_V(VP_RANGE_ROWS), 
	  SPAN((RES_V/2)-MARGIN + VP_RANGE_ROWS) 
	{
		   
		/* Create Focus Template */
			
			MatrixXi FOCUS_ROOT     = MatrixXi::Zero(SPAN + 2*VP_RANGE_V, RES_H);
			FOCUS_ROOT.block(2*VP_RANGE_V, 0, SPAN, RES_H) = MatrixXi::Constant(SPAN, RES_H, 255);			
			eigen2cv(FOCUS_ROOT, FOCUS_MASK_ROOT);
			FOCUS_MASK_ROOT.convertTo(FOCUS_MASK_ROOT, CV_8U);

			
		/* Create Depth Template */
			MatrixXf DEPTH_ROOT  = MatrixXf::Zero(2*RES_V +1,RES_H);			
			const float step 	= 45.0/RES_V;
			float angle = 90- step;
			
			for(int n = RES_V; n < 2*RES_V +1; n++)
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
			Mat Row = Mat(1, RES_H, CV_16S);
			int16_t* ptr = Row.ptr<int16_t>(0);
			for (int i=0; i<RES_H; i++)
			{
				ptr[i]= i;			
			}
			repeat(Row,SPAN,1 ,X_IRS);
			X_IRS.convertTo(X_IRS, CV_16S);
		

		
		/* Create Y Template */
			Mat Col = Mat(SPAN, 1, CV_16S);
			ptr = Col.ptr<int16_t>(0);
			for (int i=0; i<SPAN; i++)
			{
				ptr[i]= i;			
			}
			repeat(Col,1, RES_H ,Y_IRS);



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
				FileStorage loadGradientTemplate( templateFile, FileStorage::READ);
				loadGradientTemplate["ROOT_DIR_TEMPLATE"]>> GRADIENT_TAN_ROOT;
				GRADIENT_TAN_ROOT.convertTo(GRADIENT_TAN_ROOT,CV_16SC1);
			}		
	}	
  
}; 
#endif
