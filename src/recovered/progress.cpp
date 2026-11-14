// progress.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/progress.json carries the addresses and the rows that
// stay in their own files.


#define RA2_SHAPE_FRAME_RESULT
// Header-static dynamic initializers of the unit at 0x0061EAB0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0061EAB0-0x0061EC91); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x0061EAD0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0061eab0.cpp; no source of its own.


// Dynamic initializer (0x0061EB00, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0061eab0.cpp; no source of its own.


// Dynamic initializer (0x0061EB20, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0061eab0.cpp; no source of its own.


// Dynamic initializer (0x0061EB40, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0061eab0.cpp; no source of its own.


// Dynamic initializer (0x0061EB60, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0061eab0.cpp; no source of its own.


// Dynamic initializer (0x0061EB80, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0061eab0.cpp; no source of its own.


// Dynamic initializer (0x0061EBA0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0061eab0.cpp; no source of its own.


// Dynamic initializer (0x0061EBE0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0061eab0.cpp; no source of its own.


// Dynamic initializer (0x0061EC10, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0061eab0.cpp; no source of its own.


// Dynamic initializer (0x0061EC40, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0061eab0.cpp; no source of its own.


// Dynamic initializer (0x0061EC70, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0061eab0.cpp; no source of its own.


// Dynamic initializer (0x0061EC80, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0061eab0.cpp; no source of its own.


#include "shared/progress_screen.hpp"
ProgressScreenClass ProgressScreen;


// ProgressScreen destructor callback (0x0061ECC0).
// Compiler-emitted atexit callback for the unit global ProgressScreen.
// Owner-approved takeover from population-135; no independent source body.
// @symbol _$E41


// ProgressScreenClass::ProgressScreenClass() (0x0061ED00)

ProgressScreenClass::ProgressScreenClass() :
    Total(0),
    Name(0),
    Shape(0),
    ShapeAllocated(false),
    Image(0),
    unknown_60(false),
    Count(0),
    unknown_62(false),
    Window(0),
    Position(0, 0),
    ShowFlags(false),
    NameWidth(-1),
    unknown_7c(0),
    Side(-1)
{
    double *progress = Progress;
    for (int index = 0; index < 8; index++) {
        progress[index] = 0;
    }
}


// ProgressScreenClass::~ProgressScreenClass() (0x0061ED50): frees the shape.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif

ProgressScreenClass::~ProgressScreenClass()
{
    Free_Shape();
}


// void ProgressScreenClass::Init(double total, char count, HWND window) (0x0061ED80)
// Starts a progress display: the total, the player count with every
// player's progress cleared; without a window the primary surface is
// cleared instead.
#include "shared/unknown_globals.hpp"
#include "shared/surface.hpp"
#include "shared/dsurface.hpp"

void ProgressScreenClass::Init(double total, char count, HWND window)
{
    Total = total;
    Count = count;
    for (int index = 0; index < Count; index++) {
        Progress[index] = 0;
    }
    unknown_60 = true;
    if (window != 0) {
        Window = window;
    } else {
        unknown_surface_839954->Fill(0);
    }
    Name = 0;
    unknown_7c = -1;
}


// void ProgressScreenClass::End() (0x0061EDF0)

void ProgressScreenClass::End()
{
    unknown_60 = false;
}


// void ProgressScreenClass::Free_Shape() (0x0061EE00): deletes the shape when
// it was allocated.

void ProgressScreenClass::Free_Shape()
{
    if (ShapeAllocated) {
        delete (void *)Shape;
        ShapeAllocated = false;
    }
    Shape = 0;
}


// void ProgressScreenClass::Set_Side(int side) (0x0061EE30)

void ProgressScreenClass::Set_Side(int side)
{
    Side = side;
}


// int ProgressScreenClass::Get_Side() (0x0061EE40)

int ProgressScreenClass::Get_Side()
{
    return Side;
}


// ColorScheme *ProgressScreenClass::Side_Color_Scheme() (0x0061EE50)
// The loading screen's colours: Allied for side 0, Soviet otherwise.

