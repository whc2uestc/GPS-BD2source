/*
 * ichnl.h
 *
 *  Created on: 2011-11-21
 *      Author: ah
 */

#ifndef ICHNL_H_
#define ICHNL_H_

#include "inco.h"
#include "iloc.h"
#include "sat_typ.h"
#include "../cfg.h"

typedef struct channel							//通道结构体
		{
	        SatType sattype;					//卫星种类
			UBYTE	satID;						//卫星ID
			DOUB	timer;						//仿真时间对应的周内计时
			DOUB	amp;						//信号幅度
			Nco		carNco;						//载波NCO
			Nco		cacNco;						//CA码NCO
			BYTE	cacode[BD2_CA_LEN];			//CA码序列
			BYTE	nhcode[NHLEN];				//NH码序列
			BYTE	navbit[32];					//导航比特buffer
			BYTE	navdata;					//当前的导航比特
			UQUAD	cacphs;						//CA码码片计数
			UQUAD   maxcaphs;					//CA码码片计数的最大值
			UBYTE   navflg;						//导航比特读取标志位
			UBYTE	CntNav;						//导航比特计数
			UBYTE	Cntms;						//导航比特内ms计数
			UBYTE	Maxms;						//导航比特内ms计数最大值
			Sat_Loc loc;						//存放卫星位置等信息的结构体
		}ChPrms;
#endif /* ICHNL_H_ */
