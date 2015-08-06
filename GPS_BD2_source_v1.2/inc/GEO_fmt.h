/*
 * fmt.h
 *
 *  Created on: Oct 26, 2013
 *      Author: whc
 */

#ifndef GEO_FMT_H_
#define GEO_FMT_H_

#include "type.h"

typedef struct GEO_basic {
	BYTE    SID ;
	UQUAD	SOW	;
	UBYTE   SH1 ;
	UBYTE	IOD	;
	UBYTE	URA	;
	UQUAD	WN	;
	UQUAD	toc	;	// + 3
	DOUB	TG1	;	// 0.1
	DOUB	TG2	;	// 0.1
	UBYTE	A1	;	// 2
	DOUB	al0	;	// -30
	DOUB	al1	;	// -27	multiply pi
	DOUB	al2	;	// -24	multiply pi * pi
	DOUB	al3	;	// -24	multiply pi * pi * pi
	DOUB	bt0	;	// +11
	DOUB	bt1	;	// +14	multiply pi
	DOUB	bt2	;	// +16	multiply pi * pi
	DOUB	bt3	;	// +17	multiply pi * pi * pi
	DOUB	gm0	;	// + 6
	DOUB	gm1	;	// +11	multiply pi * pi *pi
	DOUB	gm2	;	// +14	multiply pi * pi *pi
	DOUB	gm3	;	// +15	multiply pi * pi *pi
	UBYTE	B	;	// 7    multiply pi
	DOUB	a0	;	// -33
	DOUB	a1	;	// -50
	DOUB	a2	;	// -66
	UBYTE	AOD	;
	DOUB	dn	;	// -43	divide pi
	DOUB	Cuc	;	// -31
	DOUB	M0	;	// -31	divide pi
	DOUB	Cus	;	// -31
	DOUB	e	;	// -33
	DOUB	srA	;	// -19
	DOUB	Cic	;	// -31
	DOUB	Cis	;	// -31
	UQUAD	toe	;	// + 3
	DOUB	i0	;	// -31	divide pi
	DOUB	Crc	;	// - 6
	DOUB	Crs	;	// - 6
	DOUB	Omg	;	// -43	divide pi
	DOUB	Om0	;	// -31	divide pi
	DOUB	omg	;	// -31	divide pi
	DOUB	IDT	;	// -43	divide pi
} GEO_Bsc;

#endif /* GEO_FMT_H_ */
