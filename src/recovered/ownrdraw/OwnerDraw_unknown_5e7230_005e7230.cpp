// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool OwnerDraw::unknown_5e7230(unsigned int key, HWND window) (0x005E7230): an owner-drawn, free control that
// passes test 0x005E6860. Adapted from ra2-decomp src/ownrdraw.cpp FUN_006096A0 (gamemd twin 0x006096A0, matched
// there): the window copy made before record is cleared (wlib Dictionary<HWND>::getPointer key); see THIRD_PARTY.md.
// @ref __imp__GetWindowLongA@8=0079a48c
// @ref ?OwnerDraw_Control_Test_005e6860@@YI_NIPAUHWND__@@@Z=005e6860
#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"

namespace OwnerDraw {

bool __fastcall unknown_5e7230(unsigned int key, HWND window)
{
    if ((GetWindowLongA(window, GWL_STYLE) & BS_OWNERDRAW) != BS_OWNERDRAW) {
        return false;
    }
    HWND control = window;
    UnknownControlRecord *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int const &)control, &record);
    if (record && record->unknown_68 == 0) {
        return ::OwnerDraw_Control_Test_005e6860(key, window);
    }
    return false;
}

}
