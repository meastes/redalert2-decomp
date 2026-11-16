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

extern "C" void ra2_enh_in_play(int on);   // platform/sdl/enhancements.cpp
extern "C" void ra2port_frame(int frame);  // port/src/run_progress.cpp

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

bool yr_native_main_loop(void)
{
    static int frames = 0;
    ra2_enh_in_play(1);
    bool over = Main_Loop();
    if (over) ra2_enh_in_play(0);
    ra2port_frame(++frames);
    return over;
}

void yr_native_play_over(void)
{
    ra2_enh_in_play(0);
}
