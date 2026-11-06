// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __fastcall _DrawMessage_005c2da0(int color, wchar_t const *message, HWND list) -- 0x005C2DA0
// From ra2-decomp _DrawMessage (YR 0x005E2820).
// Purpose: adds message to the owner-drawn list box as lines that fit its
// width (breaking at a newline or the last non-graphic character), each
// through Add_List_Text_005c2f20 with color.
// @ref _iswgraph=00784ae6
#include "shared/lobby.hpp"
#include "shared/text_print.hpp"
#include "shared/win32_window.hpp"
#include "shared/win32_gdi.hpp"

void __fastcall _DrawMessage_005c2da0(int color, unsigned short const *message, HWND window)
{
    RECT rect;
    struct { HDC hdc; struct { long cx; long cy; } size; } l;    // the twin's { HDC; SIZE } (no SIZE declared here)

    int length = wcslen(message);

    int offset = 18;
    Window_Rect_In_Main_Client(window, &rect);
    if (SendMessageA(window, 0x4a0, 0, 0)) {    // OD_HASATTACHED
        offset = 1;
    }

    l.hdc = GetDC(window);
    SendMessageA(window, 0x49e, 0, (LPARAM)l.hdc);    // OD_RESTOREDC

    while (length) {
        if (message != 0) {
            wchar_t const *newline = wcschr(message, L'\n');
            if (newline != 0) {
                int linelen = newline - message + 1;
                if (length >= linelen) {
                    length = linelen;
                }
            }
        }

        wchar_t *copy = _wcsdup(message);
        copy[length] = 0;
        BitFont->Get_Text_Area(copy, (int *)&l.size.cx, (int *)&l.size.cy, 0);
        free(copy);
        int maxWidth = rect.right - rect.left - offset;

        if (l.size.cx >= maxWidth - 4) {
            int reduceBy;
            if (l.size.cx / 2 > maxWidth) {
                reduceBy = 10;
                length -= reduceBy;
            } else {
                reduceBy = 1;
            }

            int found = -1;
            int idx = length - 1;

            while (idx > 0) {
                if (!iswgraph(message[idx])) {
                    found = idx;
                    break;
                }
                idx--;
            }

            if (found == -1) {
                length -= reduceBy;
                found = length;
            }
            length = found;
        } else {
            Add_List_Text_005c2f20(window, message, length, color);
            message += length;
            while (*message == L' ') {
                message++;
            }
            length = wcslen(message);
        }
    }

    ReleaseDC(window, l.hdc);
}
