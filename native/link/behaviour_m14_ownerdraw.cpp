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
// Since M20 the push button and since M24 the static procedure (Scout1191) follow
// their decompiles; the list, combo, tab and NewEdit painting stay simplified.
#define RA2_OWNERDRAW_CONTROL_DICTIONARY
#define RA2_OWNERDRAW_WNDPROC_DICTIONARY
#define RA2_OWNERDRAW_CHECK_FIELDS
#define RA2_OWNERDRAW_BUTTON_FIELDS
#define RA2_OWNERDRAW_EDIT_FIELDS
#define RA2_COMBO_DROP_UNIT
#include "shared/unknown_globals.hpp"
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
#include "shared/surface.hpp"
#include "shared/text_print.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/wide_string.hpp"
extern "C" char *getenv(char const *name);   // RA2_NATIVE_PERF_LOG traces
extern "C" int dprintf(int fd, char const *format, ...);
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/owner_draw_sidebar.hpp"
#include "shared/palette.hpp"
#include "shared/pcx_cache.hpp"
#include <stdio.h>
#include "shared/movie.hpp"
#include "shared/movie_list.hpp"
#include "shared/shape_file.hpp"
#include "shared/wol_ladder_list_cell.hpp"
#include "shared/hicolor.hpp"
#include "shared/rect.hpp"

extern WolDictionary<HWND, WNDPROC> unknown_window_proc_table_a73688;   // 0x00A73688 (its initializer 0x005DE9C0)
extern void *unknown_art_ac10c0;                                        // 0x00AC10C0 (kind-3 button shape; 0x006F1B60 loads it)

void __fastcall Unknown_006FA0F0(char *out, unsigned short const *text, int size);   // 0x006FA0F0 (wide to narrow)

void *Unknown_Get_Art_006f34c0();
void *Unknown_Get_Art_006f45f0();
void *Unknown_Get_Art_6f2250();

