/*
 * main.c
 *
 *  Created on: 2011-11-17
 *      Author: ah
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "./inc/FCW.h"
#include "./inc/frm.h"
#include "./inc/type.h"
#include "./inc/ioc.h"
#include "./inc/iloc.h"
#include "./inc/isrc.h"
#include "./inc/init.h"
#define _USE_MATH_DEFINES
#include <math.h>
QUAD main()
{
	User_Loc  userloc = {0};
	SrcSys    srcsys  = {0};

	QUAD i;
	QUAD  sample_cnt = 0;
	DOUB  noise_data,sample_data = 0;

	ChPrms *chnlH = NULL;

	UQUAD cacphs;
	UWORD nhphs;
	DOUB  carphs;

	UBYTE tem_data,output_data = 0;
	QUAD  output_cnt = 4;

	setbuf(stdout,NULL);
	init(&userloc,&srcsys);							//初始化各通道
	open(&srcsys);

	while(srcsys.tmr < TIMELEN)
	{
		if(sample_cnt >= INTEVAL * SMPFRQ)
		{
			//ref_loc(&userloc,srcsys.tmr);
			adjust_FCW(&userloc,&srcsys);
			sample_cnt = 0;
			srcsys.tmr = srcsys.tmr + 2;
			if((srcsys.tmr%100)==0)
				printf("%d ms is completed!\n",srcsys.tmr);
		}

		for(i=0;i<srcsys.SatNM;i++)
	    {
			chnlH  = &srcsys.chnls[i];
			cacphs = chnlH->cacphs;
			nhphs  = chnlH->Cntms;
			carphs = chnlH->carNco.phs*2.0*M_PI/pow(2,32);
		    sample_data = sample_data+chnlH->amp*chnlH->navdata*chnlH->nhcode[nhphs]*chnlH->cacode[cacphs]*cos(carphs);
			frm_frame(&srcsys.chnls[i],i);
	    }
		fscanf(FP[I_WGN],"%lf",&noise_data);
		if(feof(FP[I_WGN]))
		{
//			fclose(FP[I_WGN]);
//			FP[I_WGN] = fopen("wgn.txt","r");
//			if( FP[I_WGN] == NULL ){
//				printf("can't open wgn.txt");
//				exit( 0 );
//			}
			fseek(FP[I_WGN],0,SEEK_SET);
		}
		sample_data += noise_data;
		sample_cnt++;

		if(sample_data > 5.16*THRESHOLD/4)
			tem_data = 1;
		else if(sample_data > 0)
			tem_data = 0;
		else if(sample_data > -5.16*THRESHOLD/4)
			tem_data = 2;
		else
			tem_data = 3;

		output_cnt --;
		output_data += tem_data << (output_cnt * 2);
		if(output_cnt == 0)
		{
			fwrite(&output_data,1,1,FP[O_DAT]);
			output_data = 0;
			output_cnt  = 4;
		}

		sample_data = 0;
	}

	close();
	return 0;
}
