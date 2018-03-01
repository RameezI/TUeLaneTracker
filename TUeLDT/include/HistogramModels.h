#ifndef HISTOGRAM_MODELS_H__
#define HISTOGRAM_MODELS_H__

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




/// This model provides an efficient way to store a set of feasible points in the multimodal base histogram.
/**The complete histogram is filtered into a list of these models, 
where each element in the list represents a possible Lane model at the base line.*/
struct BaseHistogramModel
{
	int   	rowIdxFilter;	 	/**<Row-Index of the #filterBaseHistogram for the current model [index]*/

	int   	colIdxFilter;	 	/**<Col-Index of the #filterBaseHistogram for the current model [index]*/

	int   	leftOffset;	 	/**< Distance to the left boundary from the origin of Camera-Center-CS
					 given the current Model [pixels] */

	int   	rightOffset;	 	/**< Distance to the right boundary from the origin of Camera-Center-CS
					 given the current Model [pixels] */

	int   	leftOffset_cm;	 	/**< Distance to the left boundary from the origin of Vehicle-Symmetry-CS
					 given the current Model [cm] */

	int   	rightOffset_cm;	 	/**< Distance to the right boundary from the origin of Vehicle-Symmetry-CS
					 given the current Model [cm] */

	float 	width_cm;		/**< Width of the Lane given the current model [cm] */
		
	int  	binID_leftBoundary;	/**<Histogram Index representing the left boundary given the current model [index]*/

	int  	binID_rightBoundary;	/**<Histogram Index representing the right boundary given the current model [index]*/
	
	int  	binID_nonBoundaryLeft;	/**<Histogram Index representing start of the non-boundary region on the left [index] */

	int  	nbNonBoundaryBinsLeft;	/**<Number of non-boundary bins to the left of the center-line */

	int  	binID_nonBoundaryRight;	/**<Histogram Index representing start of the non-boundry region on the right [index] */

	int  	nbNonBoundaryBinsRight;	/**<Number of non-boundary bins to the right of the center-line*/
		
	
	BaseHistogramModel()
	: rowIdxFilter(-1),
	  colIdxFilter(-1),
	  leftOffset(-1),
	  rightOffset(-1),
	  leftOffset_cm(-1),
	  rightOffset_cm(-1),
	  width_cm(0),
	  binID_leftBoundary(-1),
	  binID_rightBoundary(-1),
	  binID_nonBoundaryLeft(-1),
	  nbNonBoundaryBinsLeft(-1),
	  binID_nonBoundaryRight(-1),
	  nbNonBoundaryBinsRight(-1)
	{

	}
	
};


struct PurviewHistogramModel
{
	int  binID_leftBoundary;
	int  binID_rightBoundary;
	
	int  binID_NegBoundaryLeft;
	int  binID_NegBoundaryRight;
	int  nbNonBoundaryBinsLeft;
	int  nbNonBoundaryBinsRight;
	
	PurviewHistogramModel()
	: binID_leftBoundary(-1),binID_rightBoundary(-1),
	 binID_NegBoundaryLeft(-1),binID_NegBoundaryRight(-1),
	 nbNonBoundaryBinsLeft(0), nbNonBoundaryBinsRight(0)
	{

	}
	
};

#endif
