/*
 * LLA2CGS.c
 *
 *  Created on: 2011-11-21
 *      Author: ah
 */
#include "math.h"

#include "../inc/iloc.h"
#include "../inc/CGS.h"

void LLA2CGS(User_Loc *loc)
{
	DOUB pow_e,N,b,l,h;

	b = loc->B/360.0*2*PI;			//lastitude
	l = loc->L/360.0*2*PI;			//longitude
	h = loc->H;						//high

	pow_e  = F*(2-F);
	N      = MA / sqrt(1-pow_e*sin(b)*sin(b));
	loc->X = (N+h)*cos(b)*cos(l);
	loc->Y = (N+h)*cos(b)*sin(l);
	loc->Z = (N*(1-pow_e)+h)*sin(b);
}
