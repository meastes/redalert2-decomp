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
#include "shared/unknown_globals.hpp"
#include "shared/tactical.hpp"
#include <stdio.h>

Rect __fastcall Get_Title_Screen_Area_006f1f30();     // the tactical rectangle for the screen (0x006F1F30)
extern Rect unknown_rect_839630;        // the view rectangle Set_View_Dimensions keeps

// The host's side (platform/sdl/enhancements.cpp). Weak defaults: a build without the SDL
// host keeps the original view.
extern "C" __attribute__((weak)) double ra2_enh_map_zoom(void) { return 1.0; }
extern "C" __attribute__((weak)) void ra2_enh_map_view(int, int, int, int, int, int, int, int) {}
extern "C" __attribute__((weak)) void ra2_enh_in_play(int) {}
extern "C" __attribute__((weak)) void ra2_enh_map_show(double, double, double, double) {}

namespace {
double game_zoom = 1.0;      // the zoom of the game running (taken at its start or load)
double view_zoom = 1.0;      // the zoom the view rectangle was last made with
bool zoomed = false;         // the view is smaller than the tactical rectangle
int full_right = 0;          // the tactical rectangle's right edge (the sidebar's x)
int full_bottom = 0;         // the tactical rectangle's bottom edge (the command bar's top)
}

namespace {
// Continuous zoom (lane C16, the Outpost 2 port's model; the Enhancements row "Zoom gestures"): the
// player's wheel, trackpad pinch or two-finger pinch changes the zoom in play. The part of the map
// shown (show_w x show_h game pixels, the tactical rectangle divided by the zoom) starts at a
// fraction of a pixel (show_ox, show_oy) inside the game's view, and the view is the shown part
// rounded up to whole cells (60 x 30 pixels, one pixel to spare for the fraction), so most steps of a
// pinch move only what the host shows and the game's view (its surfaces and buffers) is made again
// only when the rounded size changes. The world point under the pinch's middle or the cursor stays
// where it is on the screen: the view's position (Tactical +0xD64, its centre) is moved by whole
// pixels and the rest is the fraction.
constexpr double kZoomMin = 1.0, kZoomMax = 3.0;
double show_ox = 0, show_oy = 0, show_w = 0, show_h = 0;
double pending_factor = 1.0, pending_ax = 0, pending_ay = 0;
bool pending = false;
int round_up(double v, int step) { int n = (int)v; if (n < v) ++n; return (n + step - 1) / step * step; }
void report_show(Rect const &view)
{
    ra2_enh_map_show(view.X + show_ox, view.Y + show_oy, show_w, show_h);
}
}  // namespace

// Set_View_Dimensions' rectangle (native fixup): the tactical rectangle divided by the game's zoom,
// rounded up to whole cells with a pixel to spare (never larger than the tactical rectangle), at
// the same corner. Unchanged at 1x.
Rect Native_Map_View(Rect const &rect)
{
    Rect view = rect;
    show_w = rect.Width;
    show_h = rect.Height;
    if (game_zoom > 1.0 && rect.Width > 0 && rect.Height > 0) {
        show_w = rect.Width / game_zoom;
        show_h = rect.Height / game_zoom;
        int w = round_up(show_w + 1, 60), h = round_up(show_h + 1, 30);
        view.Width = w < rect.Width ? w : rect.Width;
        view.Height = h < rect.Height ? h : rect.Height;
    }
    if (show_ox + show_w > view.Width) show_ox = view.Width - show_w;
    if (show_oy + show_h > view.Height) show_oy = view.Height - show_h;
    if (show_ox < 0) show_ox = 0;
    if (show_oy < 0) show_oy = 0;
    view_zoom = game_zoom;
    zoomed = view.Width != rect.Width || view.Height != rect.Height;
    full_right = rect.X + rect.Width;
    full_bottom = rect.Y + rect.Height;
    ra2_enh_map_view(view.X, view.Y, view.Width, view.Height, rect.X, rect.Y, rect.Width, rect.Height);
    report_show(view);
    return view;
}

