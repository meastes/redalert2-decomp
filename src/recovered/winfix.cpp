/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// From mischa85/RedAlert2 392e904 src/winfix.cpp (matching RA2 1.006 decomp; Renegade/TibSun WinFix.CPP): function,
// global and parameter names and comments; modified (our module form and header state, Route A unit map
// manifests/original_target/modules/winfix.json).

/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                     $Archive:: /Renegade Setup/Autorun/WinFix.CPP                          $*
 *                                                                                             *
 *                      $Author:: Maria_l                                                     $*
 *                                                                                             *
 *                     $Modtime:: 11/15/01 10:44a                                             $*
 *                                                                                             *
 *                    $Revision:: 6                                                           $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Make_Identifier -- Creates a temporary string identifer.                                  *
 *   WindowsVersionInfo::WindowsVersionInfo -- Windows Version Info constructor.               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "shared/win_util.hpp"
#include "shared/crt_string.hpp"
#include "shared/ini_class.hpp"
#include "shared/mouse.hpp"
#include "shared/windows_version.hpp"
#include "shared/win32_window.hpp"
#include "shared/unknown_globals.hpp"


/// <summary>
/// Starts dragging an item out of a tree view.
/// This routine asks the dialog that owns the tree for permission with a TVDRAG_BEGIN
/// message, and if it is granted the drag image is built, the mouse is captured, and the
/// edge scroll timer is started.
/// </summary>
/// <param name="window">The dialog that owns the tree view.</param>
/// <param name="hwndTV">The tree view the item is being dragged out of.</param>
/// <param name="lpnmtv">The begin drag notification describing the item.</param>
/// <returns>bool; Is a drag now under way?</returns>
// (0x00736230)

bool __fastcall TreeView_Begin_Drag(HWND window, HWND hwndTV, NMTREEVIEWA *lpnmtv)
{
    if (lpnmtv->itemNew.lParam != 0) {
        if (!SendMessageA(window, 0x4C9, (WPARAM)hwndTV, (LPARAM)lpnmtv->itemNew.hItem)) {
            return false;
        }
        TreeView_DraggedItem = lpnmtv->itemNew.hItem;
        SendMessageA(hwndTV, 0x110B, 9, 0);                                     // TVM_SELECTITEM, TVGN_DROPHILITE
        TreeView_ImageList = (void *)SendMessageA(hwndTV, 0x1112, 0, (LPARAM)lpnmtv->itemNew.hItem);   // TVM_CREATEDRAGIMAGE
        RECT rect;
        *(void **)&rect = lpnmtv->itemNew.hItem;
        SendMessageA(hwndTV, 0x1104, 0, (LPARAM)&rect);                         // TVM_GETITEMRECT
        int indent = SendMessageA(hwndTV, 0x1106, 0, 0);                        // TVM_GETINDENT
        int x = lpnmtv->ptDrag.x - indent - rect.left;
        int y = lpnmtv->ptDrag.y - rect.top;
        SetCapture(window);
        ImageList_BeginDrag(TreeView_ImageList, 0, x, y);
        ImageList_DragEnter(window, 0, 0);
        SetTimer(window, 1, 1, 0);
        s_dragging = true;
        TreeView_LastHandle = hwndTV;
    }
    return s_dragging;
}


/// <summary>
/// Finishes a tree view drag operation.
/// This routine tears down the drag image and gives the mouse back, then tells the
/// dialog that owns the tree to take the item with a TVDRAG_DROP message -- but only if
/// the cursor came to rest somewhere that will have it.
/// </summary>
/// <param name="lParam">The cursor position, packed as it arrives with the message.</param>
/// <returns>bool; Was a drag actually under way?</returns>
// (0x00736320)

