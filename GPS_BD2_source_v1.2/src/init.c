/*
 * init.c
 *
 *  Created on: 2011-11-21
 *      Author: ah
 */
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../cfg.h"
#include "../inc/eph.h"
#include "../inc/type.h"
#include "../inc/isrc.h"
#include "../inc/init.h"
#include "../inc/FCW.h"
#include "../inc/GPS_loc.h"
#include "../inc/GEO_loc.h"
#include "../inc/MEO_loc.h"
#include "../inc/LLA2CGS.h"
#include "../inc/iloc.h"



Sat_Eph SAT_EPH[CHNLNUM];

const UBYTE CHNL[CHNLNUM] = {
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
        0,1,2,3,6
};

const UBYTE  CN0[CHNLNUM] = {
		54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,
		54,54,54,54,54
		};

const BYTE	NHCODE[NHLEN] = { +1,+1,+1,+1,+1,-1,+1,+1,-1,-1,+1,-1,+1,-1,+1,+1,-1,-1,-1,+1 };
QUAD num_v = 0;//GPS可见星数  用于选星
Angle SAT_V[GPS_SATNUM]={0};//GPS可见星的参数  用于选星
Angle SAT_SELE[max_num]={0};
QUAD num_sele=0;

void BD2_cacodeGen(UBYTE sat, BYTE code[BD2_CA_LEN] )
{
	UBYTE node[BD2_SATNUM][2] = { // out-node-table for BD2
			{ 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 7 },
			{ 0, 8 }, { 0, 9 }, { 0,10 }, { 1, 6 }, { 2, 3 },
			{ 2, 4 }, { 2, 5 }, { 2, 7 }, { 2, 8 }, { 2, 9 },
			{ 2,10 }, { 3, 4 }, { 3, 5 }, { 3, 7 }, { 3, 8 },
			{ 3, 9 }, { 3,10 }, { 4, 5 }, { 4, 7 }, { 4, 8 },
			{ 4, 9 }, { 4,10 }, { 5, 7 }, { 5, 8 }, { 5, 9 },
			{ 5,10 }, { 7, 8 }, { 7, 9 }, { 7,10 }, { 8, 9 },
			{ 8,10 }, { 9,10 }
	};

	QUAD j, k;
	BYTE save1, save2;

	BYTE reg1[11] = { 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1 };
	BYTE reg2[11] = { 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1 };

	for (j=0;j<BD2_CA_LEN;j++) {
		code[j] = reg2[node[sat][0]] * reg2[node[sat][1]] * reg1[10];
		// update G1
		save1 = reg1[0] * reg1[6] * reg1[7] * reg1[8] * reg1[9] * reg1[10];
		for (k=9;k>=0;k--) // reg1 shift to the right
			reg1[k+1] = reg1[k];
		reg1[0] = save1;
		// update G2
		save2 = reg2[0]*reg2[1]*reg2[2]*reg2[3]*reg2[4]*reg2[7]*reg2[8]*reg2[10];
		for (k=9;k>=0;k--) // reg2 shift to the right
			reg2[k+1] = reg2[k];
		reg2[0] = save2;
	}
}

