/*
 * FCW.c
 *
 *  Created on: 2011-11-22
 *      Author: ah
 */
#define _USE_MATH_DEFINES
#include <math.h>

#include "../inc/CGS.h"
#include "../inc/eph.h"
#include "../inc/FCW.h"
#include "../inc/iloc.h"
#include "../inc/ioc.h"
#include "../inc/frm.h"
#include "../inc/init.h"
#include "../inc/isrc.h"
#include "../inc/ichnl.h"
#include "../inc/GPS_loc.h"
#include "../inc/GEO_loc.h"
#include "../inc/MEO_loc.h"
#include "../inc/LLA2CGS.h"

void rota_modify(Sat_Loc *locH,DOUB dt)
{
	DOUB x,y;
	x = locH->X;
	y = locH->Y;

	locH->X =  x*cos(OMGE*dt) + y*sin(OMGE*dt);
	locH->Y = -x*sin(OMGE*dt) + y*cos(OMGE*dt);
}

DOUB GPS_clock_difference_correction(GPS_Bsc bsc,ChPrms chnl,DOUB dt)
{
	DOUB e,srA,a0,a1,a2,Ek,TGD,toc,t,dts,dtr;

	a0 = bsc.af0;
	a1 = bsc.af1;
	a2 = bsc.af2;
	e  = bsc.e;
	TGD= bsc.TGD;
	srA= bsc.srA;
	toc= bsc.toc;
	t  = chnl.timer - dt;
	Ek = chnl.loc.Ek;

	dtr = Fe * e * srA * sin(Ek);
	dts = a0 + a1 * (t-toc) + a2 * (t-toc) * (t-toc) - TGD + dtr;

	return (dt - dts);
}

DOUB GEO_clock_difference_correction(GEO_Bsc bsc,ChPrms chnl,DOUB dt)
{
	DOUB e,srA,a0,a1,a2,Ek,TG1,toc,t,dts,dtr;

	a0 = bsc.a0;
	a1 = bsc.a1;
	a2 = bsc.a2;
	e  = bsc.e;
	TG1= bsc.TG1;
	srA= bsc.srA;
	toc= bsc.toc;
	t  = chnl.timer - dt;
	Ek = chnl.loc.Ek;

	dtr = Fe * e * srA * sin(Ek);
	dts = a0 + a1 * (t-toc) + a2 * (t-toc) * (t-toc) - TG1 + dtr;

	return (dt - dts);
}

DOUB MEO_clock_difference_correction(MEO_Bsc bsc,ChPrms chnl,DOUB dt)
{
	DOUB e,srA,a0,a1,a2,Ek,TG1,toc,t,dts,dtr;

	a0 = bsc.a0;
	a1 = bsc.a1;
	a2 = bsc.a2;
	e  = bsc.e;
	TG1= bsc.TG1;
	srA= bsc.srA;
	toc= bsc.toc;
	t  = chnl.timer - dt;
	Ek = chnl.loc.Ek;

	dtr = Fe * e * srA * sin(Ek);
	dts = a0 + a1 * (t-toc) + a2 * (t-toc) * (t-toc) - TG1 + dtr;

	return (dt - dts);
}

void GPS_get_initial_phase(ChPrms *chnlH,DOUB navbitlen)
{
	DOUB dt,carrier_dely,cacode_dely;

	dt = -chnlH->loc.dt;

	carrier_dely = dt * GPS_CARFRQ;                        //carrier NCO initial phase
	chnlH->carNco.phs = (carrier_dely-floor(carrier_dely)) * pow(2,32);

	chnlH->CntNav = -floor(dt / navbitlen);            //navigation bit dely at the beginning
	dt = dt + chnlH->CntNav * navbitlen;               //modify the dely in one navigation
	chnlH->CntNav--;								   //(dt is large than 0,but small than navbitlen)

	chnlH->Cntms = (UQUAD)(dt*1000.0);			       //the cacode cycle in one navigation bit
	dt = dt - chnlH->Cntms/1000.0;

	cacode_dely = dt * GPS_CACFRQ;						   //cacode initial phase
	chnlH->cacphs = (UQUAD)cacode_dely;

	chnlH->cacNco.phs = (cacode_dely - (QUAD)cacode_dely) * pow(2,32);    //cacode NCO initial phase
}

