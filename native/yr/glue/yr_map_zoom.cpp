// Map zoom and Zoom gestures for yr-native (lane N18; native additions, not in the original game):
// Red Alert 2's link/native_map_zoom.cpp (lanes C4, C16) written again against the YR tree's classes
// and hooked by native fixups (yr/fixups.json). The game's screen keeps its size, so the menus, every
// in-play dialog, the sidebar and the command bar draw and lay out as in the original; only the
// tactical view is made smaller. DisplayClass::Set_View_Dimensions (gamemd.exe 0x004A8960) keeps the
// rectangle Yr_Map_View answers (the tactical rectangle divided by the zoom, rounded up to whole cells,
// at the same top-left corner); the host (platform/sdl/host.cpp, shared with Red Alert 2) shows that
// view over the whole tactical area and maps the mouse back into it.
//
// The original places the sidebar at the view's right edge (TacticalRect.X + Width, or its Width:
// SidebarClass's rectangle and blits, the radar's offsets, the command bar's area, the mouse over the
// sidebar, the tool tips): natively those sites ask Yr_Sidebar_X, which answers the tactical
// rectangle's right edge while the view is zoomed. Tool tips the game places for the command bar on the
// map side are moved into the view (Yr_Tooltip_Zoom).
//
// The zoom a game uses is taken when it starts (Start_Scenario 0x00683AB0, after its display mode
// change) or is loaded (Get_All 0x0067E730); at 1x (the default, the Original preset) the view is the
// tactical rectangle and every function here answers what the original did. With Zoom gestures on the
// wheel, a trackpad pinch or a two-finger pinch changes the zoom in play between 1x and 3x (Red Alert
// 2's render scale, which lets its map zoom out below 1x, is not carried over: see yr/README.md).
#include "always.h"
#include "geometry.h"
#include "_tactica.h"
#include "_surface.h"
#include "surface.h"
#include "dsurface.h"
#include "tactical.h"
#include "display.h"
#include "_map.h"
#include "gscreen.h"
#include <stdio.h>

extern Rect VisibleRect;                          // the screen
extern Rect TacticalRect;                         // the view rectangle Set_View_Dimensions keeps (DSurface::ViewBounds)
Rect __fastcall Get_Title_Screen_Area(void);      // the tactical rectangle for the screen (gamemd.exe 0x0072AD20, txtlabel2.cpp)

// The host's side (platform/sdl/enhancements.cpp). Weak defaults: a build without the SDL host keeps
// the original view.
extern "C" __attribute__((weak)) double ra2_enh_map_zoom(void) { return 1.0; }
extern "C" __attribute__((weak)) void ra2_enh_map_view(int, int, int, int, int, int, int, int) {}
extern "C" __attribute__((weak)) void ra2_enh_map_show(double, double, double, double) {}
extern "C" void ra2_enh_in_play(int on);
void Yr_Hd_Vehicles_Start();   // glue/yr_hd_vehicles.cpp: HD vehicles taken when a game starts or loads (lane N19)

