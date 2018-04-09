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


#define PROFILER_ENABLED 	/**< If program is compiled with this defined, the program logs and profiles its execution*/
#define DISPLAY_GRAPHICS 	/**< If program is compiled with this degined, the program renders the results on to a selected display */  

//#define DISPLAY_GRAPHICS_DCU /**< Define this when the graphics need to be rendered on the s32v23 DCU*/
#define TEST_APEX_CODE	       /**< S32V234 test specific flags */

/** Provide target camera and resolution [Required for loading corresponding callibration file] */

//#define CAMERA_NAME "BUMBLEBEE_640x480"
#define CAMERA_NAME "BUMBLEBEE_960x600"

// Confifure FrameFeeder

//Configuration for Algorithm
#define BASE_LINE_IBCS		0     /**< Describes base line in Image-Bottom-Coordinate-System */
#define PURVIEW_LINE_IBCS	150   /**< Describes purview line in Image-Bottom-Coordinate-System */

#define STEP_LANE_FILTER_CM	5    /**< Provide bin-size for the 1D lane filters (At base and purview lines) [cm]*/
#define STEP_VP_FILTER		10    /**< Provide bin-size for 2D vanishing point filter [pixels]*/

#define VP_RANGE_VER		25    /**< Vertical range of VP on either side of the Horizon Line [pixels] */
#define VP_RANGE_HOR		300   /**< Horizental range of VP on either side of center point on the horizon line [pixels] */

#define BUFFER_COUNT		5     /**<  Provide number of probabiltiy frames to buffer for Max-Pooling operations [Temporal Filtering]*/

#endif