namespace OwnerDraw {

BOOL __stdcall unknown_5ed180(HWND window, LPARAM lparam);                 // 0x005ED180 (subclass a control)
BOOL __stdcall unknown_restore_child_5edd30(HWND window, LPARAM lparam);   // 0x005EDD30
void __fastcall unknown_draw_dimmed_back_5fe490(Rect const &area, HWND window, int kind);   // 0x005FE490
void __fastcall unknown_draw_edit_text_600830(Surface *surface, Rect &rect, wchar_t const *text, int caret,
                                              BitFontClass *font, unsigned int color, int *scroll, int ime,
                                              int password, int frame, int unknown9);   // 0x00600830
extern int unknown_caret_hidden_a763ec;                                    // 0x00A763EC
void __fastcall unknown_draw_arrow_5fd960(Surface *surface, Rect const &rect, int upward, int pressed, bool prefixed);   // 0x005FD960

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

// The push button's paint (0x005F0320 WM_PAINT, pass-6a decompile; Scout1161, M20:
// the user saw the main menu's buttons as text on the empty slots). The button's
// kind (+0xB0; 0x005EA230 sets 2 for the menu dialogs' sidebar buttons) picks its
// art: kinds 1-3 draw a frame of an interface shape through its drawer (kind 1:
// 0x00AC10B8 through 0x006F34C0, frames 2 / 3 lit / 4 pressed; kind 2: 0x00AC0FF8
// through 0x006F45F0, frames 0 / 2 lit / 1 pressed; kind 3: 0x00AC10C0 through
// 0x006F2250, frames as kind 2) at the control's corner, clipped to the background
// surface; kind 0 draws its image (+0x14, pressed +0x18) or the three-piece PCX
// button "b<u|d>e_li<h>.pcx", "_mi" tiled, "_ri" (h 24 or 30, the largest not
// taller than the control; 7-pixel ends; 2 pixels lower while pressed). The text
// (+0x28) follows unless an image stands in for it: font +0x64, centred (5, 0xC),
// in the dialog text colour, the disabled colour for a disabled button, moved 4
// right and 2 down while pressed. NOT EXACT: the original's pressed/up click
// sound memory (0x007E8D4C), its saved background (+0x10) for kind 0, the
// side-coloured disabled text and the disabled darkening are left out.
void Paint_Button(HWND window, UnknownControlRecord *record, bool lit)
{
    Rect area = Area(window);
    bool pressed = (record->unknown_e8 & 1) != 0 || (Original(window, 0xF2, 0, 0) & 4) != 0;   // ODS_SELECTED / BST_PUSHED
    bool disabled = (GetWindowLongA(window, GWL_STYLE) & 0x8000000) != 0;
    int kind = record->unknown_b0;
    ConvertClass *drawer = 0;
    ShapeFileStruct const *shape = 0;
    int frame = 0;
    if (kind == 1) {
        drawer = (ConvertClass *)Unknown_Get_Art_006f34c0();
        shape = unknown_shape_ac10b8;
        frame = pressed ? 4 : lit ? 3 : 2;
    } else if (kind == 2 || kind == 3) {
        drawer = (ConvertClass *)(kind == 2 ? Unknown_Get_Art_006f45f0() : Unknown_Get_Art_6f2250());
        shape = kind == 2 ? unknown_shape_ac0ff8 : (ShapeFileStruct const *)unknown_art_ac10c0;   // 0x00AC10C0 (0x006F1B60 loads it)
        frame = pressed ? 1 : lit ? 2 : 0;
    }
    if (drawer && shape) {
        Rect window_rect = unknown_surface_839958->Get_Rect();
        CC_Draw_Shape(unknown_surface_839958, drawer, shape, frame, Point2D(area.X, area.Y), window_rect, 0x400, 0, 0, 0, 1000, 0, 0,
                      Point2D(0, 0));
    } else if (kind == 0 && record->image_14) {
        Image(pressed && record->alternate_image_18 ? record->alternate_image_18 : record->image_14, area);
    } else if (kind == 0) {
        int height = area.Height >= 30 ? 30 : 24;
        int y = area.Y + (area.Height - height) / 2 + (pressed ? 2 : 0);
        char state = pressed && !disabled ? 'd' : 'u';
        char name[32];
        Surface *left = 0, *middle = 0, *right = 0;
        sprintf(name, "b%c%c_li%d.pcx", state, 'e', height);
        left = unknown_bitmap_writer_a76378.Get_Image_0068ca40(name, 0);
        sprintf(name, "b%c%c_mi%d.pcx", state, 'e', height);
        middle = unknown_bitmap_writer_a76378.Get_Image_0068ca40(name, 0);
        sprintf(name, "b%c%c_ri%d.pcx", state, 'e', height);
        right = unknown_bitmap_writer_a76378.Get_Image_0068ca40(name, 0);
        if (left && middle && right) {
            int end = 7;
            Rect from(0, 0, end, height);
            unknown_surface_839958->Copy_From(Rect(area.X, y, end, height), *left, from, false, true);
            unknown_bitmap_writer_a76378.Draw_0068cce0(Rect(area.X + end, y, area.Width - 2 * end, height), unknown_surface_839958, middle, 0, 0);
            unknown_surface_839958->Copy_From(Rect(area.X + area.Width - end, y, end, height), *right, from, false, true);
        } else {
            int color = unknown_5fda40(lit ? unknown_color_a733e4 : unknown_color_a76158);
            Frame(area, color);
            if (pressed) Frame(Rect(area.X + 1, area.Y + 1, area.Width - 2, area.Height - 2), color);
        }
    }
    if (!record->image_14 && record->text_28) {
        RECT text = {area.X + 1, area.Y, area.X + area.Width, area.Y + area.Height};
        if (pressed) { text.left += 4; text.top += 2; text.bottom += 2; }
        unknown_draw_text_5fe020(unknown_surface_839958, record->text_28, &text, record->font_64,
                                 disabled ? unknown_color_a737ec : unknown_color_a733e4, 5, 0xC, 0, 0, 0);
    }
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
        Paint_Button(window, record, flash[1] != 0);
        ValidateRect(window, 0);
        return 0;
    }
    }
    return Original(window, message, wparam, lparam);
}