namespace {
double game_zoom = 1.0;      // the zoom of the game running (taken at its start or load)
double view_zoom = 1.0;      // the zoom the view rectangle was last made with
bool zoomed = false;         // the view is smaller than the tactical rectangle
int full_right = 0;          // the tactical rectangle's right edge (the sidebar's x)
int full_bottom = 0;         // the tactical rectangle's bottom edge (the command bar's top)

// Continuous zoom (Red Alert 2's lane C16 model): the part of the map shown (show_w x show_h game
// pixels, the tactical rectangle divided by the zoom) starts at a fraction of a pixel (show_ox,
// show_oy) inside the game's view, and the view is the shown part rounded up to whole cells (60 x 30
// pixels, one pixel to spare), so most steps of a pinch move only what the host shows.
constexpr double kZoomMin = 1.0, kZoomMax = 3.0;
double show_ox = 0, show_oy = 0, show_w = 0, show_h = 0;
double pending_factor = 1.0, pending_ax = 0, pending_ay = 0;
bool pending = false;
int round_up(double v, int step) { int n = (int)v; if (n < v) ++n; return (n + step - 1) / step * step; }
void report_show(Rect const &view) { ra2_enh_map_show(view.X + show_ox, view.Y + show_oy, show_w, show_h); }

// The view's origin (Tactical TacPixelX/Y, the world pixel at its top left) read fresh, and set to a
// given origin through the position (the offset between the two measured, not assumed).
Point2D view_origin(Tactical *t)
{
    t->Set_Tactical_Position(t->Get_Tactical_Position());   // the origin follows the view's size now
    return Point2D(t->TacPixelX, t->TacPixelY);
}
Point2D set_view_origin(Tactical *t, int x, int y)
{
    Point2D now = view_origin(t), centre = t->Get_Tactical_Position();
    t->Set_Tactical_Position(Point2D(x + centre.X - now.X, y + centre.Y - now.Y));
    return Point2D(t->TacPixelX, t->TacPixelY);
}
}  // namespace

// Set_View_Dimensions' rectangle (native fixup): the tactical rectangle divided by the game's zoom,
// rounded up to whole cells with a pixel to spare (never larger than the tactical rectangle), at the
// same corner. Unchanged at 1x.
Rect Yr_Map_View(Rect const &rect)
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
// The zoom asked for, applied at a frame's end (Yr_Map_Zoom_Frame): the zoom is multiplied and kept in
// range; the world point at (ax, ay) (game pixels across the tactical rectangle, from its corner) stays.
void apply_zoom()
{
    pending = false;
    Tactical *tactical = TacticalMap;
    double z0 = game_zoom, z1 = z0 * pending_factor;
    if (z1 < kZoomMin) z1 = kZoomMin;
    if (z1 > kZoomMax) z1 = kZoomMax;
    if (!tactical || z1 == z0) return;
    Rect full = Get_Title_Screen_Area();
    double ax = pending_ax < 0 ? 0 : pending_ax > full.Width ? full.Width : pending_ax;
    double ay = pending_ay < 0 ? 0 : pending_ay > full.Height ? full.Height : pending_ay;
    Point2D origin0 = view_origin(tactical);
    Rect const view0 = TacticalRect;
    double wx = origin0.X + show_ox + ax / z0;
    double wy = origin0.Y + show_oy + ay / z0;
    game_zoom = z1;
    view_zoom = z1;
    Rect view1 = Yr_Map_View(full);
    if (view1.Width != view0.Width || view1.Height != view0.Height) Map.Set_View_Dimensions(full);
    Rect const &view = TacticalRect;
    double ox = wx - ax / z1, oy = wy - ay / z1;
    int tx = (int)ox, ty = (int)oy;
    if (tx > ox) --tx;
    if (ty > oy) --ty;
    Point2D placed = set_view_origin(tactical, tx, ty);
    show_ox = ox - placed.X;
    show_oy = oy - placed.Y;
    if (show_ox < 0) show_ox = 0;
    if (show_oy < 0) show_oy = 0;
    if (show_ox + show_w > view.Width) show_ox = view.Width - show_w;
    if (show_oy + show_h > view.Height) show_oy = view.Height - show_h;
    if (z1 == 1.0) show_ox = show_oy = 0;
    report_show(view);
    fprintf(stderr, "zoom: %.3f -> %.3f at %.0f,%.0f: view %dx%d, shown %.2fx%.2f from %.2f,%.2f, origin %d,%d\n", z0, z1, ax, ay,
            view.Width, view.Height, show_w, show_h, show_ox, show_oy, placed.X, placed.Y);
    Map.Flag_To_Redraw(GS_REDRAW_TACTICAL);
}
}  // namespace

