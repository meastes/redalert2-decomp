// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int OwnerDraw::unknown_5e1ca0(HWND dialog, HWND window) (0x005E1CA0): 0xC0 when the dialog is one of the
// listed kinds (marking it band 2) and the control is an edit, a type-1 list box or a non-dropdown combo box.
// Shape adapted from ra2-decomp src/ownrdraw.cpp FUN_00603AA0 / TrySetDialogLayoutBand2 (gamemd 0x0060C7D0) with
// the class-name test as its own helper (FUN_006242B0, gamemd 0x006242B0), matched there; see THIRD_PARTY.md.
// The helper is expanded here and in the 0x005E8720 callback, which auto-inlines this body; game.exe has no body
// of the helper (GetClassNameA is called only here and inside 0x005E8720 in this unit).
// @ref __imp__GetClassNameA@12=0079a4e0
// @ref __imp__GetWindowLongA@8=0079a48c
// @ref ??_C@_04EDLP@Edit?$AA@=007e8df0
// @ref ??_C@_07FHL@NewEdit?$AA@=007e8de8
// @ref ??_C@_07NOMM@ListBox?$AA@=007e8de0
// @ref ??_C@_08EIKH@ComboBox?$AA@=007e8dd4
#include "shared/win32_api.hpp"
#include "shared/crt_string.hpp"
#include "shared/owner_draw_dialog_control.hpp"

namespace OwnerDraw {

// Whether the control is an edit, a registered list box of type 1 or a non-dropdown combo box
// (ra2-decomp FUN_006242B0).
inline bool __fastcall Is_Text_Control(HWND control)
{
    unsigned int key = (unsigned int)control;
    char name[128];
    GetClassNameA(control, name, 128);
    if (strcmp(name, "Edit") == 0) {
        return true;
    }
    if (strcmp(name, "NewEdit") == 0) {
        return true;
    }
    if (strcmp(name, "ListBox") == 0) {
        UnknownControlRecord *record = 0;
        if (unknown_control_table_a73640.Get_Pointer(key, &record)) {
            return record->unknown_68 == 1;
        }
    }
    if (strcmp(name, "ComboBox") == 0) {
        return (GetWindowLongA((HWND)key, GWL_STYLE) & 3) != 3;
    }
    return false;
}

int __fastcall unknown_5e1ca0(HWND dialog, HWND window)
{
    UnknownControlRecord *record = Get_Record(dialog);
    if (record != 0) {
        int kind = record->unknown_6c;
        if (kind == 0x10d || kind == 0xd9 || kind == 0xf0 || kind == 0xce || kind == 0x120 ||
            kind == 0x121 || kind == 0x115 || kind == 0xd3 || kind == 0xcf || kind == 0x11f ||
            kind == 0xc3 || kind == 0x11b || kind == 0xe1 || kind == 0x11e || kind == 0xc4 ||
            kind == 300 || kind == 0xd0 || kind == 0xfc || kind == 0x126) {
            record->unknown_b0 = 2;
            if (Is_Text_Control(window)) {
                return 0xc0;
            }
        }
    }
    return 0;
}

}
