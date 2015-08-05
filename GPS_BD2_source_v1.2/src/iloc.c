/*
 * iloc.c
 *
 *  Created on: 2013-11-18
 *      Author: whc
 */

#ifndef ILOC_C_
#define ILOC_C_
#include <math.h>

#include "../inc/type.h"

/****************************************/
/* get E with recursive method			*/
/****************************************/
LDOUB get_Ek(LDOUB e, LDOUB M0)
{
	LDOUB E;
	int i;
	E = M0;
	for (i = 0; i < 10; i++) {
		E = M0 + e * sin(E);
	}
	return E;
}
#endif /* ILOC_C_ */
