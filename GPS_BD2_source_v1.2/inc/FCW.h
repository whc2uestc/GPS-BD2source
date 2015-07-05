/*
 * FCW.h
 *
 *  Created on: 2011-11-22
 *      Author: ah
 */

#ifndef FCW_H_
#define FCW_H_

#include <math.h>

#include "CGS.h"
#include "iloc.h"
#include "isrc.h"
#include "ichnl.h"
#include "GPS_fmt.h"
#include "GEO_fmt.h"
#include "MEO_fmt.h"

#define GET_DT(USERLOC,SATLOC) sqrt(pow(USERLOC->X-SATLOC.X,2)+pow(USERLOC->Y-SATLOC.Y,2)+pow(USERLOC->Z-SATLOC.Z,2))/C

void rota_modify(Sat_Loc *locH,DOUB dt);
void get_FCW(ChPrms *chnlH,DOUB dt);
void ref_loc(User_Loc *locH,QUAD i);
void adjust_FCW(User_Loc *locH,SrcSys *sysH);

void BD2_get_initial_phase(ChPrms *chnlH,DOUB navbitlen);
void GPS_get_initial_phase(ChPrms *chnlH,DOUB navbitlen);

DOUB GPS_clock_difference_correction(GPS_Bsc bsc,ChPrms chnl,DOUB dt);
DOUB GEO_clock_difference_correction(GEO_Bsc bsc,ChPrms chnl,DOUB dt);
DOUB MEO_clock_difference_correction(MEO_Bsc bsc,ChPrms chnl,DOUB dt);

#endif /* FCW_H_ */