void BD2_get_initial_phase(ChPrms *chnlH,DOUB navbitlen)
{
	DOUB dt,carrier_dely,cacode_dely;

	dt = -chnlH->loc.dt;

	carrier_dely = dt * BD2_CARFRQ;                        //carrier NCO initial phase
	chnlH->carNco.phs = (carrier_dely-floor(carrier_dely)) * pow(2,32);

	chnlH->CntNav = -floor(dt / navbitlen);            //navigation bit dely at the beginning
	dt = dt + chnlH->CntNav * navbitlen;               //modify the dely in one navigation
	chnlH->CntNav--;								   //(dt is large than 0,but small than navbitlen)

	chnlH->Cntms = (UQUAD)(dt*1000.0);				   //NH phase for GEO,but for MEO ,it must be 0 or 1
	dt = dt - chnlH->Cntms/1000.0;

	cacode_dely = dt * BD2_CACFRQ;						   //cacode initial phase
	chnlH->cacphs = (UQUAD)cacode_dely;

	chnlH->cacNco.phs = (cacode_dely - (QUAD)cacode_dely) * pow(2,32);    //cacode NCO initial phase
}

void get_FCW(ChPrms *chnlH,DOUB dt)
{
	DOUB  early_dt;
	DOUB  car_fcw,cac_fcw;

	early_dt = chnlH->loc.dt;
//	early_dt = dt;

	if(chnlH->sattype == TP_GPS)
	{
		car_fcw  = chnlH->carNco.fo/chnlH->carNco.fs + (early_dt-dt)*GPS_CARFRQ/chnlH->carNco.fs/INTEVAL;
		cac_fcw  = chnlH->cacNco.fo/chnlH->cacNco.fs + (early_dt-dt)*GPS_CACFRQ/chnlH->carNco.fs/INTEVAL;
	}
	else
	{
		car_fcw  = chnlH->carNco.fo/chnlH->carNco.fs + (early_dt-dt)*BD2_CARFRQ/chnlH->carNco.fs/INTEVAL;
		cac_fcw  = chnlH->cacNco.fo/chnlH->cacNco.fs + (early_dt-dt)*BD2_CACFRQ/chnlH->carNco.fs/INTEVAL;
	}


	chnlH->carNco.fcw  = (UQUAD)(car_fcw*pow(2,32));
	chnlH->cacNco.fcw  = (UQUAD)(cac_fcw*pow(2,32));

	chnlH->carNco.dphs = (UQUAD)((car_fcw*pow(2,32)-chnlH->carNco.fcw)*INTEVAL*chnlH->carNco.fs);
	chnlH->cacNco.dphs = (UQUAD)((cac_fcw*pow(2,32)-chnlH->cacNco.fcw)*INTEVAL*chnlH->cacNco.fs);
}

//void ref_loc(User_Loc *locH,QUAD i)
//{
//	locH->B += 180.0*100*INTEVAL / ((MA+locH->H)*M_PI)*sin(2*M_PI*i/T/1000);
//	locH->L += 180.0*100*INTEVAL / ((MA+locH->H)*M_PI)*cos(2*M_PI*i/T/1000);
//	LLA2CGS(locH);
//}

void ref_loc(User_Loc *locH,QUAD i)
{
	DOUB N,vx,vy,pow_e;
	//locH->X = 100 * sin(2*M_PI*i/T/1000);
	//locH->Y = 100 * cos(2*M_PI*i/T/1000);
	//locH->Z = locH->Z + INTEVAL * 10;
	vx = sin(2*M_PI*i/T/1000);
	vy = cos(2*M_PI*i/T/1000);
	locH->L = locH->L + vx*0.00001;
	locH->B = locH->B + vy*0.00001;
	pow_e  = F*(2-F);
	N=MA/sqrt(1-pow_e*pow(sin(locH->B),2));
	locH->X=(N+locH->H)*cos(locH->B)*cos(locH->L);
	locH->Y=(N+locH->H)*cos(locH->B)*sin(locH->L);
	locH->Z=(N*(1-pow_e)+locH->H)*sin(locH->B);
}

