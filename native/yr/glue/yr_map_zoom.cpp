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
// wheel, a trackpad pinch or a two-finger pinch changes the zoom in play between 0.5x and 3x: the game's
// own display mode is then made twice the window's in its map column and above the command bar (the
// render scale, Red Alert 2's lane C16, carried over by lane N20: Yr_Game_Mode_Size), so the map can be
// shown below 1x at the art's own pixels; the menus' 800x600 shell mode keeps its size.
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
#include "goptions.h"
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
extern "C" __attribute__((weak)) int ra2_enh_zoom_gestures(void) { return 0; }
extern "C" __attribute__((weak)) void ra2_enh_set_render_scale(int) {}
extern "C" __attribute__((weak)) void ra2_enh_set_dialog_screen(int) {}
extern GameOptionsClass Options;
bool Yr_Sidebar_Remake(int screen_height);   // glue/yr_interface_size.cpp (lane N19)
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
// The zoom range the player sees: 0.5x to 3x; the game's own zoom is that times the render scale
// (Yr_Render_Size), never below 1 (the view never larger than the tactical area).
constexpr double kZoomMin = 0.5, kZoomMax = 3.0;
int render_scale = 1;          // the game's screen to the window's (2 in the game's mode with Zoom gestures on)
double zoom_min() { return kZoomMin * render_scale < 1.0 ? 1.0 : kZoomMin * render_scale; }
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
    if (z1 < zoom_min()) z1 = zoom_min();
    if (z1 > kZoomMax * render_scale) z1 = kZoomMax * render_scale;
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
    game_zoom = ra2_enh_map_zoom() * render_scale;
}

// The point the game last asked the view to centre on, before the map's edge moved it (native fixup:
// Tactical::Set_Tactical_Position 0x006D6000; RA2's Native_View_Asked, lane C16). Cleared when a
// scenario starts.
static bool view_asked = false;
static Point2D view_asked_at;
void Yr_View_Asked(int x, int y)
{
    view_asked = true;
    view_asked_at = Point2D(x, y);
}

// A scenario starts (Start_Scenario's top, native fixup): shown whole until the game's first frame.
void Yr_Map_Zoom_Leave()
{
    view_asked = false;
    ra2_enh_in_play(0);
}

