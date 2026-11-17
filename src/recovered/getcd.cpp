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
// Function comment blocks from mischa85/RedAlert2 392e904 src/getcd.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// getcd.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/getcd.json carries the addresses and the rows that
// stay in their own files.


#include "shared/get_cd.hpp"
#include "shared/debug_printf.hpp"
#include "shared/cd_control.hpp"
#include "shared/crt_string.hpp"


/// <summary>
/// Constructor for the CD drive list object.
/// This routine asks the system about every logical drive letter and records the ones
/// that turn out to be CD-ROM drives. The disc searching code walks this list when it
/// hunts for the game CD, so an empty list means there is nowhere to look.
/// </summary>
// (0x004D2940)
// Lists the CD-ROM drives: C: to Z: first; when there is none, A: and B:.

GetCDClass::GetCDClass()
{
    char root[4] = "a:\\";
    Count = 0;
    Cursor = 0;
    Debug_Printf("GetCDClass constructor\n");
    memset(Drives, 0xFF, 26);
    int upper = 'C';
    for (char letter = 'c'; letter <= 'z'; letter++, upper++) {
        root[0] = letter;
        if (GetDriveTypeA(root) == DRIVE_CDROM) {
            Drives[Count++] = upper - 'A';
            Debug_Printf("CD drive found - %c:\n", upper);
        }
    }
    if (Count == 0) {
        char letter = 'a';
        int index = 0;
        for (; letter <= 'b'; letter++, index++) {
            root[0] = letter;
            if (GetDriveTypeA(root) == DRIVE_CDROM) {
                Drives[Count++] = index;
            }
        }
        if (Count == 0) {
            Debug_Printf("No CD drives found\n");
        }
    }
}


/// <summary>
/// Destructor for the CD drive list object.
/// </summary>
// (0x004D2A10)

GetCDClass::~GetCDClass()
{
}
