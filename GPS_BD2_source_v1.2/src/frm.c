/*
 * frm.c
 *
 *  Created on: 2013-11-24
 *      Author: whc
 */
#include <stdlib.h>

#include "../inc/ichnl.h"
#include "../inc/ioc.h"

void read_data(FILE *fp,BYTE bit[32])
{
	QUAD i,navbit;
	fscanf(fp,"%d",&navbit);
	for(i=31;i>=0;i--)
	{
		bit[i] = (navbit>>i) & 0x01;
		bit[i] = bit[i]?-1:1;
	}
}

void frm_frame(ChPrms *chnlH,QUAD ID)
{
	UQUAD old_phase;
	Nco  *ncoH1,*ncoH2;

	ncoH1 = &chnlH->carNco;
	ncoH2 = &chnlH->cacNco;

	ncoH1->phs += ncoH1->fcw;

	old_phase   = ncoH2->phs;
	ncoH2->phs += ncoH2->fcw;
	if(old_phase > ncoH2->phs)
	{
		chnlH->cacphs++;
		if(chnlH->cacphs >= chnlH->maxcaphs)
		{
			chnlH->cacphs -= chnlH->maxcaphs;
			chnlH->Cntms ++;
			if(chnlH->Cntms >= chnlH->Maxms)
			{
				chnlH->Cntms -= chnlH->Maxms;
				chnlH->CntNav --;
				if(chnlH->navflg == 1)
				{
					chnlH->navdata = chnlH->navbit[chnlH->CntNav];
				}
				if(chnlH->CntNav == 0)
				{
					chnlH->navflg = 1;
					read_data(FP[ID],chnlH->navbit);
					chnlH->CntNav = 32;
				}
			}
		}
	}
}
