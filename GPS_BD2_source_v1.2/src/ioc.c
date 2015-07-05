/*
 * ioc.c
 *
 *  Created on: 2011-11-17
 *      Author: ah
 */
#include <stdio.h>
#include <stdlib.h>

#include "../cfg.h"
#include "../inc/ioc.h"
#include "../inc/type.h"

FILE *FP[IO_NM];

BYTE FILES_PATH[CHNLNUM][20] ={
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt",
								  "navbit/XXX_NOXX.txt"
							  };

void open(SrcSys *sysH)
{
	QUAD i;

	for(i=0;i<sysH->SatNM;i++)
	{
		if(sysH->chnls[i].sattype == TP_GPS)
		{
			FILES_PATH[i][7 ] = 'G';
			FILES_PATH[i][8 ] = 'P';
			FILES_PATH[i][9 ] = 'S';
			FILES_PATH[i][13] = (QUAD)(sysH->chnls[i].satID / 10) +'0';
			FILES_PATH[i][14] = (QUAD)(sysH->chnls[i].satID % 10) +'0';

			FP[i] = fopen(FILES_PATH[i],"r");
			if( FP[i] == NULL ){
				printf("can't open files");
				exit( 0 );
			}
		}
		else
		{
			FILES_PATH[i][7 ] = 'B';
			FILES_PATH[i][8 ] = 'D';
			FILES_PATH[i][9 ] = '2';
			FILES_PATH[i][13] = (QUAD)(sysH->chnls[i].satID / 10) +'0';
			FILES_PATH[i][14] = (QUAD)(sysH->chnls[i].satID % 10) +'0';

			FP[i] = fopen(FILES_PATH[i],"r");
			if( FP[i] == NULL ){
				printf("can't open files");
				exit( 0 );
			}
		}
	}

	FP[I_WGN] = fopen("wgn.txt","r");
	if( FP[I_WGN] == NULL ){
		printf("can't open wgn.txt");
		exit( 0 );
	}

	FP[O_DAT] = fopen("E:/luoxuan.bin","wb");
	if( FP[O_DAT] == NULL ){
		printf("can't creat data.txt");
		exit( 0 );
	}
	setbuf(FP[O_DAT],NULL);
}

void close()
{
   fclose(FP[O_DAT]);fflush(FP[O_DAT]);
}
