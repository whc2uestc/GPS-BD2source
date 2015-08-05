/*
 * MEO_loc.c
 *
 *  Created on: 2013-11-18
 *      Author: whc
 */
#include <math.h>

#include "../inc/CGS.h"
#include "../inc/iloc.h"
#include "../inc/MEO_fmt.h"


void MEO_get_satLoc(MEO_Bsc bsc,LDOUB tsnd,Sat_Loc *loc)
{
	LDOUB A,n,M,E,v,u,r,i,x,y,n0,tk,v1,v2,du,dr,di,phi,OMG;

    LDOUB e,dn,i0,M0,srA,toe,omg,Cus,Cuc,Crs,Crc,Cis,Cic,Omg,idot,Omg0;

	// get parameters from Basic navigation information
	e    = bsc.e	;	 dn   = bsc.dn  ;
    i0   = bsc.i0	;	 M0   = bsc.M0	 ;
    srA  = bsc.srA	;	 toe  = bsc.toe ;
    omg  = bsc.omg	;	 Cus  = bsc.Cus ;
	Cuc  = bsc.Cuc	;	 Crs  = bsc.Crs ;
	Crc  = bsc.Crc	;	 Cis  = bsc.Cis ;
	Cic  = bsc.Cic	;	 Omg  = bsc.Omg ;
	idot = bsc.IDT	;	 Omg0 = bsc.Om0 ;

	// temporary parameters
	A  = srA * srA	;	n0 = sqrt(MU/(A*A*A))	;
	tk = tsnd - toe	;	n  = n0 + dn			;
	tk = ( tk > +302400 ) ? ( tk - 604800 ) :
		 ( tk < -302400 ) ? ( tk + 604800 ) : tk;
	M  = M0  + n*tk	;	E  = get_Ek(e,M)		;
	loc->Ek = E;

	v1 = asin((sqrt(1-e*e)*sin(E))/(1-e*cos(E)));
	v2 = acos((cos(E)-e)/(1-e*cos(E)))			;
	v  = ( v1 > 0 ) ? v2 : -v2					;

	phi = v + omg;
	du = Cus*sin(2*phi) + Cuc*cos(2*phi);
	dr = Crs*sin(2*phi) + Crc*cos(2*phi);
	di = Cis*sin(2*phi) + Cic*cos(2*phi);

	u = phi            + du	;
	r = A*(1-e*cos(E)) + dr	;
	i = i0 + idot * tk + di	;

	x = r * cos(u);
	y = r * sin(u);

	OMG = Omg0 + (Omg-OMGE)*tk - OMGE*toe;

	loc->Z = y*sin(i);
	loc->Y = x*sin(OMG) + y*cos(i)*cos(OMG);
	loc->X = x*cos(OMG) - y*cos(i)*sin(OMG);
}
