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

namespace OwnerDraw {

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

// 0x005F4BA0: the combo box procedure (the selected item; the drop list is the owner-draw one).
LRESULT __stdcall unknown_combo_proc_5f4ba0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    UnknownControlRecord *record = Get_Record(window);
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
void __fastcall Unknown_006FA0F0(char *out, unsigned short const *text, int size);   // 0x006FA0F0 (wide to narrow)
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