bool __fastcall TreeView_End_Drag(unsigned long lParam)
{
    if (s_dragging) {
        bool ok = TreeView_On_Mouse_Move(lParam);
        ImageList_EndDrag();
        ReleaseCapture();
        ImageList_Destroy(TreeView_ImageList);
        TreeView_ImageList = 0;
        s_dragging = false;
        item_data info = TreeView_get_item_data(lParam);
        item_data data = info;
        if (ok) {
            SendMessageA(info.Target, 0x4CA, (WPARAM)&data, lParam);
        }
        KillTimer(data.Target, 1);
        SendMessageA(TreeView_LastHandle, 0x110B, 8, 0);      // TVM_SELECTITEM, TVGN_DROPHILITE
        return true;
    }
    return false;
}


/// <summary>
/// Fetches the drop context under the cursor during a tree view drag.
/// This routine works out which window the cursor is over and bundles it up with the
/// item being dragged and the cursor position in each of the coordinate spaces the
/// receiver might want. The bundle is what rides along with the TVDRAG messages.
/// </summary>
/// <param name="xy">The cursor position, packed as it arrives with the message.</param>
/// <returns>Returns with the drop context describing what lies under the cursor.</returns>
item_data __fastcall TreeView_get_item_data(unsigned long xy)
{
    item_data data;
    POINT point;
    point.x = xy & 0xffff;
    point.y = xy >> 16;
    HWND window = ChildWindowFromPoint((HWND)GetWindowLongA(TreeView_LastHandle, -8), point);
    HWND target = (HWND)GetWindowLongA(window, -8);
    if (window == (HWND)GetWindowLongA(TreeView_LastHandle, -8)) {
        target = window;
    }
    RECT rectangle;
    POINT screen;
    screen.x = point.x;
    screen.y = point.y;
    ClientToScreen(target, &screen);
    data.Screen = screen;
    GetWindowRect(window, &rectangle);
    screen.x -= rectangle.left;
    screen.y -= rectangle.top;
    data.Window = window;
    data.Target = target;
    data.Tree = TreeView_LastHandle;
    data.Item = TreeView_DraggedItem;
    data.Client = point;
    data.Offset = screen;
    return data;
}


/// <summary>
/// Scrolls a tree view that an item is being dragged off the end of.
/// This routine is called from the drag timer. Whenever the cursor has strayed above or
/// below the tree, the view is scrolled toward it and the timer is retuned so that the
/// further out the cursor has wandered, the faster the tree scrolls.
/// </summary>
/// <param name="timer_id">The timer identifier reported with the message.</param>
// (0x00736570)

void __fastcall TreeView_handle_item_drag(int timer_id)
{
    if (timer_id == 1 && s_dragging) {
        POINT cursor;
        GetCursorPos(&cursor);
        RECT rect;
        GetWindowRect(TreeView_LastHandle, &rect);
        int above = rect.top - cursor.y;
        int below = cursor.y - rect.bottom;
        int distance = above > below ? above : below;
        distance = distance > 0 ? distance : 0;
        if (distance > 0) {
            int delay = 500 - distance * 40;
            if (delay <= 5) {
                delay = 5;
            }
            SetTimer((HWND)GetWindowLongA(TreeView_LastHandle, -8), 1, delay, 0);
            LPARAM item = SendMessageA(TreeView_LastHandle, 0x110A, 5, 0);     // TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE
            if (above > 0) {
                item = SendMessageA(TreeView_LastHandle, 0x110A, 7, item);     // TVGN_PREVIOUSVISIBLE
            } else {
                item = SendMessageA(TreeView_LastHandle, 0x110A, 6, item);     // TVGN_NEXTVISIBLE
            }
            ImageList_DragShowNolock(false);
            SendMessageA(TreeView_LastHandle, 0x110B, 5, item);               // TVM_SELECTITEM, TVGN_FIRSTVISIBLE
            ImageList_DragShowNolock(true);
        }
    }
}


