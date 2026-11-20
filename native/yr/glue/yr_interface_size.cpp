// Game interface size for yr-native (lane N19; a native addition, not in the original game): Red
// Alert 2's link/native_enhancements.cpp (lanes C3, C7) written again against the YR tree's classes and
// hooked by native fixups (yr/fixups.json). In play the sidebar column is shown larger than the map:
// the game's sidebar surface is made shorter than the screen (Yr_Sidebar_Height, the host's
// ra2_enh_sidebar_height: the screen's height over the interface size, at least 360 rows) and the host
// (platform/sdl/host.cpp, the split layout, shared with Red Alert 2) shows its rows that many times
// larger, filling the height; the map, the command bar and every in-play dialog keep the screen's size.
//
// Where Yuri's Revenge differs from Red Alert 2: its interface art layout (FUN_0072FC60) places the
// command bar (TabClass) and the in-play dialogs' sidebar column from the screen's height, and the
// sidebar in play (SidebarClass) is laid out from its own rectangle, so the layout keeps the screen's
// height (the command bar stays at the screen's bottom, the dialogs shown whole draw their column to the
// bottom) and only the sidebar's rectangle is made from the surface's rows (Yr_Sidebar_Bottom,
// glue/yr_map_zoom.cpp). No full-height relayout around the in-play dialogs is needed (Red Alert 2's
// Native_Dialog_Screen makes its art layout full height again for them).
//
// The sidebar surface is made at the three surface allocations (WinMain 0x006BB9A0, Change_Display_Mode
// 0x00560BF0, Get_All 0x0067E730). A new game whose screen keeps its size changes no mode, so when the
// interface size changed since the surface was made Start_Scenario 0x00683AB0 (and Toggle_Display_Mode
// in game) changes the mode to the same size, the original's own path that makes the surfaces, the
// view, the sidebar's layout and its buttons again (Yr_Sidebar_Remake). At 1x (the default, the
// Original preset) every answer is the original's.
#include "always.h"
#include "geometry.h"
#include "_surface.h"
#include "surface.h"
#include "dsurface.h"
#include <stdio.h>

// The host's side (platform/sdl/enhancements.cpp). Weak default: a build without the SDL host keeps
// the screen's height.
extern "C" __attribute__((weak)) int ra2_enh_sidebar_height(int screen_height) { return screen_height; }

// The sidebar surface's rows for a screen this tall (native fixups at the sidebar surface allocations).
int Yr_Sidebar_Height(int screen_height)
{
    return ra2_enh_sidebar_height(screen_height);
}

// Whether the sidebar surface's rows differ from what the interface size asks for this screen (native
// fixups at Start_Scenario's and Toggle_Display_Mode's mode checks): the surfaces are then made again.
bool Yr_Sidebar_Remake(int screen_height)
{
    if (SidebarSurface == NULL) return false;
    int now = SidebarSurface->Get_Height(), want = Yr_Sidebar_Height(screen_height);
    if (now == want) return false;
    fprintf(stderr, "enhancements: new game: sidebar surface %d rows, was %d\n", want, now);
    return true;
}
