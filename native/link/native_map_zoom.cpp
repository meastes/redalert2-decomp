// Map zoom (native addition, not in the original; C4, enhancements v2 part 2, the Outpost 2
// port's model: the map's scale independent of the interface's). The game's screen keeps
// its size, so the menus, every in-play dialog (Options, Diplomacy, save and load), the
// sidebar and the command bar draw and lay out as in the original; only the tactical view
// is made smaller. Set_View_Dimensions 0x00498C80 (DisplayClass slot 42; native fixup in
// display.cpp) keeps the view rectangle Native_Map_View answers: the tactical rectangle
// divided by the zoom, at the same top-left corner. The tactical view, its depth and alpha
// buffers, its button, tooltip and message list, scrolling and the radar's view box all take
// that rectangle, as they take any screen size. The host (platform/sdl/host.cpp, the zoom
// layout) shows the smaller view over the whole tactical area, the zoom times larger, and
// maps the mouse back into it; with a dialog up it shows the whole picture as before.
//
// The original places the sidebar at the view's right edge (the view's width is the sidebar's
// x: SidebarClass 0x0067BA60, its draw 0x0067D420 and Reposition_Sidebar 0x006819A0, RadarClass
// AI 0x0062FCC0 and Reposition_Sidebar 0x006306D0, RTacticalClass::Action 0x0062FE30, WWMouseClass::Draw_Mouse
// 0x00772410 on the sidebar, CCToolTip, the command bar's TabClass::AI 0x006A0F30):
// natively those sites ask Native_Sidebar_X (native fixups), which answers the tactical
// rectangle's right edge while the view is zoomed, so the sidebar and the command bar stay
// where the screen has them. Tooltips the game places for the command bar or a sidebar button
// on the map side are moved into the view (Native_Tooltip_Zoom).
//
// The zoom a game uses is taken when it starts (the end of Start_Scenario 0x0065BC70: a new
// game, the next campaign mission) or is loaded (Get_All 0x00656B80), from the Enhancements
// page's Map zoom (platform/sdl/enhancements.cpp); at 1x (the default, the ORIGINAL preset)
// the view is the tactical rectangle and every function here answers what the original did.
#include "shared/display.hpp"     // before map_view.hpp (full DisplayClass)
#include "shared/map_view.hpp"
#include "shared/rect.hpp"

Rect __fastcall Get_Title_Screen_Area_006f1f30();     // the tactical rectangle for the screen (0x006F1F30)
extern Rect unknown_rect_839630;        // the view rectangle Set_View_Dimensions keeps

// The host's side (platform/sdl/enhancements.cpp). Weak defaults: a build without the SDL
// host keeps the original view.
extern "C" __attribute__((weak)) double ra2_enh_map_zoom(void) { return 1.0; }
extern "C" __attribute__((weak)) void ra2_enh_map_view(int, int, int, int, int, int, int, int) {}
extern "C" __attribute__((weak)) void ra2_enh_in_play(int) {}

namespace {
double game_zoom = 1.0;      // the zoom of the game running (taken at its start or load)
double view_zoom = 1.0;      // the zoom the view rectangle was last made with
bool zoomed = false;         // the view is smaller than the tactical rectangle
int full_right = 0;          // the tactical rectangle's right edge (the sidebar's x)
int full_bottom = 0;         // the tactical rectangle's bottom edge (the command bar's top)
}

// Set_View_Dimensions' rectangle (native fixup): the tactical rectangle divided by the
// game's zoom (even sizes), at the same corner. Unchanged at 1x.
Rect Native_Map_View(Rect const &rect)
{
    Rect view = rect;
    if (game_zoom > 1.0 && rect.Width > 0 && rect.Height > 0) {
        view.Width = ((int)(rect.Width / game_zoom + 0.5)) & ~1;
        view.Height = ((int)(rect.Height / game_zoom + 0.5)) & ~1;
    }
    view_zoom = game_zoom;
    zoomed = view.Width != rect.Width || view.Height != rect.Height;
    full_right = rect.X + rect.Width;
    full_bottom = rect.Y + rect.Height;
    ra2_enh_map_view(view.X, view.Y, view.Width, view.Height, rect.X, rect.Y, rect.Width, rect.Height);
    return view;
}

// The sidebar's x where the original takes the view's right edge (native fixups): the
// tactical rectangle's right edge while the view is zoomed, else the value asked about.
int Native_Sidebar_X(int view_right)
{
    return zoomed ? full_right : view_right;
}

// The sidebar's layout 0x0067B910 (native fixup) takes the view's bottom edge as the command
// bar's top (the sidebar's height from it): the tactical rectangle's while zoomed.
int Native_Sidebar_Bottom(int view_bottom)
{
    return zoomed ? full_bottom : view_bottom;
}

// CCToolTip::Show 0x0046DFC0 (native fixup, after it placed a tip): a tip placed for the map
// side (its area the view) that does not lie in the view (a command bar button's, or a
// sidebar button's that falls left of the sidebar) is in screen pixels: moved into the view at
// the same place on screen (divided by the zoom), kept inside it.
void Native_Tooltip_Zoom(bool map_side, int *x, int *y, int width, int height)
{
    if (!zoomed || !map_side) return;
    Rect const &view = unknown_rect_839630;
    if (*x >= view.X && *y >= view.Y && *x + width <= view.X + view.Width && *y + height <= view.Y + view.Height) return;
    int nx = view.X + (int)((*x - view.X) / view_zoom);
    int ny = view.Y + (int)((*y - view.Y) / view_zoom);
    if (nx > view.X + view.Width - width) nx = view.X + view.Width - width;
    if (ny > view.Y + view.Height - height) ny = view.Y + view.Height - height;
    if (nx < view.X) nx = view.X;
    if (ny < view.Y) ny = view.Y;
    *x = nx;
    *y = ny;
}

// A game is loaded (Get_All, native fixup before its Set_View_Dimensions call): it takes the
// zoom set now; its loading screen is shown whole until the first frame.
void Native_Map_Zoom_Load()
{
    ra2_enh_in_play(0);
    game_zoom = ra2_enh_map_zoom();
}

// A scenario starts (Start_Scenario, native fixup at its top): the loading screen and the
// movies are shown whole until the game's first frame; a changed interface size gets its
// sidebar surface (Native_Sidebar_New_Game).
void Native_Sidebar_New_Game();     // link/native_enhancements.cpp (Game interface size, C7)
void Native_Map_Zoom_Leave()
{
    ra2_enh_in_play(0);
    Native_Sidebar_New_Game();
}

// The scenario has started (the end of Start_Scenario, after its display mode toggle): the
// game takes the zoom set now, and the view is made for it when it differs from the last
// (Set_View_Dimensions with the tactical rectangle, as Set_Display_Mode 0x005434E0 does).
void Native_Map_Zoom_Start()
{
    game_zoom = ra2_enh_map_zoom();
    if (game_zoom != view_zoom) {
        Rect tactical = Get_Title_Screen_Area_006f1f30();
        Map.unknown_slot42(tactical);
        Map.Flag_To_Redraw(2);
    }
}

// Each Main_Loop frame (harness_skirmish.cpp's Main_Loop seam): the game is in play.
void Native_Map_Zoom_Frame()
{
    ra2_enh_in_play(1);
}