// 0x005F2B50: the static (label/picture) procedure (Scout1191, M24: follows the
// pass-6a decompile and the original's code case by case, replacing Scout1119's
// simplified image-or-text paint). The record's kind (+0x70): 0 text, 1 typed
// text (characters shown +0x80, step +0x88, typing sound +0x90), 2 a picture
// (+0x14, centred, magenta transparent), 3 a shape frame, 4 an animated shape
// (+0x78 through drawer +0x74, frame +0x98 of +0x94; 0x4D3 starts, 0x4D4 stops,
// 0x4D5/0x4D6 set/get the frame, 0x4D7 the window told of each frame by 0x4D8).
// Kinds 2-4 wait +0xA0 ms from +0x9C. The first paint saves the background under
// the control in a BSurface (+0x10), which kinds 2-4 and 0x4B2/0x4B4 restore;
// 0x4B1 sets a fill colour (+0xB4/+0xB8) painted first; 0x498 the text colour
// (+0xEC; the disabled colour for a disabled control). Movies (0x4E4 by name,
// 0x4DF by movie list index; 0x4E3 loops, 0x4E0/0x4E1 pause/resume, 0x4E2 stops,
// 0x4F0 blits): a MovieHandle on the background surface at the control's corner,
// the control sized to it, advanced by timer 0x65 every 34 ms (the main menu's
// Ra2ts_l loop, 0x005115F0). NATIVE: the movie handle (+0x58) and the notify
// window (+0xA4) are pointer-sized, kept in a side table by window; +0xAC, +0xB4
// and +0xB8 are read from their VC6 offsets inside the record's char runs.
namespace {
struct StaticExtra {
    MovieHandle *movie;     // +0x58
    HWND notify;            // +0xA4
};
// A small table by window (the dialogs hold a few dozen statics at most).
struct StaticExtraSlot {
    HWND window;
    StaticExtra extra;
};
StaticExtraSlot static_extras[256];
StaticExtra &Static_Extra(HWND window)
{
    StaticExtraSlot *free_slot = 0;
    for (StaticExtraSlot &slot : static_extras) {
        if (slot.window == window) return slot.extra;
        if (!slot.window && !free_slot) free_slot = &slot;
    }
    if (!free_slot) free_slot = &static_extras[0];      // full: reuse the first slot
    free_slot->window = window;
    free_slot->extra.movie = 0;
    free_slot->extra.notify = 0;
    return free_slot->extra;
}
void Static_Extra_Release(HWND window)
{
    for (StaticExtraSlot &slot : static_extras) {
        if (slot.window == window) slot.window = 0;
    }
}
int &Static_Text_Arg(UnknownControlRecord *record)          // +0xAC
{
    return *(int *)((char *)record->unknown_a9 + (0xAC - 0xA9));
}
bool &Static_Fill(UnknownControlRecord *record)             // +0xB4
{
    return *(bool *)((char *)record->unknown_b4 + 0);
}
unsigned long &Static_Fill_Color(UnknownControlRecord *record)   // +0xB8
{
    return *(unsigned long *)((char *)record->unknown_b4 + 4);
}
void Static_Stop_Movie(HWND window, StaticExtra &extra)
{
    if (extra.movie) {
        delete extra.movie;
        extra.movie = 0;
    }
    KillTimer(window, 0x65);
}
void Static_Start_Movie(HWND window, StaticExtra &extra, char const *name)
{
    Static_Stop_Movie(window, extra);
    RECT area = {0, 0, 0, 0};
    Window_Rect_In_Main_Client(window, &area);
    extra.movie = Create_Movie_Handle(name, unknown_surface_839958, 0);
    if (getenv("RA2_NATIVE_PERF_LOG")) dprintf(2, "perf: t%u static movie %s started (%s)\n", (unsigned)GetTickCount(), name ? name : "", extra.movie ? "open" : "failed");
    if (extra.movie) {
        extra.movie->Set_Position(area.left, area.top);
        MoveWindow(window, area.left, area.top, extra.movie->Width, extra.movie->Height, FALSE);
        SetTimer(window, 0x65, 0x22, 0);
        return;
    }
    KillTimer(window, 0x65);
}
}