ColorScheme *ProgressScreenClass::Side_Color_Scheme()
{
    if (Side == 0) {
        return Fetch_Scheme_By_Name("AlliedLoad", 1);
    }
    return Fetch_Scheme_By_Name("SovietLoad", 1);
}


// ColorScheme *ProgressScreenClass::Text_Color_Scheme() (0x0061EE80)
// LightGrey when the local player only observes a multiplayer game (team -1),
// else the side's colours.
#include "shared/unknown_player_entry.hpp"

ColorScheme *ProgressScreenClass::Text_Color_Scheme()
{
    if ((unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) && unknown_players_a3fa7c[0]->Team == -1) {
        return Fetch_Scheme_By_Name("LightGrey", 1);
    }
    if (Side == 0) {
        return Fetch_Scheme_By_Name("AlliedLoad", 1);
    }
    return Fetch_Scheme_By_Name("SovietLoad", 1);
}


// ColorScheme *ProgressScreenClass::Player_Color_Scheme(int player) (0x0061EED0)
// A player's colours (an observer's are those of colour -2); null for an
// index past the eight players.
#include "shared/session_state.hpp"
#include "shared/color_scheme.hpp"

ColorScheme *ProgressScreenClass::Player_Color_Scheme(int player)
{
    ColorScheme *scheme = 0;
    if (unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) {
        int team = unknown_players_a3fa7c[player]->Team;
        if (team == -1) {
            int index = unknown_session_a3d298.Unknown_006715D0(-2);
            scheme = ColorSchemes[index];
            return scheme;
        }
    }
    if (player >= 0 && player < 8) {
        int color = unknown_players_a3fa7c[player]->Get_Color_0066e2a0();
        int index = unknown_session_a3d298.Unknown_006715D0(color);
        scheme = ColorSchemes[index];
    }
    return scheme;
}


// void ProgressScreenClass::Load_Shape(char const *filename, void const *image, ConvertClass *drawer) (0x0061EF40)
// Replaces the background shape; draws the image when there is one.

void ProgressScreenClass::Load_Shape(char const *filename, void const *image, ConvertClass *drawer)
{
    if (filename != 0) {
        if (Shape != 0) {
            Free_Shape();
        }
        Shape = (ShapeFileStruct const *)Unknown_Load_Shape_00495270(filename, ShapeAllocated);
        if (Shape != 0) {
            if (image != 0) {
                Image = image;
                Unknown_00515A80(image, false);
            }
            Drawer = drawer;
        }
    }
}


// ProgressScreenClass::Set_Layout: text, row position and display flags.
Rect __fastcall Get_Title_Screen_Area_006f1f30();
void ProgressScreenClass::Set_Layout(Point2D xy, wchar_t const *text, bool flag1, bool flag2)
{
    if (xy.X == -1 && xy.Y == -1) {
        int width = 0, height = 0;
        Box_Size(&width, &height, Count, flag1);
        Rect rect = Get_Title_Screen_Area_006f1f30();
        Position = Point2D((rect.Width - width) / 2, (rect.Height - height) / 2);
    } else {
        Position = xy;
    }
    Name = text;
    ShowFlags = flag1;
    unknown_71 = flag2;
}


// void ProgressScreenClass::Set_Name_Width(int width) (0x0061F150)

void ProgressScreenClass::Set_Name_Width(int width)
{
    NameWidth = width;
}


// void ProgressScreenClass::Shape_Size(int *width, int *height) (0x0061F160)
// The size of the shape's first frame plus a 6-pixel border.

void ProgressScreenClass::Shape_Size(int *width, int *height)
{
    Rect rect = Shape->Get_Frame_Rect(0);
    *width = rect.Width + 6;
    *height = rect.Height + 6;
}


// void ProgressScreenClass::Flag_Size(int *width, int *height) (0x0061F1D0)
// The size of the first country's flag image (zero without one).

void ProgressScreenClass::Flag_Size(int *width, int *height)
{
    Surface *flag = Country_Flag(0);
    if (flag == 0) {
        *width = 0;
        *height = 0;
        return;
    }
    *width = flag->Get_Width();
    *height = flag->Get_Height();
}


