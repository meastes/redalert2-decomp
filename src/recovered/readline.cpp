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
// Function comment blocks from mischa85/RedAlert2 392e904 src/readline.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// readline.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/readline.json carries the addresses and the rows that
// stay in their own files.


#include "shared/reference_counted.hpp"
#include "shared/file_straw.hpp"
#include "shared/crt_string.hpp"
#include "shared/straw.hpp"


/***********************************************************************************************
 * Read_Line -- Read a text line from the file.                                                *
 *                                                                                             *
 *    This will read a line of text from the file specified.                                   *
 *                                                                                             *
 * INPUT:   file  -- Reference to the file to read the line from.                              *
 *                                                                                             *
 *          buffer   -- Pointer to the location to store the line of text.                     *
 *                                                                                             *
 *          len   -- The length of the buffer to store the line.                               *
 *                                                                                             *
 *          eof   -- Reference to the EOF flag that will be set to true if the source          *
 *                   file has been exhausted.                                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the number of characters stored into the buffer.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/06/1997 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006395C0; Tiberian Sun readline.cpp)

int __fastcall Read_Line(Straw &file, char *buffer, int len, bool &eof);

int __fastcall Read_Line(FileClass &file, char *buffer, int len, bool &eof)
{
    FileStraw fs(file);
    return Read_Line(fs, buffer, len, eof);
}


/***********************************************************************************************
 * Read_Line -- Reads a text line from the straw object specified.                             *
 *                                                                                             *
 *    This reads a line of text from the straw object.                                         *
 *                                                                                             *
 * INPUT:   file  -- Reference to the straw object to fetch the line of text from.             *
 *                                                                                             *
 *          buffer   -- Pointer to the buffer that will hold the line of text.                 *
 *                                                                                             *
 *          len   -- The size of the buffer.                                                   *
 *                                                                                             *
 *          eof   -- Reference to the EOF flag that will be set to true if the source          *
 *                   straw data has been exhausted.                                            *
 *                                                                                             *
 * OUTPUT:  Returns with the number of characters stored into the buffer.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/06/1997 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00639670; Tiberian Sun readline.cpp)

int __fastcall Read_Line(Straw &file, char *buffer, int len, bool &eof)
{
    if (len == 0 || buffer == 0) {
        return 0;
    }

    int count = 0;
    for (;;) {
        char c;
        if (file.Get(&c, sizeof(c)) != sizeof(c)) {
            eof = true;
            buffer[count] = '\0';
            break;
        }

        if (c == '\x0A') {
            break;
        }
        if (c != '\x0D' && count + 1 < len) {
            buffer[count++] = c;
        }
    }
    buffer[count] = '\0';
    return strlen(buffer);
}


// ReferenceCounted::~ReferenceCounted() (0x00639710; empty: only the vtable store remains).

ReferenceCounted::~ReferenceCounted()
{
}


// void ReferenceCounted::Release() (0x00639720): deletes the object when the count drops to 0.

void ReferenceCounted::Release()
{
    if (--RefCount == 0 && this != 0) {
        delete this;
    }
}


// bool ReferenceCounted::unknown_slot1(int arg) (0x00639740): false (slot 1).

bool ReferenceCounted::unknown_slot1(int arg)
{
    return false;
}


// ReferenceCounted's scalar deleting destructor (0x00639750), emitted with its vtable.