LRESULT __stdcall unknown_static_proc_5f2b50(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    UnknownControlRecord *record = Get_Record(window);
    if (!record) {
        return 0;
    }
    StaticExtra &extra = Static_Extra(window);
    switch (message) {
    case 0x2:                                       // WM_DESTROY
        if (record->surface_10) {
            delete record->surface_10;
            record->surface_10 = 0;
        }
        if (record->unknown_7c) {
            free((void *)record->unknown_78);
            record->unknown_78 = 0;
        }
        if ((record->unknown_70 == 1 && record->unknown_a8) || record->unknown_70 == 4) {
            KillTimer(window, 0);
        }
        Static_Stop_Movie(window, extra);
        {
            LRESULT result = Original(window, message, wparam, lparam);
            Static_Extra_Release(window);
            return result;
        }
    case 0x3: case 0x5: case 0x47:                  // WM_MOVE, WM_SIZE, WM_WINDOWPOSCHANGED: save the background again
        if (record->surface_10) {
            delete record->surface_10;
        }
        record->surface_10 = 0;
        InvalidateRect(window, 0, FALSE);
        return 0;
    case 0xF: {                                     // WM_PAINT
        if (record->unknown_bc || extra.movie) {
            ValidateRect(window, 0);
            return 0;
        }
        RECT client = {0, 0, 0, 0};
        if (!record->surface_10) {
            RECT where = {0, 0, 0, 0};
            Window_Rect_In_Main_Client(window, &where);
            GetClientRect(window, &client);
            int width = client.right + 1, height = client.bottom + 1;
            record->surface_10 = new BSurface(width, height, 2);
            ++unknown_surface_count_a763d8;
            record->surface_10->Copy_From(Rect(0, 0, width, height), *unknown_surface_839958,
                                          Rect(where.left, where.top, width, height), false, true);
        }
        RECT area = {0, 0, 0, 0};
        Window_Rect_In_Main_Client(window, &area);
        if (Static_Fill(record)) {
            unsigned long color = Static_Fill_Color(record);
            unknown_surface_839958->Fill_Rect(Rect(area.left, area.top, area.right - area.left, area.bottom - area.top),
                                              unknown_5fda40(color & 0xFFFFFF));
        }
        int kind = record->unknown_70;
        if (kind == 0 || kind == 1) {
            if (record->text_28 && (kind == 0 || record->unknown_a8)) {
                LONG style = GetWindowLongA(window, GWL_STYLE);
                int flags = (style & 1) ? 0x11 : (style & 2) ? 0x12 : 0x10;     // SS_CENTER / SS_RIGHT
                unsigned long color = (style & 0x8000000) ? (unsigned long)unknown_color_a737ec : (unsigned long)record->unknown_ec;
                unknown_draw_text_5fe020(unknown_surface_839958, record->text_28, &area, record->font_64, color, flags,
                                         Static_Text_Arg(record), 0, record->unknown_80, record->unknown_8c);
                if (record->unknown_90 != -1) {
                    Voc_Play(record->unknown_90, 1.0f, 0x2000, 0);
                }
                if (kind == 1) {
                    int length = 0;
                    while (record->text_28[length]) ++length;
                    length += 1 + record->unknown_8c;
                    if (record->unknown_80 < length) {
                        record->unknown_80 += record->unknown_88;
                        if (record->unknown_80 >= length) {
                            KillTimer(window, 0);
                        }
                    }
                }
            }
        } else if ((kind == 2 || kind == 3 || kind == 4) && (int)(GetTickCount() - record->unknown_9c) > record->unknown_a0) {
            Rect to(area.left, area.top, area.right - area.left, area.bottom - area.top);
            GetClientRect(window, &client);
            Rect from(client.left, client.top, client.right - client.left, client.bottom - client.top);
            if (record->surface_10) {
                unknown_surface_839958->Copy_From(to, *record->surface_10, from, false, true);
            }
            if (kind == 2) {
                Surface *image = record->image_14;
                if (image) {
                    int width = image->Get_Width(), height = image->Get_Height();
                    Rect at = to;
                    if (width < to.Width) { at.X = to.X + (to.Width - width) / 2; at.Width = width; }
                    if (height < to.Height) { at.Y = to.Y + (to.Height - height) / 2; at.Height = height; }
                    unknown_bitmap_writer_a76378.Draw_0068ce80(at, unknown_surface_839958, image, unknown_5fda40(0xFF00FF));
                }
            } else if (record->unknown_78) {
                ShapeFileStruct const *shape = (ShapeFileStruct const *)record->unknown_78;
                int frame = record->unknown_98;
                Point2D at(to.X, to.Y);
                if (shape->Height < to.Height) at.Y = to.Y + (to.Height - shape->Height) / 2;
                if (shape->Width < to.Width) at.X = to.X + (to.Width - shape->Width) / 2;
                Rect window_rect = unknown_surface_839958->Get_Rect();
                CC_Draw_Shape(unknown_surface_839958, (ConvertClass *)record->unknown_74, shape, frame, at, window_rect, 0x400, 0, 0, 0,
                              1000, 0, 0, Point2D(0, 0));
                if (kind == 4 && record->unknown_a8) {
                    int next = frame + 1;
                    if (next >= record->unknown_94) next = 0;
                    record->unknown_98 = next;
                    if (extra.notify) {
                        SendMessageA(extra.notify, 0x4D8, next, (LPARAM)window);
                    }
                }
            }
        }
        ValidateRect(window, 0);
        return 0;
    }
    case 0x113:                                     // WM_TIMER
        if (wparam == 0x65) {
            if (!extra.movie) return 0;
            if (extra.movie->AdvanceFrame()) {
                static int traced = 0;
                if (traced++ < 3 && getenv("RA2_NATIVE_PERF_LOG")) dprintf(2, "perf: t%u static movie frame\n", (unsigned)GetTickCount());
                InvalidateRect(window, 0, FALSE);
            }
            if (!extra.movie->Is_Finished()) return 0;
            if (record->unknown_5c) {
                extra.movie->SeekToFrame(1);      // loop: back to the first frame
                return 0;
            }
            Static_Stop_Movie(window, extra);
            return 0;
        } else {
            int kind = record->unknown_70;
            if (kind == 1) {
                InvalidateRect(window, 0, TRUE);
                return 0;
            }
            bool reveal = kind == 2 || kind == 3;
            if (!reveal && kind != 4) return 0;
            if ((int)(GetTickCount() - record->unknown_9c) <= record->unknown_a0) return 0;
            if (reveal || record->unknown_a8) {
                InvalidateRect(window, 0, TRUE);
                if (reveal) {
                    KillTimer(window, 0);
                    return 0;
                }
            }
            if (record->unknown_94 != 0) return 0;
            KillTimer(window, 0);
            if (!record->unknown_78) return 0;
            int frames = ((ShapeFileStruct const *)record->unknown_78)->FrameCount;
            record->unknown_94 = frames;
            SetTimer(window, 0, frames, 0);
            return 0;
        }
    case 0x497: {                                   // owner-draw set-up
        record->unknown_70 = 0;
        Static_Text_Arg(record) = 0xC;
        record->unknown_ec = unknown_color_a733e4;
        return 0;
    }
    case 0x498:                                     // the text colour (-1: the dialog text colour)
        if ((int)lparam != record->unknown_ec) InvalidateRect(window, 0, FALSE);
        record->unknown_ec = (int)lparam == -1 ? unknown_color_a733e4 : (int)lparam;
        return 0;
    case 0x4B1:                                     // a fill colour
        Static_Fill(record) = true;
        Static_Fill_Color(record) = (unsigned long)lparam;
        return 0;
    case 0x4B2: case 0x4B4:                         // put the saved background back
        if (record->surface_10) {
            RECT where = {0, 0, 0, 0}, client = {0, 0, 0, 0};
            Window_Rect_In_Main_Client(window, &where);
            GetClientRect(window, &client);
            int width = client.right + 1, height = client.bottom + 1;
            unknown_surface_839958->Copy_From(Rect(where.left, where.top, width, height), *record->surface_10,
                                              Rect(0, 0, width, height), false, true);
            InvalidateRect(window, 0, FALSE);
        }
        return 1;
    case 0x4D3: {                                   // start the animation
        if (!record->unknown_78 || record->unknown_70 != 4 || record->unknown_a8) return 0;
        record->unknown_a8 = true;
        HWND parent = GetParent(window);
        UnknownControlRecord *dialog = parent ? Get_Record(parent) : 0;
        int dialog_kind = dialog ? dialog->unknown_6c : 0;
        int id = GetDlgCtrlID(window);
        static int const listed[] = {0x94, 0xD8, 0xF5, 0xE2, 0xD5, 0x101, 0x129, 0xD7, 0xBB, 0x100, 0xD6, 0x125, 0x122, 0x112, 0xE7, 0x116,
                                     0x11D, 0x11C, 0xFE, 0x10F, 0x117, 0x114, 0xE6, 0xF3, 0xF4, 700, 0x10E, 0xBC6, 0xBC7};
        bool in_list = false;
        for (int k : listed) if (k == dialog_kind) in_list = true;
        bool slow = in_list && id == 0x71C;
        if (!slow) {
            if (dialog_kind == 0x94) slow = id == 0x6EA || id == 0x6EC || id == 0x6EB;
            else if (dialog_kind == 0x103 || dialog_kind == 0xBC7) slow = id == 0x72B;
        }
        SetTimer(window, 0, slow ? 100 : 0, 0);
        return 0;
    }
    case 0x4D4:                                     // stop it
        if (record->unknown_70 == 4 && record->unknown_a8) {
            KillTimer(window, 0);
            record->unknown_a8 = false;
        }
        return 0;
    case 0x4D5:
        if (record->unknown_70 == 4) {
            record->unknown_98 = (int)lparam;
            InvalidateRect(window, 0, TRUE);
            return 0;
        }
        return 0;
    case 0x4D6:
        return record->unknown_70 == 4 ? record->unknown_98 : -1;
    case 0x4D7:
        if (record->unknown_70 == 4) extra.notify = (HWND)lparam;
        return 0;
    case 0x4DF:                                     // a movie from the movie list
        if ((int)wparam >= 0 && (int)wparam < unknown_names_a71050.Count()) {
            Static_Start_Movie(window, extra, unknown_names_a71050[(int)wparam]);
        }
        return 0;
    case 0x4E0:
        if (extra.movie) extra.movie->Pause(true);
        return 0;
    case 0x4E1:
        if (extra.movie) extra.movie->Pause(false);
        return 0;
    case 0x4E2:
        Static_Stop_Movie(window, extra);
        return 0;
    case 0x4E3:
        record->unknown_5c = (int)wparam;
        return 0;
    case 0x4E4:                                     // a movie by name
        Static_Start_Movie(window, extra, (char const *)lparam);
        return 0;
    case 0x4EE:                                     // start typing
        if (record->unknown_70 == 1 && !record->unknown_a8) {
            record->unknown_a8 = true;
            record->unknown_80 = 1;
            SetTimer(window, 0, record->unknown_84, 0);
            InvalidateRect(window, 0, FALSE);
        }
        return 0;
    case 0x4F0:
        if (extra.movie) extra.movie->Blit();
        return 0;
    }
    return Original(window, message, wparam, lparam);   // everything else (WM_ERASEBKGND too) to the Win32 procedure
}

