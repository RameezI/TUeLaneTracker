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

#include "VanishingPtFilter.h"
#include "ScalingFactors.h"

VanishingPtFilter::VanishingPtFilter(const int HOR_H, const int HOR_V, const LaneTracker::Config& Config)
: 
  STEP((Config.step_vp_filter > 0) ? Config.step_vp_filter : throw "VP filter step-size must be a positive integer"),
  
  RANGE_V(Config.vp_range_ver), 
 
  RANGE_H(Config.vp_range_hor),
  
  COUNT_BINS_V(floor((2*RANGE_V)/STEP) +1),
  
  COUNT_BINS_H(floor((2*RANGE_H)/STEP) +1),
  
  BINS_V( VectorXi::LinSpaced(COUNT_BINS_V, HOR_V-RANGE_V,  HOR_V +RANGE_V).array()),
  
  BINS_H( VectorXi::LinSpaced(COUNT_BINS_H,-RANGE_H + HOR_H, RANGE_H + HOR_H).array()),
  
  prior(  cv::Mat::zeros( COUNT_BINS_V, COUNT_BINS_H , CV_32SC1) ),
  
  filter( cv::Mat::zeros( COUNT_BINS_V, COUNT_BINS_H,  CV_32SC1) )
{
	createPrior();
	this->filter = this->prior.clone();
}


void VanishingPtFilter::createPrior()
{
	
	float  sigma = 5.0*RANGE_V/STEP;
	double pv,ph;
	
	for (int v = 1; v <= COUNT_BINS_V; v++)
   	{
           for (int h = 1; h <= COUNT_BINS_H; h++)
	   {  
            pv = exp( -pow(v-COUNT_BINS_V/2.0,2)  / pow(sigma,2) ) *128 ;     
            ph = exp( -pow(h-COUNT_BINS_H/2.0,2)  / pow(sigma,2) ) *128;  
			
	    this->prior.at<int>(v-1,h-1) = (int)(std::round(pv*ph));                
	   }    
    	}

	// Normalize
	int32_t SUM = cv::sum(this->prior)[0];
	this->prior.convertTo(this->prior,CV_32SC1,SCALE_FILTER);
	this->prior = this->prior/(SUM);
}

VanishingPtFilter::~VanishingPtFilter()
{
}

