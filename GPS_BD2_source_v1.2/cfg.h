/*
 * cfg.h
 *
 *  Created on: 2011-11-17
 *      Author: ah
 */

#ifndef CFG_H_
#define CFG_H_

#include "./inc/sat_typ.h"

#define SMPFRQ	16000000.000			//采样率
#define INTFRQ	5000000.0000			//中频

#define BD2_CARFRQ  1561098000.0		//BD载波频率
#define BD2_CACFRQ	2046000.0000		//BD的CA码频率
#define BD2_NH_FRQ	1000.0000000		//NH码频率
#define BD2_AIDFAC	763.00000000		//BD载波频率域CA码频率比例因子
#define BD2_CA_LEN   2046				//BD的CA码长度

#define GPS_CARFRQ 1575420000.0			//GPS载波频率
#define GPS_CACFRQ 1023000.0000			//GPS的CA码频率
#define GPS_AIDFCT 1540.0000000			//GPS载波频率域CA码频率比例因子
#define GPS_CA_LEN 1023					//GPS的CA码长度

#define BD2_SATNUM  37					//BD卫星数量
#define GPS_SATNUM	37					//GPS卫星数量
#define NHLEN	20						//BD卫星数量
#define SUBFRM	30						//子帧数量

#define TIMELEN 306000                   //仿真时间，单位ms
#define INTEVAL 0.002                    //频率控制字更新间隔，单位s

#define TIMEBGN 518454                  //仿真开始时间对应的周内时，单位s


#define USERB   38.897677					//纬度
#define USERL   -77.036531        					//经度
#define USERH	0.1						//高

//#define USERB   30.75211					//纬度
//#define USERL   103.930479        					//经度
//#define USERH	498.0						//高

#define T       30						//用户运动轨迹周期

#define USERAX  0						//用户x方向加速度
#define USERAY  0						//用户Y方向加速度
#define USERAZ  0						//用户Z方向加速度

#define GPS_SMP_BIT  20					//GPS导航比特持续时间，单位ms
#define GEO_SMP_BIT  2					//GEO导航比特持续时间，单位ms
#define MEO_SMP_BIT	 20					//MEO导航比特持续时间，单位ms

#define THRESHOLD  1.77827941003892e-007	//判定门限

#define CHNLNUM 37							//仿真设定卫星颗数

extern const SatType SATTYPE[CHNLNUM];		//仿真卫星类型


#define flag_sele_sate 1		//选星标志位 1:选  0:不选
#define max_num 8  //选星
#define kz 3
#define kd 3
#define Threshold 5.0
#endif /* CFG_H_ */