// 0x005F6690's columns (M25, Scout1202; NOT EXACT: a native side table, the original
// keeps them in the record at +0xF8 as {count, ?, 0x14-byte entries}). 0x4A6 adds a
// column (wparam its width, 0 to the right edge; lparam its x, which is also its id:
// the load/save dialogs send (249, 2), (56, 255), (0, 315), the phone book (144, 2),
// (121, 150), (0, 275)); 0x4A8 sets a row's cell (wparam: the column id low, the row
// high; lparam a WolLadderListCell, Flags then Text) and answers -1 for an unknown
// column (the load dialog's game-mode "*" cell at 200 is dropped that way, as in the
// original's search). The row's own text draws in the first column.
namespace {
struct ListCell { int row; int column; wchar_t text[64]; };
struct ListColumns {
    HWND window;
    int count;
    int x[8], width[8];
    int cells_used;
    ListCell cells[96];
};
ListColumns list_columns[8];
ListColumns *List_Columns(HWND window, bool make)
{
    for (int i = 0; i < 8; ++i)
        if (list_columns[i].window == window) return &list_columns[i];
    if (make)
        for (int i = 0; i < 8; ++i)
            if (!list_columns[i].window) {
                memset(&list_columns[i], 0, sizeof list_columns[i]);
                list_columns[i].window = window;
                return &list_columns[i];
            }
    return 0;
}
// Rows at or after row move by delta (insert +1, delete -1; a deleted row's cells go).
void List_Shift_Rows(ListColumns *columns, int row, int delta)
{
    int kept = 0;
    for (int i = 0; i < columns->cells_used; ++i) {
        ListCell cell = columns->cells[i];
        if (delta < 0 && cell.row == row) continue;
        if (cell.row >= row) cell.row += delta;
        columns->cells[kept++] = cell;
    }
    columns->cells_used = kept;
}
}

