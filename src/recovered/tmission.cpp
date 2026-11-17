/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/tmission.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/tmission.cpp where both define the function (names only).

// tmission.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/tmission.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/script.hpp"
#include "shared/crt_tokens.hpp"


// Header-static dynamic initializers of the unit at 0x006EAD70:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006EAD70-0x006EAF56); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x006EADA0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006ead70.cpp; no source of its own.


// Dynamic initializer (0x006EADC0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006ead70.cpp; no source of its own.


// Dynamic initializer (0x006EADE0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006ead70.cpp; no source of its own.


// Dynamic initializer (0x006EAE00, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006ead70.cpp; no source of its own.


// Dynamic initializer (0x006EAE20, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006ead70.cpp; no source of its own.


// Dynamic initializer (0x006EAE40, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006ead70.cpp; no source of its own.


// Dynamic initializer (0x006EAE80, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006ead70.cpp; no source of its own.


// Dynamic initializer (0x006EAEB0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006ead70.cpp; no source of its own.


// Dynamic initializer (0x006EAEE0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006ead70.cpp; no source of its own.


// Dynamic initializer (0x006EAF10, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006ead70.cpp; no source of its own.


// Dynamic initializer (0x006EAF20, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006ead70.cpp; no source of its own.


// Dynamic initializer (0x006EAF40, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006ead70.cpp; no source of its own.


// void ScriptMissionClass::Read_INI_Entry(char const *text) (0x006EAF60):
// reads "action,argument" (ScriptTypeClass::Read_INI); nothing for no text.
// Not a constructor: it does not return this (name hypothesis, the
// counterpart of Build_INI_Entry).

void ScriptMissionClass::Read_INI_Entry(char const *text)
{
    if (text != 0) {
        int mission;
        int data;
        sscanf(text, "%d,%d", &mission, &data);
        Mission = mission;
        Data = data;
    }
}


/// <summary>
/// Builds the INI text form of this team mission.
/// This routine is the counterpart to Fill_In and is used when a team type is written
/// back out to the scenario file.
/// </summary>
/// <param name="ptr">Buffer to build the entry into. A NULL pointer builds nothing.</param>
/// <returns>Returns with the number of characters placed into the buffer.</returns>
/// <remarks>Be sure that the destination buffer is big enough to hold the entry.</remarks>
// (0x006EAFA0)
// writes "action,argument" and returns its length (0 for no buffer).

int ScriptMissionClass::Build_INI_Entry(char *buffer) const
{
    if (buffer != 0) {
        sprintf(buffer, "%d,%d", Mission, Data);
        return strlen(buffer);
    }
    return 0;
}


/***********************************************************************************************
 * TeamMission_Needs -- Determines what extra data is needed by team mission.                  *
 *                                                                                             *
 *    This routine will return the required extra data that the specified team mission will    *
 *    need.                                                                                    *
 *                                                                                             *
 * INPUT:   tmtype   -- The team mission type to check.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the data type needed for this team mission.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006EAFE0)
// Maps a script action number to a value (most actions give 0); no callers by direct call
// (name, argument and result types are hypotheses).

int __fastcall TeamMission_Needs(int tmtype)
{
    switch (tmtype) {
    case 0:
    case 57:
        return 15;
    case 4:
        return 19;
    case 34:
        return 32;
    case 12:
    case 38:
        return 22;
    case 39:
    case 40:
        return 29;
    case 5:
    case 6:
    case 13:
    case 32:
    case 42:
    case 50:
        return 10;
    case 1:
    case 3:
    case 15:
    case 16:
        return 9;
    case 20:
        return 13;
    case 11:
        return 18;
    case 17:
        return 20;
    case 18:
        return 12;
    case 24:
        return 4;
    case 25:
        return 3;
    case 26:
        return 2;
    case 27:
        return 1;
    case 46:
    case 47:
    case 56:
    case 58:
        return 36;
    case 8:
        return 37;
    case 51:
        return 41;
    case 52:
        return 42;
    default:
        return 0;
    }
}
