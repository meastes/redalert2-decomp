// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool OwnerDraw::unknown_5e7870(unsigned int key, HWND window) (0x005E7870): layout band 2 test inside a session.
// Adapted from ra2-decomp src/ownrdraw.cpp FUN_00609CF0 (gamemd twin 0x00609CF0, matched there); see THIRD_PARTY.md.
// @ref ?Unknown_00672F10@UnknownSessionState@@QAEEXZ=00672f10
#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/session_state.hpp"
#include "shared/unknown_globals.hpp"

namespace OwnerDraw {

bool __fastcall unknown_5e7870(unsigned int key, HWND window)
{
    if (!unknown_session_a3d298.Unknown_00672F10()) {
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
