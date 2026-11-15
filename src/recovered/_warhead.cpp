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
// Function comment blocks from mischa85/RedAlert2 392e904 src/_warhead.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/_warhead.cpp where both define the function (names only).

// _warhead.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/_warhead.json carries the addresses and the rows that
// stay in their own files.


#include "shared/warhead_type.hpp"
#include "shared/crt_string.hpp"


/// <summary>
/// Converts a warhead name into a warhead type.
/// This routine is used when the rules are read in and a warhead is referred to by name.
/// A name that has not been encountered before will have a warhead type object created
/// for it, so an unknown name is not treated as an error.
/// </summary>
/// <returns>Returns with the warhead type that matches the name. Otherwise, WARHEAD_NONE
/// is returned.</returns>
// (0x0040F2F0)
// Index of the warhead type with this ID in the unit's list 0x00839B00 (-1 for
// "<none>" and "none"); an unknown ID makes a new type and returns its index.
// No direct callers.

int __fastcall Warhead_From_Name(char const *name)
{
    if (_stricmp(name, "<none>") == 0 || _stricmp(name, "none") == 0) {
        return -1;
    }
    for (int index = 0; index < unknown_WarheadTypeClass_list_00839b00.Count(); index++) {
        if (_stricmp(name, unknown_WarheadTypeClass_list_00839b00[index]->ID) == 0) {
            return index;
        }
    }
    return unknown_WarheadTypeClass_list_00839b00.ID(new WarheadTypeClass(name));
}


/// <summary>
/// Fetches the name of the specified warhead.
/// This routine is used when a warhead must be written back out to an INI file or
/// displayed, since the rules refer to warheads by name rather than by index.
/// </summary>
/// <returns>Returns with a pointer to the warhead's name. Otherwise, NULL is returned.</returns>
// (0x0040F3A0)
// ID of the warhead type at this index of the unit's list 0x00839B00, or null.
// No direct callers.

char const *__fastcall Warhead_Name(int warhead)
{
    if (warhead != -1 && (unsigned)warhead < (unsigned)unknown_WarheadTypeClass_list_00839b00.Count()) {
        return unknown_WarheadTypeClass_list_00839b00[warhead]->ID;
    }
    return 0;
}


// ??_G?$VectorClass@PAVWarheadTypeClass@@@@UAEPAXI@Z (0x0040F680): compiler generated with the vtable unknown_WarheadTypeClass_list_00839b00 stores
// (VectorClass<WarheadTypeClass *>, shared/vector_class.hpp); emitted by this unit's header_statics/0040f0a0. No source of its own.


// ??_G?$DynamicVectorClass@PAVWarheadTypeClass@@@@UAEPAXI@Z (0x0040F6D0): compiler generated with the vtable unknown_WarheadTypeClass_list_00839b00 stores
// (DynamicVectorClass<WarheadTypeClass *>, shared/vector_class.hpp); emitted by this unit's header_statics/0040f0a0. No source of its own.
