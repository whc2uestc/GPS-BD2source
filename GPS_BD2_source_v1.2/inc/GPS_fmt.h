/*
 * GPS_fmt.h
 *
 *  Created on: 2011-12-17
 *      Author: ah
 */

#ifndef GPS_FMT_H_
#define GPS_FMT_H_

#include "./type.h"

typedef struct GPS_basic{
	BYTE    SID ;
	UQUAD	TOW	;	//6
	UBYTE   ATF ;
	UBYTE	ASF ;
	UWORD   WN  ;
    UBYTE   URA ;
    UBYTE	SVH ;
    UWORD	IODC;
    DOUB	TGD ;	//-31
    QUAD	toc	;	//+4
    DOUB	af2 ;	//-55
    DOUB	af1 ;	//-43
    DOUB	af0	;	//-31
    UBYTE	IOD1;
    DOUB	Crs	;	//-5
    DOUB	dn	;	//-43	divide pi
    DOUB	M0	;	//-31	divide pi
    DOUB	Cuc	;	//-29
    DOUB	e	;	//-33
    DOUB	Cus	;	//-29
    DOUB	srA	;	//-19
    UQUAD	toe	;	//+4
    DOUB	Cic	;	//-29
    DOUB	Omge;	//-31	divide pi
    DOUB	Cis	;	//-29
    DOUB	i0	;	//-31	divide pi
    DOUB	Crc	;	//-5
    DOUB	omg	;	//-31	divide pi
    DOUB	Omgd;	//-43	divide pi
    UBYTE	IOD2;
    DOUB	IDOT;	//-43	divide pi
}GPS_Bsc;

#endif /* GPS_FMT_H_ */
