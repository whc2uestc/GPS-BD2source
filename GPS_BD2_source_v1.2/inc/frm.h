/*
 * frm.h
 *
 *  Created on: 2013-11-24
 *      Author: whc
 */

#ifndef FRM_H_
#define FRM_H_

#include "ichnl.h"

void frm_frame(ChPrms *chnlH,QUAD ID);
void read_data(FILE *fp,BYTE bit[32]);

#endif /* FRM_FRAME_H_ */
