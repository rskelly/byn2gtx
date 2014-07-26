#ifndef __BYN2GTX_H__
#define __BYN2GTX_H__

#include <stdio.h>
#include "byn.h"
#include "gtx.h"

/**
 * Writes a BYN file to a GTX file.
 * @param The byn file.
 * @param The gtx file.
 * @returns 1 on success, 0 on failure.
 */
int byn2gtx(FILE*, FILE*);

/**
 * Writes the contents of the BynHeader into the GtxHeader.
 * @param A pointer to a BynHeader.
 * @param A pointer to a GtxHeader.
 * @return1 1 on success, 0 on failure.
 */
int bynhdr2gtxhdr(BynHeader*, GtxHeader*);

#endif
