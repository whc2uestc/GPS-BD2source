/*
 * iloc.h
 *
 *  Created on: 2013-11-18
 *      Author: whc
 */

#ifndef ILOC_H_
#define ILOC_H_

#include "type.h"

typedef struct Loction
		{
			DOUB	dt	;
			DOUB	X	;
			DOUB	Y	;
			DOUB	Z	;
			DOUB	Ek	;
		}Sat_Loc;

typedef struct UserLoc
		{
			DOUB	B	;		//latitude
			DOUB	L	;		//longitude
			DOUB	H	;		//high
			DOUB	X	;
			DOUB	Y	;
			DOUB	Z	;
			DOUB    Vx  ;
			DOUB    Vy  ;
			DOUB    Vz  ;
			DOUB    Ax  ;
			DOUB    Ay  ;
			DOUB    Az  ;
		}User_Loc;
typedef struct angle
		{
			UBYTE ID;
			DOUB El;
			DOUB Az;
			DOUB de;
			DOUB dn;
			DOUB du;
			DOUB X;
			DOUB Y;
			DOUB Z;
			DOUB Ek;
			DOUB dt;
		}Angle;
LDOUB get_Ek(LDOUB e, LDOUB M0);
#endif /* ILOC_H_ */
