// winfix.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/winfix.json carries the addresses and the rows that
// stay in their own files.


#include "shared/win_util.hpp"
#include "shared/crt_string.hpp"
#include "shared/ini_class.hpp"
#include "shared/mouse.hpp"
#include "shared/windows_version.hpp"
#include "shared/win32_window.hpp"
#include "shared/unknown_globals.hpp"


// bool __fastcall unknown_begin_drag_736230(HWND dialog, HWND tree, NMTREEVIEWA *info) (0x00736230)

bool __fastcall unknown_begin_drag_736230(HWND dialog, HWND tree, NMTREEVIEWA *info)
{
    if (info->itemNew.lParam != 0) {
        if (!SendMessageA(dialog, 0x4C9, (WPARAM)tree, (LPARAM)info->itemNew.hItem)) {
            return false;
        }
        unknown_drag_item_b25f34 = info->itemNew.hItem;
        SendMessageA(tree, 0x110B, 9, 0);                                     // TVM_SELECTITEM, TVGN_DROPHILITE
        unknown_drag_image_b25f38 = (void *)SendMessageA(tree, 0x1112, 0, (LPARAM)info->itemNew.hItem);   // TVM_CREATEDRAGIMAGE
        RECT rect;
        *(void **)&rect = info->itemNew.hItem;
        SendMessageA(tree, 0x1104, 0, (LPARAM)&rect);                         // TVM_GETITEMRECT
        int indent = SendMessageA(tree, 0x1106, 0, 0);                        // TVM_GETINDENT
        int x = info->ptDrag.x - indent - rect.left;
        int y = info->ptDrag.y - rect.top;
        SetCapture(dialog);
        ImageList_BeginDrag(unknown_drag_image_b25f38, 0, x, y);
        ImageList_DragEnter(dialog, 0, 0);
        SetTimer(dialog, 1, 1, 0);
        unknown_dragging_b25f2c = true;
        unknown_tree_b25f30 = tree;
    }
    return unknown_dragging_b25f2c;
}


// bool __fastcall unknown_end_drag_736320(unsigned long position) (0x00736320)

bool __fastcall unknown_end_drag_736320(unsigned long position)
{
    if (unknown_dragging_b25f2c) {
        bool accepted = unknown_drag_move_736670(position);
        ImageList_EndDrag();
        ReleaseCapture();
        ImageList_Destroy(unknown_drag_image_b25f38);
        unknown_drag_image_b25f38 = 0;
        unknown_dragging_b25f2c = false;
        UnknownDragInfo info = unknown_drag_info_736490(position);
        UnknownDragInfo data = info;
        if (accepted) {
            SendMessageA(info.Target, 0x4CA, (WPARAM)&data, position);
        }
        KillTimer(data.Target, 1);
        SendMessageA(unknown_tree_b25f30, 0x110B, 8, 0);      // TVM_SELECTITEM, TVGN_DROPHILITE
        return true;
    }
    return false;
}


// unknown_drag_info_736490: window and cursor coordinates for the current drag.
UnknownDragInfo __fastcall unknown_drag_info_736490(unsigned long position)
{
    UnknownDragInfo info;
    POINT client;
    client.x = position & 0xffff;
    client.y = position >> 16;
    HWND window = ChildWindowFromPoint((HWND)GetWindowLongA(unknown_tree_b25f30, -8), client);
    HWND target = (HWND)GetWindowLongA(window, -8);
    if (window == (HWND)GetWindowLongA(unknown_tree_b25f30, -8)) {
        target = window;
    }
    RECT rectangle;
    POINT screen;
    screen.x = client.x;
    screen.y = client.y;
    ClientToScreen(target, &screen);
    info.Screen = screen;
    GetWindowRect(window, &rectangle);
    screen.x -= rectangle.left;
    screen.y -= rectangle.top;
    info.Window = window;
    info.Target = target;
    info.Tree = unknown_tree_b25f30;
    info.Item = unknown_drag_item_b25f34;
    info.Client = client;
    info.Offset = screen;
    return info;
}


// void __fastcall unknown_drag_scroll_736570(int timer) (0x00736570)

