// Behaviour rows (Scout1119, native M14; NOT EXACT, native/behaviour_rewrites.json):
// the owner-draw control procedures the menus reach that have no recovered body
// (OwnerDraw::Subclass 0x005ED180 installs them per control class). Input and
// state follow the pass-6a decompiles (the click sound, the flash timer of
// message 0x4DC, the disabled flag +0xBC, everything else to the original
// Win32 procedure through the window-proc table 0x00A73688); the painting is
// simplified: the control's image (+0x14, pressed +0x18) or its text (+0x28)
// in its font (+0x64) through the recovered text drawer 0x005FE1B0, a frame for
// framed buttons, list and combo items as text. The originals also draw the
// three-piece PCX button art (b_c_c_li/mi/ri) and the tab/list/combo art.
#define RA2_OWNERDRAW_CONTROL_DICTIONARY
#define RA2_OWNERDRAW_WNDPROC_DICTIONARY
#define RA2_OWNERDRAW_CHECK_FIELDS
#define RA2_OWNERDRAW_BUTTON_FIELDS
#define RA2_OWNERDRAW_EDIT_FIELDS
#include "shared/unknown_globals.hpp"
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
#include "shared/surface.hpp"
#include "shared/text_print.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/wide_string.hpp"
#include "shared/owner_draw_dialog_control.hpp"

extern WolDictionary<HWND, WNDPROC> unknown_window_proc_table_a73688;   // 0x00A73688 (its initializer 0x005DE9C0)

void __fastcall Unknown_006FA0F0(char *out, unsigned short const *text, int size);   // 0x006FA0F0 (wide to narrow)

