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


// void __fastcall VQA_Unknown_0071DB20() (0x0071DB20): clears the 18 VQA options, then sets option 6.

void __fastcall VQA_Unknown_0071DB20()
{
    for (int option = 0; option < 18; option++) {
        VQA_Unknown_0071DB80(option);
    }
    Set_Option(6);
}


// void __fastcall Set_Option(int option) (0x0071DB60): sets the option bit.

void __fastcall Set_Option(int flag)
{
    int index = flag >> 3;
    unsigned char shift = flag & 7;
    unsigned char mask = 1 << shift;
    unknown_vqa_options_af9020[index] |= mask;
}


// void __fastcall VQA_Unknown_0071DB80(int option) (0x0071DB80): clears the option bit.

void __fastcall VQA_Unknown_0071DB80(int option)
{
    int index = option >> 3;
    unsigned char shift = option & 7;
    unsigned char mask = 1 << shift;
    unknown_vqa_options_af9020[index] &= ~mask;
}


// bool __fastcall Get_Option(int option) (0x0071DBA0): the option bit is set.

bool __fastcall Get_Option(int flag)
{
    int index = flag >> 3;
    unsigned char shift = flag & 7;
    unsigned char mask = 1 << shift;
    return (unknown_vqa_options_af9020[index] & mask) ? true : false;
}


// int __fastcall VQA_Unknown_0071DBC0(int option) (0x0071DBC0): toggles the option bit; 1 when it is now set.

int __fastcall VQA_Unknown_0071DBC0(int option)
{
    int index = option >> 3;
    unsigned char shift = option & 7;
    if (unknown_vqa_options_af9020[index] & (1 << shift)) {
        VQA_Unknown_0071DB80(option);
        return 0;
    }
    Set_Option(option);
    return 1;
}