void __fastcall unknown_drag_scroll_736570(int timer)
{
    if (timer == 1 && unknown_dragging_b25f2c) {
        POINT cursor;
        GetCursorPos(&cursor);
        RECT rect;
        GetWindowRect(unknown_tree_b25f30, &rect);
        int above = rect.top - cursor.y;
        int below = cursor.y - rect.bottom;
        int distance = above > below ? above : below;
        distance = distance > 0 ? distance : 0;
        if (distance > 0) {
            int delay = 500 - distance * 40;
            if (delay <= 5) {
                delay = 5;
            }
            SetTimer((HWND)GetWindowLongA(unknown_tree_b25f30, -8), 1, delay, 0);
            LPARAM item = SendMessageA(unknown_tree_b25f30, 0x110A, 5, 0);     // TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE
            if (above > 0) {
                item = SendMessageA(unknown_tree_b25f30, 0x110A, 7, item);     // TVGN_PREVIOUSVISIBLE
            } else {
                item = SendMessageA(unknown_tree_b25f30, 0x110A, 6, item);     // TVGN_NEXTVISIBLE
            }
            ImageList_DragShowNolock(false);
            SendMessageA(unknown_tree_b25f30, 0x110B, 5, item);               // TVM_SELECTITEM, TVGN_FIRSTVISIBLE
            ImageList_DragShowNolock(true);
        }
    }
}


// bool __fastcall unknown_drag_move_736670(unsigned long position) (0x00736670)

bool __fastcall unknown_drag_move_736670(unsigned long position)
{
    if (unknown_dragging_b25f2c) {
        bool accepted;
        ImageList_DragShowNolock(false);
        UnknownDragInfo info = unknown_drag_info_736490(position);
        UnknownDragInfo data = info;
        if (SendMessageA(info.Target, 0x4C8, (WPARAM)&data, position)) {
            SetCursor(LoadCursorA(0, (LPCSTR)0x7F00));    // IDC_ARROW
            accepted = true;
        } else {
            SetCursor(LoadCursorA(0, (LPCSTR)0x7F88));    // IDC_NO
            accepted = false;
        }
        ImageList_DragShowNolock(true);
        RECT rect;
        GetWindowRect(data.Target, &rect);
        ImageList_DragMove(data.Screen.x - rect.left, data.Screen.y - rect.top);
        return accepted;
    }
    return false;
}


// bool __fastcall unknown_set_expand_image_736750(HWND, NMTREEVIEWA *, int, int) (0x00736750)

bool __fastcall unknown_set_expand_image_736750(HWND tree, NMTREEVIEWA *info, int closed_image, int open_image)
{
    TVITEMA item;
    bool expanded = false;
    item.hItem = info->itemNew.hItem;
    item.mask = 0x32;           // TVIF_IMAGE | TVIF_HANDLE | TVIF_SELECTEDIMAGE
    switch (info->action) {
    case 1:
        item.iImage = closed_image;
        item.iSelectedImage = closed_image;
        expanded = false;
        break;
    case 2:
        item.iImage = open_image;
        item.iSelectedImage = open_image;
        expanded = true;
        break;
    }
    SendMessageA(tree, 0x110D, 0, (LPARAM)&item);
    return expanded;
}


// void __fastcall unknown_begin_label_edit_7367b0(HWND tree, char *buffer, int limit) (0x007367B0)

void __fastcall unknown_begin_label_edit_7367b0(HWND tree, char *buffer, int limit)
{
    HWND edit = (HWND)SendMessageA(tree, 0x110F, 0, 0);   // TVM_GETEDITCONTROL
    if (edit != 0) {
        SendMessageA(edit, 0xC5, limit, 0);               // EM_LIMITTEXT
        unknown_label_buffer_b25f10 = buffer;
        unknown_tree_b25f30 = tree;
    }
}


// int __fastcall unknown_end_label_edit_7367f0(NMTVDISPINFOA *info) (0x007367F0)

int __fastcall unknown_end_label_edit_7367f0(NMTVDISPINFOA *info)
{
    char *text = info->item.pszText;
    if (text != 0) {
        strtrim(text);
        strcpy(unknown_label_buffer_b25f10, text);
        TVITEMA item;
        item.hItem = info->item.hItem;
        item.mask = 1;              // TVIF_TEXT
        item.pszText = unknown_label_buffer_b25f10;
        SendMessageA(unknown_tree_b25f30, 0x110D, 0, (LPARAM)&item);   // TVM_SETITEMA
        return strlen(unknown_label_buffer_b25f10);
    }
    return 0;
}


// void __fastcall unknown_context_help_7368a0(HWND window) (0x007368A0)

