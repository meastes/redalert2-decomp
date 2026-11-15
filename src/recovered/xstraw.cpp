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
// Function comment blocks from mischa85/RedAlert2 392e904 src/xstraw.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// xstraw.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/xstraw.json carries the addresses and the rows that
// stay in their own files.


#include "shared/file_straw.hpp"


/***********************************************************************************************
 * BufferStraw::Get -- Fetch data from the straw's buffer holding tank.                        *
 *                                                                                             *
 *    This routine will copy the requested number of bytes from the buffer holding tank (as    *
 *    set up by the straw's constructor) to the buffer specified.                              *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the buffer to be filled with data.                          *
 *                                                                                             *
 *          length   -- The number of bytes requested.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes copied to the buffer. If this is less than        *
 *          requested, then it indicates that the data holding tank buffer is exhausted.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x007738A0, virtual slot 2)
// Reads from the file, opening it for reading first if it is closed (and
// remembering to close it later). Returns the bytes read, 0 on failure.

int FileStraw::Get(void *buffer, int length)
{
    if (Valid_File() && buffer && length > 0) {
        if (!File->Is_Open()) {
            HasOpened = true;
            if (!File->Is_Available()) {
                return 0;
            }
            if (!File->Open(FILE_RIGHTS_READ)) {
                return 0;
            }
        }
        return File->Read(buffer, length);
    }
    return 0;
}


/***********************************************************************************************
 * FileStraw::~FileStraw -- The destructor for the file straw.                                 *
 *                                                                                             *
 *    This destructor only needs to close the file if it was the one to open it.               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00773900)
// Closes the file if this straw opened it.
// The scalar deleting destructor the linker kept was compiled in another
// object file, where this destructor is called rather than inlined;
// auto_inline(off) reproduces that copy here.
#pragma auto_inline(off)
FileStraw::~FileStraw()
{
    if (Valid_File() && HasOpened) {
        File->Close();
        HasOpened = false;
        File = 0;
    }
}
#pragma auto_inline(on)
