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
// Function comment blocks from mischa85/RedAlert2 392e904 src/crcpipe.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// crcpipe.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/crcpipe.json carries the addresses and the rows that
// stay in their own files.


#include "shared/crc_stream.hpp"


/***********************************************************************************************
 * CRCPipe::Put -- Retrieves the data bytes specified and calculates CRC on it.                *
 *                                                                                             *
 *    This routine will fetch the number of bytes requested from the straw. The data is        *
 *    not modified by this straw segment, but it is examined by the CRC engine in order to     *
 *    keep an accurate CRC of the data that passes through this routine.                       *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the buffer that will hold the data requested.               *
 *                                                                                             *
 *          length   -- The number of bytes requested.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of bytes stored into the buffer. If this number is  *
 *          less than the number requested, then this indicates that the data stream has been  *
 *          exhausted.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnknownCRCPipe::Put(void const *source,int length)
{
    Engine(source,length);
    return Pipe::Put(source,length);
}


unsigned long UnknownCRCPipe::Value_00494240()
{
    return Engine.Value();
}