void __fastcall unknown_context_help_7368a0(HWND window)
{
    DWORD ids[4] = {0, 0, 0, 0};
    ids[0] = GetWindowLongA(window, -12);           // GWL_ID
    ids[1] = GetWindowContextHelpId(window);
    WinHelpA(window, "SUN.HLP", 0x0A, (DWORD)ids);   // HELP_CONTEXTMENU
}


// BOOL __stdcall unknown_restore_child_window_7368f0(HWND window, long parameter) (0x007368F0)
// EnumChildWindows callback of 0x00736A70; parameter is the INIClass.

BOOL __stdcall unknown_restore_child_window_7368f0(HWND window, long parameter)
{
    INIClass &ini = *(INIClass *)parameter;
    if (Is_Window_Class(window, "SysTreeView32")) {
        char name[64];
        char const *section = unknown_window_section_b25f28;
        int index = 0;
        for (LPARAM item = SendMessageA(window, 0x110A, 0, 0); item != 0;
             item = SendMessageA(window, 0x110A, 1, item)) {                    // TVGN_ROOT, TVGN_NEXT
            index++;
            wsprintfA(name, "TV%d", index);
            if (ini.Get_Bool(section, name, false)) {
                SendMessageA(window, 0x1102, 2, item);     // TVM_EXPAND, TVE_EXPAND
            } else {
                SendMessageA(window, 0x1102, 1, item);     // TVE_COLLAPSE
            }
        }
    }
    if (Is_Window_Class(window, "SysListView32")) {
        char name[64];
        char const *section = unknown_window_section_b25f28;
        if (window != 0) {
            for (int column = 0; column < 10; column++) {
                wsprintfA(name, "LV%d", column);
                unsigned width = ini.Get_Int(section, name, SendMessageA(window, 0x101D, column, 0));   // LVM_GETCOLUMNWIDTH
                if (width < 1000) {
                    SendMessageA(window, 0x101E, column, (unsigned short)width);              // LVM_SETCOLUMNWIDTH
                }
            }
        }
    }
    return 1;
}


// void __fastcall unknown_restore_window_position_736a70(char const *section, HWND window, INIClass &ini) (0x00736A70)

void __fastcall unknown_restore_window_position_736a70(char const *section, HWND window, INIClass &ini)
{
    if (window != 0) {
        RECT rect;
        GetWindowRect(window, &rect);
        int x = ini.Get_Int(section, "X", rect.left);
        int y = ini.Get_Int(section, "Y", rect.top);
        int screen_width = GetSystemMetrics(0);                       // SM_CXSCREEN
        if (rect.right - rect.left + x > screen_width) {
            x = screen_width - rect.right + rect.left;
        }
        int screen_height = GetSystemMetrics(1);                      // SM_CYSCREEN
        if (rect.bottom - rect.top + y > screen_height) {
            y = screen_height - rect.bottom + rect.top;
        }
        if (x < 0) {
            x = 0;
        }
        if (y < 0) {
            y = 0;
        }
        SetWindowPos(window, 0, x, y, 0, 0, 5);                         // SWP_NOSIZE | SWP_NOZORDER
        unknown_window_section_b25f28 = section;
        EnumChildWindows(window, unknown_restore_child_window_7368f0, (long)&ini);
    }
}


// BOOL __stdcall unknown_save_child_window(HWND window, long parameter) (0x00736B40)
// EnumChildWindows callback of 0x00736CD0; parameter is the INIClass.

BOOL __stdcall unknown_save_child_window(HWND window, long parameter)
{
    INIClass &ini = *(INIClass *)parameter;
    if (Is_Window_Class(window, "SysTreeView32")) {
        char key[32];
        char const *section = unknown_window_section_b25f28;
        int index = 0;
        for (LPARAM item = SendMessageA(window, 0x110A, 0, 0); item != 0;
             item = SendMessageA(window, 0x110A, 1, item)) {
            index++;
            wsprintfA(key, "TV%d", index);
            TVITEMA info;
            info.mask = 0x18;               // TVIF_STATE | TVIF_HANDLE
            info.stateMask = 0x20;          // TVIS_EXPANDED
            info.hItem = (void *)item;
            if (SendMessageA(window, 0x110C, 0, (LPARAM)&info)) {             // TVM_GETITEM
                bool expanded = false;
                if (info.state & 0x20) {
                    expanded = true;
                }
                ini.Put_Bool(section, key, expanded);
            }
        }
    }
    if (Is_Window_Class(window, "SysListView32")) {
        char key[32];
        char const *section = unknown_window_section_b25f28;
        if (window != 0) {
            for (int column = 0; column < 10; column++) {
                wsprintfA(key, "LV%d", column);
                unsigned width = SendMessageA(window, 0x101D, column, 0);
                if (width < 1000) {
                    ini.Put_Int(section, key, width);
                }
            }
        }
    }
    return 1;
}


