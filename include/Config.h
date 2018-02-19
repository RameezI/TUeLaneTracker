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


/* The following section provides configuration for compiling the appropriate setup*/
 #define PROFILER_ENABLED
 #define DISPLAY_GRAPHICS


//S32V234 specific flgs
 //#define DISPLAY_GRAPHICS_DCU
 #define TEST_APEX_CODE

/* The following section provides configuration for Frame Input*/
#define DIRECTORY_PNG_IMAGES	"../../DataSet"
#define SKIP_FRAMES				0


/* The following section provides configuration for  the camera parameters */
#define CAMERA_RES_H  			640
#define CAMERA_RES_V			480

#define CAMERA_FOV_H			60
#define CAMERA_FOV_V			45

#define CAMERA_HEIGHT			1.5
#define CAMERA_FOCAL_LENGTH		6.0e-3





#endif