// void __fastcall ProgressScreenClass::Text_Size(int *width, int *height) (0x0061F210)
// The size of a name of fifteen "W"s in font style 0x46.
#include "shared/text_print.hpp"
#include "shared/ipx_manager.hpp"

void __fastcall ProgressScreenClass::Text_Size(int *width, int *height)
{
    BitFontClass *font = Font_For_Style(0x46);
    wchar_t text[16];
    for (int index = 0; index < 15; index++) {
        text[index] = *Narrow_To_Wide_006fa180("W");
    }
    text[15] = 0;
    *width = font->String_Pixel_Width(text, 0);
    *height = font->Height;
}


// void ProgressScreenClass::Box_Size(int *width, int *height, int rows, bool flag) (0x0061F280)
// The size of the progress box: shape, flag and name side by side plus
// margins, one row per player.

void ProgressScreenClass::Box_Size(int *width, int *height, int rows, bool flag)
{
    int shape_width = 0, shape_height = 0;
    Shape_Size(&shape_width, &shape_height);
    int flag_width = 0, flag_height = 0;
    if (flag) {
        Flag_Size(&flag_width, &flag_height);
    }
    int text_width = 0, text_height = 0;
    Text_Size(&text_width, &text_height);
    *width = text_width + flag_width + shape_width + 20;
    *height = shape_height > flag_height ? shape_height : flag_height;
    *height = *height > text_height ? *height : text_height;
    *width += 10;
    *height += 4;
    if (rows > 1) {
        *height *= rows;
    }
}


// int ProgressScreenClass::Row_Y(int y, int row, bool flag) (0x0061F3B0)
// The top of a row: y plus row times the height of a one-row box.

int ProgressScreenClass::Row_Y(int y, int row, bool flag)
{
    int width = 0, height = 0;
    Box_Size(&width, &height, 1, flag);
    return y + height * row;
}


int ProgressScreenClass::Shape_Offset(bool flag)
{
    int width = 0, height = 0;
    Box_Size(&width, &height, 1, flag);
    Rect frame = Shape->Get_Frame_Rect(0);
    return (height - 6 - frame.Height) / 2;
}


int ProgressScreenClass::Flag_Offset(bool flag)
{
    int width = 0, height = 0;
    Box_Size(&width, &height, 1, flag);
    int flag_width = 0, flag_height = 0;
    Flag_Size(&flag_width, &flag_height);
    return (height - flag_height) / 2;
}


int ProgressScreenClass::Text_Offset(bool flag)
{
    int width = 0, height = 0;
    Box_Size(&width, &height, 1, flag);
    int text_width = 0, text_height = 0;
    Text_Size(&text_width, &text_height);
    return (height - text_height) / 2;
}


// double ProgressScreenClass::Average_Progress() (0x0061F8C0)
// The players' mean progress as a fraction of the total.

double ProgressScreenClass::Average_Progress()
{
    double sum = 0;
    int count = Count;
    for (int index = 0; index < count; index++) {
        sum += Progress[index];
    }
    return sum / count / Total;
}


// void ProgressScreenClass::Draw_Bar(Point2D xy, double fraction, Surface *surface, ColorScheme *scheme) (0x0061F8F0)
// Draws the shape's first frame cut to fraction of its width, through the
// scheme's drawer (DarkRed's colours and the GUI drawer without a scheme,
// Drawer when set), with an outline in the scheme's base colour when
// unknown_71 is set.
#include "shared/palette.hpp"
#include "shared/hsv_class.hpp"
#include "shared/hicolor.hpp"
#include "shared/gadget.hpp"

