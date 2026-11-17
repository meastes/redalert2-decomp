// Widescreen on the Mac (native addition, not in the original; user report 2026-11-16: "the iOS app
// lays the game out at the window's own aspect, the Mac app doesn't"). The iOS app picks the game's
// resolution from the screen at start (platform/sdl/app_mode.hpp game_resolution); the Mac app's
// window can take any size and go fullscreen, so its game resolution follows the window: the lines
// the player chose (Options' Set Game Resolution, 600 by default), the width that many lines times
// the window's aspect (host.cpp ra2_host_widescreen_target; a window squarer than 4:3 keeps 800 wide
// and grows taller, as on iOS). A 4:3 window keeps the chosen mode, so the original's frames stay
// as they were.
//
// The mode changes in the shell only, where the Options dialog changes it: Title_Screen_Restore
// 0x00517370 (the shell's idle step in every menu's loop) asks here first (a native fixup,
// port/fixups.json). A change in play waits for the menus (the game shows the picture letterboxed
// meanwhile). The change itself is the Options dialog's: Set_Display_Mode 0x005434E0 (the surfaces,
// the interface art, the open dialogs laid out again), and Options keeps the mode so the next start
// opens at it.
#include "shared/main_opt.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/options.hpp"
#include <stdio.h>

// The host (platform/sdl/host.cpp). Weak default: no widescreen (a build without the SDL host).
void Native_Static_Movies_Stop();       // link/behaviour_m14_ownerdraw.cpp: the statics' movies
void Native_Static_Movies_Restart();
extern "C" __attribute__((weak)) int ra2_host_widescreen_target(int, int, int *, int *) { return 0; }

void Native_Widescreen_Check()
{
    int w = 0, h = 0;
    if (!ra2_host_widescreen_target(VideoModeWidth, VideoModeHeight, &w, &h)) return;
    fprintf(stderr, "widescreen: %dx%d -> %dx%d for the window\n", VideoModeWidth, VideoModeHeight, w, h);
    Native_Static_Movies_Stop();       // they draw on the surfaces the change makes again
    bool changed = Set_Display_Mode(w, h);
    Native_Static_Movies_Restart();
    if (!changed) {
        fprintf(stderr, "widescreen: Set_Display_Mode %dx%d failed\n", w, h);
        return;
    }
    LogicalSurface = HiddenSurface;     // as Test_Display_Mode leaves it
    Options.ScreenWidth = w;
    Options.ScreenHeight = h;
    if ((w != 640 && w != 800 && w != 1024) || h > 768) Options.AllowHiResModes = true;   // Options lists the mode
}