namespace OwnerDraw {

BOOL __stdcall unknown_5ed180(HWND window, LPARAM lparam);                 // 0x005ED180 (subclass a control)
BOOL __stdcall unknown_restore_child_5edd30(HWND window, LPARAM lparam);   // 0x005EDD30

namespace {

LRESULT Original(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    WNDPROC proc = 0;
    unknown_window_proc_table_a73688.Get_Value(window, proc);
    return CallWindowProcA(proc, window, message, wparam, lparam);
}

// The control's rectangle on the background surface (0x00839958).
Rect Area(HWND window)
{
    RECT area = {0, 0, 0, 0};
    Window_Rect_In_Main_Client(window, &area);
    return Rect(area.left, area.top, area.right - area.left, area.bottom - area.top);
}

void Frame(Rect const &area, int color)
{
    Surface *s = unknown_surface_839958;
    int r = area.X + area.Width - 1, b = area.Y + area.Height - 1;
    s->Draw_Line(Point2D(area.X, area.Y), Point2D(r, area.Y), color);
    s->Draw_Line(Point2D(area.X, b), Point2D(r, b), color);
    s->Draw_Line(Point2D(area.X, area.Y), Point2D(area.X, b), color);
    s->Draw_Line(Point2D(r, area.Y), Point2D(r, b), color);
}

void Text(UnknownControlRecord *record, Rect const &area, wchar_t const *text, int x_alignment, int y_alignment)
{
    if (!text || !*text) return;
    int length = 0;
    while (text[length]) ++length;
    unknown_text_draw_005fe1b0(unknown_color_a733e4, record ? record->font_64 : 0, area, text, length, x_alignment, y_alignment,
                               unknown_surface_839958, 0);
}

void Image(Surface *image, Rect const &area)
{
    Rect from(0, 0, area.Width, area.Height);
    unknown_surface_839958->Copy_From(area, *image, from, true, true);
}

// A list or combo item's text (strings kept by the Win32 control), widened.
bool Item_Text(HWND window, bool combo, int index, wchar_t *out, int size)
{
    char text[256];
    LRESULT length = Original(window, combo ? 0x149 : 0x18A, index, 0);
    if (length < 0 || length >= (LRESULT)sizeof text) return false;
    if (Original(window, combo ? 0x148 : 0x189, index, (LPARAM)text) < 0) return false;
    int i = 0;
    for (; text[i] && i < size - 1; ++i) out[i] = (unsigned char)text[i];
    out[i] = 0;
    return true;
}

} // namespace

// 0x005F0320: the push button procedure.
LRESULT __stdcall unknown_button_proc_5f0320(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    UnknownControlRecord *record = Get_Record(window);
    if (!record) {
        return Original(window, message, wparam, lparam);
    }
    unsigned char *flash = (unsigned char *)&record->unknown_c4;     // +0xC4 flashing, +0xC5 lit
    switch (message) {
    case 0x6: case 0x8: case 0x21:                  // WM_ACTIVATE, WM_KILLFOCUS, WM_MOUSEACTIVATE
        return 0;
    case 0x113:                                     // WM_TIMER: the flash
        flash[1] = !flash[1];
        InvalidateRect(window, 0, TRUE);
        return 0;
    case 0x201: case 0x203:                         // WM_LBUTTONDOWN / DBLCLK
        if (record->unknown_bc) return 0;
        Voc_Play(Rules->unknown_180, 1.0f, 0x2000, 0);   // GUIMainButtonSound
        break;
    case 0x4DC:                                     // flash on (lparam 1) / off
        if (lparam == 1) {
            if (!flash[0]) { flash[0] = 1; SetTimer(window, 0, 1000, 0); }
        } else if (flash[0]) {
            flash[0] = 0;
            flash[1] = 0;
            KillTimer(window, 0);
            InvalidateRect(window, 0, TRUE);
        }
        break;
    case 0xF: {                                     // WM_PAINT
        if (record->unknown_bc) {
            ValidateRect(window, 0);
            return 0;
        }
        Rect area = Area(window);
        bool pressed = (Original(window, 0xF2, 0, 0) & 4) != 0;   // BM_GETSTATE: pushed
        if (record->image_14) {
            Image(pressed && record->alternate_image_18 ? record->alternate_image_18 : record->image_14, area);
        } else {
            int color = unknown_5fda40(flash[1] ? unknown_color_a733e4 : unknown_color_a76158);
            Frame(area, color);
            if (pressed) Frame(Rect(area.X + 1, area.Y + 1, area.Width - 2, area.Height - 2), color);
            Rect text = area;
            if (pressed) { text.X += 2; text.Y += 2; }
            Text(record, text, record->text_28, 1, 1);
        }
        ValidateRect(window, 0);
        return 0;
    }
    }
    return Original(window, message, wparam, lparam);
}

// 0x005F2B50: the static (label/picture) procedure.
LRESULT __stdcall unknown_static_proc_5f2b50(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    UnknownControlRecord *record = Get_Record(window);
    if (!record) {
        return Original(window, message, wparam, lparam);
    }
    switch (message) {
    case 0x14: return 1;                            // WM_ERASEBKGND
    case 0xF: {
        if (!record->unknown_bc) {
            Rect area = Area(window);
            LONG style = GetWindowLongA(window, GWL_STYLE);
            if (record->image_14) {
                Image(record->image_14, area);
            } else {
                int x_alignment = (style & 3) == 1 ? 1 : (style & 3) == 2 ? 4 : 0;   // SS_CENTER / SS_RIGHT
                Text(record, area, record->text_28, x_alignment, (style & 0x200) ? 1 : 0);   // SS_CENTERIMAGE
            }
        }
        ValidateRect(window, 0);
        return 0;
    }
    }
    return Original(window, message, wparam, lparam);
}

// 0x005F6690: the list box procedure.
LRESULT __stdcall unknown_list_proc_5f6690(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    UnknownControlRecord *record = Get_Record(window);
    if (record && (message == 0x4C8 || message == 0x4CD || message == 0x4C9 || message == 0x4CE)) {
        // Wide text rows (pass-6a decompile 005f6690 case 0x4CD: LB_ADDSTRING 0x180, or
        // LB_INSERTSTRING 0x181 at wparam, with lparam's text; LoadOptionsClass::Fill_List
        // 0x0053C040 passes a FileEntryClass, whose Description is its first member). The
        // port list box keeps narrow text: the row's text is narrowed (Scout1125, M15).
        char narrow[256];
        int n = 0;
        unsigned short const *wide = (unsigned short const *)lparam;
        for (; wide && wide[n] && n < 255; ++n) narrow[n] = wide[n] < 0x100 ? (char)wide[n] : '?';
        narrow[n] = 0;
        bool append = message == 0x4C8 || message == 0x4CD;
        LRESULT row = Original(window, append ? 0x180 : 0x181, append ? 0 : wparam, (LPARAM)narrow);
        InvalidateRect(window, 0, TRUE);
        return row;
    }
    if (record && message == 0x4A8) {
        // A row's extra column cell (0x005F6690 case 0x4A8: the date and time of a saved
        // game): not kept by the simplified list (NOT EXACT: only the first column shows).
        return 0;
    }
    if (!record || message != 0xF) {
        if (message == 0x14) return 1;
        LRESULT result = Original(window, message, wparam, lparam);
        if (record && (message == 0x201 || message == 0x203 || message == 0x186 || message == 0x180 || message == 0x184))
            InvalidateRect(window, 0, TRUE);
        return result;
    }
    if (!record->unknown_bc) {
        Rect area = Area(window);
        Frame(area, unknown_5fda40(unknown_color_a76158));
        int count = (int)Original(window, 0x18B, 0, 0);
        int top = (int)Original(window, 0x18E, 0, 0);
        int selected = (int)Original(window, 0x188, 0, 0);
        int height = (int)Original(window, 0x1A1, 0, 0);
        if (height <= 0) height = 16;
        for (int i = top; i < count; ++i) {
            Rect row(area.X + 2, area.Y + 2 + (i - top) * height, area.Width - 4, height);
            if (row.Y + row.Height > area.Y + area.Height) break;
            if (i == selected) Frame(row, unknown_5fda40(unknown_color_a733e4));
            wchar_t text[256];
            if (Item_Text(window, false, i, text, 256)) Text(record, Rect(row.X + 2, row.Y, row.Width - 4, row.Height), text, 0, 1);
        }
    }
    ValidateRect(window, 0);
    return 0;
}

// 0x005F4BA0: the combo box procedure (Scout1119; drop list Scout1121, M14b).
// Items: the wide and narrow adds (0x4BB/0x4BC narrow, 0x4C1/0x4C2 wide;
// insert/add) put the narrow text in the original list and keep the item
// record the drop list 0x005EAD20 reads ({0, data, text}, its UnknownListItem)
// as the item data; CB_GET/SETITEMDATA reach the record's data, as the
// original's cases 0x150/0x151 do. A click on the arrow (the rightmost 20
// pixels) plays GUIComboOpenSound and toggles the drop (CB_SHOWDROPDOWN), which
// creates the owner-draw "ComboDropWin" window (0x005EAD20) under the box,
// subclassed by 0x005ED180 and holding the capture until a click closes it
// (TibSun ComboBoxCtrlProc's shape; the original keeps the drop window in the
// record at +0xF4, natively a side table).
namespace {
struct ComboItem { int unknown_00; int data; wchar_t *text; ComboItem *next; };   // 0x005EAD20's UnknownListItem, then a native list link
ComboItem *combo_items = 0;
struct { HWND combo; HWND drop; } combo_drops[16];
ComboItem *Combo_Item(HWND window, int index)
{
    ComboItem *item = (ComboItem *)Original(window, 0x150, index, 0);
    for (ComboItem *known = combo_items; known; known = known->next)
        if (known == item) return item;
    return 0;
}
void Combo_Free(HWND window, int index)
{
    ComboItem *item = Combo_Item(window, index);
    if (!item) return;
    for (ComboItem **link = &combo_items; *link; link = &(*link)->next)
        if (*link == item) { *link = item->next; break; }
    delete [] item->text;
    delete item;
}
HWND &Combo_Drop(HWND combo, bool make)
{
    static HWND none;
    for (int i = 0; i < 16; ++i)
        if (combo_drops[i].combo == combo && combo_drops[i].drop) return combo_drops[i].drop;
    if (make)
        for (int i = 0; i < 16; ++i)
            if (!combo_drops[i].drop) { combo_drops[i].combo = combo; return combo_drops[i].drop; }
    none = 0;
    return none;
}
}

LRESULT __stdcall unknown_combo_proc_5f4ba0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    UnknownControlRecord *record = Get_Record(window);
    if (record) {
        switch (message) {
        case 0x4BB: case 0x4BC: case 0x4C1: case 0x4C2: {
            bool wide = message >= 0x4C1;
            bool add = message == 0x4BC || message == 0x4C2;
            int length = 0;
            char narrow[512];
            if (lparam) {
                if (wide) while (((unsigned short const *)lparam)[length]) ++length;
                else while (((char const *)lparam)[length]) ++length;
            }
            if (wide) {
                Unknown_006FA0F0(narrow, (unsigned short const *)lparam, sizeof narrow);
            } else {
                int n = length < (int)sizeof narrow - 1 ? length : (int)sizeof narrow - 1;
                for (int i = 0; i < n; ++i) narrow[i] = ((char const *)lparam)[i];
                narrow[n] = 0;
            }
            LRESULT index = Original(window, add ? 0x143 : 0x14A, add ? 0 : wparam, (LPARAM)narrow);
            if (index < 0) return index;
            ComboItem *item = new ComboItem;
            item->unknown_00 = 0;
            item->data = 0;
            item->text = new wchar_t[length + 1];
            for (int i = 0; i < length; ++i)
                item->text[i] = wide ? ((wchar_t const *)lparam)[i] : (wchar_t)(unsigned char)((char const *)lparam)[i];
            item->text[length] = 0;
            item->next = combo_items;
            combo_items = item;
            Original(window, 0x151, index, (LPARAM)item);
            return index;
        }
        case 0x150: case 0x199: {                   // CB_GETITEMDATA (LB_GETITEMDATA as the original answers it)
            ComboItem *item = Combo_Item(window, (int)wparam);
            if (item) return item->data;
            return Original(window, 0x150, wparam, lparam);
        }
        case 0x151: case 0x19A: {                   // CB_SETITEMDATA
            ComboItem *item = Combo_Item(window, (int)wparam);
            if (item) { item->data = (int)lparam; return 0; }
            return Original(window, 0x151, wparam, lparam);
        }
        case 0x144:                                 // CB_DELETESTRING
            Combo_Free(window, (int)wparam);
            break;
        case 0x14B: {                               // CB_RESETCONTENT
            int count = (int)Original(window, 0x146, 0, 0);
            for (int i = 0; i < count; ++i) Combo_Free(window, i);
            break;
        }
        case 0x201: case 0x203: {                   // WM_LBUTTONDOWN / DBLCLK: the arrow drops the list
            Voc_Play(Rules->GUIComboOpenSound, 1.0f, 0x2000, 0);
            RECT client;
            GetClientRect(window, &client);
            if ((int)(lparam & 0xFFFF) > client.right - 20) {
                LRESULT dropped = SendMessageA(window, 0x157, 0, 0);
                PostMessageA(window, 0x14F, dropped != 1, 0);
            }
            return 0;
        }
        case 0x157:                                 // CB_GETDROPPEDSTATE
            return Combo_Drop(window, false) ? 1 : 0;
        case 0x14F: {                               // CB_SHOWDROPDOWN
            HWND parent = GetParent(window);
            HWND &slot = Combo_Drop(window, false);
            if (!wparam) {
                if (!slot) return 1;
                HWND drop = slot;
                slot = 0;
                ReleaseCapture();
                SendMessageA(parent, 0x4A9, (WPARAM)drop, 0);
                DestroyWindow(drop);
                unknown_restore_child_5edd30(drop, 0);
                Original(window, 0x14F, 0, 0);
                InvalidateRect(parent, 0, FALSE);
                return 1;
            }
            if (slot) return 1;
            SetFocus(window);
            RECT client, display, parent_display;
            GetClientRect(window, &client);
            Window_Rect_In_Main_Client(window, &display);
            Window_Rect_In_Main_Client(parent, &parent_display);
            int count = (int)Original(window, 0x146, 0, 0);
            int item_height = (int)Original(window, 0x154, 0, 0);
            if (item_height < 1) item_height = 1;
            int height = count * item_height + 4;
            if (display.bottom + height > parent_display.bottom - 2 * item_height) {
                height = parent_display.bottom - (2 * item_height + 4) - display.bottom;
                if (height < item_height) {
                    height = parent_display.bottom - display.bottom;
                    height -= height % item_height;
                }
            }
            HWND drop = CreateWindowExA(0, "ComboDropWin", 0, 0x40000000, display.left - parent_display.left,
                client.bottom + display.top - parent_display.top + 2, client.right, height, parent, 0, ProgramInstance, window);
            if (!drop) return 0;
            unknown_5ed180(drop, 0);
            Combo_Drop(window, true) = drop;
            Original(window, 0x14F, 1, 0);
            SendMessageA(drop, 0x7E8, 0, 0);       // the highlight starts on the selection
            SendMessageA(parent, 0x4A9, (WPARAM)drop, 1);
            SetCapture(drop);
            ShowWindow(drop, 1);
            InvalidateRect(drop, 0, FALSE);
            return 1;
        }
        case 0x497:                                 // OD_SUBCLASSED: no item has a colour of its own
        case 0x498: {                               // OD_SETCOLOR: item wparam's text colour
            // +0x110, 50 COLORREFs, -1 for the default (the drop list 0x005EAD20 reads them;
            // RA2_COMBO_DROP_UNIT's item_colors_110). TibSun ownrdraw.cpp's OD_SUBCLASSED memsets
            // them to 0xFF; pass-6a decompile 005f4ba0 case 0x498 stores lparam below index 0x33
            // (natively below 50: the array's own extent). Scout1125 (M15): the drop list's rows
            // drew in colour 0 (black) before.
            int *colors = record->item_colors_110;
            if (message == 0x497) {
                memset(colors, 0xFF, 50 * sizeof(int));
            } else if (wparam < 50) {
                colors[wparam] = (int)lparam;
            }
            break;
        }
        case 0x2:                                   // WM_DESTROY
            SendMessageA(window, 0x14F, 0, 0);
            break;
        }
    }
    if (!record || message != 0xF) {
        if (message == 0x14) return 1;
        LRESULT result = Original(window, message, wparam, lparam);
        if (record && (message == 0x14E || message == 0x14B)) InvalidateRect(window, 0, TRUE);
        return result;
    }
    if (!record->unknown_bc) {
        Rect area = Area(window);
        Frame(area, unknown_5fda40(unknown_color_a76158));
        int selected = (int)Original(window, 0x147, 0, 0);
        wchar_t text[256];
        if (selected >= 0 && Item_Text(window, true, selected, text, 256))
            Text(record, Rect(area.X + 4, area.Y, area.Width - 8, area.Height), text, 0, 1);
    }
    ValidateRect(window, 0);
    return 0;
}