// 0x005F6690: the list box procedure.
LRESULT __stdcall unknown_list_proc_5f6690(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    UnknownControlRecord *record = Get_Record(window);
    if (record && message == 0x4A6) {
        ListColumns *columns = List_Columns(window, true);
        if (!columns || columns->count >= 8) return -1;
        int at = columns->count++;
        while (at > 0 && columns->x[at - 1] > (int)lparam) {
            columns->x[at] = columns->x[at - 1];
            columns->width[at] = columns->width[at - 1];
            --at;
        }
        columns->x[at] = (int)lparam;
        columns->width[at] = (int)wparam;
        return at;
    }
    if (record && message == 0x2) {
        if (ListColumns *columns = List_Columns(window, false)) columns->window = 0;
    }
    if (record && (message == 0x184 || message == 0x182)) {
        if (ListColumns *columns = List_Columns(window, false)) {
            if (message == 0x184) columns->cells_used = 0;
            else List_Shift_Rows(columns, (int)wparam, -1);
        }
    }
    if (record && (message == 0x4C8 || message == 0x4CD || message == 0x4C9 || message == 0x4CC || message == 0x4CE)) {
        // 0x4CC: a wide row inserted at wparam (-1 appends): the pass-6a case 0x4CC shares
        // 0x4CD's arm and sends LB_INSERTSTRING 0x181 with wparam; the LAN lobby's player list
        // (Net2_Display_Users 0x005BBDD0) fills its rows this way (Scout1209 r9).
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
        if (!append && row >= 0)
            if (ListColumns *columns = List_Columns(window, false)) List_Shift_Rows(columns, (int)row, 1);
        InvalidateRect(window, 0, TRUE);
        return row;
    }
    if (record && message == 0x4A8) {
        // A row's cell (the date and time of a saved game).
        ListColumns *columns = List_Columns(window, false);
        int id = (int)(wparam & 0xFFFF), row = (int)((wparam >> 16) & 0xFFFF);
        if (!columns || !lparam) return -1;
        int column = -1;
        for (int i = 0; i < columns->count; ++i)
            if (columns->x[i] == id) column = i;
        if (column < 0 || row >= (int)Original(window, 0x18B, 0, 0)) return -1;
        WolLadderListCell const *from = (WolLadderListCell const *)lparam;
        ListCell *cell = 0;
        for (int i = 0; i < columns->cells_used; ++i)
            if (columns->cells[i].row == row && columns->cells[i].column == column) cell = &columns->cells[i];
        if (!cell) {
            if (columns->cells_used >= 96) return -1;
            cell = &columns->cells[columns->cells_used++];
            cell->row = row;
            cell->column = column;
        }
        int n = 0;
        unsigned short const *text = from->Text.Get();
        for (; text && text[n] && n < 63; ++n) cell->text[n] = text[n];
        cell->text[n] = 0;
        InvalidateRect(window, 0, TRUE);
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
        // The saved background under the box first, as the other controls paint (0x005FE490
        // with +0xC8): without it every repaint drew the rows over the old ones, and the LAN
        // lobby's player and game lists became an overprinted smear (user report, Scout1223).
        unknown_draw_dimmed_back_5fe490(Rect(area.X, area.Y, area.Width + 1, area.Height + 1), window, record->unknown_c8);
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
            ListColumns *columns = List_Columns(window, false);
            if (!columns || columns->count == 0) {
                if (Item_Text(window, false, i, text, 256)) Text(record, Rect(row.X + 2, row.Y, row.Width - 4, row.Height), text, 0, 1);
                continue;
            }
            for (int c = 0; c < columns->count; ++c) {
                int left = area.X + columns->x[c];
                // A cell reaches the next column (the gap between columns is the cell's own
                // margin); the last column reaches the box's right edge.
                int right = c + 1 < columns->count ? area.X + columns->x[c + 1] : area.X + area.Width - 2;
                if (right > area.X + area.Width - 2) right = area.X + area.Width - 2;
                if (right <= left) continue;
                Rect cell_area(left + 1, row.Y, right - left - 2, row.Height);
                if (c == 0) {
                    if (Item_Text(window, false, i, text, 256)) Text(record, cell_area, text, 0, 1);
                    continue;
                }
                for (int k = 0; k < columns->cells_used; ++k)
                    if (columns->cells[k].row == i && columns->cells[k].column == c) Text(record, cell_area, columns->cells[k].text, 0, 1);
            }
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
        case 0x4B8: case 0x4B9: case 0x4BA: case 0x4BE: case 0x4BF: case 0x4C0: {
            // Find an item by its text after index wparam (-1: from the start): 0x4B9/0x4BF the
            // whole text, the others its start, case-insensitive; 0x4BA/0x4C0 also select it
            // (CB_SETCURSEL). 0x4B8..0x4BA take narrow text, 0x4BE..0x4C0 wide (pass-6a
            // 0x005F4BA0 cases 0x4BE-0x4C0). The LAN slot combos select their player this way:
            // every slot showed blank (user report, Scout1209 r17).
            bool wide = message >= 0x4BE;
            bool exact = message == 0x4B9 || message == 0x4BF;
            bool select = message == 0x4BA || message == 0x4C0;
            unsigned short want[256];
            int wlen = 0;
            if (lparam) {
                if (wide) for (; ((unsigned short const *)lparam)[wlen] && wlen < 255; ++wlen) want[wlen] = ((unsigned short const *)lparam)[wlen];
                else for (; ((char const *)lparam)[wlen] && wlen < 255; ++wlen) want[wlen] = (unsigned char)((char const *)lparam)[wlen];
            }
            want[wlen] = 0;
            int count = (int)Original(window, 0x146, 0, 0);
            if (count <= 0) return count < 0 ? 0 : -1;
            for (int i = (int)wparam + 1 < 0 ? 0 : (int)wparam + 1; i < count; ++i) {
                ComboItem *item = Combo_Item(window, i);
                if (!item || !item->text) continue;
                bool match = true;
                int k = 0;
                for (; k < wlen; ++k) {
                    unsigned a = item->text[k], b = want[k];
                    if (a >= 'a' && a <= 'z') a -= 32;
                    if (b >= 'a' && b <= 'z') b -= 32;
                    if (a != b) { match = false; break; }
                }
                if (match && exact && item->text[k] != 0) match = false;
                if (!match) continue;
                if (select) { Original(window, 0x14E, i, 0); InvalidateRect(window, 0, TRUE); }
                return i;
            }
            return -1;
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
            // The drop list's frame (0x005EAD20 draws it one pixel outside its
            // own rectangle into 0x839958 and copies only the rectangle) reaches
            // the screen through the dialog's repaint: 0x005EE460 paints a
            // dialog, then its ComboDropWin children last, and copies the union.
            // The dialog is repainted as the closing path below does (Scout1153, M19;
            // behaviour, not the original's trigger, which is not identified).
            InvalidateRect(parent, 0, FALSE);
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
        // M26 (Scout1208): the record's flag messages (pass-6a decompile 005f4ba0 cases
        // 0x4DD, 0x4DE, 0x4F1; 0x4DA/0x4DB go to the parent). 0x4DD with 1 makes the box a
        // colour box (+0xCC): the skirmish/LAN colour choice sends it, and the paint below
        // and the drop list fill the rows with their colours (+0x110) instead of text.
        case 0x4DD:
            record->item_fills_cc = lparam == 1;
            break;
        case 0x4DE:                                 // +0xD0 (int; meaning unknown)
            memcpy(record->unknown_ce + (0xD0 - 0xCE), &lparam, sizeof(int));
            break;
        case 0x4F1:
            record->alternate_cd = lparam == 1;
            break;
        case 0x4DA: case 0x4DB:
            return SendMessageA(GetParent(window), message, wparam, lparam);
        }
    }
    if (!record || message != 0xF) {
        if (message == 0x14) return 1;
        LRESULT result = Original(window, message, wparam, lparam);
        if (record && (message == 0x14E || message == 0x14B)) InvalidateRect(window, 0, TRUE);
        return result;
    }
    if (!record->unknown_bc) {
        // M25 (Scout1202): the calls of the original's paint (asm 0x005F4D68-0x005F5581),
        // geometry and colour choice approximate. NOT EXACT. The saved background under
        // the box (0x005FE490 with +0xC8, then 0x005FEB60), the one-pixel frame (default colour; the
        // original packs 0x00A73910 / 0x00A76154 for some states), the drop arrow (0x005FD960 at
        // the right 20 pixels, pointing up and pressed while the list is down, the
        // alternate art for +0xCD), the disabled darkening (0x005FEB60 again), then for
        // a drop list (CBS_DROPDOWNLIST) the selected item's wide text (its record, as
        // the original reads it through 0x199/0x150), in the item's colour (+0x110) when
        // it has one and +0xCC is set, shortened with "..." to fit, drawn by 0x005FE020
        // (flags 4, 0xC: vertically centred) 2 pixels in from the left.
        Rect area = Area(window);
        Surface *surface = unknown_surface_839958;
        LONG style = GetWindowLongA(window, GWL_STYLE);
        bool disabled = (style & 0x8000000) != 0;
        bool dropped = Combo_Drop(window, false) != 0;
        unknown_draw_dimmed_back_5fe490(area, window, record->unknown_c8);
        Unknown_005feb60(&area, surface, 0, record->unknown_c8);
        unknown_5fdb20(surface, area, 1, -1);
        Rect arrow(area.X + area.Width - 20, area.Y + 1, 19, area.Height - 2);
        unknown_draw_arrow_5fd960(surface, arrow, dropped, dropped, record->alternate_cd);
        if (disabled) Unknown_005feb60(&area, surface, 0, record->unknown_c8);
        if ((style & 3) == 3) {
            int selected = (int)Original(window, 0x147, 0, 0);
            wchar_t text[256];
            text[0] = 0;
            ComboItem *item = selected >= 0 ? Combo_Item(window, selected) : 0;
            if (item && item->text) {
                int n = 0;
                for (; item->text[n] && n < 255; ++n) text[n] = item->text[n];
                text[n] = 0;
            } else if (selected >= 0) {
                Item_Text(window, true, selected, text, 256);
            }
            unsigned long color = disabled ? unknown_color_a7363c : unknown_color_a733e4;
            if (record->item_fills_cc && selected >= 0 && selected < 50 && record->item_colors_110[selected] >= 0) {
                // A colour box (+0xCC, the skirmish/LAN colour choice): the item's colour
                // fills the box shrunk by 2 (0x006F1B00) under the text (asm 0x005F53AD-0x005F548F).
                unsigned long fill = record->item_colors_110[selected];
                Rect swatch(area.X, area.Y, area.Width - 20, area.Height);
                Unknown_Rect_Shrink_006F1B00(&swatch, 2);
                surface->Fill_Rect(swatch, Build_Hicolor_Pixel(fill & 0xFF, (fill >> 8) & 0xFF, (fill >> 16) & 0xFF));
                color = fill;
            }
            RECT box = {area.X + 2, area.Y, area.X + area.Width - 20, area.Y + area.Height};
            int room = box.right - box.left;
            int length = 0;
            while (text[length]) ++length;
            if (record->font_64) {
                while (length > 0 && record->font_64->String_Pixel_Width(text, 0) > room) {
                    --length;
                    text[length] = 0;
                    int cut = length >= 3 ? length - 3 : 0;
                    text[cut] = 0;
                    text[cut] = '.'; text[cut + 1] = '.'; text[cut + 2] = '.'; text[cut + 3] = 0;
                    length = cut + 3;
                    if (cut == 0) break;
                }
            }
            if (text[0]) unknown_draw_text_5fe020(surface, text, &box, record->font_64, color, 4, 0xC, 0, 0, 0);
        }
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
    int &caret_hidden = *(int *)(record->unknown_48 + 4);   // +0x4C
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
    case 0x7:                                       // WM_SETFOCUS: the caret blinks once a second
        caret_hidden = 0;
        SetTimer(window, 0, 1000, 0);
        InvalidateRect(window, 0, FALSE);
        break;
    case 0x8:                                       // WM_KILLFOCUS
        KillTimer(window, 0);
        InvalidateRect(window, 0, FALSE);
        break;
    case 0x113:                                     // WM_TIMER
        caret_hidden ^= 1;
        InvalidateRect(window, 0, FALSE);
        return 0;
    case 0x14:
    case 0xF: {
        // The original's paint (asm 0x005F261A-0x005F279E, M25 Scout1202; the same calls as
        // the recovered edit procedure 0x005F1900): the saved background under the
        // control (0x005FE490 with +0xC8), a one-pixel frame unless the parent is a combo
        // box, then the text 2 pixels in from the left through 0x00600830 with the caret
        // at +0x40, the scroll +0x44, the caret flag +0x38, ES_PASSWORD from +0x54 and
        // the blink state +0x4C in 0x00A763EC. NOT EXACT: the WM_PAINT update rectangle
        // the original offsets but does not use is skipped.
        RECT display;
        char class_name[32] = "";
        Window_Rect_In_Main_Client(window, &display);
        GetClassNameA(parent, class_name, sizeof class_name);
        bool combo = strcmp(class_name, "ComboBox") == 0;
        Rect background(display.left, display.top, display.right - display.left + 1, display.bottom - display.top + 1);
        unknown_draw_dimmed_back_5fe490(background, window, record->unknown_c8);
        if (!combo) unknown_5fdb20(unknown_surface_839958, background, 1, -1);
        Rect area(display.left + 2, display.top, display.right - (display.left + 2) + 1, display.bottom - display.top + 1);
        unknown_caret_hidden_a763ec = caret_hidden;
        unknown_draw_edit_text_600830(unknown_surface_839958, area, (wchar_t const *)text.Get(), record->unknown_40,
                                      record->font_64, unknown_color_a733e4, &record->unknown_44, record->unknown_38,
                                      ((unsigned int)record->unknown_54 >> 5) & 1, 0, 0);
        ValidateRect(window, 0);
        return message == 0x14 ? 1 : 0;
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