// int __fastcall unknown_combo_find_data_736e10(HWND combo, int start, int data) (0x00736E10)

int __fastcall unknown_combo_find_data_736e10(HWND combo, int start, int data)
{
    int count = SendMessageA(combo, 0x146, 0, 0);    // CB_GETCOUNT
    for (int index = start; index < count; index++) {
        if (SendMessageA(combo, 0x150, index, 0) == data) {   // CB_GETITEMDATA
            return index;
        }
    }
    return -1;
}


// void __fastcall unknown_mouse_activate_736e70(HWND window, short active) (0x00736E70)

void __fastcall unknown_mouse_activate_736e70(HWND window, short active)
{
    if (WWMouse != 0) {
        if (active == 0) {
            WWMouse->Capture_Mouse();
            return;
        }
        WWMouse->Release_Mouse();
        RedrawWindow(window, 0, 0, 0x587);
    }
}


// 0x00736ea0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_00736ea0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00736ea0 {

void  FUN_00736ea0(void)
{
    return;
}

}  // namespace seed_00736ea0


// char *WindowsVersionInfo::Version_Name() (0x00736F40)

char *WindowsVersionInfo::Version_Name()
{
    if (IsWin9x && MajorVersion == 4 && MinorVersion == 0) {
        strcpy(unknown_version_name_b25df0, "Windows 95 ");
    }
    if (IsWin9x && (MajorVersion > 4 || (MajorVersion == 4 && MinorVersion >= 10))) {
        strcpy(unknown_version_name_b25df0, "Windows 98 ");
    }
    if (IsWinNT && MajorVersion >= 5) {
        strcpy(unknown_version_name_b25df0, "Windows NT 5 ");
    }
    if (IsWinNT) {
        strcpy(unknown_version_name_b25df0, "Windows NT 4 ");
    }
    strcat(unknown_version_name_b25df0, AdditionalInfo);
    return unknown_version_name_b25df0;
}


// void __fastcall unknown_center_on_owner_737060(HWND window) (0x00737060)

void __fastcall unknown_center_on_owner_737060(HWND window)
{
    HWND owner = (HWND)GetWindowLongA(window, -8);   // GWL_HWNDPARENT
    if (owner != 0) {
        unknown_center_window_737080(window, owner);
    }
}


// void __fastcall unknown_center_window_737080(HWND window, HWND parent) (0x00737080)

void __fastcall unknown_center_window_737080(HWND window, HWND parent)
{
    RECT parent_rect;
    GetClientRect(parent, &parent_rect);
    if (parent == MainWindow) {
        parent_rect.right = unknown_8523e0;
        parent_rect.bottom = unknown_8523e4;
    }
    ClientToScreen(parent, (POINT *)&parent_rect.left);
    ClientToScreen(parent, (POINT *)&parent_rect.right);
    parent_rect.right -= parent_rect.left;
    parent_rect.bottom -= parent_rect.top;

    RECT rect;
    GetClientRect(window, &rect);
    ClientToScreen(window, (POINT *)&rect.left);
    ClientToScreen(window, (POINT *)&rect.right);
    rect.right -= rect.left;
    rect.bottom -= rect.top;

    int x = (parent_rect.right - rect.right + 1) / 2;
    int y = (parent_rect.bottom - rect.bottom + 1) / 2;
    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    SetWindowPos(window, 0, x, y, -1, -1, 5);   // SWP_NOSIZE | SWP_NOZORDER
}


// void __fastcall unknown_touch_pages_737160(char *buffer, int length) (0x00737160)

void __fastcall unknown_touch_pages_737160(char *buffer, int length)
{
    int offset = (int)buffer & 0xFFF;
    char *page = buffer + offset;
    length -= offset;
    if (length < 1) {
        return;
    }
    length -= length & 0xFFF;
    if (length < 1) {
        return;
    }
    int sum = 0;
    for (int index = 0x10000; index < length; index += 0x1000) {
        sum = sum + page[index] + page[index - 0x10000];
    }
}
