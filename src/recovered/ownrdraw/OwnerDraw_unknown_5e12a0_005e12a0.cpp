// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int OwnerDraw::unknown_5e12a0(HWND dialog, HWND window) (0x005E12A0): the sound slot of a dialog control
// (0x25 for the sidebar button 0x71C of the listed dialog kinds, 0xE for controls 0x6EC/0x5A8 of the option
// dialogs). Shape adapted from ra2-decomp src/ownrdraw.cpp Control_Sound_Slot (gamemd twin 0x00603040,
// matched there); see THIRD_PARTY.md. Expanded into the 0x005E8720 callback, where the Get_Record form sets
// its lookup's inline budget.
// @ref __imp__GetDlgCtrlID@4=0079a3f4
#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"

namespace OwnerDraw {

int __fastcall unknown_5e12a0(HWND dialog, HWND window)
{
    UnknownControlRecord *record = Get_Record(dialog);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (Is_Listed_Kind_5e8590(kind) && id == 0x71c) {
        return 0x25;
    }
    if (((((kind == 0xbc) || (kind == 0xbd)) || (kind == 0xc2)) ||
            ((kind == 0xc9 || (kind == 0x102)))) && ((id == 0x6ec || (id == 0x5a8)))) {
        return 0xe;
    }
    return 0;
}

}