// 0x005F0F40: the tab control procedure.
LRESULT __stdcall unknown_tab_proc_5f0f40(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    UnknownControlRecord *record = Get_Record(window);
    if (!record || message != 0xF) {
        if (message == 0x14) return 1;
        return Original(window, message, wparam, lparam);
    }
    if (!record->unknown_bc) {
        Frame(Area(window), unknown_5fda40(unknown_color_a76158));
    }
    ValidateRect(window, 0);
    return 0;
}

// 0x005F22A0: the "NewEdit" procedure (a list box standing in for an edit,
// 0x005ECB00): the text is the record's WideString (+0x3C), its limit +0x48.
// Keys: Return and Tab notify the dialog (0x4DA, 0x4DB), characters edit the
// text; 0x4B3/0x4B5 read it wide/narrow, 0x4B2/0x4B4 reset it from +0x28.
LRESULT __stdcall unknown_new_edit_proc_5f22a0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    UnknownControlRecord *record = Get_Record(window);
    if (!record || !record->unknown_3c) {
        return Original(window, message, wparam, lparam);
    }
    WideString &text = *record->unknown_3c;
    int &limit = *(int *)record->unknown_48;
    HWND parent = GetParent(window);
    switch (message) {
    case 0x100: case 0x101:                         // WM_KEYDOWN / WM_KEYUP
        if (wparam == 9) {
            if (message == 0x100) SendMessageA(parent, 0x4DB, (GetAsyncKeyState(0x10) & 0x8000) ? 1 : 0, (LPARAM)window);
            return 0;
        }
        if (message == 0x100 && wparam == 0xD) {
            SendMessageA(parent, 0x4DA, 0, (LPARAM)window);
            return 0;
        }
        break;
    case 0x497: {                                   // fit inside the frame
        RECT rect, client, parent_rect;
        GetWindowRect(window, &rect);
        GetClientRect(window, &client);
        GetWindowRect(parent, &parent_rect);
        SetWindowPos(window, 0, rect.left - parent_rect.left + 1, rect.top - parent_rect.top + 1, client.right - 2, client.bottom - 2, 0x40);
        return 0;
    }
    case 0xC5:                                      // EM_LIMITTEXT
        limit = (int)wparam;
        if (wparam) text.Truncate((unsigned)wparam);
        break;
    case 0x4B3: {                                   // the text, wide
        unsigned short *out = (unsigned short *)lparam;
        unsigned short const *from = text.Get();
        int i = 0;
        for (; from && from[i] && i < (int)wparam - 1; ++i) out[i] = from[i];
        out[i] = 0;
        return i;
    }
    case 0x4B5: {                                   // the text, narrow
        char *out = (char *)lparam;
        Unknown_006FA0F0(out, text.Get(), (int)wparam);
        out[wparam - 1] = 0;
        int i = 0;
        while (out[i]) ++i;
        return i;
    }
    case 0xE:                                       // WM_GETTEXTLENGTH
        return text.Length();
    case 0x4B2: case 0x4B4:                         // reset from the record's text
        text.Set((unsigned short const *)L"");
        record->unknown_40 = 0;
        record->unknown_44 = 0;
        if (record->text_28) text.Set((unsigned short const *)record->text_28);
        if (limit) text.Truncate((unsigned)limit);
        record->unknown_40 = text.Length();
        InvalidateRect(window, 0, TRUE);
        break;
    case 0x102:                                     // WM_CHAR
        if (wparam == 8) {
            if (text.Length()) text.Truncate(text.Length() - 1);
        } else if (wparam >= 0x20 && (!limit || (int)text.Length() < limit)) {
            unsigned short add[2] = {(unsigned short)wparam, 0};
            text.Concatenate(add);
        } else {
            return 0;
        }
        record->unknown_40 = text.Length();
        InvalidateRect(window, 0, TRUE);
        SendMessageA(parent, 0x111, (WPARAM)((GetDlgCtrlID(window) & 0xFFFF) | (0x300 << 16)), (LPARAM)window);   // EN_CHANGE
        return 0;
    case 0x14:
        return 1;
    case 0xF: {
        if (!record->unknown_bc) {
            Rect area = Area(window);
            Frame(area, unknown_5fda40(unknown_color_a76158));
            Text(record, Rect(area.X + 3, area.Y, area.Width - 6, area.Height), (wchar_t const *)text.Get(), 0, 1);
        }
        ValidateRect(window, 0);
        return 0;
    }
    case 0x201:
        SetFocus(window);
        return 0;
    }
    return Original(window, message, wparam, lparam);
}

// 0x005EAD00 (decompile: DefWindowProcA): the drop list's frame window procedure.
LRESULT __stdcall unknown_new_edit_proc_5ead00(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    return DefWindowProcA(window, message, wparam, lparam);
}

} // namespace OwnerDraw
