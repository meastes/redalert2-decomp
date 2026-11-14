// windlg.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/windlg.json carries the addresses and the rows that
// stay in their own files.


#include "shared/win_util.hpp"
#include "shared/win32_window.hpp"
#include "shared/crt_string.hpp"
#include "shared/crt_math.hpp"


class Leaf_735670 { public: void get(); };
void Leaf_735670::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int zero_value; unsigned int unused_value; zero_value = 0; *(unsigned int*)(0xb25db8) = zero_value; *(unsigned int*)(0xb25dbc) = zero_value; *(unsigned int*)(0xb25dc0) = zero_value; *(unsigned int*)(0xb25dc4) = zero_value; }


// void *__fastcall unknown_create_dialog_735700(void *instance, int id, HWND parent,
//     UnknownDialogProc procedure, long arg) (0x00735700)

void *__fastcall unknown_create_dialog_735700(void *instance, int id, HWND parent, UnknownDialogProc procedure, long arg)
{
    DialogStack[DialogStackCount].Window = 0;
    DialogStack[DialogStackCount].Resource = 0;
    DialogStackEntry *entry = &DialogStack[DialogStackCount];
    DLGTEMPLATE const *dialog_template = (DLGTEMPLATE const *)Load_Resource_004954e0(id, (char const *)5);   // RT_DIALOG
    if (dialog_template == 0) {
        return 0;
    }
    DialogStackCount++;
    HWND dialog = CreateDialogIndirectParamA((HINSTANCE)instance, dialog_template, parent, (DLGPROC)procedure, 0);
    if (dialog == 0) {
        DialogStackCount--;
        return 0;
    }
    unknown_int_a40d34++;
    Unknown_005B6000(dialog);
    EnumChildWindows(dialog, unknown_prepare_control_735bc0, 1);
    unknown_prepare_control_735bc0(dialog, 0);
    entry->Window = dialog;
    entry->Resource = id;
    if (arg != 0) {
        ShowWindow(dialog, 1);   // SW_SHOWNORMAL
    }
    SetForegroundWindow(dialog);
    SetFocus(dialog);
    CurrentDialog = dialog;
    CurrentDialogResource = id;
    return dialog;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool __fastcall End_Dialog_007357e0(HWND dialog, int result) (0x007357E0)
// Adopted from ra2-decomp WS_Destroy_Dialog (YR 0x007757E0).
// Closes dialog (the top dialog when null) and every dialog above it on the
// stack, makes the one below current and records result; false when dialog
// is not on the stack.

bool __fastcall End_Dialog_007357e0(HWND dialog, int result)
{
    int index;
    int i;
    if (dialog == 0 && DialogStackCount != 0) {
        dialog = DialogStack[DialogStackCount - 1].Window;
    }
    if (dialog == 0) {
        return false;
    }

    index = -1;
    for (i = 0; i < DialogStackCount; i++) {
        if (DialogStack[i].Window == dialog) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return false;
    }

    OwnerDraw::unknown_5e5fc0(dialog);

    for (i = DialogStackCount - 1; i >= index; i--) {
        Unknown_005B6060(DialogStack[i].Window);
        DestroyWindow(DialogStack[i].Window);
        unknown_int_a40d34--;
    }

    DialogStackCount = index;

    if (DialogStackCount != 0) {
        HWND window = DialogStack[DialogStackCount - 1].Window;
        int resource = DialogStack[DialogStackCount - 1].Resource;
        SetForegroundWindow(window);
        InvalidateRect(window, 0, FALSE);
        UpdateWindow(window);
        CurrentDialog = window;
        CurrentDialogResource = resource;
        SetFocus(window);
        SendMessageA(CurrentDialog, 0x4B0, 0, 0);   // ra2-decomp OD_REFOCUS
    } else {
        CurrentDialog = 0;
        CurrentDialogResource = 0;
        SetFocus(MainWindow);
    }
    unknown_dialog_result_b25ddc = result;

    return true;
}


// int __fastcall unknown_is_listed_b25bb8(int key) -- 0x00735910
// Purpose: whether key is the first word of one of the 0x00B25DE0 used
// 8-byte records at 0x00B25BB8: whether a window is on the dialog stack.
// Assumptions: the int parameter type is kept from the first recovery.

int __fastcall unknown_is_listed_b25bb8(int key)
{
    for (int index = 0; index < DialogStackCount; index++) {
        if ((int)DialogStack[index].Window == key) {
            return 1;
        }
    }
    return 0;
}


// int __fastcall Dialog_Loop_007359e0(HWND dialog, bool (*callback)(), int unused, bool modal, bool show) (0x007359E0)

int __fastcall Dialog_Loop_007359e0(HWND dialog, bool (*callback)(), int unused, bool modal, bool show)
{
    if (modal) {
        SetForegroundWindow(dialog);
        SendMessageA(dialog, 0x4A9, 0, 1);
    }
    if (show) {
        ShowWindow(dialog, 1);
    }
    while (unknown_is_listed_b25bb8((int)dialog)) {
        if (callback != 0 && callback() == true) {
            End_Dialog_007357e0(dialog, 2);
        }
        if (!unknown_b2abd9) {
            unknown_517370(false);
        }
        unknown_message_count_a52298++;
        MSG message;
        while (unknown_peek_message_522ed0(&message, 0, 0, 0, 1)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
            if (!unknown_is_listed_b25bb8((int)dialog)) {
                break;
            }
        }
        Sleep(0);
    }
    if (modal) {
        SendMessageA(dialog, 0x4A9, 0, 0);
    }
    return unknown_dialog_result_b25ddc;
}


class Leaf_735b10 { public: unsigned int get(); };
unsigned int Leaf_735b10::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int global_value; unsigned int unused_value; global_value = *(unsigned int*)(0xb25dd4); return global_value; }