// The scenario has started (Start_Scenario after its display mode change, native fixup): the game takes
// the zoom set now, and the view is made for it when it differs from the last (Set_View_Dimensions with
// the tactical rectangle), centred on the point the game framed the start on (its base: with a render
// scale the larger screen's map edge can have moved the view off it, so the point asked is used, else the
// centre of what the game shows now).
void Yr_Map_Zoom_Start()
{
    Yr_Hd_Vehicles_Start();
    game_zoom = ra2_enh_map_zoom() * render_scale;
    if (game_zoom == view_zoom) return;
    Rect tactical = Get_Title_Screen_Area();
    Tactical *t = TacticalMap;
    bool asked = view_asked;
    Point2D asked_at = view_asked_at;
    double cx = 0, cy = 0;
    if (t) {
        Point2D o = view_origin(t);
        cx = asked ? asked_at.X : o.X + TacticalRect.Width / 2.0;
        cy = asked ? asked_at.Y : o.Y + TacticalRect.Height / 2.0;
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

// ---- The render scale (lane N20: Map zoom below 1x; Red Alert 2's lane C16) ------------------------
// With Zoom gestures on, the game's own display mode (Options.ScreenWidth x ScreenHeight, 800x600 or
// more) is made render scale (2) times larger in its map column and in the height above the command
// bar: (w - 168) * 2 + 168 by (h - 32) * 2 + 32. The sidebar's 168 columns and the command bar's 32 rows
// keep their sizes (the host shows them twice as large), so the window shows the same layout and the
// map can be shown down to 0.5x with the art's own pixels. Red Alert 2 scales its one screen size where
// it is read; Yuri's Revenge runs its menus in their own 800x600 shell mode (Options.ShellWidth x
// ShellHeight) and changes to the game's at Start_Scenario 0x00683AB0 and Toggle_Display_Mode 0x006842F0,
// so the size is scaled where the game's mode is asked for: those two mode checks (Yr_Game_Mode_Change),
// the tactical rectangle for the game's size FUN_0072AD90 (SidebarClass's layout, the command bar's
// width) and the interface art layout FUN_0072FC60's callers. Options keeps the player's size (the
// Options dialog and RA2MD.INI see it). Off (the default and the Original preset): the size asked for.
namespace {
bool dialog_screen = false;    // the in-play dialogs are laid out at the window's size (Yr_Dialog_Screen)
int art_w = 0, art_h = 0;      // the size the interface art layout FUN_0072FC60 was last made for
int art_for_w = 0, art_for_h = 0;   // while the art is laid out for the in-play dialogs: the window's size
}

// The size the game's own display mode is laid out for: Options' size, scaled with Zoom gestures on.
// While the interface art is laid out for the in-play dialogs (relayout_art below), the window's size,
// so the command bar's width FUN_0072FC60 takes from FUN_0072AD90 is the window's too.
void Yr_Game_Mode_Size(int *w, int *h)
{
    if (art_for_w > 0) {
        *w = art_for_w;
        *h = art_for_h;
        return;
    }
    *w = Options.ScreenWidth;
    *h = Options.ScreenHeight;
    if (!ra2_enh_zoom_gestures() || *w < 800 || *h < 600) return;
    *w = (*w - 168) * 2 + 168;
    *h = (*h - 32) * 2 + 32;
}

// The mode checks of Start_Scenario 0x00683AB0 and Toggle_Display_Mode 0x006842F0 (in game; native
// fixups): the original changes the mode when the screen differs from Options' size; here from the game
// mode's size, so a scaled screen is not changed again at every check (and with lane N19's sidebar
// surface check). The size to change to in *w, *h.
bool Yr_Game_Mode_Change(int *w, int *h)
{
    Yr_Game_Mode_Size(w, h);
    return VisibleRect.Width != *w || VisibleRect.Height != *h || Yr_Sidebar_Remake(*h);
}

// Change_Display_Mode 0x00560BF0 (native fixup at its top): the render scale of the mode set now: 2 for
// the game's scaled mode, 1 for any other (the shell's 800x600, a size the Options dialog tries). Told
// to the host before the surfaces are made (the sidebar surface's rows, the window's layout). The zoom
// of a game running keeps the player's zoom across a change (a game loaded from the menus takes its zoom
// in the shell's mode, Get_All, and changes to the game's mode after: Toggle_Display_Mode).
void Yr_Render_Size(int width, int height)
{
    int w, h;
    Yr_Game_Mode_Size(&w, &h);
    int scale = (w != Options.ScreenWidth || h != Options.ScreenHeight) && width == w && height == h ? 2 : 1;
    if (scale != render_scale || scale > 1)
        fprintf(stderr, "zoom: render scale %d: the game's screen %dx%d for %dx%d\n", scale, width, height,
                scale > 1 ? Options.ScreenWidth : width, scale > 1 ? Options.ScreenHeight : height);
    game_zoom = game_zoom * scale / render_scale;
    render_scale = scale;
    ra2_enh_set_render_scale(scale);
}

// FUN_0072FC60 (native fixup at its top): the size the interface art (the in-play sidebar column's and
// the command bar's pieces) is laid out for.
void Yr_Art_Laid_Out(int width, int height)
{
    art_w = width;
    art_h = height;
}

// Change_Display_Mode (native fixup after its backdrop layout): the interface art is laid out once, at
// Init_Game, for Options' size; with a render scale it is made again for the game mode's size when that
// differs (the command bar at the larger screen's bottom, the column at its right edge), and back.
extern bool DAT_00B0FC0C;                                  // the interface art layout is made
void __fastcall FUN_00730720(int width, int height);       // frees it and makes it for a size (txtlabel2.cpp)
static void relayout_art(int w, int h, bool for_dialogs = false)
{
    if (!DAT_00B0FC0C || (w == art_w && h == art_h)) return;
    if (for_dialogs) {
        art_for_w = w;
        art_for_h = h;
    }
    FUN_00730720(w, h);
    art_for_w = art_for_h = 0;
}
void Yr_Art_Relayout()
{
    int w, h;
    Yr_Game_Mode_Size(&w, &h);
    relayout_art(w, h);
}

// The in-play dialogs at the window's size (native fixups: Ingame_Menu_Dialog 0x0048C8B0 around its
// dialog loop; RA2's Native_Dialog_Screen): the game lays its full-screen dialogs out for the screen
// size (Drawing::RenderWidth x RenderHeight: the dialog's size, its backdrop and sidebar column art, its
// controls centred or anchored to its edges), and at render scale 2 that is the larger screen, the
// backdrop left in a corner. For the dialogs' time the screen size is the window's (Options' size): each
// dialog is laid out at the top left of the screen as the original lays it out at the window's size, the
// interface art and backdrop layouts are made for it, and the host shows that part at the window's scale
// (ra2_enh_set_dialog_screen). Restored when the loop ends; at render scale 1 nothing changes.
namespace Drawing { extern int RenderWidth; extern int RenderHeight; }
extern int VideoModeWidth, VideoModeHeight;
void __fastcall FUN_0072E1B0(int width, int height);       // the dialogs' backdrop layout for a size (txtlabel2.cpp)
bool FUN_0072E260(void);                                   // that layout is made
void Yr_Dialog_Screen(bool on)
{
    static int saved_w = 0, saved_h = 0, saved_mw = 0, saved_mh = 0;
    if (on && !dialog_screen) {
        if (render_scale == 1) return;
        saved_w = Drawing::RenderWidth;
        saved_h = Drawing::RenderHeight;
        saved_mw = VideoModeWidth;
        saved_mh = VideoModeHeight;
        dialog_screen = true;
        int w = (saved_w - 168) / render_scale + 168, h = (saved_h - 32) / render_scale + 32;
        Drawing::RenderWidth = VideoModeWidth = w;
        Drawing::RenderHeight = VideoModeHeight = h;
        relayout_art(w, h, true);
        if (FUN_0072E260()) FUN_0072E1B0(w, h);
        ra2_enh_set_dialog_screen(1);
        fprintf(stderr, "zoom: in-play dialogs at %dx%d (the screen %dx%d)\n", w, h, saved_w, saved_h);
    } else if (!on && dialog_screen) {
        Drawing::RenderWidth = saved_w;
        Drawing::RenderHeight = saved_h;
        VideoModeWidth = saved_mw;
        VideoModeHeight = saved_mh;
        dialog_screen = false;
        Yr_Art_Relayout();
        if (FUN_0072E260()) FUN_0072E1B0(saved_w, saved_h);
        ra2_enh_set_dialog_screen(0);
        fprintf(stderr, "zoom: in-play dialogs end\n");
    }
}
