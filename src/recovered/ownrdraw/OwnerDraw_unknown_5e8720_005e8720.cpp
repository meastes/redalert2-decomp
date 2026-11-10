// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// BOOL OwnerDraw::unknown_5e8720(HWND window, LPARAM) (0x005E8720): EnumChildWindows callback that stores a
// registered control's sound slot (+0xDC, from 0x005E12A0) and text-control alpha (+0xC8, from 0x005E1CA0).
// Adapted from ra2-decomp src/ownrdraw.cpp OwnerDraw::InitCompactDialogControlsProc (gamemd twin 0x0060AAB0,
// matched there); see THIRD_PARTY.md. Run by 0x005E8B90.
// @ref __imp__GetParent@4=0079a500
#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"

namespace OwnerDraw {

BOOL __stdcall unknown_5e8720(HWND window, LPARAM)
{
    HWND parent = GetParent(window);
    UnknownControlRecord *record = Get_Record(window);
    if (record) {
        record->unknown_dc = unknown_5e12a0(parent, window);
        record->unknown_c8 = unknown_5e1ca0(parent, window);
    }
    return TRUE;
}

}
