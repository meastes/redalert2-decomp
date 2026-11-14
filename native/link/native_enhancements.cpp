// The Enhancements page (native addition, not in the original; C3, docs/HANDOFF.md
// "ENHANCEMENTS MENU MODEL"). The shell Options dialog's ENHANCEMENTS button (port/src/
// native_dialogs.cpp adds it to template 0xD5; Main_Options_Proc 0x00542590, a native
// behaviour row, link/behaviour_m22_options.cpp, opens this page) runs the native dialog
// through the game's own dialog code, as the Options dialog runs its pages: one row
// per setting (name, value, CHANGE), the ORIGINAL and ENHANCED presets, a one-line hint,
// ACCEPT and CANCEL. The page is created under the Options dialog's id (0xD5) so the
// game's dialog code gives it the Options look. The settings and what they change are the host's
// (platform/sdl/enhancements.cpp, ra2_enh_*); every one is off by default.
#include "shared/unknown_globals.hpp"
#include "shared/rect.hpp"
#include "shared/surface.hpp"
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
#include "shared/dialog_create.hpp"
#include "shared/dialog_default_proc.hpp"
#include <stdio.h>

void __fastcall unknown_dialog_5ff7c0(HWND window);    // 0x005FF7C0 shows a created dialog
void __fastcall unknown_dialog_5ff6e0(HWND window);    // 0x005FF6E0 closes it
void __fastcall unknown_517370(bool unknown1);         // 0x00517370 the shell's idle step
extern "C" void ra2port_native_page(int on);           // port/src/native_dialogs.cpp

// The host's settings (platform/sdl/enhancements.cpp). Weak defaults: a build without
// the SDL host has no rows.
extern "C" __attribute__((weak)) int ra2_enh_rows(void) { return 0; }
extern "C" __attribute__((weak)) const char *ra2_enh_label(int) { return ""; }
extern "C" __attribute__((weak)) const char *ra2_enh_value(int) { return ""; }
extern "C" __attribute__((weak)) const char *ra2_enh_help(int) { return ""; }
extern "C" __attribute__((weak)) int ra2_enh_automatic(int) { return 0; }
extern "C" __attribute__((weak)) const char *ra2_enh_preset(void) { return ""; }
extern "C" __attribute__((weak)) void ra2_enh_begin(void) {}
extern "C" __attribute__((weak)) void ra2_enh_change(int) {}
extern "C" __attribute__((weak)) void ra2_enh_set_preset(int) {}
extern "C" __attribute__((weak)) void ra2_enh_accept(void) {}

namespace {

constexpr int kRows = 6;            // rows the template has (port/src/native_dialogs.cpp)
constexpr int kLabel = 0x7F20, kValue = 0x7F30, kChange = 0x7F10, kPresetText = 0x7F40;
constexpr int kOriginal = 0x7F00, kEnhanced = 0x7F01, kAccept = 0x7F02, kCancel = 0x7F03;
constexpr int kHint = 0x695, kTitle = 0x694, kHeading = 0x7F7E;   // hint line and title box: the Options ids
constexpr int kRowHelp = 0x7F70;    // under the presets: what the last changed row does

// A control's text, through the owner-draw message the game itself uses to change one
// (0x4B2 with a wide string; Main_Options_Proc sets its slider labels this way), which
// stores it in the control's record and redraws it over the backdrop.
void set_text(HWND dialog, int id, char const *text)
{
    HWND control = GetDlgItem(dialog, id);
    if (!control) return;
    wchar_t wide[256];
    int n = 0;
    for (; text && text[n] && n < 255; ++n) wide[n] = (unsigned char)text[n];
    wide[n] = 0;
    SendMessageA(control, 0x4B2, 0, (LPARAM)wide);
}

int hint_row = -1;      // the row the hint line describes (the last one changed; -1: the page)

void refresh(HWND dialog)
{
    int rows = ra2_enh_rows();
    for (int i = 0; i < kRows; ++i) {
        bool shown = i < rows;
        if (!shown) {
            ShowWindow(GetDlgItem(dialog, kLabel + i), 0);
            ShowWindow(GetDlgItem(dialog, kValue + i), 0);
            ShowWindow(GetDlgItem(dialog, kChange + i), 0);
            continue;
        }
        set_text(dialog, kLabel + i, ra2_enh_label(i));
        set_text(dialog, kValue + i, ra2_enh_value(i));
        EnableWindow(GetDlgItem(dialog, kChange + i), !ra2_enh_automatic(i));
    }
    set_text(dialog, kPresetText, ra2_enh_preset());
    set_text(dialog, kHint, ra2_enh_help(-1));
    set_text(dialog, kRowHelp, hint_row >= 0 ? ra2_enh_help(hint_row) : "");
}

int __stdcall Native_Enhancements_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int handled = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (handled) return handled;
    int *result = (int *)ra2port_get_dialog_user(window);
    switch (message) {
    case 0x497:                                     // the dialog's controls are set up
        set_text(window, kTitle, "ENHANCEMENTS");
        set_text(window, kHeading, "Native additions (not in the original game)");
        set_text(window, kOriginal, "ORIGINAL");
        set_text(window, kEnhanced, "ENHANCED");
        set_text(window, kAccept, "ACCEPT");
        set_text(window, kCancel, "CANCEL");
        for (int i = 0; i < kRows; ++i) set_text(window, kChange + i, "CHANGE");
        refresh(window);
        return 0;
    case 0x111: {                                   // WM_COMMAND
        if ((wparam >> 16) != 0) return 0;          // clicks only
        int id = wparam & 0xFFFF;
        if (id >= kChange && id < kChange + ra2_enh_rows()) {
            hint_row = id - kChange;
            ra2_enh_change(hint_row);
            refresh(window);
        } else if (id == kOriginal || id == kEnhanced) {
            ra2_enh_set_preset(id == kEnhanced);
            hint_row = -1;
            refresh(window);
        } else if (id == kAccept) {
            ra2_enh_accept();
            if (result) *result = 1;
        } else if (id == kCancel) {
            if (result) *result = 0;
        }
        return 0;
    }
    }
    return 0;
}

}  // namespace