void adjust_FCW(User_Loc *locH,SrcSys *sysH)
{
	QUAD i;
	DOUB dt;
	UQUAD old_phase;

	for(i=0;i<sysH->SatNM;i++)
	{
		old_phase = sysH->chnls[i].cacNco.phs;
		sysH->chnls[i].carNco.phs += sysH->chnls[i].carNco.dphs;
		sysH->chnls[i].cacNco.phs += sysH->chnls[i].cacNco.dphs;

		if(old_phase > sysH->chnls[i].cacNco.phs)
		{
			sysH->chnls[i].cacphs++;
			if(sysH->chnls[i].cacphs >= sysH->chnls[i].maxcaphs)
			{
				sysH->chnls[i].cacphs -= sysH->chnls[i].maxcaphs;
				sysH->chnls[i].Cntms ++;
				if(sysH->chnls[i].Cntms >= sysH->chnls[i].Maxms)
				{
					sysH->chnls[i].Cntms -= sysH->chnls[i].Maxms;
					sysH->chnls[i].CntNav --;
					if(sysH->chnls[i].navflg == 1)
					{
						sysH->chnls[i].navdata = sysH->chnls[i].navbit[sysH->chnls[i].CntNav];
					}
					if(sysH->chnls[i].CntNav == 0)
					{
						sysH->chnls[i].navflg = 1;
						read_data(FP[i],sysH->chnls[i].navbit);
						sysH->chnls[i].CntNav = 32;
					}
				}
			}
		}

		if(sysH->chnls[i].sattype == TP_GPS)
		{

			//last 2ms satellite location and transport time
			sysH->chnls[i].timer += INTEVAL;
			GPS_get_satLoc(SAT_EPH[i].gps_eph,sysH->chnls[i].timer,&sysH->chnls[i].loc);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			GPS_get_satLoc(SAT_EPH[i].gps_eph,sysH->chnls[i].timer-dt,&sysH->chnls[i].loc);
			rota_modify(&sysH->chnls[i].loc,dt);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			dt = GPS_clock_difference_correction(SAT_EPH[i].gps_eph,sysH->chnls[i],dt);

			get_FCW(&sysH->chnls[i],dt);
			sysH->chnls[i].loc.dt = dt;
		}
		else if(sysH->chnls[i].sattype == TP_GEO)
		{
			//last 2ms satellite location and transport time
			sysH->chnls[i].timer += INTEVAL;
			GEO_get_satLoc(SAT_EPH[i].geo_eph,sysH->chnls[i].timer,&sysH->chnls[i].loc);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			GEO_get_satLoc(SAT_EPH[i].geo_eph,sysH->chnls[i].timer-dt,&sysH->chnls[i].loc);
			rota_modify(&sysH->chnls[i].loc,dt);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			dt = GEO_clock_difference_correction(SAT_EPH[i].geo_eph,sysH->chnls[i],dt);

			get_FCW(&sysH->chnls[i],dt);
			sysH->chnls[i].loc.dt = dt;
		}
		else
		{
			//last 2ms satellite location and transport time
			sysH->chnls[i].timer += INTEVAL;
			MEO_get_satLoc(SAT_EPH[i].meo_eph,sysH->chnls[i].timer,&sysH->chnls[i].loc);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			MEO_get_satLoc(SAT_EPH[i].meo_eph,sysH->chnls[i].timer-dt,&sysH->chnls[i].loc);
			rota_modify(&sysH->chnls[i].loc,dt);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			dt = MEO_clock_difference_correction(SAT_EPH[i].meo_eph,sysH->chnls[i],dt);

			get_FCW(&sysH->chnls[i],dt);
			sysH->chnls[i].loc.dt = dt;
		}
	}
}
