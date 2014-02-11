#ifndef __BYN2GTX_H__
#define __BYN2GTX_H__

#include <stdio.h>
#include "byn.h"
#include "gtx.h"

/**
 * Writes a BYN file to a GTX file.
 * @param FILE* The byn file.
 * @param FILE* The gtx file.
 * @returns 0 on success.
 */
int byn2gtx(FILE*, FILE*);

int bynhdr2gtxhdr(BynHeader*, GtxHeader*);

#endif
