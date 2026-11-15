// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Parameter names from mischa85/RedAlert2 392e904 src/msgbox.cpp where both define the function (names only).

// msgbox.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/msgbox.json carries the addresses and the rows that
// stay in their own files.


#include "shared/ms_message_box.hpp"


// Dynamic initializer of unknown_empty_rect (0x005B4690, _$E), the file-local
// empty Rect from shared/shape_file.hpp (0x00A71878 in this unit).


// int WWMessageBox::Process(unsigned short const *text, unsigned short const *button1,
//     unsigned short const *button2, unsigned short const *button3, bool unknown5) (0x005B46B0)
// Shows the message box dialog (0xCE, 0x120 with a second, 0x121 with a
// third button), waits for a button and restores the mouse hide count.

int WWMessageBox::Process(unsigned short const *msg, unsigned short const *b1txt,
                          unsigned short const *b2txt, unsigned short const *b3txt, bool preserve)
{
    int result = -1;
    int buttons = 0;
    int state = WWMouse->Get_Mouse_State();
    bool has_text = msg != 0 && *msg != 0;
    bool has_button1 = b1txt != 0 && *b1txt != 0;
    bool has_button2 = b2txt != 0 && *b2txt != 0;
    bool has_button3 = b3txt != 0 && *b3txt != 0;
    Darken_Surface(0, true);

    int resource = 0xCE;
    if (has_button3) {
        resource = 0x121;
    } else if (has_button2) {
        resource = 0x120;
    }
    HWND dialog = unknown_create_dialog_5ff610(resource, Message_Box_Proc, 0);
    if (dialog != 0) {
        SetWindowLongA(dialog, 8, (LONG)&result);
        if (has_text) {
            HWND control = GetDlgItem(dialog, 0x5B0);
            SendMessageA(control, 0x4B2, 0, (LPARAM)msg);
        }
        if (has_button1) {
            HWND control = GetDlgItem(dialog, 0x5AE);
            SendMessageA(control, 0x4B2, 0, (LPARAM)b1txt);
            buttons = 1;
        }
        if (has_button2) {
            HWND control = GetDlgItem(dialog, 2);
            SendMessageA(control, 0x4B2, 0, (LPARAM)b2txt);
            buttons = 2;
        }
        if (has_button3) {
            HWND control = GetDlgItem(dialog, 0x5AF);
            SendMessageA(control, 0x4B2, 0, (LPARAM)b3txt);
            buttons = 3;
        }
        unknown_dialog_5ff7c0(dialog);
        if (buttons > 0) {
            while (WWMouse->Get_Mouse_State() != 0) {
                WWMouse->Show_Mouse();
            }
            while (result < 0) {
                if (unknown_6000d0()) {
                    break;
                }
                if (!GameActive) {
                    Title_Screen_Restore(false);
                }
            }
        } else {
            result = 0;
        }
        unknown_dialog_5ff6e0(dialog);
    }
    while (state < WWMouse->Get_Mouse_State()) {
        WWMouse->Hide_Mouse();
    }
    return result;
}


// int __stdcall Message_Box_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) (0x005B48C0)
// Dialog procedure of the message box: the shared handler first, then
// WM_COMMAND (Message_Box_Command, expanded inline) and WM_MOVING.

int __stdcall Message_Box_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int handled = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (handled != 0) {
        return handled;
    }
    switch (message) {
        case 0x111:
            Message_Box_Command(window, wparam & 0xFFFF, (HWND)lparam, wparam >> 16);
            break;
        case 0x216:
            return unknown_moving_736d80(window, wparam, (Rect *)lparam);
    }
    return 0;
}


// void __fastcall Message_Box_Command(HWND window, int id, HWND control, unsigned code) (0x005B4980)
// WM_COMMAND of the message box: stores the pressed button's result through
// the DWL_USER pointer (0x5AE: 0, IDOK/IDCANCEL: 1, 0x5AF: 2). Expanded into
// Message_Box_Proc; this out-of-line copy has no callers.

void __fastcall Message_Box_Command(HWND window, int id, HWND control, unsigned code)
{
    int *result = (int *)GetWindowLongA(window, 8);
    switch (id) {
        case 1:
        case 2:
            if (code == 0) {
                *result = 1;
            }
            break;
        case 0x5AE:
            if (code == 0) {
                *result = 0;
            }
            break;
        case 0x5AF:
            if (code == 0) {
                *result = 2;
            }
            break;
    }
}
