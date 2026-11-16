/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/crcstraw.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// crcstraw.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/crcstraw.json carries the addresses and the rows that
// stay in their own files.


#include "shared/crc_stream.hpp"


/***********************************************************************************************
 * CRCStraw::Get -- Fetch the data requested and calculate CRC on it.                          *
 *                                                                                             *
 *    This routine will fetch the number of bytes requested. The data will not be modified     *
 *    by this straw segment, but the CRC engine will examine the data so as to keep an         *
 *    accurate CRC value.                                                                      *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the buffer to hold the data requested.                      *
 *                                                                                             *
 *          length   -- The number of bytes requested.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of bytes stored in the buffer. If this number is    *
 *          less than that requested, then this indicates that the data stream has been        *
 *          exhausted.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int CRCStraw::Get(void *buffer,int length)
{
    if(buffer && length>=1) {
        int received=Straw::Get(buffer,length);
        Engine(buffer,received);
        return received;
    }
    return 0;
}


unsigned long CRCStraw::Result()
{
    return Engine.Value();
}