// Runs the page over the Options dialog (its button 0x7F80), as the Options dialog's own
// procedure runs the resolution test dialog: created, shown, the shell's idle steps until
// ACCEPT or CANCEL, closed.
void Native_Run_Enhancements()
{
    int result = -1;
    hint_row = -1;
    ra2_enh_begin();
    ra2port_native_page(1);     // the page's template under the Options id (port/src/native_dialogs.cpp)
    HWND dialog = unknown_create_dialog_5ff610(0xD5, Native_Enhancements_Proc, 0);
    ra2port_native_page(0);
    if (!dialog) return;
    ra2port_set_dialog_user(dialog, &result);
    unknown_dialog_5ff7c0(dialog);
    while (result < 0) {
        if (unknown_6000d0() == true) break;
        unknown_517370(false);
    }
    unknown_dialog_5ff6e0(dialog);
    fprintf(stderr, "enhancements: page closed (%s)\n", result == 1 ? "accept" : "cancel");
}

// The Options dialog's ENHANCEMENTS button text (its template label is empty).
void Native_Enhancements_Button_Text(HWND options)
{
    set_text(options, 0x7F80, "ENHANCEMENTS");
}

// The sidebar surface's height for a screen this tall (Game interface size, C3): the
// screen's own height unless the interface size is above 1x (the host then shows the
// sidebar column that much larger; Map zoom, C4, is a separate scale: link/native_map_zoom.cpp). Called by native fixups at the sidebar surface
// allocations (Allocate_Surfaces' callers) and the sidebar layout's callers (0x006F4C90).
extern "C" __attribute__((weak)) int ra2_enh_sidebar_height(int screen_height) { return screen_height; }
int Native_Sidebar_Height(int screen_height)
{
    return ra2_enh_sidebar_height(screen_height);
}
// startup.cpp's call site sits in C linkage (its unit's extern "C" block): the same function
// under the plain C name.
extern "C" int Native_Sidebar_Height_C(int screen_height) __asm__("_Native_Sidebar_Height");
int Native_Sidebar_Height_C(int screen_height)
{
    return ra2_enh_sidebar_height(screen_height);
}

// A new game in the same process (Start_Scenario 0x0065BC70, its top: Native_Map_Zoom_Leave,
// link/native_map_zoom.cpp; C7, codex4 R700). The sidebar surface is made at start-up, at a
// resolution change and when a game is loaded (Get_All 0x00656B80); a new game reuses the
// one it finds. When the interface size changed since that surface was made, the surfaces
// are made again here as Get_All makes them (the sidebar Native_Sidebar_Height rows tall,
// tile and composite the tactical width and the screen's height), before the scenario is
// read: its Read_Scenario_INI makes a new Tactical for the new surfaces, as Get_All's load
// does, and the sidebar's layout asks the height again when it is drawn. With an unchanged
// size (always at 1x, the ORIGINAL preset) nothing is done.
Rect __fastcall Get_Title_Screen_Area_006f1f30();     // the tactical rectangle for the screen (0x006F1F30)
bool __fastcall Allocate_Surfaces(Rect *hidden, Rect *composite, Rect *tile, Rect *sidebar, bool hidden_first);   // 0x00519500
void Native_Sidebar_New_Game()
{
    if (unknown_surface_839948 == 0) return;
    int height = Native_Sidebar_Height(unknown_window_839640.Height);
    if (unknown_surface_839948->Get_Height() == height) return;
    fprintf(stderr, "enhancements: new game: sidebar surface %d rows, was %d\n", height, unknown_surface_839948->Get_Height());
    Rect temp = Get_Title_Screen_Area_006f1f30();
    Rect sidebar(0, 0, unknown_int_7add80, height);
    Rect tile(0, 0, temp.Width, unknown_window_839640.Height);
    Rect composite(0, 0, temp.Width, unknown_window_839640.Height);
    Allocate_Surfaces(&unknown_window_839640, &composite, &tile, &sidebar, false);
    unknown_surface_83995c = unknown_surface_839954;    // the drawing surface, as Set_Display_Mode 0x005434E0 sets it
}
