// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool OwnerDraw::unknown_5e7740(unsigned int key, HWND window) (0x005E7740): layout band 1 test outside a session.
// Adapted from ra2-decomp src/ownrdraw.cpp FUN_00609BC0 and Is_Plain_Control (gamemd twin 0x00609BC0, matched
// there); see THIRD_PARTY.md.
// @ref ?Unknown_00672F10@UnknownSessionState@@QAEEXZ=00672f10
#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/session_state.hpp"
#include "shared/unknown_globals.hpp"

namespace OwnerDraw {

// Whether window is an owner-drawn control with a free registry record (ra2-decomp Is_Plain_Control; inline).
inline bool Is_Plain_Control(HWND window)
{
    if ((GetWindowLongA(window, GWL_STYLE) & BS_OWNERDRAW) == BS_OWNERDRAW) {
        HWND control = window;
        UnknownControlRecord *record = 0;
        unknown_control_table_a73640.Get_Pointer((unsigned int const &)control, &record);
        if (record != 0 && record->unknown_68 == 0) {
            return true;
        }
    }
    return false;
}

bool __fastcall unknown_5e7740(unsigned int key, HWND window)
{
    if (unknown_session_a3d298.Unknown_00672F10()) {
        return false;
    }
    if (unknown_5e7230(key, window)) {
        return true;
    }
    if (Is_Plain_Control(window) && unknown_5e72c0(key, window)) {
        return true;
    }
    return false;
}

}
