/*
 * isrc.h
 *
 *  Created on: 2013-11-21
 *      Author: whc
 */

#ifndef ISRC_H_
#define ISRC_H_

#include "../cfg.h"
#include "ichnl.h"

typedef struct SourceSystem    				//信号源主体结构体
		{
			UQUAD	 tmr;                   //仿真计时，单位ms
			QUAD	 SatNM;					//仿真卫星数目
			ChPrms	 chnls[CHNLNUM];		//各通道结构体，存放各卫星的相关信息
		}SrcSys;

#endif /* ISRC_H_ */
