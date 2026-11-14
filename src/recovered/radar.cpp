// radar.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/radar.json carries the addresses and the rows that
// stay in their own files.


#define RA2_COORD_FROM_CELL_INLINE
#define RA2_RECT_POINT_INLINE
#define RA2_RECT_MINUS_INLINE
#define RA2_RADAR_RENDER_VIEW
#define RA2_RADAR_BACKGROUND_MEMBERS
#define RGB_DECLARE_ACCESSORS
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/radar.hpp"
#include "shared/hicolor.hpp"
#include "shared/id_source.hpp"
#include "shared/debug_printf.hpp"
#include "shared/techno.hpp"
#include "shared/sidebar.hpp"
#include "shared/palette.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/tactical.hpp"
#include "shared/keyboard.hpp"
#include "shared/mouse.hpp"
#include "shared/object.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/super.hpp"
#include "shared/blit.hpp"
#include "shared/house_screen.hpp"
#include "shared/house_type.hpp"
#include "shared/text_print.hpp"
#include "shared/convert.hpp"
#include "shared/color_scheme.hpp"
#include "shared/tool_tip.hpp"
#include "shared/dsurface.hpp"
#include "shared/surface.hpp"
#include "shared/crt_memory.hpp"
#include "shared/always.hpp"
#define RB_MIN(a, b) ((a) < (b) ? (a) : (b))
#include "shared/rgb_class.hpp"
#include "shared/house.hpp"
#include "shared/progress_screen.hpp"
#include "shared/crt_string.hpp"
#include "shared/radar_event_class.hpp"
#include "shared/rules.hpp"
#include "shared/movie.hpp"
#include "shared/gdlg_supp.hpp"
#include "shared/session_state.hpp"
#include "shared/multiplayer_game_mode.hpp"
#include "shared/wide_string.hpp"
#include "shared/dialog_default_proc.hpp"
#include "shared/uicmnds.hpp"
#include "shared/dialog_create.hpp"
#include "shared/ms_message_box.hpp"


// CoordStruct(Cell) is inline here: RTacticalClass::Action 0x0062FE30 converts its cell as TibSun Coord(cell).
// Rect corner point operators are inline where Render_Radar 0x00633210 expands them.
// RadarClass +0x1238 as three members (Compute_Background 0x00630B70).
// Existing RGB accessors are used by Radar Init_For_House in this unit.
// Header-static dynamic initializers of the unit at 0x0062EB00: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0062EB00-0x0062ECE1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0062EB20, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0062eb00.cpp; no source of its own.


// Dynamic initializer (0x0062EB50, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0062eb00.cpp; no source of its own.


// Dynamic initializer (0x0062EB70, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0062eb00.cpp; no source of its own.


// Dynamic initializer (0x0062EB90, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0062eb00.cpp; no source of its own.


// Dynamic initializer (0x0062EBB0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0062eb00.cpp; no source of its own.


// Dynamic initializer (0x0062EBD0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0062eb00.cpp; no source of its own.


// Dynamic initializer (0x0062EBF0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0062eb00.cpp; no source of its own.


// Dynamic initializer (0x0062EC30, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0062eb00.cpp; no source of its own.


// Dynamic initializer (0x0062EC60, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0062eb00.cpp; no source of its own.


// Dynamic initializer (0x0062EC90, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0062eb00.cpp; no source of its own.


// Dynamic initializer (0x0062ECC0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0062eb00.cpp; no source of its own.


// Dynamic initializer (0x0062ECD0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0062eb00.cpp; no source of its own.


// Dynamic initializer of RadarClass::RadarButton (0x0062ECF0, _$E) and its
// guarded atexit destructor (0x0062ED20), emitted by this definition; the
// two shape buttons' pairs (0x0062ED50/0x0062ED70, 0x0062EDA0/0x0062EDC0)
// follow from theirs, with the do-nothing initializers of the cell and
// coordinate statics (0x0062ED40, 0x0062ED90) between them. The guard byte of the three is 0x00AB6308.

RTacticalClass RadarClass::RadarButton;
Cell RadarClass::unknown_cell_ab6330;
ShapeButtonClass RadarClass::unknown_button_ab63e8;
CoordStruct RadarClass::unknown_coord_ab6370;
ShapeButtonClass RadarClass::unknown_button_ab6380;


// Static-member initializer or guarded atexit destructor (0x0062ED20, _$E) emitted by the definitions in RadarClass_RadarButton_init_0062ecf0.cpp; no source of its own.


// Dynamic initializer of RadarClass::unknown_cell_ab6330 (0x0062ED40, _$E): the
// empty Cell constructor leaves only a return; emitted by the definition
// in RadarClass_RadarButton_init_0062ecf0.cpp, no source of its own.


// Static-member initializer or guarded atexit destructor (0x0062ED50, _$E) emitted by the definitions in RadarClass_RadarButton_init_0062ecf0.cpp; no source of its own.


// Static-member initializer or guarded atexit destructor (0x0062ED70, _$E) emitted by the definitions in RadarClass_RadarButton_init_0062ecf0.cpp; no source of its own.


// Dynamic initializer of RadarClass::unknown_coord_ab6370 (0x0062ED90, _$E): the
// empty CoordStruct constructor leaves only a return; emitted by the definition
// in RadarClass_RadarButton_init_0062ecf0.cpp, no source of its own.


// Static-member initializer or guarded atexit destructor (0x0062EDA0, _$E) emitted by the definitions in RadarClass_RadarButton_init_0062ecf0.cpp; no source of its own.


// Static-member initializer or guarded atexit destructor (0x0062EDC0, _$E) emitted by the definitions in RadarClass_RadarButton_init_0062ecf0.cpp; no source of its own.


// RadarClass::RadarClass() (0x0062EDE0)
// Clears the radar's state, builds its vectors and its 256-bucket tracking
// table (hash 0x00631C60, growth 10), and sets the point list's growth step
// to 500.

static inline HashTableClass<RadarTrackingStruct, TechnoClass *> *Make_Table(int count, int growth)
{
    return new HashTableClass<RadarTrackingStruct, TechnoClass *>(growth, count, Unknown_00631C60);
}

RadarClass::RadarClass()
    : unknown_11e4(0), unknown_11e8(0), unknown_11ec(0), unknown_11f0(0), unknown_11f4(0), unknown_11f8(0),
      unknown_11fc(0), unknown_1200(0), unknown_1208(unknown_rect_ab6320), unknown_1218(0), unknown_121c(0),
      BackgroundColors(0), RadarCellWidth(0), RadarCellHeight(0), unknown_1244(0, 0, 0, 0), unknown_1254(0), unknown_1270(0), unknown_1484(0), unknown_1488(1), unknown_148c(0), unknown_1490(0),
      unknown_1494(0), unknown_1498(unknown_rect_ab6320), unknown_14a8(0), unknown_14ac(0), unknown_14b0(0),
      unknown_14b8(false), unknown_14b9(false), unknown_14d0(-1), unknown_14d4(false), unknown_14d5(false),
      unknown_14d6(false), unknown_14d8(unknown_rect_ab6320), unknown_14f8(0), unknown_14fc(0)
{
    unknown_1254 = Make_Table(256, 10);
    unknown_1258.Set_Growth_Step(500);
    unknown_14bc.Unknown_00405BF0();
}


// RadarClass::~RadarClass() (0x0062F080)
// Releases the radar's list at +0x14BC and its tracking table (0x00631E40);
// the member vectors and the map layers follow.

RadarClass::~RadarClass()
{
    unknown_14bc.Unknown_00405C10();
    Clear_Radar();
}


// void RadarClass::Queue_Next_Movie() (0x0062F150)
// Sets +0x14A8 to 4 and +0x14F8 to 25. No direct callers (inlined elsewhere).

void RadarClass::Queue_Next_Movie()
{
    unknown_14a8 = 4;
    unknown_14f8 = 25;
}


// void RadarClass::One_Time() (0x0062F170, slot 5)
// Fixes the radar's screen metrics, runs the display's one-time setup,
// places the radar button and looks up the incoming-transmission voice.

void RadarClass::One_Time()
{
    Debug_Printf("RadarClass::One_Time()\n");
    unknown_11fc = 140;
    unknown_11e0 = 0;
    unknown_11e4 = 16;
    unknown_11e8 = 48;
    unknown_1200 = 108;
    unknown_11f4 = unknown_11fc;
    unknown_11f8 = unknown_1200;
    unknown_11f0 = 49;
    DisplayClass::One_Time();
    RadarButton.Y = unknown_11f0;
    RadarButton.Width = unknown_11fc;
    RadarButton.Height = unknown_1200;
    RadarButton.Flags = 0xDF;
    unknown_14d0 = Unknown_00716640("EVA_IncomingTransmission");
}


// void RadarClass::DisposeOfArt() (0x0062F210, slot 50)
// Frees the radar's two shapes when they were loaded rather than retrieved.

void RadarClass::DisposeOfArt()
{
    if (unknown_shape_ab6350_allocated) {
        operator delete((void *)unknown_shape_ab6350);
        unknown_shape_ab6350_allocated = false;
    }
    unknown_shape_ab6350 = 0;
    if (unknown_shape_ab6318_allocated) {
        operator delete((void *)unknown_shape_ab6318);
        unknown_shape_ab6318_allocated = false;
    }
    unknown_shape_ab6318 = 0;
}


// void RadarClass::Init_Clear() (0x0062F260, slot 7)
// Clears the display, then the radar's state and its two lists.

void RadarClass::Init_Clear()
{
    DisplayClass::Init_Clear();
    Debug_Printf("RadarClass::Init_Clear\n");
    unknown_14a8 = 0;
    unknown_14ac = 0;
    unknown_14b9 = false;
    unknown_14b8 = false;
    unknown_14f8 = 0;
    unknown_14d4 = false;
    unknown_14d5 = true;
    unknown_1220.Clear();
    unknown_1258.Clear();
    Debug_Printf("RadarClass::Init_Clear done\n");
}


// void RadarClass::Set_Buttons(bool add) (0x0062F2D0)
// Adds the two radar shape buttons to the button list, or removes them.

void RadarClass::Set_Buttons(bool add)
{
    if (add) {
        Add_A_Button(unknown_button_ab63e8);
        Add_A_Button(unknown_button_ab6380);
    } else {
        Remove_A_Button(unknown_button_ab63e8);
        Remove_A_Button(unknown_button_ab6380);
    }
}


// Radar Init_For_House0062F310, independent372+12.
void RadarClass::Init_For_House()
{
    Debug_Printf("RadarClass::Init_For_House()\n");
    DisposeOfArt();
    if(unknown_id_source_a3d290->unknown_2239) {
        unknown_11ec=(unknown_int_7add80-144)/2+4;
        unknown_int_ab6468=unknown_11e0+11;
        unknown_int_ab646c=unknown_11e4+4;
        unknown_int_ab6338=unknown_11e0+83;
    } else {
        unknown_11ec=(unknown_int_7add80-145)/2+5;
        unknown_int_ab6468=unknown_11e0+14;
        unknown_int_ab646c=unknown_11e4+5;
        unknown_int_ab6338=unknown_11e0+86;
    }
    unknown_int_ab633c=unknown_int_ab646c;
    RadarButton.X=unknown_11ec;
    unknown_shape_ab6350=Unknown_Load_Shape_00495270(unknown_name_7ee064,unknown_shape_ab6350_allocated);
    unknown_shape_ab6318=Unknown_Load_Shape_00495270(unknown_name_7ee068,unknown_shape_ab6318_allocated);
    unknown_int_ab6470=((ShapeFileStruct const *)unknown_shape_ab6350)->Width;
    unknown_int_ab6474=((ShapeFileStruct const *)unknown_shape_ab6350)->Height;
    unknown_int_ab6340=((ShapeFileStruct const *)unknown_shape_ab6318)->Width;
    unknown_int_ab6344=((ShapeFileStruct const *)unknown_shape_ab6318)->Height;
    RadarButton.unknown_28=unknown_art_ac1070;
    unknown_1204=Build_Hicolor_Pixel(unknown_color_ac1028.Get_Red(),unknown_color_ac1028.Get_Green(),unknown_color_ac1028.Get_Blue());
}


