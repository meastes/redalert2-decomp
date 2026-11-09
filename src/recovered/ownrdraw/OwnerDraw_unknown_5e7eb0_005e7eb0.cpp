// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// BOOL OwnerDraw::unknown_5e7eb0(HWND window, LPARAM) (0x005E7EB0): EnumChildWindows callback that sorts each
// registered control into layout band 1, 2 or 3 (record +0xB0). Adapted from ra2-decomp src/ownrdraw.cpp
// OwnerDraw::ClassifyLayoutBand (gamemd twin 0x0060A330, matched there); see THIRD_PARTY.md.
// @ref __imp__GetParent@4=0079a500
#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"

namespace OwnerDraw {

BOOL __stdcall unknown_5e7eb0(HWND window, LPARAM)
{
    UnknownControlRecord *record = Get_Record(window);
    if (record != 0) {
        HWND parent = GetParent(window);
        if (unknown_5e7740((unsigned int)parent, window)) {
            record->unknown_b0 = 1;
            return TRUE;
        }
        if (unknown_5e7870((unsigned int)parent, window)) {
            record->unknown_b0 = 2;
            return TRUE;
        }
        if (unknown_5e79a0((unsigned int)parent, window)) {
            record->unknown_b0 = 3;
        }
    }
    return TRUE;
}

}