/// <summary>
/// Handles mouse movement during a tree view drag.
/// This routine asks the dialog that owns the tree, by way of a TVDRAG_OVER message,
/// whether the item may be dropped where the cursor now rests. The cursor shape is set
/// to suit the answer and the drag image is dragged along behind it.
/// </summary>
/// <param name="lParam">The cursor position, packed as it arrives with the message.</param>
/// <returns>bool; Is the cursor over somewhere the item may be dropped?</returns>
// (0x00736670)

bool __fastcall TreeView_On_Mouse_Move(unsigned long lParam)
{
    if (s_dragging) {
        bool ok;
        ImageList_DragShowNolock(false);
        item_data info = TreeView_get_item_data(lParam);
        item_data data = info;
        if (SendMessageA(info.Target, 0x4C8, (WPARAM)&data, lParam)) {
            SetCursor(LoadCursorA(0, (LPCSTR)0x7F00));    // IDC_ARROW
            ok = true;
        } else {
            SetCursor(LoadCursorA(0, (LPCSTR)0x7F88));    // IDC_NO
            ok = false;
        }
        ImageList_DragShowNolock(true);
        RECT rect;
        GetWindowRect(data.Target, &rect);
        ImageList_DragMove(data.Screen.x - rect.left, data.Screen.y - rect.top);
        return ok;
    }
    return false;
}


/// <summary>
/// Sets the icon of a tree view item that is opening or closing.
/// This routine is called from the item expand notification so that a folder style item
/// can show the appropriate icon for its new state.
/// </summary>
/// <param name="nmtv">The expand notification naming the item and what is happening to
/// it.</param>
/// <param name="collapse_image">The image to show when the item closes.</param>
/// <param name="expand_image">The image to show when the item opens.</param>
/// <returns>bool; Is the item now open?</returns>
// (0x00736750)

bool __fastcall TreeView_set_item_image(HWND window, NMTREEVIEWA *nmtv, int collapse_image, int expand_image)
{
    TVITEMA item;
    bool expanded = false;
    item.hItem = nmtv->itemNew.hItem;
    item.mask = 0x32;           // TVIF_IMAGE | TVIF_HANDLE | TVIF_SELECTEDIMAGE
    switch (nmtv->action) {
    case 1:
        item.iImage = collapse_image;
        item.iSelectedImage = collapse_image;
        expanded = false;
        break;
    case 2:
        item.iImage = expand_image;
        item.iSelectedImage = expand_image;
        expanded = true;
        break;
    }
    SendMessageA(window, 0x110D, 0, (LPARAM)&item);
    return expanded;
}


/// <summary>
/// Prepares a tree view label for editing.
/// This routine is called when the label editor appears. It caps how much the player may
/// type and remembers the buffer that TreeView_set_item_text should write the finished
/// label back into.
/// </summary>
/// <param name="text">The buffer that will receive the edited label.</param>
/// <param name="limit">The most characters the player may type.</param>
// (0x007367B0)

void __fastcall TreeView_set_edit_text(HWND window, char *text, int limit)
{
    HWND ctrl = (HWND)SendMessageA(window, 0x110F, 0, 0);   // TVM_GETEDITCONTROL
    if (ctrl != 0) {
        SendMessageA(ctrl, 0xC5, limit, 0);               // EM_LIMITTEXT
        TreeView_Text = text;
        TreeView_LastHandle = window;
    }
}


/// <summary>
/// Commits an edited tree view label.
/// This routine is called when the label editor closes. The new text is trimmed of the
/// white space the player left around it, copied back into the buffer that
/// TreeView_set_edit_text was given, and put back into the item.
/// </summary>
/// <param name="info">The label edit notification carrying the new text.</param>
/// <returns>Returns with the length of the text accepted, or zero if the edit was
/// abandoned.</returns>
// (0x007367F0)

