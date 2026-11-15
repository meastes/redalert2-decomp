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
// Parameter names from mischa85/RedAlert2 392e904 src/vqoption.cpp where both define the function (names only).

// vqoption.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vqoption.json carries the addresses and the rows that
// stay in their own files.


#include "shared/movie.hpp"


// void __fastcall Initialize_Options() (0x0071DB20): clears the 18 VQA options, then sets option 6.

void __fastcall Initialize_Options()
{
    for (int option = 0; option < 18; option++) {
        Clear_Option(option);
    }
    Set_Option(6);
}


// void __fastcall Set_Option(int option) (0x0071DB60): sets the option bit.

void __fastcall Set_Option(int flag)
{
    int index = flag >> 3;
    unsigned char shift = flag & 7;
    unsigned char mask = 1 << shift;
    VQOptions[index] |= mask;
}


// void __fastcall Clear_Option(int option) (0x0071DB80): clears the option bit.

void __fastcall Clear_Option(int option)
{
    int index = option >> 3;
    unsigned char shift = option & 7;
    unsigned char mask = 1 << shift;
    VQOptions[index] &= ~mask;
}


// bool __fastcall Get_Option(int option) (0x0071DBA0): the option bit is set.

bool __fastcall Get_Option(int flag)
{
    int index = flag >> 3;
    unsigned char shift = flag & 7;
    unsigned char mask = 1 << shift;
    return (VQOptions[index] & mask) ? true : false;
}


// int __fastcall Toggle_Option(int option) (0x0071DBC0): toggles the option bit; 1 when it is now set.

int __fastcall Toggle_Option(int option)
{
    int index = option >> 3;
    unsigned char shift = option & 7;
    if (VQOptions[index] & (1 << shift)) {
        Clear_Option(option);
        return 0;
    }
    Set_Option(option);
    return 1;
}