// void RadarClass::Init_IO() (0x0062F490, slot 8)
// After the display's controls, sets up the two radar shape buttons (ids
// 0xF2 and 0xF3) at their positions with their shapes.

void RadarClass::Init_IO()
{
    DisplayClass::Init_IO();
    if (!Debug_Map) {
        unknown_button_ab63e8.ID = 0xF2;
        unknown_button_ab63e8.DrawOnSidebar = true;
        unknown_button_ab63e8.ShapeDrawer = unknown_drawer_8323cc;
        unknown_button_ab63e8.IsOn = false;
        unknown_button_ab63e8.ToggleType = 0;
        unknown_button_ab63e8.Flags = 5;
        unknown_button_ab63e8.Set_Position(unknown_int_ab6468, unknown_int_ab646c);
        unknown_button_ab63e8.Set_Shape(unknown_shape_ab6350);
        unknown_button_ab63e8.Flag_To_Redraw();

        unknown_button_ab6380.ID = 0xF3;
        unknown_button_ab6380.DrawOnSidebar = true;
        unknown_button_ab6380.ShapeDrawer = unknown_drawer_8323cc;
        unknown_button_ab6380.IsOn = false;
        unknown_button_ab6380.ToggleType = 0;
        unknown_button_ab6380.Flags = 5;
        unknown_button_ab6380.Set_Position(unknown_int_ab6338, unknown_int_ab633c);
        unknown_button_ab6380.Set_Shape(unknown_shape_ab6318);
        unknown_button_ab6380.Flag_To_Redraw();
    }
}


// void RadarClass::Draw_It(bool complete) (0x0062F580, slot 16)
// Draws the radar frame: the background art and the two radar buttons when a
// redraw is due, the radar button's open/close animation (+0x14A8 2 closes,
// 3 opens, one frame per four ticks of +0x14FC), the radar events and image,
// the per-mode button frame (+0x14AC), the two sidebar buttons on a complete
// redraw, and grows the redraw rectangle +0x1208 by the radar buttons that
// need drawing.

void RadarClass::Draw_It(bool complete)
{
    bool redraw = false;
    if (Debug_Map) {
        unknown_14d6 = false;
        unknown_14d5 = false;
        return;
    }
    unknown_14d6 = (unknown_14d6 == true || complete == true);
    unknown_14d5 = (unknown_14d5 == true || unknown_14d6 == true);
    if (unknown_14d5) {
        ShapeFileStruct const *shape = (ShapeFileStruct const *)unknown_buffer_ac0ff0;
        CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, shape, 0,
                      Point2D(unknown_11e0, unknown_11e4), unknown_surface_839948->Get_Rect(), 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        unknown_button_ab63e8.Draw_Me(true);
        unknown_button_ab6380.Draw_Me(true);
    }

    if (unknown_14a8 != 1) {
        if (unknown_14a8 != 0 && unknown_14fc.Value() == 0) {
            unknown_14fc = 4;
            redraw = true;
            if (unknown_14a8 == 2) {
                if (--unknown_14f8 <= 0) {
                    unknown_14a8 = 0;
                    unknown_14bc.Unknown_00406070();
                    unknown_14f8 = 0;
                }
            } else {
                if (++unknown_14f8 >= 32) {
                    unknown_14bc.Unknown_00406070();
                    unknown_14a8 = 1;
                    unknown_14d6 = true;
                    unknown_14f8 = 32;
                }
            }
        }
        if (unknown_14a8 != 1 && (unknown_14d5 == true || redraw)
            && unknown_14f8 >= 0 && unknown_14f8 < ((ShapeFileStruct const *)RadarButton.unknown_28)->FrameCount) {
            CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)RadarButton.unknown_28, unknown_14f8,
                          Point2D(unknown_11e0, unknown_11e8), unknown_surface_839948->Get_Rect(), 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            unknown_1208 = Rect(unknown_11e0, unknown_11e8, unknown_int_7add80, 110);
        }
    }

    RadarEventClass_unknown_all_63b910();
    Render_Radar();
    if (unknown_14a8 == 1) {
        switch (unknown_14ac) {
        case 1:
        case 4:
            break;
        case 2:
            if (unknown_14d5) {
                unknown_14d5 = false;
                if (unknown_14d6) {
                    CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)RadarButton.unknown_28, 32,
                                  Point2D(unknown_11e0, unknown_11e8), unknown_surface_839948->Get_Rect(), 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                    Draw_Names();
                    unknown_1208 = Rect(unknown_11e0, unknown_11e8, unknown_int_7add80, 110);
                } else {
                    Draw_Names();
                    unknown_1208 = Rect(unknown_11ec, unknown_11f0, unknown_11fc, unknown_1200);
                }
            }
            break;
        case 3:
            Play_Movie();
            break;
        default:
            if (unknown_14d5) {
                unknown_14d5 = false;
                CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)RadarButton.unknown_28, 0,
                              Point2D(unknown_11e0, unknown_11e8), unknown_surface_839948->Get_Rect(), 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                unknown_1208 = Rect(unknown_11e0, unknown_11e8, unknown_int_7add80, 110);
            }
            break;
        }
    }

    if (unknown_14d6) {
        unknown_14d6 = false;
        SidebarClass::unknown_button_abcc58.Draw_Me(true);
        SidebarClass::unknown_button_ab96b8.Draw_Me(true);
    }
    if (unknown_button_ab63e8.IsDrawn) {
        unknown_1208 = Union(unknown_1208, unknown_rect_ab6468);
        unknown_flag_abcd84 = true;
    }
    if (unknown_button_ab6380.IsDrawn) {
        unknown_1208 = Union(unknown_1208, unknown_rect_ab6338);
        unknown_flag_abcd84 = true;
    }
}


// 0x0062fbc0: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_0062fbc0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0062fbc0 {

void __stdcall FUN_0062fbc0(int unused_0, int unused_1)
{
    return;
}

}  // namespace seed_0062fbc0


// Cell RadarClass::Click_Cell_Calc(Point2D const &xy) (0x0062FBD0, slot 51)
// The cell under a screen point: the cached cell (0x00AB6330) when the point
// maps to the cached coordinate (0x00AB6370), else the tactical view's cell.
// Both branches on the flag at 0x00A40D13 compile to the same call.

Cell RadarClass::Click_Cell_Calc(Point2D const &xy)
{
    Point2D point = xy;
    CoordStruct coord = unknown_tactical_83996c->Pixel_To_Coord(point);
    if (coord != unknown_coord_ab6370) {
        if (Debug_Map) {
            return unknown_tactical_83996c->Pixel_To_Cell(point);
        }
        return unknown_tactical_83996c->Pixel_To_Cell(point);
    }
    return unknown_cell_ab6330;
}


// bool RadarClass::Map_Cell(Cell const &cell, HouseClass *house) (0x0062FC80)
// Passes the call on (the radar adds nothing).

bool RadarClass::Map_Cell(Cell const &cell, HouseClass *house)
{
    if (DisplayClass::Map_Cell(cell, house)) {
        return true;
    }
    return false;
}


// bool RadarClass::RevealFogShroud(Cell const &cell, HouseClass *house, bool arg3) (0x0062FCA0)
// Passes the call on (the radar adds nothing).

bool RadarClass::RevealFogShroud(Cell const &cell, HouseClass *house, bool arg3)
{
    if (DisplayClass::RevealFogShroud(cell, house, arg3)) {
        return true;
    }
    return false;
}


// void RadarClass::AI(int &input, Point2D const &xy) (0x0062FCC0, slot 10)
// After the display's AI: the normal mouse over either radar button (shifted
// right by the tactical width when the sidebar is on the right), the
// button presses (0x80F2 selects mode 9 outside multiplayer or with a
// ready session in game modes 3/4, else 8; 0x80F3 mode 1), the forced
// transmission mode 4 while 0x00AF8FE8 is positive, then the mode-4 update
// or 0x00716950.

void RadarClass::AI(int &input, Point2D const &xy)
{
    DisplayClass::AI(input, xy);
    int offset = unknown_flag_a40b30 ? unknown_int_839638 : 0;
    Rect area = unknown_rect_ab6468;
    area.X += offset;
    if (area.Is_Point_Within(xy)) {
        Map.Override_Mouse_Shape(0, false);
    }
    area = unknown_rect_ab6338;
    area.X += offset;
    if (area.Is_Point_Within(xy)) {
        Map.Override_Mouse_Shape(0, false);
    }
    if (input == 0x80F2) {
        if (unknown_game_mode_a3d298 == 0
            || ((unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) && unknown_session_a3d29c != 0
                && unknown_session_a3d29c->unknown_slot2() && unknown_session_a3d29c->unknown_slot1())) {
            unknown_int_a40d48 = 9;
        } else {
            unknown_int_a40d48 = 8;
        }
    } else if (input == 0x80F3) {
        unknown_int_a40d48 = 1;
    }
    if (unknown_int_af8fe8 > 0 && unknown_14ac != 3 && unknown_14ac != 4) {
        unknown_14b0 = unknown_14ac;
        Radar_Activate(4, false);
        unknown_14b4 = 0;
    }
    if (unknown_14ac == 4) {
        Update_Transmission();
    } else {
        Unknown_00716950();
    }
}


// int RTacticalClass::Action(unsigned flags, unsigned &key) (0x0062FE30, slot 31)
// The radar's tactical gadget: a click on the radar picks the cell (and object) under it; with a
// pending action it shows or performs it on the map, otherwise a click recentres the tactical view
// on that cell, clamped to the playable diamond (TibSun RadarClass::RTacticalClass::Action; the
// ra2-decomp YR twin has the same four edge clamps). Draft: Scout704/Scout829, Scout836, Scout894.
// The nested press/cell tests through Cell's operator!= and the CoordStruct(Cell) conversion follow
// the twin (radar.cpp 0x006539D0, Coord(cell)): writing cell.X/cell.Y directly before the clamps makes
// VC6 regroup the last clamp's (X + Y) - bottom_edge as (Y - bottom_edge) + X.
// Declared in shared/scroll.hpp and shared/main_loop.hpp, which this unit cannot include
// (scroll.hpp refuses the light map_view; main_loop.hpp brings always.hpp's min next to the
// unit's own).
ObjectClass *__fastcall Unknown_0051A690(Cell const *cell, ObjectClass *object);
extern bool unknown_flag_a40d45;
extern int unknown_int_8325d0;
extern int unknown_int_8325d4;