int __fastcall TreeView_set_item_text(NMTVDISPINFOA *info)
{
    char *text = info->item.pszText;
    if (text != 0) {
        strtrim(text);
        strcpy(TreeView_Text, text);
        TVITEMA item;
        item.hItem = info->item.hItem;
        item.mask = 1;              // TVIF_TEXT
        item.pszText = TreeView_Text;
        SendMessageA(TreeView_LastHandle, 0x110D, 0, (LPARAM)&item);   // TVM_SETITEMA
        return strlen(TreeView_Text);
    }
    return 0;
}


/// <summary>
/// Handles the WM_CONTEXTMENU message for a dialog control.
/// This routine puts up the "What's This?" help menu for the control that was right
/// clicked, drawing the text from SUN.HLP.
/// </summary>
/// <param name="window">Handle of the control that was clicked.</param>
/// <returns>bool; Was the help menu put up?</returns>
// (0x007368A0)

void __fastcall On_WM_CONTEXTMENU(HWND window)
{
    DWORD identifiers[4] = {0, 0, 0, 0};
    identifiers[0] = GetWindowLongA(window, -12);           // GWL_ID
    identifiers[1] = GetWindowContextHelpId(window);
    WinHelpA(window, "SUN.HLP", 0x0A, (DWORD)identifiers);   // HELP_CONTEXTMENU
}


/// <summary>
/// Reads one child view's state back from the INI database.
/// This is the child window callback that read_views_from_ini hands to Windows. A tree
/// view has its root items expanded or collapsed as recorded, a list view has its column
/// widths restored, and anything else is left alone.
/// </summary>
/// <returns>Always true, so the enumeration carries on to the next child.</returns>
// (0x007368F0)

