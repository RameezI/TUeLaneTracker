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


// General definitions


#define CAMERA_NAME "BUMBLEBEE_640x480" /** Target camera and resolution [Loads corresponding calibration file] */
#define PROFILER_ENABLED 	        /**< If defined, the program logs and profiles its execution*/
#define DISPLAY_GRAPHICS 	        /**< If defined, the program renders results on to a display */
#define DISPLAY_GRAPHICS_DCU            /**< If defined, the graphics are rendered on the s32v23 DCU*/



//S32V specific definitions
#define S32V2XX			        /**< If defined, the program runs s32v-hardware accelerated algorithm */
#ifdef  S32V2XX
 #include "GraphsConfig.h"
#endif


// Configure FrameFeeder

//Configuration for the Algorithm
#define BASE_LINE_IBCS	 	  0     /**< Describes base line in Image-Bottom-Coordinate-System */
#define PURVIEW_LINE_IBCS	160     /**< Describes purview line in Image-Bottom-Coordinate-System */

#define STEP_LANE_FILTER_CM	10    /**< Provide bin-size for the 1D lane filters (At base and purview lines) [cm]*/
#define STEP_VP_FILTER		 5    /**< Provide bin-size for 2D vanishing point filter [pixels]*/

#define VP_RANGE_VER		20    /**< Vertical range of VP on either side of the Horizon Line [pixels] */
#define VP_RANGE_HOR		300   /**< Horizontal range of VP on either side of center point on the horizon line [pixels] */

#define BUFFER_COUNT		 3    /**<  Provide number of probability frames to buffer for Max-Pooling operations [Temporal Filtering]*/


#endif

