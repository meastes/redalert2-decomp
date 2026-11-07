#define RA2_COORD_EQUAL_MEMBER
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __fastcall Planning_Unlist_Token(PlanningTokenClass *token) (0x006144F0)
// From ra2-decomp (YR 0x00637750).
// Removes a token from the static list unknown_list_00a76738 unless the
// planning lists are being torn down.
#include "shared/planning.hpp"
// @ref ?unknown_flag_a767a5@@3_NA=00a767a5

void __fastcall Planning_Unlist_Token(PlanningTokenClass *token)
{
    if (!unknown_flag_a767a5) {
        PlanningTokenClass::unknown_list_00a76738.Delete(token);
    }
}