void GPS_cacodeGen( UBYTE sat, BYTE code[GPS_CA_LEN] )
{
	UBYTE node[GPS_SATNUM][2] ={
		    { 1, 5 },{ 2, 6 },{ 3, 7 },{ 4, 8 },{ 0, 8 },
		    { 1, 9 },{ 0, 7 },{ 1, 8 },{ 2, 9 },{ 1, 2 },
		    { 2, 3 },{ 4, 5 },{ 5, 6 },{ 6, 7 },{ 7, 8 },
			{ 8, 9 },{ 0, 3 },{ 1, 4 },{ 2, 5 },{ 3, 6 },
			{ 4, 7 },{ 5, 8 },{ 0, 2 },{ 3, 5 },{ 4, 6 },
		    { 5, 7 },{ 6, 8 },{ 7, 9 },{ 0, 5 },{ 1, 6 },
			{ 2, 7 },{ 3, 8 },{ 4, 9 },{ 3, 9 },{ 0, 6 },
			{ 0, 7 },{ 3, 9 }
	};

	QUAD j, k;
	BYTE save1, save2;

	BYTE reg1[10] ={ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	BYTE reg2[10] ={ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

	for (j = 0; j < 1023; j++) {
		code[j] = reg2[node[sat][0]] * reg2[node[sat][1]] * reg1[9];
        //update G1
		save1 = reg1[2] * reg1[9];
        for (k = 8; k >= 0; k--) {
			reg1[k + 1] = reg1[k];
		}
		reg1[0] = save1;
        //update G2
		save2 = reg2[1] * reg2[2] * reg2[5] * reg2[7] * reg2[8] * reg2[9];
		for (k = 8; k >= 0; k--) {
			reg2[k + 1] = reg2[k];
		}
		reg2[0] = save2;
	}

}

void judge_visibleness(User_Loc *locH,SrcSys *sysH)
{
	QUAD i;
	DOUB xi,yi,zi,theta,lamda,de,dn,du,El,Az;
	DOUB EA = 5.0 ;
	Sat_Loc temploc = {0};

    QUAD gps_cnt = 0;
    QUAD geo_cnt = 0;
    QUAD meo_cnt = 0;


	
	
	sysH->SatNM = 0;
	for(i=0;i<CHNLNUM;i++)
	{
		if(SATTYPE[i] == TP_GPS)
		{
			GPS_get_satLoc(GPS_EPH[gps_cnt],TIMEBGN,&temploc);
		}
		else if(SATTYPE[i] == TP_GEO)
		{
			GEO_get_satLoc(GEO_EPH[geo_cnt],TIMEBGN,&temploc);
		}
		else
		{
			MEO_get_satLoc(MEO_EPH[meo_cnt],TIMEBGN,&temploc);
		}

		theta = locH->B / 180.0 * PI ;
		lamda = locH->L / 180.0 * PI ;

		xi = temploc.X - locH->X ;
		yi = temploc.Y - locH->Y ;
		zi = temploc.Z - locH->Z ;

		de = -xi * sin(lamda)              + yi * cos(lamda) ;
		dn = -xi * sin(theta) * cos(lamda) - yi * sin(theta) * sin(lamda) + zi * cos(theta);
		du =  xi * cos(theta) * cos(lamda) + yi * cos(theta) * sin(lamda) + zi * sin(theta);

		El = asin(du / sqrt(de * de + dn * dn + du * du));
		Az = atan(de / dn);

		El = El * 180.0 / PI ;
		Az = Az * 180.0 / PI ;

		if(de > 0 && dn > 0)
		{
		}
		else if(de < 0 && dn > 0)
		{
			Az = Az + 360;
		}
		else
		{
			Az = Az + 180;
		}
		if(El > EA)
		{
			if(flag_sele_sate==0)
			{
				if(SATTYPE[i] == TP_GPS)
				{
					memcpy(&SAT_EPH[sysH->SatNM],&GPS_EPH[gps_cnt],sizeof(GPS_Bsc));
				}
				else if(SATTYPE[i] == TP_GEO)
				{
					memcpy(&SAT_EPH[sysH->SatNM],&GEO_EPH[geo_cnt],sizeof(GEO_Bsc));
				}
				else
				{
					memcpy(&SAT_EPH[sysH->SatNM],&MEO_EPH[meo_cnt],sizeof(MEO_Bsc));
				}
				sysH->chnls[sysH->SatNM].sattype = SATTYPE[i];
				sysH->chnls[sysH->SatNM].satID   = CHNL[i];
				sysH->chnls[sysH->SatNM].amp     = sqrt(2*pow(10,(CN0[i]-204)/10));
				sysH->SatNM ++;
			}
			if(flag_sele_sate == 1)
			{
				if(SATTYPE[i] == TP_GPS)
				{	
					SAT_V[num_v].ID=GPS_EPH[gps_cnt].SID;
					SAT_V[num_v].El=El;
					SAT_V[num_v].Az=Az;
					SAT_V[num_v].de=de;
					SAT_V[num_v].dn=dn;
					SAT_V[num_v].du=du;
					SAT_V[num_v].X=temploc.X;
					SAT_V[num_v].Y=temploc.Y;
					SAT_V[num_v].Z=temploc.Z;
					SAT_V[num_v].Ek=temploc.Ek;
					SAT_V[num_v].dt=temploc.dt;
					num_v=num_v+1;

				}
				else if(SATTYPE[i] == TP_GEO)
				{
					memcpy(&SAT_EPH[sysH->SatNM],&GEO_EPH[geo_cnt],sizeof(GEO_Bsc));
					sysH->chnls[sysH->SatNM].sattype = SATTYPE[i];
					sysH->chnls[sysH->SatNM].satID   = CHNL[i];
					sysH->chnls[sysH->SatNM].amp     = sqrt(2*pow(10,(CN0[i]-204)/10));
					sysH->SatNM ++;
				}
				else
				{
					memcpy(&SAT_EPH[sysH->SatNM],&MEO_EPH[meo_cnt],sizeof(MEO_Bsc));
					sysH->chnls[sysH->SatNM].sattype = SATTYPE[i];
					sysH->chnls[sysH->SatNM].satID   = CHNL[i];
					sysH->chnls[sysH->SatNM].amp     = sqrt(2*pow(10,(CN0[i]-204)/10));
					sysH->SatNM ++;
				}
				
			}
			
			
		}

		if(SATTYPE[i] == TP_GPS)
		{
			gps_cnt ++ ;
		}
		else if(SATTYPE[i] == TP_GEO)
		{
			geo_cnt ++ ;
		}
		else
		{
			meo_cnt ++ ;
		}
	}
	

}

void init(User_Loc *locH,SrcSys *sysH)
{
	QUAD i,j;
	DOUB dt;
	

	locH->B  = USERB;
	locH->L  = USERL;
	locH->H  = USERH;


	LLA2CGS(locH);

	judge_visibleness(locH,sysH);
	sele_sate(0);
	for(i=0;i<num_sele;i++)
	{
		for(j=0;j<32;j++)
		{
			if(GPS_EPH[j].SID==SAT_SELE[i].ID)
			{
				memcpy(&SAT_EPH[sysH->SatNM],&GPS_EPH[j],sizeof(GPS_Bsc));
				sysH->chnls[sysH->SatNM].sattype = SATTYPE[j];
				sysH->chnls[sysH->SatNM].satID   = CHNL[j];
				sysH->chnls[sysH->SatNM].amp     = sqrt(2*pow(10,(CN0[j]-204)/10));
				sysH->SatNM++;
			}
		}
	}


	for(i=0;i<sysH->SatNM;i++)
	{
		sysH->chnls[i].timer = TIMEBGN;
		sysH->chnls[i].navdata = 1;

		sysH->chnls[i].carNco.fs = SMPFRQ;
		sysH->chnls[i].carNco.fo = INTFRQ;
		sysH->chnls[i].cacNco.fs = SMPFRQ;

		if(sysH->chnls[i].sattype == TP_GPS)
		{
			sysH->chnls[i].cacNco.fo = GPS_CACFRQ;
			sysH->chnls[i].Maxms     = GPS_SMP_BIT;
			sysH->chnls[i].maxcaphs  = GPS_CA_LEN;
			memset(sysH->chnls[i].nhcode,-1,NHLEN);

			GPS_cacodeGen(sysH->chnls[i].satID,sysH->chnls[i].cacode);

			//t = 0ms satellite location and transport time
			GPS_get_satLoc(SAT_EPH[i].gps_eph,sysH->chnls[i].timer,&sysH->chnls[i].loc);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			GPS_get_satLoc(SAT_EPH[i].gps_eph,sysH->chnls[i].timer-dt,&sysH->chnls[i].loc);
			rota_modify(&sysH->chnls[i].loc,dt);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			dt = GPS_clock_difference_correction(SAT_EPH[i].gps_eph,sysH->chnls[i],dt);
			sysH->chnls[i].loc.dt = dt;
			sysH->chnls[i].timer += INTEVAL;
			GPS_get_initial_phase(&sysH->chnls[i],GPS_SMP_BIT/1000.0);

			//t = 2ms satellite location and transport time
			GPS_get_satLoc(SAT_EPH[i].gps_eph,sysH->chnls[i].timer,&sysH->chnls[i].loc);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			GPS_get_satLoc(SAT_EPH[i].gps_eph,sysH->chnls[i].timer-dt,&sysH->chnls[i].loc);
			rota_modify(&sysH->chnls[i].loc,sysH->chnls[i].loc.dt);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			dt = GPS_clock_difference_correction(SAT_EPH[i].gps_eph,sysH->chnls[i],dt);

			get_FCW(&sysH->chnls[i],dt);
			sysH->chnls[i].loc.dt = dt;
		}
		else if(sysH->chnls[i].sattype == TP_GEO)
		{
			sysH->chnls[i].cacNco.fo = BD2_CACFRQ;
			sysH->chnls[i].Maxms     = GEO_SMP_BIT;
			sysH->chnls[i].maxcaphs  = BD2_CA_LEN;
			memset(sysH->chnls[i].nhcode,-1,NHLEN);

			BD2_cacodeGen(sysH->chnls[i].satID,sysH->chnls[i].cacode);

			//t = 0ms satellite location and transport time
			GEO_get_satLoc(SAT_EPH[i].geo_eph,sysH->chnls[i].timer,&sysH->chnls[i].loc);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			GEO_get_satLoc(SAT_EPH[i].geo_eph,sysH->chnls[i].timer-dt,&sysH->chnls[i].loc);
			rota_modify(&sysH->chnls[i].loc,dt);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			dt = GEO_clock_difference_correction(SAT_EPH[i].geo_eph,sysH->chnls[i],dt);
			sysH->chnls[i].loc.dt = dt;
			sysH->chnls[i].timer += INTEVAL;
			BD2_get_initial_phase(&sysH->chnls[i],GEO_SMP_BIT/1000.0);

			//t = 2ms satellite location and transport time
			GEO_get_satLoc(SAT_EPH[i].geo_eph,sysH->chnls[i].timer,&sysH->chnls[i].loc);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			GEO_get_satLoc(SAT_EPH[i].geo_eph,sysH->chnls[i].timer-dt,&sysH->chnls[i].loc);
			rota_modify(&sysH->chnls[i].loc,sysH->chnls[i].loc.dt);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			dt = GEO_clock_difference_correction(SAT_EPH[i].geo_eph,sysH->chnls[i],dt);

			get_FCW(&sysH->chnls[i],dt);
			sysH->chnls[i].loc.dt = dt;
		}
		else
		{
			sysH->chnls[i].cacNco.fo = BD2_CACFRQ;
			sysH->chnls[i].Maxms     = MEO_SMP_BIT;
			sysH->chnls[i].maxcaphs  = BD2_CA_LEN;
			memcpy(sysH->chnls[i].nhcode,NHCODE,NHLEN*sizeof(BYTE));

			BD2_cacodeGen(sysH->chnls[i].satID,sysH->chnls[i].cacode);

			//t = 0ms satellite location and transport time
			MEO_get_satLoc(SAT_EPH[i].meo_eph,sysH->chnls[i].timer,&sysH->chnls[i].loc);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			MEO_get_satLoc(SAT_EPH[i].meo_eph,sysH->chnls[i].timer-dt,&sysH->chnls[i].loc);
			rota_modify(&sysH->chnls[i].loc,dt);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			dt = MEO_clock_difference_correction(SAT_EPH[i].meo_eph,sysH->chnls[i],dt);
			sysH->chnls[i].loc.dt = dt;
			sysH->chnls[i].timer += INTEVAL;
			BD2_get_initial_phase(&sysH->chnls[i],MEO_SMP_BIT/1000.0);

			//t = 2ms satellite location and transport time
			MEO_get_satLoc(SAT_EPH[i].meo_eph,sysH->chnls[i].timer,&sysH->chnls[i].loc);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			MEO_get_satLoc(SAT_EPH[i].meo_eph,sysH->chnls[i].timer-dt,&sysH->chnls[i].loc);
			rota_modify(&sysH->chnls[i].loc,sysH->chnls[i].loc.dt);
			dt = GET_DT(locH,sysH->chnls[i].loc);
			dt = MEO_clock_difference_correction(SAT_EPH[i].meo_eph,sysH->chnls[i],dt);

			get_FCW(&sysH->chnls[i],dt);
			sysH->chnls[i].loc.dt = dt;
		}
	}
}









void sele_sate(QUAD count_num)
{
	Angle SAT_h[max_num]={0},SAT_m[max_num]={0},SAT_l[max_num]={0};
	Angle cvs={0};
	QUAD num_h=0,num_m=0,num_l=0,dele_ID,dele_chnl,flag_sat_sele;
	QUAD i,j,k;
	DOUB gdop,gdop_min;
	DOUB subAz,subAz_max=0,sub_min,subEl,sub,Az;

	for(i=0;i<num_v;i++)
	{
/* * * * *     按仰角大小分为高中低仰角区     * * * * */
		if(75.0<SAT_V[i].El && SAT_V[i].El<=90.0)
		{
			
			SAT_h[num_h].ID=SAT_V[i].ID;
			SAT_h[num_h].El=SAT_V[i].El;
			SAT_h[num_h].Az=SAT_V[i].Az;
			SAT_h[num_h].de=SAT_V[i].de;
			SAT_h[num_h].dn=SAT_V[i].dn;
			SAT_h[num_h].du=SAT_V[i].du;
			SAT_h[num_h].X=SAT_V[i].X;
			SAT_h[num_h].Y=SAT_V[i].Y;
			SAT_h[num_h].Z=SAT_V[i].Z;
			SAT_h[num_h].Ek=SAT_V[i].Ek;
			SAT_h[num_h].dt=SAT_V[i].dt;
			num_h=num_h+1;
		}
		else if(40.0<SAT_V[i].El && SAT_V[i].El<=75.0)
		{
			
			SAT_m[num_m].ID=SAT_V[i].ID;
			SAT_m[num_m].El=SAT_V[i].El;
			SAT_m[num_m].Az=SAT_V[i].Az;
			SAT_m[num_m].de=SAT_V[i].de;
			SAT_m[num_m].dn=SAT_V[i].dn;
			SAT_m[num_m].du=SAT_V[i].du;
			SAT_m[num_m].X=SAT_V[i].X;
			SAT_m[num_m].Y=SAT_V[i].Y;
			SAT_m[num_m].Z=SAT_V[i].Z;
			SAT_m[num_m].Ek=SAT_V[i].Ek;
			SAT_m[num_m].dt=SAT_V[i].dt;
			num_m=num_m+1;
		}
		else if(SAT_V[i].El<=40.0)
		{
			
			SAT_l[num_l].ID=SAT_V[i].ID;
			SAT_l[num_l].El=SAT_V[i].El;
			SAT_l[num_l].Az=SAT_V[i].Az;
			SAT_l[num_l].de=SAT_V[i].de;
			SAT_l[num_l].dn=SAT_V[i].dn;
			SAT_l[num_l].du=SAT_V[i].du;
			SAT_l[num_l].X=SAT_V[i].X;
			SAT_l[num_l].Y=SAT_V[i].Y;
			SAT_l[num_l].Z=SAT_V[i].Z;
			SAT_l[num_l].Ek=SAT_V[i].Ek;
			SAT_l[num_l].dt=SAT_V[i].dt;
			num_l=num_l+1;
		}
	}
/* * * * *   将所有可见星按仰角大小排序 大者在前   * * * * */
	for(i=0;i<num_h;i++)
	{
		for(j=i;j<num_h;j++)
		{
			if(SAT_h[j].El>SAT_h[i].El)
			{
				cvs=SAT_h[j];
				SAT_h[j]=SAT_h[i];
				SAT_h[i]=cvs;
			}
		}
	}
	for(i=0;i<num_m;i++)
	{
		for(j=i;j<num_m;j++)
		{
			if(SAT_m[j].El>SAT_m[i].El)
			{
				cvs=SAT_m[j];
				SAT_m[j]=SAT_m[i];
				SAT_m[i]=cvs;
			}
		}
	}
	for(i=0;i<num_l;i++)
	{
		for(j=i;j<num_l;j++)
		{
			if(SAT_l[j].El>SAT_l[i].El)
			{
				cvs=SAT_l[j];
				SAT_l[j]=SAT_l[i];
				SAT_l[i]=cvs;
			}
		}
	}
	for(i=0;i<num_h;i++)
		SAT_V[i]=SAT_h[i];
	for(i=num_h;i<num_h+num_m;i++)
		SAT_V[i]=SAT_m[i-num_h];
	for(i=num_h+num_m;i<num_h+num_m+num_l;i++)
		SAT_V[i]=SAT_l[i-num_h-num_m];

	if(count_num==0)//选星
	{
		if(num_v<=max_num)
		{
			for(i=0;i<num_v;i++)
				memcpy(&SAT_SELE[i],&SAT_V[i],sizeof(Angle));
			num_sele=num_v;
			gdop=cal_gdop();
		}
		else if(num_v>max_num)
		{
			SAT_SELE[0]=SAT_V[0];//高仰角选星
			if(num_h==0)
				k=0;
			else
				k=1;
			if(SAT_SELE[0].El<80.0 && num_h>1)
			{
				for(i=0;i<num_h;i++)
				{
					subAz=fabs(SAT_SELE[0].Az-SAT_h[i].Az);
					if(subAz>180.0)
						subAz=360.0-subAz;
					if(subAz>30.0 && subAz>subAz_max)
					{
						subAz_max=subAz;
						SAT_SELE[1]=SAT_h[i];
						k=2;
					}
				}
			}


			if((num_h!=0 && num_m<=kz)||(num_h==0 && num_m<=kz+1))//中仰角选星
			{
				for(i=0;i<num_m;i++)
				{
					for(j=i;j<num_m;j++)
					{
						subAz=fabs(SAT_m[i].Az-SAT_m[j].Az);
						if(subAz>180.0)
							subAz=360.0-subAz;
						subEl=fabs(SAT_m[i].El-SAT_m[j].El);
						sub_min=subAz+subEl;
						if(sub_min<30.0 && SAT_m[i].ID!=SAT_m[j].ID)
							SAT_m[i].ID=0;
						if(SAT_m[i].ID==0) 
							break;
					}
					if(SAT_m[i].ID==0) 
							break;
				}
				for(i=k;i<k+num_m;i++)
				{
					if(SAT_m[i-k].ID!=0)
					{
						SAT_SELE[i]=SAT_m[i-k];
					}
				}
				k=k+num_m-1;
			}
			if((num_h!=0 && num_m>kz)||(num_h==0 && num_m>kz+1))
			{
				for(i=k,sub_min=150.0;i<k+num_m;i++)
				{
					subAz=fabs(SAT_m[i-k].Az-SAT_SELE[0].Az);
					if(subAz>180.0)
						subAz=360.0-subAz;
					sub=fabs(30.0-subAz);
					if(sub<sub_min)
					{
						sub_min=sub;
						SAT_SELE[k]=SAT_m[i-k];
					}
				}
				for(i=1;i<kz;i++)
				{
					Az=SAT_SELE[k].Az+360.0*i/kz;
					if(Az>360.0)
						Az=Az-360.0;
					for(j=0,sub_min=180.0;j<num_m;j++)
					{
						if(SAT_SELE[k].ID!=SAT_l[j].ID)
						{
							subAz=fabs(Az-SAT_m[j].Az);
							if(subAz>180.0)
								subAz=360.0-subAz;
							if(subAz<sub_min)
							{
								sub_min=subAz;
								SAT_SELE[k+i]=SAT_m[j];
							}
						}
					}
				}
				k=k+kz;
			}
			

			if(((num_h!=0 || num_m!=0)&&num_l<=kd)||(num_h==0&&num_m==0&&num_l<=kd+1))//低仰角选星
			{
				for(i=0;i<num_l;i++)
				{
					for(j=i;j<num_l;j++)
					{
						subAz=fabs(SAT_l[i].Az-SAT_l[j].Az);
						if(subAz>180.0)
							subAz=360.0-subAz;
						subEl=fabs(SAT_l[i].El-SAT_l[j].El);
						sub_min=subAz+subEl;
						if(sub_min<30.0 && SAT_l[i].ID!=SAT_l[j].ID)
							SAT_l[i].ID=0;
						if(SAT_l[i].ID==0) 
							break;
					}
					if(SAT_l[i].ID==0) 
							break;
				}
				for(i=k;i<k+num_l;i++)
				{
					if(SAT_l[i-k].ID!=0)
					{
						SAT_SELE[i]=SAT_l[i-k];
					}
				}
			}
			if(((num_h!=0 || num_m!=0)&&num_l>kd)||(num_h==0&&num_m==0&&num_l>kd+1))
			{
				for(i=k,sub_min=180.0;i<k+num_l;i++)
				{
					subAz=fabs(SAT_l[i-k].Az-SAT_SELE[0].Az);
					if(subAz>180.0)
						subAz=360.0-subAz;
					sub=fabs(30.0-subAz);
					if(sub<sub_min)
					{
						sub_min=sub;
						SAT_SELE[k]=SAT_l[i-k];
					}
				}
				for(i=1;i<kd;i++)
				{
					Az=SAT_SELE[k].Az+360.0*i/kz;
					if(Az>360.0)
						Az=Az-360.0;
					for(j=0,sub_min=180.0;j<num_l;j++)
					{
						if(SAT_SELE[k].ID!=SAT_l[j].ID)
						{
							subAz=fabs(Az-SAT_l[j].Az);
							if(subAz>180.0)
								subAz=360.0-subAz;
							if(subAz<sub_min)
							{
								sub_min=subAz;
								SAT_SELE[k+i]=SAT_l[j];
							}
						}
					}
				}
			}
			for(i=0;i<max_num;i++)
			{
				if(SAT_SELE[i].ID!=0)
				{
					for(j=i+1;j<max_num;j++)
					{
						if(SAT_SELE[i].ID==SAT_SELE[j].ID)
						{
							SAT_SELE[j].ID=0;
							SAT_SELE[j].El=0;
							SAT_SELE[j].Az=0;
							SAT_SELE[j].de=0;
							SAT_SELE[j].dn=0;
							SAT_SELE[j].du=0;
							SAT_SELE[j].X=0;
							SAT_SELE[j].Y=0;
							SAT_SELE[j].Z=0;
							SAT_SELE[j].dt=0;
						}
					}
				}
			}

			for(i=0;i<max_num;i++)
			{
				if(SAT_SELE[i].ID!=0)
					num_sele=num_sele+1;//入选卫星数
			}

			if(num_sele<4)
				exit(0);
			else
			{
				gdop=cal_gdop();
				for(j=0;((gdop>Threshold&&num_sele<max_num)||(gdop>Threshold*1.05&&num_sele==max_num)||(num_sele<8))&&(j<=max_num-num_sele);j++)
				{
					if(num_sele<max_num)//增选
					{
						add_sele();
						gdop=cal_gdop();
					}
					else if(num_sele==max_num)//改选
					{
						for(i=1,dele_chnl=2;i<num_sele;i++,dele_chnl++)
						{
							if(SAT_SELE[i].El<=75.0)
							{
								for(k=i+dele_chnl;k<max_num;k++)
								{
									subAz=fabs(SAT_SELE[i].Az-SAT_V[k].Az);
									if(subAz>180.0)
										subAz=360.0-subAz;
									if(subAz<sub_min)
									{
										sub_min=subAz;
										if(SAT_SELE[i].El>SAT_V[k].El)
											cvs=SAT_SELE[i];
										else
											cvs=SAT_V[k];
									}
								}
							}
						}
						for(i=0;i<max_num;i++)
						{
							if(cvs.ID==SAT_SELE[i].ID)
							{
								dele_chnl=i;
								dele_ID=SAT_SELE[i].ID;
								break;
							}
						}
						for(i=0,gdop_min=gdop,cvs=SAT_SELE[dele_chnl];i<num_v;i++)
						{
							if(SAT_V[i].ID==dele_ID)
							{
								flag_sat_sele=0;
								continue;
							}
							for(j=0;j<max_num;j++)
							{
								if(j==dele_chnl)
									continue;
								if(SAT_V[i].ID==SAT_SELE[j].ID)
								{
									flag_sat_sele=1;
									break;
								}
							}
							if(flag_sat_sele==0)
							{
								SAT_SELE[dele_chnl]=SAT_V[i];
								gdop=cal_gdop();
								if(gdop<gdop_min)
								{
									cvs=SAT_V[i];
									gdop_min=gdop;
								}
							}
							flag_sat_sele=0;
						}
						SAT_SELE[dele_chnl]=cvs;
						gdop=cal_gdop();
					}
				}
			}



		}
	}	
}

DOUB cal_gdop()
{
	DOUB gdop;
	DOUB gdop_a[max_num][4]={0},gdop_b[4][max_num]={0},gdop_c[4][max_num]={0},gdop_d[4][max_num]={0};
	QUAD i,j,k,m,n;
	for(i=0;i<num_sele;)//构建方向余弦矩阵H
	{
		if(SAT_SELE[i].ID!=0)
		{
			gdop_a[i][0]=SAT_SELE[i].de/sqrt(pow(SAT_SELE[i].de,2.0)+pow(SAT_SELE[i].dn,2.0)+pow(SAT_SELE[i].du,2.0));
			gdop_a[i][1]=SAT_SELE[i].dn/sqrt(pow(SAT_SELE[i].de,2.0)+pow(SAT_SELE[i].dn,2.0)+pow(SAT_SELE[i].du,2.0));
			gdop_a[i][2]=SAT_SELE[i].du/sqrt(pow(SAT_SELE[i].de,2.0)+pow(SAT_SELE[i].dn,2.0)+pow(SAT_SELE[i].du,2.0));
			gdop_a[i][3]=1.0;
			i++;
		}
	}
	for(i=0;i<num_sele;i++)//求方向余弦矩阵H的转置
	{
		for(j=0;j<4;j++)
		{
			gdop_b[j][i]=gdop_a[i][j];
		}
	}
	for(m=0;m<4;m++)
	{
		for(n=0;n<4;n++)
		{
			for(i=0;i<num_sele;i++)
				gdop_c[m][n]=gdop_c[m][n]+gdop_b[m][i]*gdop_a[i][n];
		}
	}
	//对矩阵gdop_c求逆
	for(m=0;m<4;m++)
	{
		for(n=4;n<max_num;n++)
		{
			if(n-m==4)
				gdop_c[m][n]=1.0;
		}
	}
	for(m=0;m<4;m++)
	{
		for(k=0;k<4;k++)
		{
			if(k!=m)
			{
				for(n=0;n<max_num;n++)
					gdop_d[k][n]=gdop_c[k][n]-gdop_c[k][m]*(gdop_c[m][n]/gdop_c[m][m]);
			}
			else
			{
				for(n=0;n<max_num;n++)
					gdop_d[m][n]=gdop_c[m][n]/gdop_c[m][m];
			}
		}
		for(i=0;i<4;i++)
		{
			for(j=0;j<max_num;j++)
				gdop_c[i][j]=gdop_d[i][j];
		}
	}
	gdop=sqrt(gdop_d[0][4]+gdop_d[1][5]+gdop_d[2][6]+gdop_d[3][7]);//gdop_d对角线之后开方即为GDOP
	return gdop;

}

void add_sele()
{
	QUAD k,i,j,flag_add_sele=0;
	DOUB subAz;
	
	for(k=20;k>=0;k=k-5)
	{
		for(i=0;i<num_v;i++)
		{
			for(j=0,subAz=180.0;j<max_num;j++)
			{
				if(SAT_SELE[i].ID!=0)
				{
					subAz=fabs(SAT_V[i].Az-SAT_SELE[j].Az);
					if(subAz>180.0)
						subAz=360.0-subAz;
				}
				if(subAz<=k) break;
			}
			if(subAz>k)
			{
				for(j=0;j<max_num;j++)
				{
					if(SAT_SELE[j].ID==0)
					{
						SAT_SELE[j]=SAT_V[i];
						break;
					}
				}
				flag_add_sele=1;
				num_sele=num_sele+1;
			}
			if(flag_add_sele==1) break;
		}
		if(flag_add_sele==1) break;
	}
}