class Leaf_735b20 { public: unsigned int get(); };
unsigned int Leaf_735b20::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int global_value; unsigned int unused_value; global_value = *(unsigned int*)(0xb25dd8); return global_value; }


// 0x00735b30: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 FUN_00735b30(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00735b30 {

extern "C" unsigned int DAT_00b25ddc;

unsigned int  FUN_00735b30(void)
{
    return DAT_00b25ddc;
}

}  // namespace seed_00735b30


// BOOL __stdcall unknown_null_dialog_proc_735b40(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam) (0x00735B40)
// A dialog procedure that handles nothing: Dialog_Template_Size measures
// templates with it (0x00735BC0).

BOOL __stdcall unknown_null_dialog_proc_735b40(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam)
{
    return 0;
}


// int __fastcall Dialog_Template_Size(int dialog_id, DLGPROC procedure, int unused, LONG *size) -- 0x00735B50
// Purpose: measures a dialog template: creates it (ProgramInstance, no
// parent, init parameter 0), stores its client width and height in size[0]
// and size[1], destroys it and returns 1. The third argument is not read.

int __fastcall Dialog_Template_Size(int dialog_id, DLGPROC procedure, int unused, LONG *size)
{
    HWND dialog;
    RECT client;

    dialog = CreateDialogParamA(ProgramInstance, MAKEINTRESOURCEA(dialog_id), 0, procedure, 0);
    GetClientRect(dialog, &client);
    DestroyWindow(dialog);
    size[0] = client.right;
    size[1] = client.bottom;
    return 1;
}


// void __fastcall unknown_735ba0(HWND window) (0x00735BA0)
// Prepares every child control of a dialog, then the dialog itself, with
// 0x00735BC0 (unknown_show_dialog 0x005FF7E0 calls it).

void __fastcall unknown_735ba0(HWND window)
{
    EnumChildWindows(window, unknown_prepare_control_735bc0, 1);
    unknown_prepare_control_735bc0(window, 0);
}


// BOOL __stdcall unknown_prepare_control_735bc0(HWND window, LPARAM parameter) (0x00735BC0)
// Scales window (a dialog, or with parameter set a control placed relative to its parent) from
// the layout size to the client size of template 0xC6 and moves it; combo boxes use their
// dropped-down rectangle. EnumChildWindows callback of 0x00735700.