int RTacticalClass::Action(unsigned flags, unsigned &key)
{
    if (flags & 0x22) {
        return false;
    }
    int mode = Map.unknown_14ac;
    int state = Map.unknown_14a8;
    bool playing = mode == 3 && state == 1;
    bool tactical = mode == 1 && state == 1;
    if (playing) {
        return false;
    }
    if (!tactical) {
        return false;
    }
    int x;
    int y;
    if (flags & 0x55) {
        x = WWKeyboard->MouseQX;
        y = WWKeyboard->MouseQY;
    } else {
        x = WWMouse->Get_Mouse_X();
        y = WWMouse->Get_Mouse_Y();
    }
    x -= unknown_flag_a40b30 ? unknown_int_839638 : 0;
    if (x < Map.unknown_1498.X || x >= Map.unknown_1498.X + Map.unknown_1498.Width || y < Map.unknown_1498.Y || y >= Map.unknown_1498.Y + Map.unknown_1498.Height) {
        Map.Override_Mouse_Shape(0, false);
        return false;
    }
    Point2D click(x, y);
    Cell cell(0, 0);
    bool shadow;
    ObjectClass *object = 0;
    Map.Resolve_Radar_Point(click, cell, (TechnoClass *&)object);
    if (cell != RadarClass::unknown_cell_ab6330) {
        CoordStruct coord = Cell_Coord(cell);
        coord.Z = Map.Get_Height_GL(coord);
        shadow = Map.Is_Shrouded(coord) && unknown_b263d8;
        int action = 0;
        bool nuke = unknown_pending_super_833694 == 0;
        bool first = unknown_pending_super_833694 == 1;
        bool second = unknown_pending_super_833694 == 2;
        bool third = unknown_pending_super_833694 == 5;
        bool fourth = unknown_pending_super_833694 == 6;
        bool pressed = (flags & 0x50) != 0;
        if (!pressed && (unknown_objects_a40c60.Count() != 0 || nuke || first || second || third || fourth)) {
            if (nuke) {
                action = 0x14;
            } else if (first) {
                action = 0x25;
            } else if (second) {
                action = 0x26;
            } else if (third) {
                action = 0x29;
            } else if (fourth) {
                action = 0x41;
            } else if (object) {
                action = Unknown_0051A690(0, object)->unknown_slot28(object, false);
            } else {
                action = Unknown_0051A690(&cell, 0)->unknown_slot27(&cell, false, false);
            }
            switch (action) {
            case 1: case 2: case 3: case 5: case 6: case 9: case 0x10: case 0x14: case 0x1A: case 0x25: case 0x26: case 0x29: case 0x41:
                break;
            default:
                action = 0;
                object = 0;
                break;
            }
        }
        if (action != 0) {
            if (flags & 8) {
                Map.unknown_slot46(cell, shadow, object, action, true);
            }
            if ((flags & 4) && !unknown_flag_a40d45) {
                Map.Mouse_Left_Release(CoordStruct(cell), cell, object, action, true);
            }
        } else {
            if (!pressed) {
                Map.Set_Default_Mouse(0, true);
            }
            if (flags & 0x11) {
                if (cell != Cell(0, 0)) {
                    int side_edge = unknown_int_8325d0 - (unknown_int_839638 / 60 + 2) / 2 - 1;
                    int half_view_height = unknown_int_83963c / 60;
                    int top_edge = half_view_height + unknown_int_8325d0 + 1;
                    int bottom_edge = 2 * unknown_int_8325d4 - half_view_height + unknown_int_8325d0 - 1;
                    int adjust;
                    if (cell.Y - cell.X > side_edge) {
                        adjust = cell.Y - cell.X - side_edge;
                        cell.Y -= adjust;
                        cell.X += adjust;
                    }
                    if (cell.X - cell.Y > side_edge - 1) {
                        adjust = cell.X - cell.Y - side_edge + 1;
                        cell.Y += adjust;
                        cell.X -= adjust;
                    }
                    if (cell.X + cell.Y < top_edge) {
                        adjust = top_edge - cell.Y - cell.X;
                        cell.X += adjust;
                        cell.Y += adjust;
                    }
                    if (cell.X + cell.Y > bottom_edge) {
                        adjust = cell.X + cell.Y - bottom_edge;
                        cell.X -= adjust;
                        cell.Y -= adjust;
                    }
                    CoordStruct center = reinterpret_cast<UnknownCellClass &>(Map[cell]).unknown_475d60();
                    unknown_tactical_83996c->Set_Tactical_Position_006a66b0(center);
                    Map.Flag_To_Redraw(1);
                    if (DepthBuffer != 0) {
                        DepthBuffer->MaxValue = 0x8000;
                    }
                }
            }
        }
    }
    GadgetClass::Action(0, key);
    return true;
}


// void RadarClass::Set_Map_Dimensions(Rect const *rect, bool arg2, int arg3, bool arg4) (0x00630300, slot 28)
// Passes the map dimensions on to the base class (TibSun RadarClass::Set_Map_Dimensions).

void RadarClass::Set_Map_Dimensions(Rect const *rect, bool arg2, int arg3, bool arg4)
{
    DisplayClass::Set_Map_Dimensions(rect, arg2, arg3, arg4);
}


// void RadarClass::Set_Tactical_Position(CoordStruct const &coord) (0x00630320)
// Centres the tactical view on a coordinate; passes the call on (the radar
// adds nothing). Fill_In_Data 0x0065CDD0 passes a CoordStruct with its ground
// height.

void RadarClass::Set_Tactical_Position(CoordStruct const &coord)
{
    unknown_tactical_83996c->Set_Tactical_Position_006a66b0(coord);
}


// bool RadarClass::Cell_On_Radar(Cell const &cell) (0x00630340)
// Passes the call on (the radar adds nothing).

bool RadarClass::Cell_On_Radar(Cell const &cell)
{
    return Unknown_0055A660(cell, true);
}


// void RadarClass::Draw_Names() (0x00630350)
// The multiplayer player list in the radar area (Draw_It, mode 2): the frame,
// the "Name:" and "Kills:" headings in grey, a rule, then one line per
// multiplayer house with its name (cut to 18 characters) and the sum of its
// unit and building kills.

ColorScheme *__fastcall Fetch_Scheme_By_Name(char const *name, int kind);

void RadarClass::Draw_Names()
{
    if (unknown_flag_8373c1) {
        CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shape_ab64a0, 32,
                      Point2D(unknown_11e0, unknown_11e8), unknown_surface_839948->Get_Rect(), 0, 0, 0, 0, 1000, 0, 0,
                      Point2D(0, 0));
        int y = unknown_11f0 + 2;
        Fancy_Text_Printf(StringTable::Load_String("TXT_NAME_COLON", 0, "D:\\Ra2\\Radar.CPP", 1217), unknown_surface_839948,
                          &unknown_surface_839948->Get_Rect(), &Point2D(unknown_11ec, y), Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);
        Fancy_Text_Printf(StringTable::Load_String("TXT_KILLS_COLON", 0, "D:\\Ra2\\Radar.CPP", 1219), unknown_surface_839948,
                          &unknown_surface_839948->Get_Rect(), &Point2D(unknown_11ec + unknown_11f4 - 2, y),
                          Fetch_Scheme_By_Name("Grey", 1), 0, 0x219);
        y += 7;
        unknown_surface_839948->Draw_Line(Point2D(unknown_11ec, y), Point2D(unknown_11ec + unknown_11f4 - 1, y),
                                          unknown_drawer_8323cc->Convert(14));
        y += 4;
        for (int index = 0; index < Houses.Count(); index++) {
            HouseClass *house = Houses[index];
            if (house != 0 && house->Type->Multiplay) {
                ColorScheme *scheme;
                unsigned flags;
                int color = house->ColorSchemeIndex;
                if (house->unknown_13d) {
                    scheme = Fetch_Scheme_By_Name("Grey", 1);
                    flags = 0x19;
                } else {
                    scheme = ColorSchemes[color];
                    flags = 0x4019;
                }
                wchar_t text[40];
                text[0] = 0;
                swprintf(text, L"%s", house->UIName);
                if (wcslen(text) == 0) {
                    wcscpy(text, L"________");
                }
                if (wcslen(text) > 18) {
                    text[18] = '.';
                    text[19] = 0;
                }
                Fancy_Text_Printf(text, unknown_surface_839948, &unknown_surface_839948->Get_Rect(), &Point2D(unknown_11ec, y),
                                  scheme, 0, flags);
                int kills = 0;
                for (int other = 0; other < 20; other++) {
                    kills += house->unknown_5310[other] + house->unknown_5364[other];
                }
                swprintf(text, L"%2d", kills);
                Fancy_Text_Printf(text, unknown_surface_839948, &unknown_surface_839948->Get_Rect(),
                                  &Point2D(unknown_11ec + unknown_11f4 - 2, y), scheme, 0, flags | 0x200);
                y += 9;
            }
        }
    }
}


// void RadarClass::Reposition_Sidebar() (0x006306D0, slot 34)
// After the display's, places the radar's buttons (shifted right by the
// tactical width when the sidebar is on the right, 0x00A40B30), and
// registers their tooltips (briefing or diplomacy, options).

void RadarClass::Reposition_Sidebar()
{
    DisplayClass::Reposition_Sidebar();
    int offset = unknown_flag_a40b30 ? unknown_int_839638 : 0;
    unknown_button_ab63e8.DrawOffsetX = -offset;
    unknown_button_ab63e8.Set_Position(unknown_int_ab6468 + offset, unknown_int_ab646c);
    unknown_button_ab63e8.Flag_To_Redraw();
    unknown_button_ab6380.DrawOffsetX = -offset;
    unknown_button_ab6380.Set_Position(unknown_int_ab6338 + offset, unknown_int_ab633c);
    unknown_button_ab6380.Flag_To_Redraw();
    RadarButton.Set_Position(unknown_11ec + offset, unknown_11f0);
    RadarButton.Flag_To_Redraw();

    ToolTip tip;
    tip.ID = 0xF2;
    tip.Label = "Tip:BriefingButton";
    if (unknown_game_mode_a3d298) {
        tip.Label = "Tip:DiplomacyButton";
    }
    tip.Region.X = unknown_button_ab63e8.X;
    tip.Region.Y = unknown_button_ab63e8.Y;
    tip.Region.Width = unknown_button_ab63e8.Width;
    tip.Region.Height = unknown_button_ab63e8.Height;
    ToolTips->Remove(0xF2);
    ToolTips->Add(tip);
    tip.ID = 0xF3;
    tip.Label = "Tip:OptionsButton";
    tip.Region.X = unknown_button_ab6380.X;
    tip.Region.Y = unknown_button_ab6380.Y;
    tip.Region.Width = unknown_button_ab6380.Width;
    tip.Region.Height = unknown_button_ab6380.Height;
    ToolTips->Remove(0xF3);
    ToolTips->Add(tip);
    unknown_14d6 = true;
}


void RadarClass::Set_Local_Dimensions(Rect const &size)
{
    MapClass::Set_Local_Dimensions(size);
    unknown_1498.Y = 10000;
    unknown_1498.X = 10000;
    unknown_1498.Height = 0;
    unknown_1498.Width = 0;
    Cell corner(MapLocalSize.Y + MapLocalSize.X + 1,
        MapLocalSize.Y + MapSize.Width - MapLocalSize.X);
    Cell original_corner(corner);
    int x = MapLocalSize.Width - 1;
    int y = 1 - MapLocalSize.Width;
    Cell opposite = corner + Cell(x, y);
    unknown_148c = original_corner.Y - corner.X;
    unknown_1490 = opposite.X - opposite.Y;
    unknown_1494 = -1;
    Map.Reset_Iterator();
    CellClass *iter = Iterate();
    while (iter != 0) {
        Cell cell = iter->MapCoords;
        if (Unknown_0055A660(cell, true)) {
            if (unknown_1494 == -1) unknown_1494 = iter->MapCoords.X + iter->MapCoords.Y;
            cell = iter->MapCoords;
            Rect area = Cell_Radar_Rect(cell);
            if (area.X < unknown_1498.X) unknown_1498.X = area.X;
            else if (area.X + area.Width > unknown_1498.X + unknown_1498.Width) unknown_1498.Width = area.X + area.Width - unknown_1498.X;
            if (area.Y < unknown_1498.Y) unknown_1498.Y = area.Y;
            else if (area.Y + area.Height > unknown_1498.Y + unknown_1498.Height) unknown_1498.Height = area.Y + area.Height - unknown_1498.Y;
        }
        iter = Iterate();
    }
}


// void RadarClass::Compute_Radar_Image() (0x00630A00)
// Rebuilds the radar surfaces for a non-empty radar rectangle: drops the old
// image, buffers and drawing surface, renders the image (0x00630B70),
// centres the rectangle in the 140 x 108 radar area, and makes a cleared
// drawing surface of the image's size before 0x00632700.

