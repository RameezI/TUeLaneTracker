#ifndef CONFIG__H
#define CONFIG__H

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
* ****************************************************************************/

#define PROFILER_ENABLED		/**< If defined, the program logs and profiles its execution*/
//#define WRITE_FRAMES_TO_FILE  /**< If defined, the output is saved to a video file */

namespace LaneTracker
{
  struct Config
  {

   public:

	//Lane Properties
	float  lane_avg_width    = 300;
	float  lane_std_width    = 15;
	float  lane_min_width    = 250;
	float  lane_max_width    = 500;
	float  lane_marker_width = 15;
	  
	//Camera Resolution
	int    cam_res_v = 480;					// Vertical   Resolution  [pixels].
	int    cam_res_h = 640;					// Horizontal Resolution  [pixels].

	// Camera Intrinsic parameters
	float  cam_fx  = 563.9912133;	        //Focal length x-axis [pixels]
	float  cam_fy  = 526.0339806; 	        //Focal length y-axis [pixels]

	float  cam_cx	= 0;	   				//Principal point x-axis in Image-Center-CS [pixels]
	float  cam_cy   = 0;					//Principal point y-axis in Image-Center-CS [pixels]

	// Camera Extrinsic parameters
	float  cam_pitch   		= 0;	        //Pitch of the camera [degrees].
	float  cam_yaw     		= 0;		    //Yaw of the camera [degrees].
	float  cam_height  		= 1.5;		    // Camera height from the road [meters].
	float  cam_lateral_offset = 0;          // Lateral-Offset of the camera from the vehicle symmetry plane[meters].


	//LaneFilter VpFilter Paramters
        int    base_line_IBCS	= 0;        /**< Describes base line in Image-Bottom-Coordinate-System */
	int    purview_line_IBCS    = 160;      /**< Describes purview line in Image-Bottom-Coordinate-System */
	int    step_lane_filter_cm  = 10; 		/**< Provides bin-size for the 1D lane filters */
	int    step_vp_filter	    = 10;       /**< Provides bin-size for 2D vanishing-point filter */

	int    vp_range_ver	    = 20;           /**< Vertical range of VP on either side of the Horizon Line [pixels] */
	int    vp_range_hor	    = 300;          /**< Horizontal range of VP on either sides of center point [pixels] */

	
	int    buffer_count	    = 3;    	    /**<  Provide number of frames to buffer[Temporal Filtering]*/
	bool   display_graphics = true;         /**< Renders graphics on to a HDMI display */

  };

}

#endif