// The host's zoom (platform/sdl/host.cpp: the wheel, a trackpad pinch, a two-finger pinch), taken at
// the frame's end; and the zoom in play now (tests).
extern "C" void ra2_native_zoom_by(double factor, double ax, double ay)
{
    pending_factor = pending ? pending_factor * factor : factor;
    pending_ax = ax;
    pending_ay = ay;
    pending = true;
}
extern "C" double ra2_native_zoom(void) { return game_zoom; }

// The sidebar's x where the original takes the view's right edge or width (native fixups): the
// tactical rectangle's right edge while the view is zoomed, else the value asked about.
int Yr_Sidebar_X(int view_right)
{
    return zoomed ? full_right : view_right;
}
// The sidebar's rectangle's height (SidebarClass's layout, native fixups) runs from the view's bottom
// edge: the tactical rectangle's while zoomed, less the rows the sidebar surface is shorter than the
// screen (Game interface size, lane N19: glue/yr_interface_size.cpp), so its strips and scroll buttons
// lie inside the surface.
int Yr_Sidebar_Bottom(int view_bottom)
{
    int bottom = zoomed ? full_bottom : view_bottom;
    if (SidebarSurface != NULL && SidebarSurface->Get_Height() < VisibleRect.Height)
        bottom -= VisibleRect.Height - SidebarSurface->Get_Height();
    return bottom;
}

// CCToolTip's placement (native fixup, after a tip is placed): a tip placed for the map side (its area
// the view) that does not lie in the view (a command bar button's) is in screen pixels: moved into the
// view at the same place on screen (divided by the zoom), kept inside it.
void Yr_Tooltip_Zoom(bool map_side, int *x, int *y, int width, int height)
{
    if (!zoomed || !map_side) return;
    Rect const &view = TacticalRect;
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

// A game is loaded (Get_All, native fixup before its Set_View_Dimensions call): it takes the zoom set
// now; its loading screen is shown whole until the first frame.
void Yr_Map_Zoom_Load()
{
    ra2_enh_in_play(0);
    Yr_Hd_Vehicles_Start();
    game_zoom = ra2_enh_map_zoom();
}

// A scenario starts (Start_Scenario's top, native fixup): shown whole until the game's first frame.
void Yr_Map_Zoom_Leave()
{
    ra2_enh_in_play(0);
}

// The scenario has started (Start_Scenario after its display mode change, native fixup): the game takes
// the zoom set now, and the view is made for it when it differs from the last (Set_View_Dimensions with
// the tactical rectangle), centred on what the game shows now.
void Yr_Map_Zoom_Start()
{
    Yr_Hd_Vehicles_Start();
    game_zoom = ra2_enh_map_zoom();
    if (game_zoom == view_zoom) return;
    Rect tactical = Get_Title_Screen_Area();
    Tactical *t = TacticalMap;
    double cx = 0, cy = 0;
    if (t) {
        Point2D o = view_origin(t);
        cx = o.X + TacticalRect.Width / 2.0;
        cy = o.Y + TacticalRect.Height / 2.0;
    }
    Map.Set_View_Dimensions(tactical);
    if (t) {
        show_ox = show_oy = 0;
        set_view_origin(t, (int)(cx - show_w / 2), (int)(cy - show_h / 2));
        report_show(TacticalRect);
    }
    Map.Flag_To_Redraw(GS_REDRAW_ALL);
}

// Each game loop frame (yr_app_hooks.cpp yr_native_main_loop): a zoom asked for is applied.
void Yr_Map_Zoom_Frame()
{
    if (pending) apply_zoom();
}

// The game is over (Main_Loop's Do_Win or Do_Lose, native fixups): a zoomed game draws its view into a
// part of the screen that the in-play layout shows enlarged; shown whole, the last frame would be that
// part in a corner. The screen is cleared, so those frames are black until the score screen draws.
void Yr_Map_Zoom_End()
{
    ra2_enh_in_play(0);
    if (view_zoom != 1.0) {
        if (VisibleSurface) VisibleSurface->Fill(0);
        if (HiddenSurface) HiddenSurface->Fill(0);
        if (CompositeSurface) CompositeSurface->Fill(0);
    }
}
