/*
 * bd2_frmfmt.h
 *
 *  Created on: Aug 29, 2013
 *      Author: whc
 */

#ifndef MEO_FMT_H_
#define MEO_FMT_H_

#include "./type.h"

typedef struct MEO_basic{
	BYTE    SID ;
	UQUAD	SOW	;
	UBYTE   SH1 ;
	UBYTE	IOD	;
	UBYTE	URA	;
	UQUAD	WN	;
	UQUAD	toc	;	// + 3
	DOUB	TG1	;	// 0.1
	DOUB	TG2	;	// 0.1
	DOUB	al0	;	// -30
	DOUB	al1	;	// -27  multiply pi
	DOUB	al2	;	// -24  multiply pi * pi
	DOUB	al3	;	// -24  multiply pi * pi *pi
	DOUB	bt0	;	// +11
	DOUB	bt1	;	// +14  multiply pi
	DOUB	bt2	;	// +16	multiply pi * pi
	DOUB	bt3	;	// +16  multiply pi * pi *pi
	DOUB	a2	;	// -33
	DOUB	a0	;	// -50
	DOUB	a1	;	// -66
	UBYTE	AOD	;
	DOUB	dn	;	// -43  divide pi
	DOUB	Cuc	;	// -31
	DOUB	M0	;	// -31  divide pi
	DOUB	e	;	// -33
	DOUB	Cus	;	// -31
	DOUB	Crc	;	// - 6
	DOUB	Crs	;	// - 6
	DOUB	srA	;	// -19
	UQUAD	toe	;	// + 3
	DOUB	i0	;	// -31  divide pi
	DOUB	Cic	;	// -31
	DOUB	Omg	;	// -43  divide pi
	DOUB	Cis	;	// -31
	DOUB	IDT	;	// -43  divide pi
	DOUB	Om0	;	// -31  divide pi
	DOUB	omg	;	// -31  divide pi
} MEO_Bsc;

#endif /* MEO_FMT_H_ */
