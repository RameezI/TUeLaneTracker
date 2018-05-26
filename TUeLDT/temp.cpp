
			/*
		   	{

	   		 int 	  	lPURV_LB  		= mLaneFilter->PURVIEW_BINS.at<int32_t>(lIdx_LB,0);	
	   		 int 	  	lPURV_RB 		= mLaneFilter->PURVIEW_BINS.at<int32_t>(lIdx_RB,0);


			  cv::Mat Frame;
			  cv::cvtColor(FrameGRAY, Frame, CV_GRAY2BGR);

		    	  size_t lCOUNT_BINS 		= mLaneFilter->COUNT_BINS;
			  int    lBASE_LINE_ICS		= mLaneFilter->BASE_LINE_ICCS + mLaneFilter->O_ICCS_ICS.y;
			  int    lPURVIEW_LINE_ICS	= mLaneFilter->PURVIEW_LINE_ICCS + mLaneFilter->O_ICCS_ICS.y;
		    
		    	  // Draw Histogram-Bins at the Base
		    	  for (size_t i=0; i < lCOUNT_BINS; i++)
		    	  {
			    int x =  mLaneFilter->BASE_BINS.at<int32_t>(i,0) + mLaneFilter->O_ICCS_ICS.x ;

			    if ((x== lBASE_LB + mLaneFilter->O_ICCS_ICS.x) | (x== lBASE_RB + mLaneFilter->O_ICCS_ICS.x))
			    {
			    	cv::line(Frame, cv::Point(x, lBASE_LINE_ICS), cv::Point(x,lBASE_LINE_ICS - 60), 
				     cv::Scalar(0,0,255), 1);
			    }
			    else
			    {
			    	cv::line(Frame, cv::Point(x, lBASE_LINE_ICS), cv::Point(x,lBASE_LINE_ICS - 30),
				cv::Scalar(0,0,0), 1);
			    }
		    	  }

		    	  // Draw Histogram-Bins at the Purview
		    	  for (size_t i=0; i < lCOUNT_BINS; i++)
		    	  {
			    	int x; 

				x = mLaneFilter->PURVIEW_BINS.at<int32_t>(i,0) + mLaneFilter->O_ICCS_ICS.x;

			    	 cv::line(Frame, cv::Point(x, lPURVIEW_LINE_ICS), cv::Point(x, lPURVIEW_LINE_ICS - 30),
				 cvScalar(0,0,0), 1);

				 x= lPURV_LB + mLaneFilter->O_ICCS_ICS.x;

			    	 cv::line(Frame, cv::Point(x, lPURVIEW_LINE_ICS), cv::Point(x, lPURVIEW_LINE_ICS - 60),
				 cvScalar(0,0,255), 1);


				 x= lPURV_RB + mLaneFilter->O_ICCS_ICS.x;

			    	 cv::line(Frame, cv::Point(x, lPURVIEW_LINE_ICS), cv::Point(x, lPURVIEW_LINE_ICS - 60),
				 cvScalar(0,0,255), 1);
		    	  }


		    	  // Draw lines to Vanishing Point
		    	  for (size_t i=0; i < lCOUNT_BINS; i++)
		    	  {
			       int  x		=  lBASE_LB + mLaneFilter->O_ICCS_ICS.x ;
			       auto VPt_ICS	=  cv::Point(lBIN_H + mLaneFilter->O_ICCS_ICS.x, lBIN_V + mLaneFilter->O_ICCS_ICS.y);

			       cv::line(Frame, cv::Point(x, lBASE_LINE_ICS), VPt_ICS, cvScalar(0,255,0), 2);

			       x =  lBASE_RB + mLaneFilter->O_ICCS_ICS.x ;
			       cv::line(Frame, cv::Point(x, lBASE_LINE_ICS), VPt_ICS, cvScalar(0,255,0), 2);
		    	  }



     		    	  cv::imshow( "Test VanishgingPt", Frame);
     		    	  while(cv::waitKey(10) != 32);

		          cout<< "Width_cm at Base " <<mBaseHistModel.width_cm<<" 	Width_cm at Purview"<<lWidth_cm<<endl;

		   	} */
