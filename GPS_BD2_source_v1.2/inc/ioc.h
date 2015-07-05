/*
 * ioc.h
 *
 *  Created on: 2011-11-17
 *      Author: ah
 */

#ifndef IOC_H_
#define IOC_H_

#include <stdio.h>

#include "isrc.h"

#define	I_WGN	CHNLNUM
#define O_DAT	CHNLNUM+1
#define IO_NM	CHNLNUM+2

void open(SrcSys *sysH);
void close();

extern FILE *FP[];

#endif /* IOC_H_ */