void ProgressScreenClass::Draw_Bar(Point2D xy, double fraction, Surface *surface, ColorScheme *scheme)
{
    Rect rect = Shape->Get_Frame_Rect(0);
    rect.X = xy.X + 3;
    rect.Y = xy.Y + 3;
    Rect outline = rect;
    Unknown_Rect_Shrink_006F1B00(&outline, -3);
    ConvertClass *drawer;
    if (scheme == 0) {
        scheme = Fetch_Scheme_By_Name("DarkRed", 1);
        drawer = unknown_drawer_8323c4;
    } else {
        drawer = scheme->Converter;
    }
    if (Drawer != 0) {
        drawer = Drawer;
    }
    if (unknown_71) {
        RGBClass color = scheme->BaseColor.To_RGB();
        surface->Draw_Rect(outline, Build_Hicolor_Pixel(color.Red, color.Green, color.Blue));
    }
    Rect bar = rect;
    bar.Width = rect.Width * fraction;
    CC_Draw_Shape(surface, drawer, Shape, 0, Point2D(0, 0), bar, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
}


// void ProgressScreenClass::Draw_Flag(Point2D xy, Surface *flag) (0x0061FAB0)
// Copies a country flag image to xy on the composite surface, magenta
// transparent, in the size of the first country's flag.
#include "shared/hicolor_pixel.hpp"
#include "shared/pcx_cache.hpp"

void ProgressScreenClass::Draw_Flag(Point2D xy, Surface *flag)
{
    int width = 0, height = 0;
    Flag_Size(&width, &height);
    Rect rect(xy.X, xy.Y, width, height);
    int transparent = RGB_To_Pixel(255, 0, 255);
    if (unknown_surface_839954 != 0 && !static_cast<DSurface *>(unknown_surface_839954)->Is_Surface_Lost()) {
        unknown_bitmap_writer_a76378.DrawTrans(rect, unknown_surface_839954, flag, transparent);
    }
}


// void ProgressScreenClass::Draw_Name(Point2D xy, wchar_t const *name, ColorScheme *scheme, int width) (0x0061FB70)
// Prints a player's name in the progress font, clipped to the name column
// (width -1: the width of fifteen "W"s).

void ProgressScreenClass::Draw_Name(Point2D xy, wchar_t const *name, ColorScheme *scheme, int width)
{
    int text_width, text_height;
    Text_Size(&text_width, &text_height);
    if (width != -1) {
        text_width = width;
    }
    Rect clip(0, 0, xy.X + text_width, xy.Y + text_height);
    Fancy_Text_Printf(name, unknown_surface_839954, &clip, &xy, scheme, 0, 0x19);
}


void ProgressScreenClass::Draw_Player(ColorScheme *scheme, wchar_t const *name, Point2D &xy, int player, bool flag)
{
    double fraction = Get_Progress(player);
    int y = Row_Y(xy.Y, player, flag);
    int width = 0, height = 0;
    Box_Size(&width, &height, 1, flag);
    if (NameWidth != -1) width = NameWidth;
    if (unknown_71) {
        Rect shadow(xy.X, y, width, height);
        Unknown_005feb60(&shadow, unknown_surface_839954, 0, 0x9f);
    }
    int shape_width = 0, shape_height = 0;
    Shape_Size(&shape_width, &shape_height);
    Point2D bar = xy;
    int bar_offset = Shape_Offset(flag);
    bar.X += 5;
    bar.Y = y + bar_offset;
    Point2D item = xy;
    item.X += shape_width + 15;
    Point2D flag_point = item;
    if (flag) flag_point.Y = y + Flag_Offset(flag);
    Point2D name_point = item;
    int text_offset = Text_Offset(flag);
    if (flag) {
        int flag_width = 0, flag_height = 0;
        Flag_Size(&flag_width, &flag_height);
        name_point.X += flag_width + 10;
    }
    name_point.Y = y + text_offset;
    Draw_Bar(bar, fraction, unknown_surface_839954, scheme);
    if (flag) {
        int country = unknown_players_a3fa7c[player]->unknown_66e290();
        if ((unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) && unknown_players_a3fa7c[player]->Team == -1) country = -3;
        Surface *image = Country_Flag(country);
        if (image) Draw_Flag(flag_point, image);
    }
    Draw_Name(name_point, name, scheme, xy.X - name_point.X - 3 + width);
}


// void ProgressScreenClass::Draw_It(Point2D xy) (0x00620050)
// With a window, draws the first player's bar into its control 0x639 on
// the background surface. Otherwise draws a row per player from xy
// (Position when xy is -1,-1): a single player shows Name (empty without
// one) in the side's colours in a skirmish, else the first player's name and
// colours. Then shows the screen.
#include "shared/abstract_type.hpp"
#include "shared/win32_window.hpp"
#include "shared/gscreen.hpp"

void ProgressScreenClass::Draw_It(Point2D xy)
{
    if (unknown_60 && Shape != 0) {
        Point2D point;
        if (Window != 0) {
            RECT rect;
            HWND control = GetDlgItem(Window, 0x639);
            Window_Rect_In_Main_Client(control, &rect);
            point = Point2D(rect.left, rect.top);
            Draw_Bar(point, Get_Progress(0), unknown_surface_839958, 0);
            return;
        }
        point = xy;
        if (point.X == -1 && point.Y == -1) {
            point = Position;
        }
        wchar_t const *name = Name;
        if (Count == 1) {
            ColorScheme *scheme;
            if (unknown_game_mode_a3d298 == 0) {
                if (name == 0) {
                    name = unknown_empty_wide_839d4c;
                }
                scheme = Side_Color_Scheme();
            } else {
                if (name == 0) {
                    name = unknown_players_a3fa7c[0]->Name;
                }
                scheme = Player_Color_Scheme(0);
            }
            Draw_Player(scheme, name, point, 0, ShowFlags);
        } else {
            for (int player = 0; player < Count; player++) {
                ColorScheme *scheme = Player_Color_Scheme(player);
                Draw_Player(scheme, unknown_players_a3fa7c[player]->Name, point, player, ShowFlags);
            }
        }
        GScreenClass::Blit(true, unknown_surface_839954);
    }
}


#include "shared/win32_api.hpp"


// void ProgressScreenClass::Set_Progress_Percent(int index, double value, Point2D point) (0x006201C0)
// Sets a player's progress to value percent of the total (at most the
// total); when it changed, repaints the window or redraws at point. The
// listener (while the screen is active) is told the mean percent before and
// after.

void ProgressScreenClass::Set_Progress_Percent(int index, double value, Point2D point)
{
    int before = Average_Progress() * 100.0;
    double old = Progress[index];
    Progress[index] = (Total * 0.01) * value;
    if (Progress[index] > Total) {
        Progress[index] = Total;
    }
    if (old != Progress[index]) {
        if (Window != 0) {
            SendMessageA(Window, 0x0F, 0, 0);
        } else {
            Draw_It(point);
        }
    }
    int after = Average_Progress() * 100.0;
    int percent[2] = { before, after };
    if (Listener != 0 && unknown_60) {
        Listener->Message_00536ea0(0x11AE, percent);
    }
}


// void ProgressScreenClass::Add_Progress(int index, double value, Point2D point) (0x006202C0)
// Adds value percent of the total to a player's progress (at most the
// total); when it changed, repaints the window or redraws at point. The
// listener is told the mean percent before and
// after.

void ProgressScreenClass::Add_Progress(int index, double value, Point2D point)
{
    int before = Average_Progress() * 100.0;
    double old = Progress[index];
    Progress[index] += (Total * 0.01) * value;
    if (Progress[index] > Total) {
        Progress[index] = Total;
    }
    if (old != Progress[index]) {
        if (Window != 0) {
            SendMessageA(Window, 0x0F, 0, 0);
        } else {
            Draw_It(point);
        }
    }
    int after = Average_Progress() * 100.0;
    int percent[2] = { before, after };
    if (Listener != 0) {
        Listener->Message_00536ea0(0x11AE, percent);
    }
}


// bool ProgressScreenClass::Has_Window() (0x006203C0)

bool ProgressScreenClass::Has_Window()
{
    return Window != 0;
}


// void ProgressScreenClass::Clear_Window() (0x006203D0)

void ProgressScreenClass::Clear_Window()
{
    Window = 0;
}


// void ProgressScreenClass::Set_Listener(Unknown535910 *listener) (0x006203E0; vtable slot 0)

void ProgressScreenClass::Set_Listener(Unknown535910 *listener)
{
    Listener = listener;
}


// double ProgressScreenClass::Get_Progress(int player) (0x006203F0)
// A player's share of the total (the total itself past the last player).

double ProgressScreenClass::Get_Progress(int player)
{
    if (player >= Count) {
        return Total;
    }
    return Progress[player] / Total;
}
