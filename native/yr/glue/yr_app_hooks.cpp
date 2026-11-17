// The host's app hooks for yr-native (lane Y7; platform/sdl/host.cpp declares weak defaults, the
// RA2 game's strong ones are native/link/app_hooks.cpp): the touch layer's map pan and the
// settings' save, written against the YR tree's own classes.
//   ra2_native_scroll_map: a swipe over the tactical view (touch.cpp; trackpad panning on the Mac)
//     moves the view by the fingers' pixels: Tactical::Get_Tactical_Position (gamemd.exe
//     0x006D6170) and Set_Tactical_Position (0x006D6000, which clamps to the map and flags the
//     redraw), as the RA2 hook does with RA2's (0x006A67B0, 0x006A6640). Without it a swipe over
//     the map turned the wheel.
//   ra2_native_save_settings: RA2MD.INI saved as the main menu's Exit Game does
//     (GameOptionsClass::Save_Settings).
//   yr_native_main_loop / yr_native_play_over (fixups.json, conquer.cpp's game loop): the host's
//     per-frame work and in-play mark, as the RA2 game's Main_Loop seam gives them
//     (link/harness_skirmish.cpp): ra2_enh_in_play (the touch toolbar shows in play only) and
//     ra2port_frame (the host's frame: the touch layer's held buttons and pans, the input
//     script's frame-numbered lines, the frame-rate line). Main_Loop's result is returned as is.
#include "always.h"
#include "geometry.h"
#include "tactical.h"
#include "_tactica.h"
#include "goptions.h"
#include "globals.h"
#include "mainloop.h"
#include "saveload.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" void ra2_enh_in_play(int on);   // platform/sdl/enhancements.cpp
extern "C" void ra2port_frame(int frame);  // port/src/run_progress.cpp
extern "C" void ra2port_progress(const char *milestone, const char *detail);   // port/src/run_progress.cpp
extern "C" int ra2port_skirmish_frames(void);                                   // the same: --frames=N

extern "C" int ra2_native_scroll_map(int, int, int dx, int dy)
{
    Tactical *tactical = TacticalMap;
    if (!tactical) return 0;
    Point2D view = tactical->Get_Tactical_Position();
    view.X += dx;
    view.Y += dy;
    tactical->Set_Tactical_Position(view);
    return 1;
}

extern "C" void ra2_native_save_settings(void)
{
    Options.Save_Settings();
}

// Test harness (lane Y9; RA2's link/harness_save.cpp, test scaffolding, never game behaviour):
// RA2_NATIVE_SAVE_AT=<frame>[,<file>] saves the game at that loop frame as the options menu's
// Save ends in (Save_Game), under <file> (default SAVE0001.SAV) in the game's write folder.
static void yr_native_harness_save(int frame)
{
    static int at = -2;
    static char file[64] = "SAVE0001.SAV";
    if (at == -2) {
        at = -1;
        if (char const *env = getenv("RA2_NATIVE_SAVE_AT")) {
            at = atoi(env);
            if (char const *comma = strchr(env, ',')) snprintf(file, sizeof file, "%s", comma + 1);
        }
    }
    if (at < 0 || frame != at) return;
    bool saved = Save_Game(file, L"Native harness save", false);
    char detail[128];
    snprintf(detail, sizeof detail, "frame %d: %s %s", frame, saved ? "saved" : "save failed", file);
    fprintf(stderr, "harness-save: %s\n", detail);
    ra2port_progress("harness-save", detail);
}

bool yr_native_main_loop(void)
{
    static int frames = 0;
    ra2_enh_in_play(1);
    bool over = Main_Loop();
    if (over) ra2_enh_in_play(0);
    ra2port_frame(++frames);
    yr_native_harness_save(frames);
    // --run-to=frames --frames=N (lane Y10, as the RA2 game's Main_Loop seam): N frames of a game
    // started from the menus, or loaded (--load-save: the count starts with the loaded game).
    if (frames == ra2port_skirmish_frames()) {
        char detail[16];
        snprintf(detail, sizeof detail, "%d", frames);
        ra2port_progress("frames", detail);
    }
    return over;
}

void yr_native_play_over(void)
{
    ra2_enh_in_play(0);
}

// port/src/user32.cpp's question after a focus change (lane Y11, as native/link/app_hooks.cpp answers
// it for Red Alert 2): is the game's screen redraw mark still set? Yuri's Revenge's mark is the same
// SurfacesRestored (msengine.cpp sets it on WM_ACTIVATEAPP, init.cpp's menu redraw clears it).
// Without this hook the one-game app's Yuri's Revenge engine had a link-closure trap here and
// aborted whenever the app lost focus in the menus (user report 2026-11-17, the Duo's log).
extern bool SurfacesRestored;
extern "C" int ra2port_game_redraw_pending(void)
{
    return SurfacesRestored ? 1 : 0;
}

// The touch app hides the game's own cursor (lane Y11, user report 2026-11-17: a cursor showed on
// taps in Yuri's Revenge; Red Alert 2's hook is native/link/native_touch_cursor.cpp, Scout1249): one
// extra Hide_Mouse whenever the cursor is shown keeps its nesting count below zero, so the game's
// balanced hide/show pairs leave it hidden. Nothing else changes. The host calls it as it pumps events.
#include "xmouse.h"
extern Mouse *MouseCursor;      // gamemd.exe 0x00887640, WinMain's WWMouseClass
extern "C" int ra2_native_hide_game_cursor(void)
{
    if (!MouseCursor) return 0;
    if (!MouseCursor->Is_Hidden()) MouseCursor->Hide_Mouse();
    return 1;
}