void RadarClass::Compute_Radar_Image()
{
    if (unknown_1498.Is_Valid()) {
        unknown_1498.Y = 0;
        unknown_1498.X = 0;
        if (unknown_121c) {
            delete unknown_121c;
            unknown_121c = 0;
        }
        if (BackgroundColors) {
            delete[] BackgroundColors;
            BackgroundColors = 0;
        }
        if (unknown_1270) {
            delete[] unknown_1270;
            unknown_1270 = 0;
        }
        Compute_Background(unknown_1498, unknown_1498, true);
        unknown_1498.Width = unknown_121c->Get_Width();
        unknown_1498.Height = unknown_121c->Get_Height();
        unknown_1498.X = unknown_11ec;
        if (unknown_1498.Width < 140) {
            unknown_1498.X = (140 - unknown_1498.Width) / 2 + unknown_11ec;
        }
        unknown_1498.Y = unknown_11f0;
        if (unknown_1498.Height < 108) {
            unknown_1498.Y = (108 - unknown_1498.Height) / 2 + unknown_11f0;
        }
        if (unknown_1218) {
            delete unknown_1218;
            unknown_1218 = 0;
        }
        unknown_1218 = new DSurface(unknown_121c->Get_Width(), unknown_121c->Get_Height(), true);
        unknown_1218->Fill(0);
        Compute_Foundations();
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Rect RadarClass::Compute_Background(Rect const &, Rect &, bool) (0x00630B70)
// TibSun radar.cpp RadarClass::Compute_Background: makes the background colour array (+0x1238)
// and the radar drawing surface (+0x121C) on first use, fills in the colours of update_rect,
// then box-filters the cell colours down into the surface pixels over the updated area.
// From ra2-decomp RadarClass::Compute_Background (YR 0x006547C0), adapted to our names.


// The surface pixel span of a cell rectangle at scale, each far edge one past and clipped to the surface.
static inline void Radar_Pixel_Bounds(Rect const &rect, float scale, int width, int height, int &left, int &right, int &top, int &bottom)
{
    left = (int)(rect.X * scale);
    right = RB_MIN((int)((rect.X + rect.Width) * scale) + 1, width);
    top = (int)(rect.Y * scale);
    bottom = RB_MIN((int)((rect.Y + rect.Height) * scale) + 1, height);
}

Rect RadarClass::Compute_Background(Rect const &cell_rect, Rect &update_rect, bool fill_in)
{
    if (fill_in && !update_rect.Is_Valid()) {
        update_rect = cell_rect;
    }

    if (BackgroundColors == 0) {
        BackgroundColors = new RGBClass[cell_rect.Height * cell_rect.Width];
        memset(BackgroundColors, 0, cell_rect.Height * cell_rect.Width * sizeof(RGBClass));
        RadarCellWidth = cell_rect.Width;
        RadarCellHeight = cell_rect.Height;
    }

    if (fill_in) {
        Fill_In_Background(cell_rect, update_rect);
    }

    int rect1_width = cell_rect.Width;
    float zoom_scale = unknown_1484;
    int surface_width;
    int surface_height;

    if (unknown_121c == 0) {
        zoom_scale = 140 / (float)rect1_width;
        float scaled_height = cell_rect.Height * zoom_scale;
        if (scaled_height < 108) {
            surface_width = 140;
            surface_height = scaled_height;
        } else {
            zoom_scale = 108 / (float)cell_rect.Height;
            float scaled_width = rect1_width * zoom_scale;
            surface_width = scaled_width;
            surface_height = 108;
        }
        unknown_1484 = zoom_scale;

        unknown_121c = new BSurface(surface_width, surface_height, 2);
        unknown_121c->Fill(0);
    } else {
        surface_width = unknown_121c->Get_Width();
        surface_height = unknown_121c->Get_Height();
    }

    float pixels_per_cell_x = (float)cell_rect.Width / (float)surface_width;
    float pixels_per_cell_y = (float)cell_rect.Height / (float)surface_height;
    float inv_area = 1.0 / (pixels_per_cell_x * pixels_per_cell_y);

    unsigned short *data = (unsigned short *)unknown_121c->Lock();

    int surf_left, surf_right, surf_top, surf_bottom;
    Radar_Pixel_Bounds(update_rect, zoom_scale, surface_width, surface_height, surf_left, surf_right, surf_top, surf_bottom);

    float src_x = surf_left / zoom_scale;
    float src_y = surf_top / zoom_scale;

    if (surf_top < surf_bottom) {

        for (int surfy = surf_top; surfy < surf_bottom; ++surfy) {

            int src_y_min = RB_MIN(RadarCellHeight, (int)src_y);
            float src_y_end = src_y + pixels_per_cell_y;
            int src_y_max = RB_MIN(RadarCellHeight, (int)src_y_end + 1);

            if (surf_left < surf_right) {

                unsigned short *pixel_data = data + surf_left + surface_width * surfy;

                for (int surfx = surf_left; surfx < surf_right; ++surfx) {

                    float accum_r = 0.0f;
                    float accum_g = 0.0f;
                    float accum_b = 0.0f;

                    int src_x_min = RB_MIN(RadarCellWidth, (int)src_x);
                    float src_x_end = src_x + pixels_per_cell_x;
                    int src_x_max = RB_MIN(RadarCellWidth, (int)src_x_end + 1);

                    for (int srcy = src_y_min; srcy < src_y_max; ++srcy) {

                        float y_weight;
                        if (src_y_max - src_y_min <= 1) {
                            y_weight = pixels_per_cell_y;
                        } else if (srcy == src_y_min) {
                            y_weight = (float)(srcy + 1) - src_y;
                        } else if (srcy == src_y_max - 1) {
                            y_weight = src_y_end - (float)srcy;
                        } else {
                            y_weight = 1.0f;
                        }

                        int idx = src_x_min + srcy * rect1_width;

                        for (int srcx = src_x_min; srcx < src_x_max; ++srcx, ++idx) {

                            float x_weight;
                            if (src_x_max - src_x_min <= 1) {
                                x_weight = pixels_per_cell_x;
                            } else if (srcx == src_x_min) {
                                x_weight = (float)(srcx + 1) - src_x;
                            } else if (srcx == src_x_max - 1) {
                                x_weight = src_x_end - (float)srcx;
                            } else {
                                x_weight = 1.0f;
                            }

                            float weight = x_weight * y_weight * inv_area;

                            if (idx < RadarCellHeight * RadarCellWidth) {
                                RGBClass &color = BackgroundColors[idx];
                                accum_r += (float)color.Get_Red() * weight;
                                accum_g += (float)color.Get_Green() * weight;
                                accum_b += (float)color.Get_Blue() * weight;
                            }
                        }
                    }

                    *pixel_data++ = Build_Hicolor_Pixel(RB_MIN(255, (int)(accum_r + 0.5)), RB_MIN(255, (int)(accum_g + 0.5)), RB_MIN(255, (int)(accum_b + 0.5)));
                    src_x = src_x_end;
                }
            }

            src_x = surf_left / zoom_scale;
            src_y = src_y_end;
        }
    }

    unknown_121c->Unlock();

    int pixel_count = (surface_width * surface_height) / 8 + 1;
    if (unknown_1270 == 0) {
        unknown_1270 = new unsigned char[pixel_count];
        memset(unknown_1270, 0, pixel_count);
    }

    Rect result(surf_left, surf_top, surf_right - surf_left, surf_bottom - surf_top);
    return result;
}


// void RadarClass::Fill_In_Background(Rect const &cell_rect, Rect const &update_rect) (0x00631250)
// TibSun's Fill_In_Background: for every map cell inside the update area of
// the radar cell grid, stores its two radar colours (CellClass 0x004712C0)
// into the background colour buffer (+0x1238), one at the left/right edges.

void RadarClass::Fill_In_Background(Rect const &cell_rect, Rect const &update_rect)
{
    RGBClass low(0, 0, 0);
    RGBClass high(0, 0, 0);
    Reset_Iterator();
    CellClass *iter = Map.Iterate();
    int a = 2 * Map.MapLocalSize.X - Map.MapSize.Width + 1;
    while (iter != 0) {
        int x = iter->MapCoords.X;
        int y = iter->MapCoords.Y;
        if (x + y >= unknown_1494) {
            int origx = x - y - a;
            int origy = x + y - unknown_1494;
            Rect mrect(origx, origy, 2, 1);
            mrect = Intersect(mrect, update_rect);
            if (mrect.Is_Valid()) {
                int idx = mrect.X + mrect.Y * cell_rect.Width;
                iter->Unknown_004712c0(&low, &high);
                if (origx == update_rect.X - 1) {
                    BackgroundColors[idx] = high;
                } else if (origx == update_rect.Width + update_rect.X - 1) {
                    BackgroundColors[idx] = low;
                } else {
                    BackgroundColors[idx] = low;
                    BackgroundColors[idx + 1] = high;
                }
            }
        }
        iter = Map.Iterate();
    }
}


// Rect RadarClass::Cell_Radar_Rect(Cell const &cell) (0x00631400)
// A cell's place on the radar's diamond map: two pixels wide (one at the
// left and right edges of the local map), one high.

Rect RadarClass::Cell_Radar_Rect(Cell const &cell)
{
    int cx = cell.X;
    int cy = cell.Y;
    int x = unknown_148c - cy + cx;
    int y = cy - unknown_1494 + cx;
    int width = 2;
    if (x == -1) {
        x = 0;
        width = 1;
    } else if (x == MapLocalSize.Width * 2 - 1) {
        width = 1;
    }
    return Rect(x, y, width, 1);
}


// Rect RadarClass::Cell_To_Radar_Pixel(Cell const &cell) (0x00631470)
// A cell's place on the zoomed radar: the diamond position scaled by the
// zoom at +0x1484 and offset by the radar rectangle's origin (+0x1498); one
// pixel wider at the right edge.

Rect RadarClass::Cell_To_Radar_Pixel(Cell const &cell)
{
    int cx = cell.X;
    int cy = cell.Y;
    int x = (int)((unknown_148c - cy + cx) * unknown_1484 + unknown_1498.X);
    int y = (int)((cy - unknown_1494 + cx) * unknown_1484 + unknown_1498.Y);
    if (x == unknown_1498.X - 1) {
        x++;
    }
    return Rect(x, y, 1, 1);
}


// Cell RadarClass::Radar_Pixel_To_Cell(Point2D const &point) (0x00631500)
// The cell under a radar pixel: the pixel unscaled by the zoom (+0x1484),
// moved back by the diamond offsets and rotated, rounding to nearest.

Cell RadarClass::Radar_Pixel_To_Cell(Point2D const &point)
{
    float x = point.X / unknown_1484 - unknown_148c;
    float y = point.Y / unknown_1484 + unknown_1494;
    Cell cell;
    float sum = y;
    sum += x;
    cell.X = (short)(sum * 0.5f + 0.5);
    cell.Y = (short)((y - x) * 0.5f + 0.5);
    return cell;
}


// void RadarClass::Radar_Background(Cell const &cell) (0x00631570)
// Queues a cell for the radar to redraw (newest first in the search), and
// notes that cells are pending (+0x14D5).

void RadarClass::Radar_Background(Cell const &cell)
{
    for (int index = unknown_1220.Count() - 1; index >= 0; index--) {
        if (unknown_1220[index] == cell) {
            return;
        }
    }
    unknown_1220.Add(cell);
    unknown_14d5 = true;
}


// void RadarClass::Plot_Radar_Background() (0x00631600)
// TibSun's Plot_Radar_Background: recolours the background (+0x1238) of every
// cell waiting at +0x1220 that is inside the local radar, rebuilds the radar
// image over the area it changed (0x00630B70) and keeps the union of those
// areas at +0x1244 (TibSun CellRedrawRect).

void RadarClass::Plot_Radar_Background()
{
    Rect r(0, 0, 0, 0);
    for (int i = unknown_1220.Count() - 1; i >= 0; i--) {
        Cell cell = unknown_1220[i];
        if (Unknown_0055A660(cell, true)) {
            Rect update_rect = Cell_Radar_Rect(cell);
            int x = update_rect.X;
            Rect cellgrid(0, 0, RadarCellWidth, RadarCellHeight);
            update_rect = Intersect(update_rect, cellgrid);
            if (update_rect.Is_Valid()) {
                RGBClass low(0, 0, 0);
                RGBClass high(0, 0, 0);
                CellClass *cptr = &Map[cell];
                int idx = update_rect.X + update_rect.Y * RadarCellWidth;
                cptr->Unknown_004712c0(&low, &high);
                if (x == 0 && update_rect.Width == 1) {
                    BackgroundColors[idx + 1] = high;
                } else if (update_rect.Width == 1) {
                    BackgroundColors[idx] = low;
                } else {
                    BackgroundColors[idx] = low;
                    BackgroundColors[idx + 1] = high;
                }
                Rect radarcellrect(0, 0, RadarCellWidth, RadarCellHeight);
                Rect rr = Compute_Background(radarcellrect, update_rect, false);
                r = Union(r, rr);
            }
        }
    }
    unknown_1220.Clear();
    unknown_1244 = r;
}


// void RadarClass::Radar_Track(TechnoClass *techno, Point2D point) (0x00631910)
// Starts tracking an object at a radar point: a point off the radar surface
// is clamped into it (and kept as the object's +0x1A0) unless the object is
// a building (kind 6), which is not tracked there at all. A new key goes to
// the front of its bucket for the player's objects and to the back for the
// others; then the point is queued for redraw (0x00632620).

void RadarClass::Radar_Track(TechnoClass *techno, Point2D point)
{
    Rect area = unknown_1218->Get_Rect();
    if (!area.Is_Point_Within(point)) {
        if (techno->Kind_Of() == 6) {
            return;
        }
        if (point.X < area.X) {
            point.X = area.X;
        }
        if (point.X >= area.X + area.Width) {
            point.X = area.X + area.Width - 1;
        }
        if (point.Y < area.Y) {
            point.Y = area.Y;
        }
        if (point.Y >= area.Y + area.Height) {
            point.Y = area.Y + area.Height - 1;
        }
        techno->unknown_1a0 = point;
    }
    DynamicVectorClass<HashObject<RadarTrackingStruct, TechnoClass *> > &bucket = unknown_1254->Buckets[Unknown_00631C40(point) % 256];
    HashObject<RadarTrackingStruct, TechnoClass *> object;
    object.Key.unknown_04 = point;
    object.Key.unknown_00 = techno;
    object.Value = techno;
    int count = bucket.Count();
    HashObject<RadarTrackingStruct, TechnoClass *> *objects = &bucket[0];
    for (int index = 0; index < count; index++) {
        if (objects[index] == object) {
            return;
        }
    }
    if (techno->House == unknown_player_a35db4) {
        bucket.Add_Head(object);
    } else {
        bucket.Add(object);
    }
    Radar_Pixel(point);
    unknown_14d5 = true;
}


// void RadarClass::Radar_Untrack(TechnoClass *techno, Point2D point) (0x00631AF0)
// Stops tracking an object at a radar point: removes it from its bucket of
// the tracking table and, when it was there, redraws the point (0x00632620)
// and notes pending work (+0x14D5).

void RadarClass::Radar_Untrack(TechnoClass *techno, Point2D point)
{
    DynamicVectorClass<HashObject<RadarTrackingStruct, TechnoClass *> > &bucket = unknown_1254->Buckets[Unknown_00631C40(point) % 256];
    HashObject<RadarTrackingStruct, TechnoClass *> object;
    object.Key.unknown_04 = point;
    object.Key.unknown_00 = techno;
    object.Value = techno;
    if (bucket.Delete(object)) {
        Radar_Pixel(point);
        unknown_14d5 = true;
    }
}


// Point2D RadarClass::Coord_To_Radar_Pixel(Point2D const &position, bool clip) (0x00631BA0)
// The radar pixel of a lepton position (256 leptons per cell): rotated onto
// the diamond, scaled by the zoom (+0x1484), and kept inside the radar
// rectangle's size when clip is set.

Point2D RadarClass::Coord_To_Radar_Pixel(Point2D const &position, bool clip)
{
    int px = position.X;
    int py = position.Y;
    int x = (int)((unknown_148c * 256 - py + px) * unknown_1484 * (1.0f / 256.0f));
    int y = (int)((py - unknown_1494 * 256 + px) * unknown_1484 * (1.0f / 256.0f));
    if (clip) {
        if (x < 0) {
            x = 0;
        }
        if (x >= unknown_1498.Width) {
            x = unknown_1498.Width - 1;
        }
        if (y < 0) {
            y = 0;
        }
        if (y >= unknown_1498.Height) {
            y = unknown_1498.Height - 1;
        }
    }
    return Point2D(x, y);
}


// unsigned __fastcall Unknown_00631C40(Point2D const &point) (0x00631C40)
// The hash of a radar point: x + y * 251.

unsigned __fastcall Unknown_00631C40(Point2D const &point)
{
    return point.X + point.Y * 251;
}


// int __fastcall Unknown_00631C60(RadarTrackingStruct const &key) (0x00631C60)
// The hash of a tracking key: its point's hash (the tracking table's hash).

int __fastcall Unknown_00631C60(RadarTrackingStruct const &key)
{
    return Unknown_00631C40(key.unknown_04);
}


// void RadarClass::Init_Radar() (0x00631C80)
// Builds a new tracking table (256 buckets, hash 0x00631C60, growth 10).

void RadarClass::Init_Radar()
{
    unknown_1254 = new HashTableClass<RadarTrackingStruct, TechnoClass *>(10, 256, Unknown_00631C60);
}


// void RadarClass::Reset_Radar() (0x00631D40)
// Rebuilds the radar: a fresh tracking table, the radar refitted to the
// map's local area (slot 29), 0x00630A00, and the +0x37B flag of every
// object in the list at 0x00A40C20 cleared.


void RadarClass::Reset_Radar()
{
    delete unknown_1254;
    unknown_1254 = New_Tracking_Table(256, 10);
    RadarClass *map = &Map;
    map->RadarClass::Set_Local_Dimensions(map->MapLocalSize);
    Compute_Radar_Image();
    for (int index = 0; index < unknown_list_a40c20.Count(); index++) {
        unknown_list_a40c20[index]->unknown_37b = false;
    }
}


// void RadarClass::Clear_Radar() (0x00631E40)
// Releases the radar's two surfaces, its buffers and its tracking table.

void RadarClass::Clear_Radar()
{
    if (unknown_1218) {
        delete unknown_1218;
        unknown_1218 = 0;
    }
    if (unknown_121c) {
        delete unknown_121c;
        unknown_121c = 0;
    }
    if (BackgroundColors) {
        delete[] BackgroundColors;
        BackgroundColors = 0;
    }
    if (unknown_1254) {
        delete unknown_1254;
        unknown_1254 = 0;
    }
    if (unknown_1270) {
        delete[] unknown_1270;
        unknown_1270 = 0;
    }
}


// void RadarClass::Post_Load_Radar_Fixup() (0x00631ED0)
// Builds the radar after a load: forgets the old surfaces, buffers and
// table (their pointers are stale), makes a new tracking table, refits the
// radar to the map (slot 29) and its surfaces (0x00630A00), clears the
// +0x37B flag of the listed objects, and finishes a pending switch from
// mode 3 (state 5, then the kept mode).

void RadarClass::Post_Load_Radar_Fixup()
{
    unknown_1218 = 0;
    unknown_121c = 0;
    BackgroundColors = 0;
    unknown_1254 = 0;
    unknown_1270 = 0;
    unknown_1254 = New_Tracking_Table(256, 10);
    RadarClass *map = &Map;
    map->RadarClass::Set_Local_Dimensions(map->MapLocalSize);
    Compute_Radar_Image();
    for (int index = 0; index < unknown_list_a40c20.Count(); index++) {
        unknown_list_a40c20[index]->unknown_37b = false;
    }
    if (unknown_14ac == 3) {
        unknown_14a8 = 5;
        Radar_Activate(unknown_14b0, false);
        unknown_set_frame_rate_0053d820(Rules->DetailMinFrameRateNormal);
    }
}


extern MouseClass Map;
ColorScheme *__fastcall Fetch_Scheme_By_Name(char const *name,int kind);

void RadarClass::Plot_Radar_Pixel(Point2D const &point)
{
    if(!unknown_player_a35db4) return;
    if(!unknown_1218->Get_Rect().Is_Point_Within(point)) return;
    Cell cell=Radar_Pixel_To_Cell(point);
    CoordStruct coord(cell.X*256+128,cell.Y*256+128,0);
    coord.Z=Map.Get_Height_GL(coord);
    bool shadow=unknown_b263d8 && Map.Is_Shrouded(coord);
    bool dim=unknown_b263d8 && Map.Is_Under_Fog(coord);
    TechnoClass *tech=0;
    DynamicVectorClass<HashObject<RadarTrackingStruct,TechnoClass *> > &bucket=unknown_1254->Buckets[Unknown_00631C40(point)&255];
    int count=bucket.Count();
    HashObject<RadarTrackingStruct,TechnoClass *> *items=&bucket[0];
    for(int index=0;index<count;++index) {
        TechnoClass *candidate=items[index].Key.unknown_00;
        if(items[index].Key.unknown_04==point &&
            (!shadow || candidate->House->Is_Player_Control()) &&
            (!candidate->Techno_Type_Class()->unknown_1ff || unknown_player_a35db4->Is_Ally(candidate))) {
            tech=candidate;
            break;
        }
    }
    if(tech) {
        HouseClass *owner=tech->House;
        if(tech->unknown_slot48()) owner=tech->unknown_slot51(false);
        int color;
        if(owner) color=Build_Hicolor_Pixel(owner->unknown_55e9.Red,owner->unknown_55e9.Green,owner->unknown_55e9.Blue);
        else color=Fetch_Scheme_By_Name("LightGrey",1)->unknown_color_0x330;
        int remaining=tech->unknown_13c.Value();
        if(remaining>0 && ((remaining-1)/Rules->FlashFrameTime)%2==1 && tech->House==unknown_player_a35db4)
            unknown_1218->Put_Pixel(point,~color);
        else unknown_1218->Put_Pixel(point,color);
    } else {
        if(dim) {
            RGBClass color=Unpack_Hicolor_Pixel((unsigned short)unknown_121c->Get_Pixel(point));
            unknown_1218->Put_Pixel(point,Build_Hicolor_Pixel(color.Red>>1,color.Green>>1,color.Blue>>1));
        } else {
            if(!shadow) unknown_1218->Put_Pixel(point,unknown_121c->Get_Pixel(point));
            else unknown_1218->Put_Pixel(point,0);
        }
    }
    unknown_1208=Union(unknown_1208,Rect(point.X,point.Y,1,1)+Point2D(unknown_1498.X,unknown_1498.Y));
}

// External references verified against complete original body operands.


// void RadarClass::Render_Tracked_Objects() (0x006324A0)
// TibSun's Render_Tracked_Objects: clears the pixel flags, then plots each
// tracked object once per pixel in its house's bright colour (slot 48 objects
// in the colour of the slot 51 house, LightGrey without one).

void RadarClass::Render_Tracked_Objects()
{
    memset(unknown_1270, 0, unknown_1218->Get_Width() * unknown_1218->Get_Height() / 8 + 1);
    for (int b = 0; b < 256; b++) {
        DynamicVectorClass<HashObject<RadarTrackingStruct, TechnoClass *> > &bucket = unknown_1254->Buckets[b];
        int count = bucket.Count();
        HashObject<RadarTrackingStruct, TechnoClass *> *ptr = &bucket[0];
        for (int t = 0; t < count; t++) {
            RadarTrackingStruct *track = &ptr[t].Key;
            TechnoClass *tech = track->unknown_00;
            int id = track->unknown_04.X + track->unknown_04.Y * unknown_1218->Get_Width();
            int i = id >> 3;
            int bit = 1 << (id & 7);
            if ((unknown_1270[i] & bit) == 0) {
                unknown_1270[i] |= bit;
                ColorScheme *scheme = ColorSchemes[tech->House->ColorSchemeIndex];
                if (tech->unknown_slot48()) {
                    if (tech->unknown_slot51(false) != 0) {
                        scheme = ColorSchemes[tech->unknown_slot51(false)->ColorSchemeIndex];
                    } else {
                        scheme = Fetch_Scheme_By_Name("LightGrey", 1);
                    }
                }
                int color = scheme->Converter->Convert(scheme->unknown_color_0x330);
                unknown_1218->Put_Pixel(track->unknown_04, color);
            }
        }
    }
}


// void RadarClass::Radar_Pixel(Point2D const &point) (0x00632620)
// TibSun's Radar_Pixel: marks a radar pixel inside the radar surface (+0x1218)
// once in the pixel bit flags (+0x1270), queues it (+0x1258) and notes pending
// work (+0x14D5).

void RadarClass::Radar_Pixel(Point2D const &point)
{
    if (point.X >= 0 && point.X < unknown_1218->Get_Width() && point.Y >= 0 && point.Y < unknown_1218->Get_Height()) {
        int id = point.X + point.Y * unknown_1218->Get_Width();
        int i = id >> 3;
        int bit = 1 << (id & 7);
        if ((unknown_1270[i] & bit) == 0) {
            unknown_1270[i] |= bit;
            unknown_1258.Add(point);
            unknown_14d5 = true;
        }
    }
}


// void RadarClass::Compute_Foundations() (0x00632700)
// Builds the 22 dot shapes at +0x1274: shape i is a diamond of cells a by b
// in size, each side its scale (0x007CFC70 / 0x007CFCC8) times the zoom
// +0x1484, rounded, at least 1 for scale 1 and 2 otherwise.
extern int unknown_dot_width_7cfc70[22];
extern int unknown_dot_height_7cfcc8[22];

void RadarClass::Compute_Foundations()
{
    for (int index = 0; index < 22; index++) {
        DynamicVectorClass<Point2D> &dots = unknown_1274[index];
        dots.Clear();
        int scale_a = unknown_dot_width_7cfc70[index];
        int scale_b = unknown_dot_height_7cfcc8[index];
        int a;
        if (scale_a == 1) {
            double size = unknown_1484 + 0.5;
            a = (int)(1.0 > size ? 1.0 : size);
        } else {
            double size = scale_a * unknown_1484 + 0.5;
            a = (int)(2.0 > size ? 2.0 : size);
        }
        int b;
        if (scale_b == 1) {
            double size = unknown_1484 + 0.5;
            b = (int)(1.0 > size ? 1.0 : size);
        } else {
            double size = scale_b * unknown_1484 + 0.5;
            b = (int)(2.0 > size ? 2.0 : size);
        }
        int rows = b + a - 1;
        for (int y = 0; y < rows; y++) {
            int start = (y < b) ? -y : y - b * 2 + 2;
            int end = (y < a) ? y : a * 2 - y - 2;
            for (int x = start; x <= end; x++) {
                dots.Add(Point2D(x, y));
            }
        }
    }
}


class Leaf_6328d0 { public: unsigned int get(unsigned int type_address); };
unsigned int Leaf_6328d0::get(unsigned int type_address) { unsigned int radar_address = (unsigned int)this; unsigned int shape_or_address; unsigned int scaled_index; shape_or_address = type_address; shape_or_address = *(unsigned int*)(shape_or_address + 0xc78); scaled_index = shape_or_address + shape_or_address*2; shape_or_address = radar_address + scaled_index*8 + 0x1274; return shape_or_address; }


// void RadarClass::Radar_Cell(Cell const &cell) (0x006328F0)
// TibSun's Radar_Cell: the cell's diamond rectangle (0x00631400, expanded),
// clipped to the radar cell size (+0x1238), scaled by the zoom (+0x1484) with
// a one-pixel margin, every pixel marked through 0x00632620.

void RadarClass::Radar_Cell(Cell const &cell)
{
    if (unknown_1218 != 0) {
        Rect clipped_cell_radar_rect = Cell_Radar_Rect(cell);
        clipped_cell_radar_rect = Intersect(clipped_cell_radar_rect, Rect(0, 0, RadarCellWidth, RadarCellHeight));
        int swidth = unknown_1218->Get_Width();
        int sheight = unknown_1218->Get_Height();
        if (clipped_cell_radar_rect.Is_Valid()) {
            int zx = (int)(clipped_cell_radar_rect.X * unknown_1484);
            int startx = (zx - 1 < 0) ? 0 : zx - 1;
            int ex = (int)((double)(clipped_cell_radar_rect.X + clipped_cell_radar_rect.Width) * unknown_1484) + 1;
            int width = (ex < swidth) ? ex : swidth;
            int zy = (int)(clipped_cell_radar_rect.Y * unknown_1484);
            int starty = (zy - 1 < 0) ? 0 : zy - 1;
            int ey = (int)((double)(clipped_cell_radar_rect.Y + clipped_cell_radar_rect.Height) * unknown_1484) + 1;
            int height = (ey < sheight) ? ey : sheight;
            for (int y = starty; y < height; y++) {
                for (int x = startx; x < width; x++) {
                    Radar_Pixel(Point2D(x, y));
                }
            }
        }
    }
}


// Resolve the tracked object at a radar pixel, or the underlying terrain cell.

void RadarClass::Resolve_Radar_Point(Point2D const &pixel, Cell &cell, TechnoClass *&found)
{
    Point2D origin = *reinterpret_cast<Point2D const *>(&unknown_1498);
    Point2D point = pixel - origin;
    DynamicVectorClass<HashObject<RadarTrackingStruct, TechnoClass *> > &bucket = unknown_1254->Buckets[(point.X + point.Y * 251) & 255];
    int count = bucket.Count();
    found = 0;
    for (int index = count - 1; index >= 0; --index) {
        RadarTrackingStruct track = bucket[index].Key;
        if (track.unknown_04 == point) {
            found = track.unknown_00;
            break;
        }
    }
    if (found != 0) {
        cell = Cell(found->unknown_slot19(0));
    } else {
        cell = Radar_Pixel_To_Cell(point);
    }
}


// HRESULT RadarClass::Load(IStream *stream) (0x00632BF0, slot 30)
// Loads the radar after the display: rebuilds its list (+0x14BC) and its
// vectors in place, reads back the queued points and cells, empties the 22
// point vectors, reads the mode values, and resets the list again.

HRESULT RadarClass::Load(IStream *stream)
{
    HRESULT result = DisplayClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    int count;
    result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    unknown_14bc.Unknown_00405BF0();
    new (&unknown_1258) DynamicVectorClass<Point2D>;
    for (int index = 0; index < count; index++) {
        Point2D point;
        result = stream->Read(&point, sizeof(point), 0);
        if (FAILED(result)) {
            return result;
        }
        unknown_1258.Add(point);
    }
    result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    new (&unknown_1220) DynamicVectorClass<Cell>;
    for (index = 0; index < count; index++) {
        Cell cell;
        result = stream->Read(&cell, sizeof(cell), 0);
        if (FAILED(result)) {
            return result;
        }
        unknown_1220.Add(cell);
    }
    for (index = 0; index < 22; index++) {
        new (&unknown_1274[index]) DynamicVectorClass<Point2D>;
    }
    result = stream->Read(&unknown_14ac, sizeof(unknown_14ac), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Read(&unknown_14a8, sizeof(unknown_14a8), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Read(&unknown_14b0, sizeof(unknown_14b0), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Read(&unknown_14b4, sizeof(unknown_14b4), 0);
    if (FAILED(result)) {
        return result;
    }
    unknown_14bc.Unknown_00405BF0();
    return S_OK;
}


// HRESULT RadarClass::Save(IStream *stream) (0x00632E10, slot 31)
// Saves the radar after the display: the queued points (+0x1258) and cells
// (+0x1220), each with its count, then the mode values at +0x14AC, +0x14A8,
// +0x14B0 and +0x14B4.

HRESULT RadarClass::Save(IStream *stream)
{
    HRESULT result = DisplayClass::Save(stream);
    if (FAILED(result)) {
        return result;
    }
    int count = unknown_1258.Count();
    result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int index = 0; index < count; index++) {
        result = stream->Write(&unknown_1258[index], sizeof(Point2D), 0);
        if (FAILED(result)) {
            return result;
        }
    }
    count = unknown_1220.Count();
    result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (index = 0; index < count; index++) {
        result = stream->Write(&unknown_1220[index], sizeof(Cell), 0);
        if (FAILED(result)) {
            return result;
        }
    }
    result = stream->Write(&unknown_14ac, sizeof(unknown_14ac), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(&unknown_14a8, sizeof(unknown_14a8), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(&unknown_14b0, sizeof(unknown_14b0), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(&unknown_14b4, sizeof(unknown_14b4), 0);
    return FAILED(result) ? result : S_OK;
}


// void RadarClass::Set_Radar_State(bool on, bool sound) (0x00632F30)
// Switches the radar on (state 3, from off or deactivated) or off (state 2),
// clearing its list at +0x14BC, with the rules' activate/deactivate sound.

// Not expanded into its callers in the original (kept out of line).
#pragma auto_inline(off)
void RadarClass::Set_Radar_State(bool on, bool sound)
{
    if (on == true) {
        if (unknown_14a8 == 0 || unknown_14a8 == 2) {
            unknown_14a8 = 3;
            unknown_14bc.Unknown_00406070();
            if (sound) {
                Voc_Play(Rules->unknown_610, 1.0f, 0x2000, 0);
            }
            Debug_Printf("Radar: ACTIVATING\n");
        }
    } else {
        unknown_14bc.Unknown_00406070();
        unknown_14a8 = 2;
        if (sound) {
            Voc_Play(Rules->unknown_614, 1.0f, 0x2000, 0);
        }
        Debug_Printf("Radar: DEACTIVING\n");
    }
}
#pragma auto_inline(on)


// 0x00632ff0: function not yet identified
//
// Body: 15 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: bool __fastcall FUN_00632ff0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00632ff0 {

bool __fastcall FUN_00632ff0(int radar_address)
{
    return *(int *)(radar_address + 0x14a8) == 1;
}

}  // namespace seed_00632ff0


// void RadarClass::Radar_Activate(int mode, bool sound) (0x00633000)
// TibSun's Radar_Activate: switches the radar mode (+0x14AC); an observer in a
// multiplayer game always gets mode 1; a running movie keeps the request
// (+0x14B0); modes 1 and above queue the next movie (0x0062F150, expanded)
// when the radar is up, otherwise turn it on or off (0x00632F30).
void RadarClass::Radar_Activate(int mode, bool sound)
{
    if (unknown_game_mode_a3d298 && unknown_player_a35db4->unknown_13d && unknown_int_a3d548) {
        mode = 1;
    }
    if (unknown_14ac == mode) {
        return;
    }
    if ((unknown_14ac == 3 || (unknown_14ac == 4 && mode != 3)) && unknown_14a8 != 5) {
        unknown_14b0 = mode;
        return;
    }
    switch (mode) {
    case 0:
        Set_Radar_State(false, unknown_14ac == 1);
        break;
    case 1:
        if ((unknown_14a8 == 1 || unknown_14a8 == 5) && unknown_14d4 == true) {
            Queue_Next_Movie();
            break;
        }
        Set_Radar_State(unknown_14d4, sound);
        break;
    case 4:
        break;
    default:
        if (unknown_14a8 == 1 || unknown_14a8 == 5) {
            Queue_Next_Movie();
            break;
        }
        Set_Radar_State(true, false);
        break;
    }
    unknown_14ac = mode;
}


// 0x00633110: function not yet identified
//
// Body: 26 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall FUN_00633110(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00633110 {

unsigned int __fastcall FUN_00633110(int radar_address)
{
    unsigned int result;

    result = (unsigned int)(1);
    if ((*(int *)(radar_address + 0x14ac) != 1) || (*(int *)(radar_address + 0x14a8) != 1)) {
        result = (unsigned int)(0);
    }
    return result;
}

}  // namespace seed_00633110


// RadarClass::Is_Radar_Existing (0x00633130): the player has radar (TibSun RadarClass::Is_Radar_Existing).

bool RadarClass::Is_Radar_Existing()
{
    return unknown_14d4;
}


// RadarClass::Toggle_Radar (0x00633140): gains or loses radar (TibSun RadarClass::Toggle_Radar).

void RadarClass::Toggle_Radar(bool on)
{
    if (unknown_14d4 != on) {
        unknown_14d4 = on;
        char const *txt = on ? "on" : "off";
        Debug_Printf("Radar: TacticalMap availability is %s\n", txt);
        if (unknown_14ac == 1) {
            Set_Radar_State(on, true);
        } else {
            Radar_Activate(1, true);
        }
    }
}


// RadarClass::Is_Player_Names (0x006331A0): the radar shows the player names (TibSun RadarClass::Is_Player_Names).

bool RadarClass::Is_Player_Names()
{
    return unknown_14ac == 2 && unknown_14a8 == 1;
}


// 0x006331c0: function not yet identified
//
// Body: 27 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall FUN_006331c0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006331c0 {

unsigned int __fastcall FUN_006331c0(int radar_address)
{
    unsigned int result;

    if ((*(int *)(radar_address + 0x14ac) != 3) || (result = 1, *(int *)(radar_address + 0x14a8) != 1)) {
        result = (unsigned int)(0);
    }
    return result;
}

}  // namespace seed_006331c0


// void RadarClass::Redraw_Radar(bool flag) (0x006331E0)
// Asks for a radar redraw (+0x14D5), a complete one (+0x14D6) when flag is set.

void RadarClass::Redraw_Radar(bool flag)
{
    unknown_14d5 = true;
    unknown_14d6 = unknown_14d6 || flag;
}


// void RadarClass::Render_Radar() (0x00633210): TibSun Render_Radar. Redraws the radar view,
// its old outline, pending cells and events, then copies the dirty area to the sidebar surface.
bool RadarEventClass_unknown_none();
void RadarClass::Render_Radar()
{
    int edge;
    Rect previous = unknown_1208;
    bool tactical = unknown_14ac == 1 && unknown_14a8 == 1;
    Cell center = Map[unknown_tactical_83996c->Pixel_To_Cell(Point2D(
        unknown_rect_839630.X + unknown_rect_839630.Width / 2,
        unknown_rect_839630.Y + unknown_rect_839630.Height / 2))].Fetch_CellID();
    unknown_14d8 = Cell_To_Radar_Pixel(center);
    unknown_14d8.Width = (2 * unknown_rect_839630.Width) / (60.0f / unknown_1484) + 1.0f;
    unknown_14d8.Height = (2 * unknown_rect_839630.Height) / (30.0f / unknown_1484);
    unknown_14d8 -= Point2D(unknown_rect_839630.Width / (60.0f / unknown_1484),
        (2 * unknown_rect_839630.Height) / (30.0f / unknown_1484) * 0.5f);
    if (unknown_14d8.X < unknown_1498.X) unknown_14d8.X = unknown_1498.X;
    else if (unknown_14d8.X + unknown_14d8.Width >= unknown_1498.X + unknown_1498.Width)
        unknown_14d8.X = unknown_1498.X - (unknown_14d8.Width - unknown_1498.Width) - 1;
    if (unknown_14d8.Y < unknown_1498.Y) unknown_14d8.Y = unknown_1498.Y;
    else if (unknown_14d8.Y + unknown_14d8.Height >= unknown_1498.Y + unknown_1498.Height)
        unknown_14d8.Y = unknown_1498.Y - (unknown_14d8.Height - unknown_1498.Height) - 1;
    if (unknown_14d6) unknown_14e8 = unknown_14d8;
    if (unknown_14d5 || unknown_14e8 != unknown_14d8 || !RadarEventClass_unknown_none()
        || unknown_1258.Count() > 0 || unknown_1220.Count() > 0 || unknown_object_84e8d0.Is_Flashing_0042efd0()) {
        if (tactical) unknown_14d5 = false;
        Plot_Radar_Background();
        bool moved = false;
        if (unknown_14e8 != unknown_14d8) {
            Point2D pixel;
            for (edge = 0; edge < unknown_14e8.Height; ++edge) {
                pixel = Point2D(unknown_14e8.X, edge + unknown_14e8.Y) - Point2D(unknown_1498.X, unknown_1498.Y);
                Radar_Pixel(pixel);
                pixel = Point2D(unknown_14e8.X + unknown_14e8.Width - 1, edge + unknown_14e8.Y) - Point2D(unknown_1498.X, unknown_1498.Y);
                Radar_Pixel(pixel);
            }
            for (edge = 0; edge < unknown_14e8.Width; ++edge) {
                pixel = Point2D(edge + unknown_14e8.X, unknown_14e8.Y) - Point2D(unknown_1498.X, unknown_1498.Y);
                Radar_Pixel(pixel);
                pixel = Point2D(edge + unknown_14e8.X, unknown_14e8.Height + unknown_14e8.Y - 1) - Point2D(unknown_1498.X, unknown_1498.Y);
                Radar_Pixel(pixel);
            }
            moved = true;
        }
        if (unknown_1244.Is_Valid()) {
            unknown_1218->Copy_From(unknown_1244, *unknown_121c, unknown_1244, false, true);
            unknown_1208 = Union(unknown_1208, unknown_1244 + unknown_1498.Top_Left());
            for (int x = unknown_1244.X; x < unknown_1244.X + unknown_1244.Width; ++x) {
                for (int y = unknown_1244.Y; y < unknown_1244.Y + unknown_1244.Height; ++y) Plot_Radar_Pixel(Point2D(x, y));
            }
            unknown_1244 = Rect(0, 0, 0, 0);
        }
        for (edge = unknown_1258.Count() - 1; edge >= 0; --edge) Plot_Radar_Pixel(unknown_1258[edge]);
        unknown_1258.Clear();
        RadarEventClass::Draw_Events();
        unknown_object_84e8d0.Draw_On_Radar_0042eed0(unknown_1218, unknown_1218->Get_Rect());
        if (tactical) {
            if (unknown_14d6) {
                unknown_1208 = unknown_1498;
                CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shape_ab64a0, 32,
                    Point2D(unknown_11e0, unknown_11e8), unknown_surface_839948->Get_Rect(), 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
            if (unknown_1208.Is_Valid()) {
                Rect from = unknown_1208 - RectPoint2D(unknown_1498.X, unknown_1498.Y);
                unknown_surface_839948->Copy_From(unknown_1208, *unknown_1218, from, false, true);
            }
            unknown_surface_839948->Draw_Rect(unknown_14d8, unknown_1204);
            Rect area = unknown_1498;
            Rect border(area.X - 1, area.Y - 1, area.Width + 2, area.Height + 2);
            unknown_surface_839948->Draw_Rect(border, unknown_1204);
        }
        if (moved) {
            Rect const &inner = Union(unknown_14e8, unknown_1208);
            unknown_1208 = Union(unknown_14d8, inner);
        }
        unknown_14e8 = unknown_14d8;
        RadarEventClass::Remove_Finished();
    }
    if (unknown_1270) memset(unknown_1270, 0, unknown_1218->Get_Height() * unknown_1218->Get_Width() / 8 + 1);
    if (!tactical) unknown_1208 = previous;
}


// void RadarClass::Update_Transmission() (0x00633C40)
// The mode-4 (transmission) update: state +0x14B4 0 starts the voice
// sequence, 1 waits for it (fading the audio streams while 0x00AF8FE8 is
// set), 2 waits for the fade, then switches to mode 3 and plays the sound.

void RadarClass::Update_Transmission()
{
    switch (unknown_14b4) {
    case 0:
        Speech_Clear();
        Speech_Play(unknown_14d0, 0, 3);
        Unknown_00716940();
        unknown_14b4 = 1;
        break;
    case 1:
        if (Unknown_00716690(unknown_14d0)) {
            if (unknown_int_af8fe8 != 0) {
                Unknown_004082B0();
            }
            unknown_14b4 = 2;
        }
        break;
    case 2:
        if (unknown_int_af8fe8 != 0) {
            if (Unknown_00408330() == 0) {
                return;
            }
            if (unknown_int_af8fe8 != 0) {
                Unknown_00716950();
                Unknown_005A0B10();
            }
        }
        Radar_Activate(3, false);
        unknown_14bc.Unknown_00406070();
        unknown_14bc.Unknown_00406100(Voc_Play(Rules->unknown_618, 1.0f, 0x2000, 0), 0);
        unknown_set_frame_rate_0053d820(Rules->DetailMinFrameRateMovie);
        break;
    }
}


// void RadarClass::Play_Movie() (0x00633D30)
// The mode-3 (movie) update: redraws the frame when a full radar redraw is
// due, steps the first queued movie and, when it is finished, deletes it and
// starts the next one; with none left the radar returns to its previous mode
// (+0x14B0). Follows the movie's flag at +0x04 through +0x14B8.

void RadarClass::Play_Movie()
{
    MovieHandle *movie = 0;
    bool flag = false;
    bool playing = queued_movies_00af8fd8.Count() > 0;
    if (!unknown_14b9 && playing) {
        Speech_Is_Playing();
    }
    if (unknown_14d6 == true && !unknown_14b9) {
        CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shape_ab64a0, 32,
                      Point2D(unknown_11e0, unknown_11e8), unknown_surface_839948->Get_Rect(), 0, 0, 0, 0, 1000, 0, 0,
                      Point2D(0, 0));
        unknown_1208 = Rect(unknown_11e0, unknown_11e8, unknown_int_7add80, 110);
        Debug_Printf("Radar: Movie full redrawn\n");
    }
    bool done;
    if (playing && queued_movies_00af8fd8.Count() > 0 && (movie = queued_movies_00af8fd8[0]) != 0) {
        if (movie->AdvanceFrame() == true) {
            unknown_1208 = Rect(unknown_11e0, unknown_11e8, unknown_int_7add80, 110);
        }
        done = movie->Is_Finished();
        flag = movie->unknown_04;
    } else if (playing) {
        done = true;
    } else {
        done = false;
    }
    if (done == true) {
        if (movie != 0) {
            queued_movies_00af8fd8.Delete(movie);
            delete movie;
        }
        playing = queued_movies_00af8fd8.Count() != 0;
        if (playing) {
            Debug_Printf("Radar: Next movie.\n");
            unknown_14bc.Unknown_00406070();
            unknown_14bc.Unknown_00406100(Voc_Play(Rules->unknown_618, 1.0f, 0x2000, 0), 0);
            unknown_14a8 = 4;
            unknown_14f8 = 25;
        }
    }
    unknown_14d5 = false;
    if (!playing) {
        Debug_Printf("Radar: Movie done.\n");
        unknown_14a8 = 5;
        Radar_Activate(unknown_14b0, false);
        Unknown_005A0AC0();
        flag = playing;
        unknown_14bc.Unknown_00406070();
        unknown_14bc.Unknown_00406100(Voc_Play(Rules->MovieOff, 1.0f, 0x2000, 0), 0);
        unknown_set_frame_rate_0053d820(Rules->DetailMinFrameRateNormal);
        unknown_14d5 = true;
    }
    unknown_14b9 = !done;
    if (unknown_14b8 != flag) {
        if (flag) {
            Unknown_0065C1F0(0);
            unknown_14b8 = true;
        } else {
            Unknown_0065C310();
            unknown_14b8 = false;
        }
    }
}


// void RadarClass::Complete_Radar_Refresh() (0x00634030)
// Redraws the whole radar: with no window, copies the surface at +0x121C
// onto the one at +0x1218 and finishes with 0x006324A0; otherwise draws
// every point of the radar surface.

void RadarClass::Complete_Radar_Refresh()
{
    if (!MainWindow) {
        unknown_1218->Copy_From(unknown_1218->Get_Rect(), *unknown_121c, unknown_121c->Get_Rect(), false, true);
        Render_Tracked_Objects();
    } else {
        int width = unknown_1218->Get_Width();
        int height = unknown_1218->Get_Height();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Plot_Radar_Pixel(Point2D(x, y));
            }
        }
    }
}


// int __stdcall Unknown_006340F0(HWND window, UINT message, WPARAM wparam, LPARAM lparam) (0x006340F0)
// The in-game diplomacy dialog (0x73, opened by 0x00634A00): 0x497 fills a row per
// house 1-7 (alliance check box, chat check box, name and side labels), hides the
// rows of absent or non-multiplayer houses and the observer flag, then starts a
// 250 ms timer; each timer tick refreshes the rows through 0x00634970. The alliance
// boxes queue an alliance event, the chat boxes set unknown_flags_a3f118 and 0x730
// the game mode's team chat flag; 0x423 closes the dialog.

void __fastcall unknown_send_byte_message_005e1fb0(HWND window, unsigned char value);   // 0x005E1FB0
void __fastcall unknown_5e1fd0(HWND control, unsigned char value);   // 0x005E1FD0
void __fastcall Unknown_006F62D0(unsigned long argument);

extern int Diplomacy_Name_Controls[8];   // 0x007EE06C 0x411-0x418
extern int Diplomacy_Side_Controls[8];   // 0x007EE08C 0x419-0x420
extern int Diplomacy_Ally_Controls[8];   // 0x007EE0AC -1, 0x6CF-0x6D5
extern int Diplomacy_Chat_Controls[8];   // 0x007EE0CC -1, 0x6D6-0x6D9, 0x6E2-0x6E4

static inline HWND Get_Item(HWND window, int id)
{
    return GetDlgItem(window, id);
}

static inline LRESULT Send_Dlg_Item_Message(HWND dialog, int id, UINT message, WPARAM wparam, LPARAM lparam)
{
    HWND control = Get_Item(dialog, id);
    return SendMessageA(control, message, wparam, lparam);
}

static inline BOOL Show_Window(HWND window, int show)
{
    return ShowWindow(window, show);
}

static inline bool Is_Control(int const *table, int id)
{
    for (int index = 0; index < 8; index++) {
        if (table[index] == id) {
            return true;
        }
    }
    return false;
}

static inline int Control_Index(int const *table, int id)
{
    for (int index = 0; index < 8; index++) {
        if (table[index] == id) {
            return index;
        }
    }
    return -1;
}

int __stdcall Unknown_006340F0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int handled = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (handled != 0) {
        return handled;
    }
    switch (message) {
        case 0x497: {
            int index;
            for (index = 0; index < 8; index++) {
                Send_Dlg_Item_Message(window, Diplomacy_Name_Controls[index], 0x4B2, 0, (LPARAM)L"");
                Send_Dlg_Item_Message(window, Diplomacy_Side_Controls[index], 0x4B2, 0, (LPARAM)L"");
            }
            bool any_ally = false;
            bool any_chat = false;
            int count = Houses.Count();
            for (index = 1; index < 8; index++) {
                HWND ally = Get_Item(window, Diplomacy_Ally_Controls[index]);
                HWND chat = Get_Item(window, Diplomacy_Chat_Controls[index]);
                int house_index = index;
                if (index == unknown_player_a35db4->ArrayIndex) {
                    house_index = 0;
                }
                HouseClass *house;
                if (house_index < count && (house = Houses[house_index]) != 0 && house->Type->Multiplay) {
                    if (Rules->unknown_111d && house->Unknown_004F52F0()) {
                        if (unknown_house_a72d28 != unknown_player_a35db4) {
                        unknown_send_byte_message_005e1fb0(ally, 1);
                        bool allied = unknown_player_a35db4->Is_Ally(house);
                        SendMessageA(ally, 0xF1, allied, 0);
                        unknown_5e1fd0(ally, house->Is_Ally(unknown_player_a35db4));
                        any_ally = true;
                        if (unknown_int_a3d548) {
                            EnableWindow(ally, 0);
                        }
                        } else {
                            ShowWindow(ally, 0);
                        }
                    } else {
                        ShowWindow(ally, 0);
                    }
                    if (!house->Unknown_004F52F0()) {
                        ShowWindow(chat, 0);
                    } else {
                        if (!unknown_int_a3d548) {
                            SendMessageA(chat, 0xF1, unknown_flags_a3f118[house_index], 0);
                        } else {
                            SendMessageA(chat, 0xF1, 1, 0);
                            EnableWindow(chat, 0);
                        }
                        any_chat = true;
                    }
                } else {
                    HWND flag = Get_Item(window, unknown_player_control_4d0ee0(index));
                    EnableWindow(flag, 0);
                    ShowWindow(flag, 0);
                    HWND name = Get_Item(window, Diplomacy_Name_Controls[index]);
                    EnableWindow(name, 0);
                    ShowWindow(name, 0);
                    EnableWindow(ally, 0);
                    ShowWindow(ally, 0);
                    EnableWindow(chat, 0);
                    ShowWindow(chat, 0);
                    HWND side = Get_Item(window, Diplomacy_Side_Controls[index]);
                    EnableWindow(side, 0);
                    ShowWindow(side, 0);
                }
            }
            if (!any_ally) {
                Show_Window(Get_Item(window, 0x597), 0);
            }
            if (!any_chat) {
                Show_Window(Get_Item(window, 0x425), 0);
            }
            HWND team = Get_Item(window, 0x730);
            if (team) {
                if (unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) {
                    Show_Window(team, 5);
                    if (unknown_game_mode_a3d298 == 3) {
                        SendMessageA(team, 0xF1, unknown_session_a3d298.LANTaunts, 0);
                    } else if (unknown_game_mode_a3d298 == 4) {
                        SendMessageA(team, 0xF1, unknown_session_a3d298.WOLTaunts, 0);
                    }
                } else {
                    Show_Window(team, 0);
                }
            }
            Map.Unknown_00634970((int)window);
            if (unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) {
                HWND mode = Get_Item(window, 0x426);
                SendMessageA(mode, 0x4B2, 0, (LPARAM)unknown_session_a3d298.Mode->Get_Name().unknown_text_770430());
            }
            HWND map_name = Get_Item(window, 0x71B);
            SendMessageA(map_name, 0x4B2, 0, (LPARAM)unknown_map_name_a3d342);
            SetTimer(window, 0, 250, 0);
            return 0;
        }
        case 0x113:
            Map.Unknown_00634970((int)window);
            InvalidateRect(window, 0, 1);
            return 0;
        case 0x111: {
            int id = GetDlgCtrlID((HWND)lparam);
            if (id == 0x423) {
                unsigned short code = wparam >> 16;
                if (code == 0) {
                    KillTimer(window, 0);
                    *(int *)GetWindowLongA(window, 8) = 0x423;
                    return 0;
                }
            } else if (Is_Control(Diplomacy_Ally_Controls, id)) {
                unsigned int code = wparam >> 16;
                if (code == 1 || code == 0) {
                    int house_index = Control_Index(Diplomacy_Ally_Controls, id);
                    if (house_index == unknown_player_a35db4->ArrayIndex) {
                        house_index = 0;
                    }
                    if (code == 0) {
                        SendMessageA((HWND)lparam, 0xF1, 1, 0);
                    } else if (code == 1) {
                        SendMessageA((HWND)lparam, 0xF1, 0, 0);
                    }
                    Unknown_006F62D0(house_index);
                    return 0;
                }
            } else if (Is_Control(Diplomacy_Chat_Controls, id)) {
                int value = (unsigned short)(wparam >> 16);
                if (value == 1 || value == 0) {
                    bool checked = (unsigned short)(wparam >> 16) == 1;
                    int house_index = Control_Index(Diplomacy_Chat_Controls, id);
                    if (house_index == unknown_player_a35db4->ArrayIndex) {
                        house_index = 0;
                    }
                    unknown_flags_a3f118[house_index] = checked;
                    return 0;
                }
            } else if (id == 0x730) {
                unsigned int code = wparam >> 16;
                if (code == 1 || code == 0) {
                    if (unknown_game_mode_a3d298 == 3) {
                        unknown_session_a3d298.LANTaunts = code == 1;
                        return 0;
                    }
                    if (unknown_game_mode_a3d298 == 4) {
                        unknown_session_a3d298.WOLTaunts = code == 1;
                    }
                }
            }
            break;
        }
    }
    return 0;
}


// void RadarClass::Unknown_00634970(int arg1) (0x00634970)
// Runs 0x00634670 for every house that answers 0x004F52B0 or is not a
// passive multiplayer house, then once more for the last house that
// answered (as index 0) and for the first house (at that index).

void RadarClass::Unknown_00634970(int arg1)
{
    int last = -1;
    int count = Houses.Count();
    for (int index = 0; index < count; index++) {
        HouseClass *house = Houses[index];
        if (house) {
            if (house->Is_Player_Control()) {
                last = index;
            } else if (house->Type->MultiplayPassive) {
                continue;
            }
            Unknown_00634670(arg1, index, house);
        }
    }
    HouseClass *player = Houses[last];
    HouseClass *first = Houses[0];
    Unknown_00634670(arg1, 0, player);
    Unknown_00634670(arg1, last, first);
}


// void __fastcall Unknown_00634A00(MouseClass *map) (0x00634A00)
// Shows dialog 0x73 (procedure 0x006340F0) with input held off, waits until
// it sets its result or the game quits (0x423), then restores the input flag
// from the pause state and redraws the map.


void __fastcall Unknown_00634A00(MouseClass *map)
{
    HWND window = unknown_create_dialog_5ff610(0x73, Unknown_006340F0, 0);
    unknown_flag_a40d44 = true;
    WWKeyboard->Clear();
    if (window) {
        int result;
        SetWindowLongA(window, 8, (LONG)&result);
        unknown_dialog_5ff7c0(window);
        result = 0;
        while (true) {
            if (unknown_6000d0()) {
                result = 0x423;
                break;
            }
            if (result) {
                break;
            }
        }
        unknown_dialog_5ff6e0(window);
    }
    WWKeyboard->Clear();
    unknown_flag_a40d44 = unknown_id_source_a3d290->unknown_2306;
    Map.Flag_To_Redraw(2);
}


// wchar_t const *RadarClass::Help_Text(int id) (0x00634AA0, slot 33)
// The display's help text (the last function of Radar.CPP).

wchar_t const *RadarClass::Help_Text(int id)
{
    return DisplayClass::Help_Text(id);
}


// RadarClass::RTacticalClass scalar deleting destructor (0x00634AB0, slot 0)
// Compiler generated where RadarButton's vtable is emitted: the implicit
// destructor (GadgetClass's), then operator delete when bit 0 is set.


// RadarClass scalar deleting destructor (0x00634AD0)
// Compiler generated with RadarClass's vtable: the destructor (0x0062F080)
// inlined, then operator delete.


// ?Clear@?$DynamicVectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEXXZ (0x00634BC0)
// Template instance of the radar tracking table's bucket vectors (shared/vector_class.hpp),
// emitted with the vtable RadarClass's constructor references.


// ?ID@?$DynamicVectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEHPBU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@Z (0x00634BF0)
// Template instance of the radar tracking table's bucket vectors (shared/vector_class.hpp),
// emitted with the vtable RadarClass's constructor references.


// ?Fetch@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UBE?AU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@H@Z (0x00634C10)
// Template instance of the radar tracking table's bucket vectors (shared/vector_class.hpp),
// emitted with the vtable RadarClass's constructor references. The element copy is
// RadarTrackingStruct's member-wise copy constructor.


// ??8?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UBE_NABV0@@Z (0x00634C40)
// Template instance of the radar tracking table's bucket vectors (shared/vector_class.hpp),
// emitted with the vtable RadarClass's constructor references.


// ?Resize@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAE_NHPBU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@Z (0x00634CA0)
// Template instance of the radar tracking table's bucket vectors (shared/vector_class.hpp),
// emitted with the vtable RadarClass's constructor references.


// ?Clear@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEXXZ (0x00634DA0)
// Template instance of the radar tracking table's bucket vectors (shared/vector_class.hpp),
// emitted with the vtable RadarClass's constructor references.


// ?ID@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEHABU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@Z (0x00634DD0)
// Template instance of the radar tracking table's bucket vectors (shared/vector_class.hpp),
// emitted with the vtable RadarClass's constructor references.


// ?ID@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEHPBU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@Z (0x00634E20)
// Template instance of the radar tracking table's bucket vectors (shared/vector_class.hpp),
// emitted with the vtable RadarClass's constructor references.


// ?Resize@?$DynamicVectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAE_NHPBU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@Z (0x00634E40)
// Template instance of the radar tracking table's bucket vectors (shared/vector_class.hpp),
// emitted with the vtable RadarClass's constructor references.


// ?ID@?$DynamicVectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEHABU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@Z (0x00634F50)
// Template instance of the radar tracking table's bucket vectors (shared/vector_class.hpp),
// emitted with the vtable RadarClass's constructor references.


// DynamicVectorClass<HashObject<RadarTrackingStruct, TechnoClass *> > vector_deleting_destructor (0x00634F90)
// Template instance of the radar tracking table's bucket vectors (shared/vector_class.hpp),
// emitted with the vtable RadarClass's constructor references.


extern "C" unsigned char vtable_635040[]; extern "C" void __cdecl release_buffer_635040(void*); extern "C" void __cdecl release_object_635040(void*);
class Leaf_635040 { public: void* get(unsigned int flags); };
void* Leaf_635040::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_635040; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_635040((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_635040(this); return this; }


// DynamicVectorClass<HashObject<RadarTrackingStruct, TechnoClass *> >::DynamicVectorClass(unsigned size, HashObject const *array) (0x00635110)
// Template instance emitted out of line by RadarClass's constructor (the
// tracking table's bucket array), with VectorClass's constructor inlined.