BOOL __stdcall read_view_from_ini(HWND window, long parameter)
{
    INIClass &ini = *(INIClass *)parameter;
    if (Is_Window_Class(window, "SysTreeView32")) {
        char name[64];
        char const *section = last_view_ini_section_name;
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
        char const *section = last_view_ini_section_name;
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


/// <summary>
/// Reads a window's position and view layout back from the INI database.
/// This routine restores the position that write_views_to_ini recorded, nudging the
/// window back onto the display if the screen has shrunk since, then visits every child
/// control so each tree view and list view can restore its own state.
/// </summary>
/// <param name="section">The INI section holding the window's state.</param>
// (0x00736A70)

void __fastcall read_views_from_ini(char const *section, HWND window, INIClass &ini)
{
    if (window != 0) {
        RECT rect;
        GetWindowRect(window, &rect);
        int x = ini.Get_Int(section, "X", rect.left);
        int y = ini.Get_Int(section, "Y", rect.top);
        int screen_x = GetSystemMetrics(0);                       // SM_CXSCREEN
        if (rect.right - rect.left + x > screen_x) {
            x = screen_x - rect.right + rect.left;
        }
        int screen_y = GetSystemMetrics(1);                      // SM_CYSCREEN
        if (rect.bottom - rect.top + y > screen_y) {
            y = screen_y - rect.bottom + rect.top;
        }
        if (x < 0) {
            x = 0;
        }
        if (y < 0) {
            y = 0;
        }
        SetWindowPos(window, 0, x, y, 0, 0, 5);                         // SWP_NOSIZE | SWP_NOZORDER
        last_view_ini_section_name = section;
        EnumChildWindows(window, read_view_from_ini, (long)&ini);
    }
}


/// <summary>
/// Writes one child view's state to the INI database.
/// This is the child window callback that write_views_to_ini hands to Windows. A tree
/// view records which of its root items are expanded, a list view records its column
/// widths, and anything else is left alone.
/// </summary>
/// <returns>Always true, so the enumeration carries on to the next child.</returns>
// (0x00736B40)

BOOL __stdcall write_view_to_ini(HWND window, long parameter)
{
    INIClass &ini = *(INIClass *)parameter;
    if (Is_Window_Class(window, "SysTreeView32")) {
        char key[32];
        char const *section = last_view_ini_section_name;
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
        char const *section = last_view_ini_section_name;
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


/// <summary>
/// Finds the combo box entry that carries a particular item data value.
/// This routine stands in for the Windows macro of the same purpose, walking the entries
/// from the starting index onward and stopping at the first one whose attached data
/// matches.
/// </summary>
/// <param name="indexStart">The entry to begin the search at. Pass -1 to search from the
/// top of the list.</param>
/// <param name="data">The item data value being looked for.</param>
/// <returns>Returns with the index of the matching entry, or -1 if no entry carries that
/// data.</returns>
// (0x00736E10)

int __fastcall ComboBox_Find_Item_Data(HWND hwndCtl, int indexStart, int data)
{
    int count = SendMessageA(hwndCtl, 0x146, 0, 0);    // CB_GETCOUNT
    for (int index = indexStart; index < count; index++) {
        if (SendMessageA(hwndCtl, 0x150, index, 0) == data) {   // CB_GETITEMDATA
            return index;
        }
    }
    return -1;
}


/// <summary>
/// Takes the mouse for the game, or hands it back to Windows.
/// This routine is used when a window has to be given the mouse for a while. Handing the
/// mouse back also forces a full repaint of the window and everything inside it, since
/// the game cursor will have scribbled over the frame while it had control.
/// </summary>
/// <param name="window">The window to repaint when the mouse is handed back.</param>
/// <param name="release">Zero to take the mouse, otherwise hand it back.</param>
// (0x00736E70)

void __fastcall Capture_Or_Release_Mouse(HWND window, short release)
{
    if (WWMouse != 0) {
        if (release == 0) {
            WWMouse->Capture_Mouse();
            return;
        }
        WWMouse->Release_Mouse();
        RedrawWindow(window, 0, 0, 0x587);
    }
}


// (0x00736EA0)
void noop(void)
{
}


/***********************************************************************************************
 * WindowsVersionInfo::Version_String -- Get the version number in human readable form         *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Ptr to string containing version info                                             *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   3/30/99 10:29PM ST : Created                                                              *
 *=============================================================================================*/
// (0x00736F40)

char *WindowsVersionInfo::Version_String()
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


/// <summary>
/// Centers a window over its owner.
/// This is the convenient form for dialog handlers that have no parent handle at hand.
/// A window with no owner is left where it is.
/// </summary>
/// <param name="window">The window to be moved.</param>
// (0x00737060)

void __fastcall Center_Window_Within_Window(HWND window)
{
    HWND owner = (HWND)GetWindowLongA(window, -8);   // GWL_HWNDPARENT
    if (owner != 0) {
        Center_Window_Within_Window(window, owner);
    }
}


/// <summary>
/// Centers a window over another window.
/// This routine is used by the dialog handlers to place a dialog over the window that
/// spawned it. The main game window is measured by the current video mode rather than by
/// its client area, so a dialog lands centered on what the player can actually see.
/// </summary>
/// <param name="window">The window to be moved.</param>
/// <param name="parent">The window to center over.</param>
// (0x00737080)

void __fastcall Center_Window_Within_Window(HWND window, HWND parent)
{
    RECT parent_rect;
    GetClientRect(parent, &parent_rect);
    if (parent == MainWindow) {
        parent_rect.right = VideoModeWidth;
        parent_rect.bottom = VideoModeHeight;
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


// void __fastcall Prefetch_Audio_Buffer(char *src, int size) (0x00737160)

void __fastcall Prefetch_Audio_Buffer(char *src, int size)
{
    int offset = (int)src & 0xFFF;
    char *page = src + offset;
    size -= offset;
    if (size < 1) {
        return;
    }
    size -= size & 0xFFF;
    if (size < 1) {
        return;
    }
    int sum = 0;
    for (int index = 0x10000; index < size; index += 0x1000) {
        sum = sum + page[index] + page[index - 0x10000];
    }
}