namespace {
// The zoom asked for, applied at a frame's end (Native_Map_Zoom_Frame): the zoom is multiplied and
// kept in range; the world point at (ax, ay) (game pixels across the tactical rectangle, from its
// corner) stays there.
void apply_zoom()
{
    pending = false;
    Tactical *tactical = unknown_tactical_83996c;
    double z0 = game_zoom, z1 = z0 * pending_factor;
    if (z1 < kZoomMin) z1 = kZoomMin;
    if (z1 > kZoomMax) z1 = kZoomMax;
    if (!tactical || z1 == z0) return;
    Rect full = Get_Title_Screen_Area_006f1f30();
    double ax = pending_ax < 0 ? 0 : pending_ax > full.Width ? full.Width : pending_ax;
    double ay = pending_ay < 0 ? 0 : pending_ay > full.Height ? full.Height : pending_ay;
    // The world pixel under the anchor now: the view's origin (its centre less half the view) and
    // the fraction shown past it.
    Point2D centre = tactical->Get_Tactical_Position();
    Rect const &view0 = unknown_rect_839630;
    double wx = centre.X - view0.Width / 2 + show_ox + ax / z0;
    double wy = centre.Y - view0.Height / 2 + show_oy + ay / z0;
    game_zoom = z1;
    view_zoom = z1;
    Rect view1 = Native_Map_View(full);
    if (view1.Width != view0.Width || view1.Height != view0.Height) {
        Map.unknown_slot42(full);    // Set_View_Dimensions: the game's view takes the new size
    }
    Rect const &view = unknown_rect_839630;
    // The new origin, split into whole pixels for the game and the fraction shown.
    double ox = wx - ax / z1, oy = wy - ay / z1;
    int tx = (int)ox, ty = (int)oy;
    if (tx > ox) --tx;
    if (ty > oy) --ty;
    tactical->Set_Tactical_Position_006a6640(Point2D(tx + view.Width / 2, ty + view.Height / 2));
    Point2D placed = tactical->Get_Tactical_Position();
    show_ox = ox - (placed.X - view.Width / 2);
    show_oy = oy - (placed.Y - view.Height / 2);
    if (show_ox < 0) show_ox = 0;
    if (show_oy < 0) show_oy = 0;
    if (show_ox + show_w > view.Width) show_ox = view.Width - show_w;
    if (show_oy + show_h > view.Height) show_oy = view.Height - show_h;
    if (z1 == 1.0) show_ox = show_oy = 0;
    report_show(view);
    fprintf(stderr, "zoom: %.3f -> %.3f at %.0f,%.0f: view %dx%d, shown %.2fx%.2f from %.2f,%.2f, origin %d,%d\n", z0, z1, ax, ay,
            view.Width, view.Height, show_w, show_h, show_ox, show_oy, placed.X - view.Width / 2, placed.Y - view.Height / 2);
    Map.Flag_To_Redraw(1);
}
}  // namespace

// The host's zoom (platform/sdl/host.cpp: the wheel, a trackpad pinch, a two-finger pinch): the zoom
// is multiplied by factor about (ax, ay), game pixels across the tactical rectangle from its corner;
// taken at the frame's end.
extern "C" void ra2_native_zoom_by(double factor, double ax, double ay)
{
    pending_factor = pending ? pending_factor * factor : factor;
    pending_ax = ax;
    pending_ay = ay;
    pending = true;
}
// The zoom in play now (the host shows it; tests).
extern "C" double ra2_native_zoom(void) { return game_zoom; }

// The sidebar's x where the original takes the view's right edge (native fixups): the
// tactical rectangle's right edge while the view is zoomed, else the value asked about.
int Native_Sidebar_X(int view_right)
{
    return zoomed ? full_right : view_right;
}

// The sidebar's layout 0x0067B910 (native fixup) takes the view's bottom edge as the command
// bar's top (the sidebar's height from it): the tactical rectangle's while zoomed. With a Game
// interface size above 1x the sidebar surface is shorter than the screen (Native_Sidebar_Height,
// link/native_enhancements.cpp), so the edge moves up by the rows the surface lacks: the
// cameo rows, the scroll buttons under them (0x0067B910's abcd78) and the background then
// lie inside the surface (C7; at 1.5x the scroll buttons sat at y 734 of 512 rows and their
// draws ran past the surface, the heap damage behind codex4 R703/R705/R706).
int Native_Sidebar_Height(int screen_height);
int Native_Sidebar_Bottom(int view_bottom)
{
    int bottom = zoomed ? full_bottom : view_bottom;
    int screen = unknown_window_839640.Height;
    return bottom - (screen - Native_Sidebar_Height(screen));
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
void Native_Hd_Vehicles_Start();   // link/native_hd_vehicles.cpp (HD vehicles, C16): taken with the zoom
void Native_Map_Zoom_Load()
{
    ra2_enh_in_play(0);
    game_zoom = ra2_enh_map_zoom();
    Native_Hd_Vehicles_Start();
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
    Native_Hd_Vehicles_Start();
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
    if (pending) apply_zoom();
}

// The game is over (Main_Loop returned true: won, lost, restarted or aborted). The score screen,
// the next mission's movies and the menus are shown whole (the host's split and zoom layouts are
// for play only; C11).
void Native_Map_Zoom_End()
{
    ra2_enh_in_play(0);
}
