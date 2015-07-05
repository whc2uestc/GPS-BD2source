/*
 * init.h
 *
 *  Created on: 2011-11-21
 *      Author: ah
 */

#ifndef INIT_H_
#define INIT_H_

#include "iloc.h"
#include "isrc.h"
#include "GPS_fmt.h"
#include "GEO_fmt.h"
#include "MEO_fmt.h"


typedef union sat_eph				//不同种类卫星星历共用体
		{
            GPS_Bsc  gps_eph;
	        GEO_Bsc	 geo_eph;
	        MEO_Bsc  meo_eph;
        }Sat_Eph;



extern void add_sele();
extern void sele_sate(QUAD count_num);
extern Sat_Eph SAT_EPH[CHNLNUM];
DOUB cal_gdop();
void init(User_Loc *loc,SrcSys *sys);
#endif /* INIT_H_ */