BOOL __stdcall unknown_prepare_control_735bc0(HWND window, LPARAM parameter)
{
    RECT rect;
    GetWindowRect(window, &rect);
    char name[128];
    GetClassNameA(window, name, sizeof(name));
    if (strcmp(name, "ComboBox") == 0) {
        SendMessageA(window, 0x152, 0, (LPARAM)&rect);     // CB_GETDROPPEDCONTROLRECT
    }
    if (parameter != 0) {
        HWND owner = (HWND)GetWindowLongA(window, -8);
        RECT parent;
        GetWindowRect(owner, &parent);
        rect.left -= parent.left;
        rect.right -= parent.left;
        rect.top -= parent.top;
        rect.bottom -= parent.top;
    }
    LONG *size = unknown_template_size_b25de4;
    if (size[0] == 0) {
        Dialog_Template_Size(0xC6, (DLGPROC)unknown_null_dialog_proc_735b40, 0, size);
    }
    int width = (rect.right - rect.left + 1) * unknown_layout_width_7fd048 / size[0];
    int height = (rect.bottom - rect.top + 1) * unknown_layout_height_7fd04c / size[1];
    rect.left = unknown_layout_width_7fd048 * rect.left / size[0];
    rect.top = unknown_layout_height_7fd04c * rect.top / size[1];
    rect.right = width + rect.left - 1;
    rect.bottom = height + rect.top - 1;
    MoveWindow(window, rect.left, rect.top, width, height, 1);
    return 1;
}


// void unknown_clear_font_cache_735d90() (0x00735D90)

void unknown_clear_font_cache_735d90()
{
    unknown_font_cache_b25dc8.Count = 0;
    operator delete(unknown_font_cache_b25dc8.Vector);
}


// HFONT __fastcall unknown_find_font_735db0(HDC dc, char const *face, int width, int height, unsigned attributes) (0x00735DB0)

HFONT __fastcall unknown_find_font_735db0(HDC dc, char const *face, int width, int height, unsigned attributes)
{
    for (int index = 0; index < unknown_font_cache_b25dc8.Count; index++) {
        UnknownFontEntry entry;
        unknown_font_cache_b25dc8.Get(index, entry);
        if (strcmp(entry.Face, face) == 0 && entry.Width == width && entry.Height == height
            && entry.Attributes == attributes) {
            return entry.Font;
        }
    }
    if (dc == 0) {
        return 0;
    }
    HFONT font = unknown_create_font_7360b0(dc, face, width, height, attributes);
    if (font == 0) {
        return font;
    }
    UnknownFontEntry entry;
    strcpy(entry.Face, face);
    entry.Width = width;
    entry.Height = height;
    entry.Attributes = attributes;
    entry.Font = font;
    unknown_font_cache_b25dc8.Add(entry);
    return font;
}


// HFONT __fastcall unknown_create_font_7360b0(HDC dc, char const *face, int width, int height, unsigned attributes) (0x007360B0)

HFONT __fastcall unknown_create_font_7360b0(HDC dc, char const *face, int width, int height, unsigned attributes)
{
    POINT point;
    TEXTMETRICA metrics;
    LOGFONTA font;

    SaveDC(dc);
    SetGraphicsMode(dc, 2);                 // GM_ADVANCED
    ModifyWorldTransform(dc, 0, 1);         // MWT_IDENTITY
    SetViewportOrgEx(dc, 0, 0, 0);
    SetWindowOrgEx(dc, 0, 0, 0);
    point.x = width;
    point.y = height;
    DPtoLP(dc, &point, 1);
    font.lfHeight = -point.y;
    font.lfWidth = 0;
    font.lfEscapement = 0;
    font.lfOrientation = 0;
    font.lfWeight = (attributes & 1) ? 700 : 0;
    font.lfItalic = (attributes & 2) ? 1 : 0;
    font.lfUnderline = (attributes & 4) ? 1 : 0;
    font.lfStrikeOut = (attributes & 8) ? 1 : 0;
    font.lfCharSet = 0;
    font.lfOutPrecision = 0;
    font.lfClipPrecision = 0;
    font.lfQuality = 0;
    font.lfPitchAndFamily = 0;
    strcpy(font.lfFaceName, face);
    HFONT handle = CreateFontIndirectA(&font);
    if (width != 0) {
        handle = (HFONT)SelectObject(dc, handle);
        GetTextMetricsA(dc, &metrics);
        DeleteObject(SelectObject(dc, handle));
        font.lfWidth = (int)(metrics.tmAveCharWidth * fabs(point.x) / fabs(point.y) + 0.5);
        handle = CreateFontIndirectA(&font);
    }
    RestoreDC(dc, -1);
    return handle;
}
