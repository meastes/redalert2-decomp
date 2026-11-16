// display.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/display.json carries the addresses and the rows that
// stay in their own files.


#define RA2_POINT_LENGTH
#define RA2_CELL_AS_COORD
#define RA2_DISPLAY_START_POS
#define RA2_DISPLAY_READ_INI
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/display.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/tactical.hpp"
#include "shared/mouse.hpp"
#include "shared/blit.hpp"
#include "shared/tool_tip.hpp"
#include "shared/map_view.hpp"
#include "shared/crt_memory.hpp"
#include "shared/house_screen.hpp"
#include "shared/techno.hpp"
#include "shared/building_type_class.hpp"
#include "shared/map_cell.hpp"
#include "shared/session_state.hpp"
#include "shared/rect.hpp"
#include "shared/techno_type.hpp"
#include "shared/object.hpp"
#include "shared/id_source.hpp"
#include "shared/abstract.hpp"
#include "shared/always.hpp"
#include "shared/fast_math.hpp"
#include "shared/keyboard.hpp"
#include "shared/event.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/vox_class.hpp"
#include "shared/rules.hpp"
#include "shared/locomotion.hpp"
#include "shared/ccini_class.hpp"
#include "shared/tag.hpp"
#include "shared/tag_type.hpp"
#include "shared/surface.hpp"
#include "shared/buffer_straw.hpp"
#include "shared/ms_engine.hpp"
#include "shared/tileset_index.hpp"
#include "shared/anim_type.hpp"
#include "shared/overlay_type.hpp"
#include "shared/smudge_type.hpp"
#include "shared/terrain_type.hpp"
#include "shared/veinhole.hpp"
#include "shared/lobby.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/buffer_pipe.hpp"
#include "shared/win32_api.hpp"
#include "shared/house.hpp"
#include "shared/coord_distance.hpp"
#include "shared/foot.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/string_table.hpp"
#include "shared/convert.hpp"
#include "shared/hicolor.hpp"
#include "shared/pipe.hpp"


// Header-static dynamic initializers of the unit at 0x00498760: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00498760-0x00498941); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00498780, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00498760.cpp; no source of its own.


// Dynamic initializer (0x004987B0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00498760.cpp; no source of its own.


// Dynamic initializer (0x004987D0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00498760.cpp; no source of its own.


// Dynamic initializer (0x004987F0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00498760.cpp; no source of its own.


// Dynamic initializer (0x00498810, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00498760.cpp; no source of its own.


// Dynamic initializer (0x00498830, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00498760.cpp; no source of its own.


// Dynamic initializer (0x00498850, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00498760.cpp; no source of its own.


// Dynamic initializer (0x00498890, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00498760.cpp; no source of its own.


// Dynamic initializer (0x004988C0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00498760.cpp; no source of its own.


// Dynamic initializer (0x004988F0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00498760.cpp; no source of its own.


// Dynamic initializer (0x00498920, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00498760.cpp; no source of its own.


// Dynamic initializer (0x00498930, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00498760.cpp; no source of its own.


// Dynamic initializers of Display.CPP's static members (0x00498950 Layer[5],
// 0x00498A00 TacButton, _$E) and their guarded atexit destructors
// (0x004989A0, 0x00498A30), emitted by these definitions.

LayerClass DisplayClass::Layer[5];
DisplayClass::TacticalClass DisplayClass::TacButton;


// Static-member initializer or guarded atexit destructor (0x004989A0, _$E) emitted by the definitions in DisplayClass_static_members_init_00498950.cpp; no source of its own.


// Static-member initializer or guarded atexit destructor (0x00498A00, _$E) emitted by the definitions in DisplayClass_static_members_init_00498950.cpp; no source of its own.


// Static-member initializer or guarded atexit destructor (0x00498A30, _$E) emitted by the definitions in DisplayClass_static_members_init_00498950.cpp; no source of its own.


// DisplayClass::DisplayClass() (0x00498A50)
// Constructs the map, clears the display state (the two cursor cells start
// at the 0x00852610 cell, the -1 members are unset indexes) and the unit's
// shape and counter statics.

DisplayClass::DisplayClass()
    : ZoneCell(unknown_geometry_k), ZoneOffset(0, 0), CursorSize(0), ProximityCheck(false),
      ShroudCheck(false), CopyZoneCell(unknown_geometry_k), CopyZoneOffset(0, 0), CopyCursorSize(0),
      CopyPendingObjectPtr(0), CopyPendingObject(0), CopyPendingHouse(-1), FollowingObject(false), FollowingObjectPtr(0),
      PendingObjectPtr(0), PendingObject(0), PendingHouse(-1), IsRepairMode(false), IsSellMode(false),
      IsPowerMode(false), IsWaypointMode(false), IsBeaconMode(false), IsTargettingMode(-1), DraggedWaypoint(0),
      unknown_11bc(0), unknown_11c0(0), unknown_11c4(0), WaypointColor(0, 0, 0), IsRubberBand(false), IsTentative(false), IsShadowPresent(false), unknown_11d0(0, 0),
      unknown_11d8(0, 0)
{
    ShadowShapes = 0;
    TransIconset = 0;
    unknown_852630 = 0;
}


// void DisplayClass::One_Time() (0x00498B70, slot 5)
// Loads the placement and shadow shapes and sizes the tactical view.

void DisplayClass::One_Time()
{
    MapClass::One_Time();
    TransIconset = MixFileClass::Retrieve("PLACE.SHP");
    ShadowShapes = MixFileClass::Retrieve("SHADOW.SHP");
    Rect rect = Get_Title_Screen_Area_006f1f30();
    unknown_slot42(rect);
}


// void DisplayClass::Init_Clear() (0x00498BE0, slot 7)
// Clears the map, then the display's pending state and every layer.

void DisplayClass::Init_Clear()
{
    MapClass::Init_Clear();
    PendingObjectPtr = 0;
    PendingObject = 0;
    PendingHouse = -1;
    CursorSize = 0;
    IsTargettingMode = -1;
    IsRepairMode = false;
    IsRubberBand = false;
    IsTentative = false;
    IsSellMode = false;
    IsPowerMode = false;
    for (int layer = 0; layer < 5; layer++) {
        LayerClass &list = Layer[layer];
        list.Clear();
    }
}


// void DisplayClass::Init_IO() (0x00498C50, slot 8)
// Resets the button list and, unless 0x00A40D13 is set, adds the tactical
// area's button to it.

void DisplayClass::Init_IO()
{
    GScreenClass::Init_IO();
    if (!Debug_Map) {
        TacButton.Zap();
        Add_A_Button(TacButton);
    }
}


// void DisplayClass::unknown_slot42(Rect const &rect) (0x00498C80, slot 42: Set_View_Dimensions)
// Takes the new tactical view rectangle: keeps it (0x00839630), passes it
// to the tactical view and, with the mouse hidden, rebuilds the alpha and
// depth buffers for it; then the sidebar places its controls (slot 34),
// the view's tooltip (id 500) is replaced and the message list is moved to
// the view's top edge.

void DisplayClass::unknown_slot42(Rect const &rect)
{
    Debug_Printf("Set_View_Dimensions(%d,%d,%d,%d)\n", rect.X, rect.Y, rect.Width, rect.Height);
    TacticalRect = rect;
    if (TacticalMap != 0) {
        TacticalMap->Set_View_Dimensions_006a65a0(TacticalRect);
        WWMouse->Hide_Mouse();
        if (DepthBuffer != 0) {
            Debug_Printf("Deleting ZBuffer\n");
            delete DepthBuffer;
            DepthBuffer = 0;
        }
        DepthBuffer = new ZBuffer(TacticalRect);
        DepthBuffer->MaxValue = 0x8000;
        Debug_Printf("Allocating ZBuffer (%dx%d)\n", TacticalRect.Width, TacticalRect.Height);
        if (AlphaBuffer != 0) {
            Debug_Printf("Deleting ABuffer\n");
            delete AlphaBuffer;
            AlphaBuffer = 0;
        }
        AlphaBuffer = new ABuffer(TacticalRect);
        Debug_Printf("Allocating ABuffer (%dx%d)\n", TacticalRect.Width, TacticalRect.Height);
        WWMouse->Show_Mouse();
    }
    Map.Reposition_Sidebar();
    TacButton.X = TacticalRect.X;
    TacButton.Y = TacticalRect.Y;
    TacButton.Width = TacticalRect.Width;
    TacButton.Height = TacticalRect.Height;
    if (ToolTips != 0) {
        ToolTip tip;
        tip.unknown_18 = false;
        tip.ID = 500;
        tip.Label = 0;
        tip.Region = TacticalRect;
        ToolTips->Remove(500);
        ToolTips->Add(tip);
    }
    unknown_object_a3dc70.Unknown_005B4C60(TacticalRect.X + 3, TacticalRect.Y, 6, 0x62, 0xE, -1, -1, 0, 0x14, 0x62,
                                          TacticalRect.Width - 6);
    unknown_object_a3dc70.Unknown_005B5C60(TacticalRect.Width);
    Debug_Printf("Set_View_Dimensions(exit)\n");
}


// void DisplayClass::Unknown_00498F10(Cell const *list) (0x00498F10)
// Set_Cursor_Shape's twin for the cursor list at +0x1178 (cell +0x1170 plus offset +0x1174, cell
// flag 2 through 0x004998C0): unmarks the old list, copies the new 0x7FFF-terminated list into a
// function static (0x00852634, 120 cells, guard 0x00852838; its empty atexit thunk is 0x00499060)
// and centres it on the cell.

void DisplayClass::Unknown_00498F10(Cell const *list)
{
    if (CursorSize) {
        Unknown_004998C0(ZoneCell + ZoneOffset, false);
    }
    ZoneOffset = unknown_geometry_k;

    if (list) {
        static Cell _list[120];

        memcpy(_list, list, sizeof(_list));
        CursorSize = _list;
        Cell size = Get_Occupy_Dimensions(CursorSize);
        size -= Cell(1, 1);
        ZoneOffset = -size / 2;
        Unknown_004998C0(ZoneCell + ZoneOffset, true);
    } else {
        CursorSize = 0;
    }
}


// atexit destructor (0x00499060) of DisplayClass::Unknown_00498F10's static cell list, emitted by DisplayClass_Unknown_00498F10_00498f10.cpp; no source of its own.


// void DisplayClass::Set_Cursor_Shape(Cell const *list) (0x00499070)
// Red Alert's DisplayClass::Set_Cursor_Shape: unmarks the old placement
// cursor (+0x1188 list at zone cell +0x117E plus offset +0x1182), copies the
// new 0x7FFF-terminated list into a function static (0x008524B0, guard
// 0x008525F0; its empty atexit thunk is 0x004991C0) and centres it on the
// zone cell.
// The size less (1, 1) is negated and halved with TPoint2D<short>'s operators.

void DisplayClass::Set_Cursor_Shape(Cell const *list)
{
    if (CopyCursorSize) {
        Unknown_00499970(CopyZoneCell + CopyZoneOffset, false);
    }
    CopyZoneOffset = unknown_geometry_k;

    if (list) {
        static Cell _list[50];

        memcpy(_list, list, sizeof(_list));
        CopyCursorSize = _list;
        Cell size = Get_Occupy_Dimensions(CopyCursorSize);
        size -= Cell(1, 1);
        CopyZoneOffset = -size / 2;
        Unknown_00499970(CopyZoneCell + CopyZoneOffset, true);
    } else {
        CopyCursorSize = 0;
    }
}


// atexit destructor (0x004991C0) of Set_Cursor_Shape's static cell list, emitted by DisplayClass_Set_Cursor_Shape_00499070.cpp; no source of its own.


// bool DisplayClass::Passes_Proximity_Check_004991D0(ObjectTypeClass const *object, int house, Cell const *list, Cell const &trycell) const (0x004991D0)
// TibSun DisplayClass::Passes_Proximity_Check: whether a building placed at trycell touches one of the
// house's base buildings (or, with the session's ally-building option, an allied eligible one). The
// foundation is a Rect built from the two size calls as constructor arguments (as Passes_Shroud_Check);
// the loops test against xmin + xsize and ymin + ysize, which VC6 hoists into the loop preheaders.

bool DisplayClass::Passes_Proximity_Check_004991D0(ObjectTypeClass const *object, int house, Cell const *list, Cell const &trycell) const
{
    if (house != PlayerPtr->ArrayIndex) {
        return true;
    }

    if (Debug_Map) {
        return true;
    }

    if (list == 0 || trycell == unknown_geometry_k) {
        return true;
    }

    if (object == 0 || object->Kind_Of() != 7) {
        return true;
    }

    BuildingTypeClass const *building = (BuildingTypeClass const *)object;

    Rect rect(trycell.X, trycell.Y, building->Width(), building->Foundation_Height(false));

    int adj = (building->Adjacent + 1);

    int xmin = rect.X - adj;
    int ymin = rect.Y - adj;

    int xsize = (2 * adj) + rect.Width;
    int ysize = (2 * adj) + rect.Height;

    bool retval = false;

    for (int x = xmin; x < xmin + xsize; x++) {
        for (int y = ymin; y < ymin + ysize; y++) {
            Cell newcell(x, y);

            if (!rect.Is_Point_Within(Point2D(newcell.X, newcell.Y))) {
                CellClass *cellptr = &Map[newcell];

                BuildingClass *newbase = (BuildingClass *)cellptr->Cell_Building();

                if (newbase != 0) {
                    if (newbase->House->ArrayIndex == house && newbase->Type->BaseNormal) {
                        retval = true;
                    }
                    if (unknown_session_a3d298.unknown_2c && newbase->House->Is_Ally(Houses[house]) && newbase->Type->EligibileForAllyBuilding) {
                        retval = true;
                    }
                }
            }
        }
    }

    return retval;
}


// bool DisplayClass::Unknown_00499390(BuildingTypeClass *type, int house, int arg3, Cell const *cell) (0x00499390)
// TibSun DisplayClass::Passes_Shroud_Check: whether a building placed at cell avoids the shroud. Always true
// for another house than the player's, with 0x00A40D13 set, without a cell list (arg3), at the null cell or
// for anything but a building type. Otherwise a shrouded foundation cell fails unless the type's +0xBF0
// (TibSun ToTile) is set. The foundation is a Rect built from the two size calls as constructor arguments
// (Foundation_Height is called first; only Y spills, to the Rect's own slot).

bool DisplayClass::Unknown_00499390(BuildingTypeClass *type, int house, int arg3, Cell const *cell)
{
    if (house != PlayerPtr->ArrayIndex) {
        return true;
    }
    if (Debug_Map) {
        return true;
    }
    if (arg3 == 0 || *cell == unknown_geometry_k) {
        return true;
    }
    if (type == 0 || type->Kind_Of() != 7) {
        return true;
    }

    Rect rect(cell->X, cell->Y, type->Width(), type->Foundation_Height(false));

    bool shrouded = false;
    for (int x = rect.X; x < rect.X + rect.Width; x++) {
        for (int y = rect.Y; y < rect.Y + rect.Height; y++) {
            CoordStruct coord = Cell_Coord(XY_Cell(x, y));
            coord.Z = Map.Get_Height_GL(coord);
            if (Map.Is_Shrouded(coord)) {
                shrouded = true;
                break;
            }
        }
        if (shrouded) {
            if (type->ToTile == 0) {
                return false;
            }
            break;
        }
    }
    return true;
}


// Not exact (RA2_NON_MATCHING): DisplayClass::Set_Cursor_Pos at 0x004994D0, best score 69.20, 2026-11-13; differs: 223 instructions in the original, 225 in the draft (shape 96.88)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x4994D0 DisplayClass::Set_Cursor_Pos (TibSun Set_Cursor_Pos)
// entry 0x4994D0; exact-match score 78.48 (misses_index best, Scout620/Scout1075).
// plan "draft, verify". Origin: build/class-work/Scout1075/keep/scp_k6_natural_7848.cpp.
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by Scout1115
// (native/drafts.json). Not original bytes. Reached by the placement cursor once a finished
// building is picked from the sidebar (native M13 interactive input).
// Native adaptation (Scout1115): the radar's cell under the mouse (RadarClass slot 51, which this
// unit's light MouseClass does not declare) through native/link/behaviour_m13_commands.cpp's
// Native_Radar_Cell_At; unknown_a40d13 is HEAD's Debug_Map (0x00A40D13). Marked NATIVE.
// Cell DisplayClass::Set_Cursor_Pos(Cell const &xpos) (0x004994D0)
// TibSun DisplayClass::Set_Cursor_Pos: moves the placement cursor (zone cell +0x1170) to a cell, or to the
// cell under the mouse for the null cell, re-marks the cursor list (+0x1178) and recomputes the proximity
// (+0x117C) and shroud (+0x117D) checks for the pending object (+0x11A4) and house (+0x11A8). Returns the old
// zone cell (the new one when it did not move).

Cell Native_Radar_Cell_At(Point2D const &xy);   // NATIVE: RadarClass slot 51 on Map
Cell DisplayClass::Set_Cursor_Pos(Cell const &xpos)
{
    Cell prevpos;
    Cell pos = xpos;

    if (pos == unknown_geometry_k) {
        Point2D xy = WWMouse->Get_Mouse_XY();
        Point2D tl = Point2D(TacticalRect.X, TacticalRect.Y);
        if (xy.X >= tl.X && xy.X < tl.X + TacticalRect.Width && xy.Y >= tl.Y && xy.Y < tl.Y + TacticalRect.Height) {
            Point2D mouse = WWMouse->Get_Mouse_XY();
            Cell click = Native_Radar_Cell_At(mouse);   // NATIVE: Map.Click_Cell_Calc(mouse)
            if (click != unknown_geometry_k) {
                pos = click;
            }
        } else {
            Point2D mouse = WWMouse->Get_Mouse_XY();
            pos = TacticalMap->Pixel_To_Cell(mouse - tl);
        }
    }

    if (CursorSize == 0) {
        prevpos = ZoneCell;
        ZoneCell = pos;
        return prevpos;
    }

    if (!Debug_Map) {   // NATIVE: unknown_a40d13
        pos = TacticalMap->Clamp_Cursor_To_Tactical(pos + ZoneOffset, CursorSize) - ZoneOffset;
    }

    if (pos == ZoneCell) return pos;

    prevpos = ZoneCell;

    if (CursorSize != 0) {
        if (pos != ZoneCell && ZoneCell != unknown_geometry_k) {
            Unknown_004998C0(ZoneCell + ZoneOffset, false);
        }
        if (pos != unknown_geometry_k) {
            Unknown_004998C0(pos + ZoneOffset, true);
        }
    }
    ZoneCell = pos;
    ProximityCheck = Passes_Proximity_Check_004991D0(PendingObject, PendingHouse, CursorSize, ZoneCell + ZoneOffset);
    Cell cell = ZoneCell + ZoneOffset;
    ShroudCheck = Unknown_00499390((BuildingTypeClass *)PendingObject, PendingHouse, (int)CursorSize, &cell);

    return prevpos;
}
#endif

// Cell DisplayClass::Get_Occupy_Dimensions(Cell const *list) (0x00499810)
// The size of a cell list (offsets ending with 0x7FFF, 0x7FFF): its extent
// in x and y, at least one cell each; 0 x 0 when there is no list.

Cell DisplayClass::Get_Occupy_Dimensions(Cell const *list)
{
    int min_x = 512;
    int max_x = -512;
    int min_y = 512;
    int max_y = -512;
    Cell size(0, 0);
    if (list) {
        while (list->X != 0x7FFF || list->Y != 0x7FFF) {
            int x = list->X;
            int y = list->Y;
            if (max_x <= x) {
                max_x = x;
            }
            if (min_x >= x) {
                min_x = x;
            }
            if (max_y <= y) {
                max_y = y;
            }
            if (min_y >= y) {
                min_y = y;
            }
            list++;
        }
        size.X = (short)(max_x - min_x + 1 < 1 ? 1 : max_x - min_x + 1);
        size.Y = (short)(max_y - min_y + 1 < 1 ? 1 : max_y - min_y + 1);
    }
    return size;
}


// void DisplayClass::Unknown_004998C0(Cell const &cell, bool set) (0x004998C0)
// Sets (or clears) flag 2 of every cell of the list at +0x1178 placed at
// the given cell, for the cells on the map.

void DisplayClass::Unknown_004998C0(Cell const &cell, bool set)
{
    if (cell.X == unknown_geometry_k.X && cell.Y == unknown_geometry_k.Y) {
        return;
    }
    Cell const *offset = CursorSize;
    while (*offset != Cell(0x7FFF, 0x7FFF)) {
        Cell target = cell + *offset++;
        if (In_Radar(target)) {
            CellClass &target_cell = (*this)[target];
            if (set) {
                target_cell.unknown_12c = target_cell.unknown_12c | 2;
            } else {
                target_cell.unknown_12c = target_cell.unknown_12c & ~2;
            }
        }
    }
}


// void DisplayClass::Unknown_00499970(Cell const &cell, bool set) (0x00499970)
// Sets (or clears) flag 4 of every cell of the list at +0x1188 placed at
// the given cell, for the cells on the map. The flag updates are written
// `x = x | 4`: the wordier body is above VC6's auto-inline size cap, so
// Set_Cursor_Shape (0x00499070) keeps both calls out of line as the original does.

void DisplayClass::Unknown_00499970(Cell const &cell, bool set)
{
    if (cell.X == unknown_geometry_k.X && cell.Y == unknown_geometry_k.Y) {
        return;
    }
    Cell const *offset = CopyCursorSize;
    while (*offset != Cell(0x7FFF, 0x7FFF)) {
        Cell target = cell + *offset++;
        if (In_Radar(target)) {
            CellClass &target_cell = (*this)[target];
            if (set) {
                target_cell.unknown_12c = target_cell.unknown_12c | 4;
            } else {
                target_cell.unknown_12c = target_cell.unknown_12c & ~4;
            }
        }
    }
}


class Target_499a20 { public: unsigned int AI_004e10c0(unsigned int target_input, unsigned int target_mouse_position); };
class Leaf_499a20 { public: unsigned int get(unsigned int input_address, unsigned int mouse_position_address); };
unsigned int Leaf_499a20::get(unsigned int input_address, unsigned int mouse_position_address) { return ((Target_499a20*)(this))->AI_004e10c0((unsigned int)(input_address), (unsigned int)(mouse_position_address)); }


// void DisplayClass::Submit(ObjectClass *object) (0x00499A40)
// Puts an object into the display layer it belongs to (its slot 29), after
// taking it out of the layer it was in (+0x80, -1 when none); ground layer 2
// is kept sorted.

void DisplayClass::Submit(ObjectClass *object)
{
    if (object) {
        if (object->Layer != -1) {
            Remove(object);
        }
        int layer = object->In_Which_Layer();
        if (layer != -1) {
            if (Layer[layer].Submit(object, layer == 2)) {
                object->Layer = layer;
            }
        }
    }
}


// void DisplayClass::Remove(ObjectClass *object) (0x00499A90)
// Takes an object out of its display layer (+0x80); when it was not found
// there, out of every layer, as often as it appears. (Each search is given
// a fresh copy of the pointer, as the original passes a temporary.)

// Not expanded into Submit in the original (kept out of line).
#pragma auto_inline(off)
void DisplayClass::Remove(ObjectClass *object)
{
    ObjectClass *entry;
    if (object && object->Layer != -1) {
        if (Layer[object->Layer].Delete(entry = object)) {
            object->Layer = -1;
        }
        if (object->Layer != -1) {
            for (int layer = 0; layer < 5; layer++) {
                while (Layer[layer].Delete(entry = object)) {
                }
            }
            object->Layer = -1;
        }
    }
}
#pragma auto_inline(on)


// bool DisplayClass::Scroll_Map(int arg1, int *arg2, bool arg3) (0x00499B60, slot 41)
// Passes a request on to the tactical view: with a value or arg3 set it
// calls 0x006A8B70 (true); otherwise answers whether 0x006AA840 finds one.

bool DisplayClass::Scroll_Map(int arg1, int *arg2, bool arg3)
{
    int value = *arg2;
    if (value == 0 && !arg3) {
        return false;
    }
    if (value != 0 && !arg3) {
        return TacticalMap->Scroll_Dir_006aa840(arg1) != -1;
    }
    TacticalMap->Scroll_Map_006a8b70(arg1, value);
    return true;
}


// bool DisplayClass::Map_Cell(Cell const &cell, HouseClass *house) (0x00499BB0, slot 36)
// Maps a cell and spreads the reveal (hypothesis: Westwood's Map_Cell): marks
// it mapped and visible (+0x140 bit 1, +0x12C bit 3), takes the tactical
// view's shroud frames of both layers and has the view redraw it when they
// changed. Then for each layer and each adjacent cell not yet fully shrouded
// in that layer: a neighbour the view now shows fully shrouded is marked so
// (+0x12C bit 4 / +0x140 bit 0) and its own neighbours' frames refreshed, or
// mapped in turn when it is not yet mapped; other neighbours take their new
// frame. When the cell changed the map redraws it (0x00568520, as slot 37).
// True when it changed.

bool DisplayClass::Map_Cell(Cell const &cell, HouseClass *house)
{
    CellClass &target = (*this)[cell];
    bool unmapped = !(target.unknown_140 & 2);
    bool changed = !(target.unknown_140 & 2) || !(target.unknown_12c & 8);
    bool mapped = changed;
    target.unknown_140 = (target.unknown_140 & ~0x40) | 2;
    target.unknown_12c |= 8;
    signed char frame = TacticalMap->Cell_Shadow(cell, 0);
    if (frame != target.ShadowFrame) {
        changed = true;
        target.ShadowFrame = frame;
    }
    if (target.ShadowFrame == -1) {
        target.unknown_12c |= 0x10;
    }
    frame = TacticalMap->Cell_Shadow(cell, 1);
    if (frame != target.FogFrame) {
        changed = true;
        target.FogFrame = frame;
    }
    if (target.FogFrame == -1) {
        target.unknown_140 |= 1;
    }
    if (changed) {
        TacticalMap->Flag_Cell(&target);
    }

    for (int facing = 0; facing < 8; facing++) {
        Cell adjacent = AdjacentCell[facing & 7] + cell;
        CellClass &near_cell = (*this)[adjacent];

        if (adjacent != cell && !(near_cell.unknown_12c & 0x10)) {
            int shade = TacticalMap->Cell_Shadow(adjacent, 0);
            if (shade == -1) {
                if (!(near_cell.unknown_12c & 8)) {
                    Map_Cell(adjacent, house);
                } else {
                    near_cell.unknown_12c |= 0x10;
                    TacticalMap->Flag_Cell(&near_cell);
                    for (int around = 0; around < 8; around++) {
                        Cell next = AdjacentCell[around & 7] + adjacent;
                        CellClass &next_cell = (*this)[next];
                        signed char next_frame = TacticalMap->Cell_Shadow(next, 0);
                        if (next_frame != next_cell.ShadowFrame) {
                            next_cell.ShadowFrame = next_frame;
                            TacticalMap->Flag_Cell(&next_cell);
                        }
                    }
                }
            } else if (shade != -2 && !(near_cell.unknown_12c & 8)) {
                Map_Cell(adjacent, house);
            } else if (shade >= 0 && shade != near_cell.ShadowFrame) {
                near_cell.ShadowFrame = shade;
                TacticalMap->Flag_Cell(&near_cell);
            }
        }

        if (adjacent != cell && !(near_cell.unknown_140 & 1)) {
            int shade = TacticalMap->Cell_Shadow(adjacent, 1);
            if (shade == -1) {
                if (!(near_cell.unknown_140 & 2)) {
                    Map_Cell(adjacent, house);
                } else {
                    near_cell.unknown_140 |= 1;
                    TacticalMap->Flag_Cell(&near_cell);
                    for (int around = 0; around < 8; around++) {
                        Cell next = AdjacentCell[around & 7] + adjacent;
                        CellClass &next_cell = (*this)[next];
                        signed char next_frame = TacticalMap->Cell_Shadow(next, 1);
                        if (next_frame != next_cell.FogFrame) {
                            next_cell.FogFrame = next_frame;
                            TacticalMap->Flag_Cell(&next_cell);
                        }
                    }
                }
            } else if (shade != -2 && !(near_cell.unknown_140 & 2)) {
                Map_Cell(adjacent, house);
            } else if (shade >= 0 && shade != near_cell.FogFrame) {
                near_cell.FogFrame = shade;
                TacticalMap->Flag_Cell(&near_cell);
            }
        }
    }

    if (changed) {
        Map.Reveal_Nearby_Technos(&target, house, mapped);
    }
    if ((target.unknown_140 & 2) && unmapped && (unknown_id_source_a3d290->unknown_000 & 0x1000)) {
        target.Unfog_Cell();
    }
    return changed;
}


// bool DisplayClass::RevealFogShroud(Cell const &cell, HouseClass *house, bool arg3) (0x00499FC0, slot 37)
// Maps a cell (hypothesis: Westwood's Map_Cell): marks it mapped (+0x140 bit
// 1, bit 6 cleared), counts the viewer in or out (arg3), takes the tactical
// view's shroud frames of both layers, and when anything changed has the
// view and the map redraw it. True when it changed.

bool DisplayClass::RevealFogShroud(Cell const &cell, HouseClass *house, bool arg3)
{
    CellClass &target = (*this)[cell];
    bool unmapped = !(target.unknown_140 & 2);
    bool changed = !(target.unknown_140 & 2) || !(target.unknown_12c & 8);
    bool mapped = changed;
    target.unknown_140 = (target.unknown_140 & ~0x40) | 2;
    if (arg3) {
        target.IncreaseShroudCounter();
    } else {
        target.ReduceShroudCounter();
    }
    signed char frame = TacticalMap->Cell_Shadow(cell, 0);
    if (frame != target.ShadowFrame) {
        changed = true;
        target.ShadowFrame = frame;
    }
    if (target.ShadowFrame == -1) {
        target.unknown_12c |= 0x10;
    }
    frame = TacticalMap->Cell_Shadow(cell, 1);
    if (frame != target.FogFrame) {
        changed = true;
        target.FogFrame = frame;
    }
    if (target.FogFrame == -1) {
        target.unknown_140 |= 1;
    }
    if (changed) {
        TacticalMap->Flag_Cell(&target);
        Map.Reveal_Nearby_Technos(&target, house, mapped);
    }
    if ((target.unknown_140 & 2) && unmapped && (unknown_id_source_a3d290->unknown_000 & 0x1000)) {
        target.Unfog_Cell();
    }
    return changed;
}


// bool DisplayClass::unknown_slot38(Cell const &cell, HouseClass *house) (0x0049A0F0, slot 38)
// Maps a cell for the tactical view's layer 1 (hypothesis: Westwood's
// Map_Cell for the second shroud layer): marks it mapped, takes its layer-1
// shroud frame, then looks at the eight neighbours that are not fully
// shrouded: a neighbour the view reports fully shrouded (-1) is mapped in
// turn when it is still unmapped, else is marked fully shrouded and its own
// neighbours refreshed; other unmapped neighbours (not -2) are mapped; mapped
// ones take their new frame. When the cell was newly mapped the map redraws
// it. True when it was newly mapped or its frame changed.

bool DisplayClass::unknown_slot38(Cell const &cell, HouseClass *house)
{
    CellClass &target = (*this)[cell];
    char unmapped = !(target.unknown_140 & 2);
    bool changed = !(target.unknown_140 & 2);
    bool mapped = changed;
    target.unknown_140 = (target.unknown_140 & ~0x40) | 2;
    signed char frame = TacticalMap->Cell_Shadow(cell, 1);
    if (frame != target.FogFrame) {
        changed = true;
        target.FogFrame = frame;
    }
    if (target.FogFrame == -1) {
        target.unknown_140 |= 1;
    }
    if (changed) {
        TacticalMap->Flag_Cell(&target);
    }
    for (int facing = 0; facing < 8; facing++) {
        Cell adjacent = Adjacent_Cell(cell, facing);
        CellClass &next = (*this)[adjacent];
        if (adjacent == cell || (next.unknown_140 & 1)) {
            continue;
        }
        int next_frame = TacticalMap->Cell_Shadow(adjacent, 1);
        if (next_frame == -1) {
            if (!(next.unknown_140 & 2)) {
                unknown_slot38(adjacent, house);
            } else {
                next.unknown_140 |= 1;
                TacticalMap->Flag_Cell(&next);
                for (int around = 0; around < 8; around++) {
                    Cell beyond = Adjacent_Cell(adjacent, around);
                    CellClass &far_cell = (*this)[beyond];
                    signed char far_frame = TacticalMap->Cell_Shadow(beyond, 1);
                    if (far_frame != far_cell.FogFrame) {
                        far_cell.FogFrame = far_frame;
                        TacticalMap->Flag_Cell(&far_cell);
                    }
                }
            }
        } else if (next_frame != -2 && !(next.unknown_140 & 2)) {
            unknown_slot38(adjacent, house);
        } else if (next_frame >= 0 && next_frame != next.FogFrame) {
            next.FogFrame = next_frame;
            TacticalMap->Flag_Cell(&next);
        }
    }
    if (changed) {
        Map.Reveal_Nearby_Technos(&target, house, mapped);
    }
    if ((target.unknown_140 & 2) && unmapped && (unknown_id_source_a3d290->unknown_000 & 0x1000)) {
        target.Unfog_Cell();
    }
    return changed;
}


// bool DisplayClass::unknown_slot39(Cell const &cell, HouseClass *house) (0x0049A370, slot 39)
// The layer-0 form of slot 38 (hypothesis: Westwood's Map_Cell): marks the
// cell visible (+0x12C bit 3), takes its layer-0 shroud frame, then maps or
// refreshes the eight neighbours not fully shrouded (+0x12C bit 4) the same
// way. When the cell was newly visible or its frame changed, the map redraws
// it; that is also the result.

bool DisplayClass::unknown_slot39(Cell const &cell, HouseClass *house)
{
    CellClass &target = (*this)[cell];
    bool changed = !(target.unknown_12c & 8);
    target.unknown_12c |= 8;
    bool shown = changed;
    signed char frame = TacticalMap->Cell_Shadow(cell, 0);
    if (frame != target.ShadowFrame) {
        changed = true;
        target.ShadowFrame = frame;
    }
    if (target.ShadowFrame == -1) {
        target.unknown_12c |= 0x10;
    }
    if (changed) {
        TacticalMap->Flag_Cell(&target);
    }
    for (int facing = 0; facing < 8; facing++) {
        Cell adjacent = AdjacentCell[facing & 7] + cell;
        CellClass &next = (*this)[adjacent];
        if (adjacent == cell || (next.unknown_12c & 0x10)) {
            continue;
        }
        int next_frame = TacticalMap->Cell_Shadow(adjacent, 0);
        if (next_frame == -1) {
            if (!(next.unknown_12c & 8)) {
                unknown_slot39(adjacent, house);
            } else {
                next.unknown_12c |= 0x10;
                TacticalMap->Flag_Cell(&next);
                for (int around = 0; around < 8; around++) {
                    Cell beyond = Adjacent_Cell(adjacent, around);
                    CellClass &far_cell = (*this)[beyond];
                    signed char far_frame = TacticalMap->Cell_Shadow(beyond, 0);
                    if (far_frame != far_cell.ShadowFrame) {
                        far_cell.ShadowFrame = far_frame;
                        TacticalMap->Flag_Cell(&far_cell);
                    }
                }
            }
        } else if (next_frame != -2 && !(next.unknown_12c & 8)) {
            unknown_slot39(adjacent, house);
        } else if (next_frame >= 0 && next_frame != next.ShadowFrame) {
            next.ShadowFrame = next_frame;
            TacticalMap->Flag_Cell(&next);
        }
    }
    if (changed) {
        Map.Reveal_Nearby_Technos(&target, house, shown);
    }
    return changed;
}


class Call_49a5b0_0 {public: unsigned int target(unsigned int forwarded_cell, unsigned int forwarded_point, unsigned int forwarded_flag);};
class Call_49a5b0_1 {public: unsigned int Cell_Object_00471830();};
class Leaf_49a5b0 { public: unsigned int get(unsigned int cell_address, unsigned int point_address); };
unsigned int Leaf_49a5b0::get(unsigned int cell_address, unsigned int point_address) { unsigned int cell_result_address = ((Call_49a5b0_0*)((unsigned int)this))->target((unsigned int)(cell_address), (unsigned int)(point_address), (unsigned int)(0)); unsigned int object_result_address = ((Call_49a5b0_1*)(cell_result_address))->Cell_Object_00471830(); return object_result_address; }


// ObjectClass *DisplayClass::Next_Object(ObjectClass *object) (0x0049A5D0)
// The next object (by slot 24) after the given one in layers 2, 3 and 4;
// the first one when the given object is null or last.

ObjectClass *DisplayClass::Next_Object(ObjectClass *object)
{
    ObjectClass *first = 0;
    bool found = false;
    if (!object) {
        found = true;
    }
    for (int order = 0; order < 3; order++) {
        for (int index = 0; index < Layer[unknown_layers_7d5cdc[order]].Count(); index++) {
            ObjectClass *candidate = Layer[unknown_layers_7d5cdc[order]][index];
            if (candidate && candidate->unknown_slot24()) {
                if (!first) {
                    first = candidate;
                }
                if (found) {
                    return candidate;
                }
                if (object == candidate) {
                    found = true;
                }
            }
        }
    }
    return first;
}


// ObjectClass *DisplayClass::Prev_Object(ObjectClass *object) (0x0049A670)
// The object (by slot 24) before the given one, walking layers 4, 3 and 2
// backwards; the last one when the given object is null or first.

ObjectClass *DisplayClass::Prev_Object(ObjectClass *object)
{
    ObjectClass *first = 0;
    bool found = false;
    if (!object) {
        found = true;
    }
    for (int order = 0; order < 3; order++) {
        for (int index = Layer[unknown_layers_7d5ce8[order]].Count() - 1; index >= 0; index--) {
            ObjectClass *candidate = Layer[unknown_layers_7d5ce8[order]][index];
            if (candidate && candidate->unknown_slot24()) {
                if (!first) {
                    first = candidate;
                }
                if (found) {
                    return candidate;
                }
                if (object == candidate) {
                    found = true;
                }
            }
        }
    }
    return first;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Cell DisplayClass::Calculated_Cell(int dir, Cell const &waypoint, Cell const &cell, int loco, bool zonecheck, int mzone) (0x0049A700)
// TibSun DisplayClass::Calculated_Cell: a reinforcement cell on the map edge nearest the waypoint
// (else the cell, else a random point on edge dir); the south edge collects candidates and picks the
// closest. From ra2-decomp DisplayClass::Calculated_Cell (YR 0x004AA440, src/display.cpp), adapted to
// our names (RA2 has no In_Local_Radar test on the west/east and north edges).


Cell DisplayClass::Calculated_Cell(int dir, Cell const &waypoint, Cell const &cell, int loco, bool zonecheck, int mzone)
{
    bool vert = false;
    bool horz = false;
    int x;
    int y;
    int start;
    Cell fallback;
    int zone = -1;

    Cell trycell(0, 0);
    if (waypoint != unknown_geometry_k) {
        trycell = waypoint;
    }
    if (trycell == unknown_geometry_k) {
        trycell = cell;
    }

    if (zonecheck && trycell != unknown_geometry_k) {
        zone = Map.Zone(trycell, mzone, true);
    }

    Point2D trypoint = Cell_To_LocalRect_Point(Point2D(trycell.X, trycell.Y));

    if (trycell != unknown_geometry_k) {
        int left = trypoint.X - MapLocalSize.X;
        x = (left < MapLocalSize.X + (MapLocalSize.Width - trypoint.X)
            ? left : MapLocalSize.X + (MapLocalSize.Width - trypoint.X)) * 2;
        y = trypoint.Y - 2 * MapLocalSize.Y;
        y = min(y, -trypoint.Y + 2 * (MapLocalSize.Y + MapLocalSize.Height));

        if (x < y) {
            vert = true;
            horz = false;
            if ((trypoint.X - MapLocalSize.X) < MapLocalSize.Width / 2) {
                x = -1;
                dir = 3;
            } else {
                x = MapLocalSize.Width;
                dir = 1;
            }
            y = 0;
            start = trypoint.Y - MapLocalSize.Y;
        } else {
            vert = false;
            horz = true;
            if ((trypoint.Y - 2 * MapLocalSize.Y) < (2 * MapLocalSize.Height) / 2) {
                y = -1;
                dir = 0;
            } else {
                y = 2 * MapLocalSize.Height;
                dir = 2;
            }
            x = 0;
            start = trypoint.X - MapLocalSize.X;
        }
    } else {
        y = x = 0;
        switch (dir) {
            default:
            case 0:
                horz = true;
                start = Random_Pick(1, MapLocalSize.Width) - 1;
                break;

            case 2:
                horz = true;
                y = 2 * MapLocalSize.Height + 2;
                start = Random_Pick(1, MapLocalSize.Width) - 1;
                break;

            case 1:
                vert = true;
                x = MapLocalSize.Width;
                start = Random_Pick(1, 2 * MapLocalSize.Height) - 1;
                break;

            case 3:
                vert = true;
                start = Random_Pick(0, 2 * MapLocalSize.Height) - 1;
                break;
        }
    }

    Point2D defaultPoint = LocalRect_To_Cell_Point(Point2D(1, MapLocalSize.Width / 2));
    fallback = Cell(defaultPoint.X, defaultPoint.Y);

    if (vert) {
        int modifier = (dir != 1) ? 1 : -1;

        for (int index = 0; index < MapLocalSize.Height * 2; index++) {
            int yy = (start + index) % (MapLocalSize.Height * 2);

            Point2D outcellPoint = LocalRect_To_Cell_Point(Point2D(x, yy));
            Cell outcell(outcellPoint.X, outcellPoint.Y);
            Point2D incellPoint = LocalRect_To_Cell_Point(Point2D(x + modifier, yy));

            if (Unknown_0049AD90(outcell, Cell(incellPoint.X, incellPoint.Y), loco, zone, mzone)) {
                return outcell;
            }
        }
    } else if (horz) {
        int modifier = (dir == 0) ? 1 : -1;

        if (dir == 2) {
            DynamicVectorClass<Cell> cells;

            for (x = 0; x < MapLocalSize.Width; x++) {
                for (y = 0; y < 15; y++) {
                    Point2D newcellPoint = LocalRect_To_Cell_Point(Point2D(x, y + 2 * MapLocalSize.Height));
                    Cell newcell(newcellPoint.X, newcellPoint.Y);
                    if (!Unknown_0055A660(newcell, true) && Unknown_0049AD90(newcell, trycell, loco, zone, mzone)) {
                        cells.Add(newcell);
                        break;
                    }
                }
            }
            int count = cells.Count();

            if (trycell == unknown_geometry_k) {
                if (count > 0) {
                    fallback = cells[Random_Pick(0, count - 1)];
                } else {
                    fallback = unknown_geometry_k;
                }
            } else {
                int dist = -1;
                Cell best = unknown_geometry_k;
                for (int i = 0; i < count; i++) {
                    int d = trycell.Distance_To(cells[i]);
                    if (dist == -1 || d < dist) {
                        best = cells[i];
                        dist = d;
                    }
                }
                fallback = best;
            }

        } else {
            for (int index = 0; index < MapLocalSize.Width; index++) {
                int xx = (start + index) % MapLocalSize.Width;

                Point2D outcellPoint = LocalRect_To_Cell_Point(Point2D(xx, y));
                Cell outcell(outcellPoint.X, outcellPoint.Y);
                Point2D incellPoint = LocalRect_To_Cell_Point(Point2D(xx, y + modifier));

                if (Unknown_0049AD90(outcell, Cell(incellPoint.X, incellPoint.Y), loco, zone, mzone)) {
                    return outcell;
                }
            }
        }
    }

    return fallback;
}


// bool DisplayClass::Unknown_0049AD90(Cell const &from, Cell const &to, int speed, int arg4, int arg5) (0x0049AD90)
// Whether a move between two cells is open: always for speed 4; otherwise
// both cells must be enterable (0x00478760) and hold no object (0x00471660).

bool DisplayClass::Unknown_0049AD90(Cell const &from, Cell const &to, int speed, int arg4, int arg5)
{
    if (speed == 4) {
        return true;
    }
    if (!(*this)[from].Unknown_00478760(speed, true, true, -1, 0, -1, true)) {
        return false;
    }
    if (!(*this)[to].Unknown_00478760(speed, true, true, arg4, arg5, -1, true)) {
        return false;
    }
    if ((*this)[from].Unknown_00471660(Point2D(0, 0), 0, 0)) {
        return false;
    }
    return (*this)[to].Unknown_00471660(Point2D(0, 0), 0, 0) == 0 ? true : false;
}


// int DisplayClass::TacticalClass::Action(unsigned flags, unsigned &key) (0x0049AE70, slot 31)
// The tactical area's gadget: takes the mouse point (the queued one for a
// mouse key, else the live one), finds the cell, coordinate and object under
// it (ScrollClass 0x00669650) and moves the cursor cell there when it
// changed, then lets GadgetClass handle the key.

int DisplayClass::TacticalClass::Action(unsigned flags, unsigned &key)
{
    ObjectClass *object = 0;
    Point2D xy;
    xy.X = 0;
    xy.Y = 0;
    if (flags & 0x55) {
        xy = Point2D(WWKeyboard->MouseQX, WWKeyboard->MouseQY);
    } else {
        WWMouse->Get_Mouse_X();
        WWMouse->Get_Mouse_Y();
        xy = WWMouse->Get_Mouse_XY();
    }
    TacticalMap->Pixel_To_Coord(xy);
    xy -= Point2D(TacticalRect.X, TacticalRect.Y);
    Cell cell;
    CoordStruct coord;
    bool arg5;
    bool arg6;
    Map.Resolve_Point(xy, cell, coord, object, arg5, arg6);
    if (cell != Map.ZoneCell) {
        Map.Set_Cursor_Pos(cell);
    }
    return GadgetClass::Action(0, key);
}


// void DisplayClass::Mouse_Right_Press(Point2D const &point) (0x0049AF80, slot 44): does nothing.

void DisplayClass::Mouse_Right_Press(Point2D const &)
{
}


// void DisplayClass::Mouse_Right_Release(Point2D const &xy) (0x0049AF90, slot 49)
// Cancels whatever the right mouse button cancels (hypothesis: Westwood's
// Mouse_Right_Press), first match only: a techno waiting for placement, then
// the modes at +0x11AC, +0x11AD, +0x11AE, the target at +0x11B4, the
// waypoint mode +0x11AF and the mode +0x11B0; with none of them, 0x00482BC0.
// Each case restores the default mouse shape.

void Unselect_All();

void DisplayClass::Mouse_Right_Release(Point2D const &xy)
{
    ObjectClass *pending = PendingObjectPtr;
    bool techno;
    if (pending != 0) {
        techno = pending->AbstractFlags.IsTechno;
    }
    if (pending != 0 && techno) {
        PendingObjectPtr = 0;
        PendingObject = 0;
        PendingHouse = -1;
        Unknown_00498F10(0);
        Set_Default_Mouse(0, unknown_flag_83740c);
        return;
    }
    if (IsRepairMode) {
        Unknown_0049CA20(0);
        Set_Default_Mouse(0, unknown_flag_83740c);
        return;
    }
    if (IsSellMode) {
        Unknown_0049C7C0(0);
        Set_Default_Mouse(0, unknown_flag_83740c);
        return;
    }
    if (IsPowerMode) {
        Unknown_0049C980(0);
        Set_Default_Mouse(0, unknown_flag_83740c);
        return;
    }
    if (IsTargettingMode != -1) {
        IsTargettingMode = -1;
        Set_Default_Mouse(0, unknown_flag_83740c);
        return;
    }
    if (IsWaypointMode) {
        Unknown_0049C860(0, false);
        Set_Default_Mouse(0, unknown_flag_83740c);
        return;
    }
    if (IsBeaconMode) {
        Unknown_0049CAC0(0);
        Set_Default_Mouse(0, unknown_flag_83740c);
        return;
    }
    Unselect_All();
    Set_Default_Mouse(0, unknown_flag_83740c);
}


// Existing machine contracts, source identities pending separate signature audit.
void Unknown_Click_End_006F6770();
void __fastcall Unknown_006F7020(char const *id);
bool __fastcall Unknown_Planning_00615DE0();
bool __fastcall Planning_Can_Add();
void __cdecl Clear_Selected_Objects_00482bc0();
void __fastcall Unknown_0049C3F0(ObjectClass *object);
extern bool drag_select_aborted;
extern int unknown_height_step_852948;

void DisplayClass::Mouse_Left_Release(CoordStruct const &coord, Cell const &cell, ObjectClass *object, int action, bool small_mouse)
{
    if (object && ScenarioActive && GameActive && !Debug_Map &&
        object->unknown_slot25(false, 0) == 5) object = 0;
    if (PendingObjectPtr) {
        if (PendingObject->Kind_Of() == 7) {
            TPoint2D<short> const *placement=&cell;
            Cell const where=placement->operator+(ZoneOffset);
            ProximityCheck = Passes_Proximity_Check_004991D0(PendingObject, PendingHouse, CursorSize, where);
        }
        if (PendingObject->Kind_Of() == 7 && object && object->Kind_Of() == 6 &&
            static_cast<BuildingClass *>(object)->Can_Upgrade(static_cast<BuildingTypeClass const *>(PendingObject), PlayerPtr))
            ProximityCheck = true;
        if (ProximityCheck && ShroudCheck) {
            TechnoTypeClass const *techno_type = dynamic_cast<TechnoTypeClass const *>(PendingObject);
            bool naval = techno_type && techno_type->Naval;
            ObjectClass *pending = PendingObjectPtr;
            OutList.Add(EventClass(PlayerPtr->ArrayIndex, 0xb, pending->Kind_Of(),
                PendingObject->Fetch_Heap_ID(), naval, cell + ZoneOffset));
            pending = PendingObjectPtr;
            BuildingClass *placed;
            if (!pending) placed = 0;
            else placed = pending->Kind_Of() == 6 ? static_cast<BuildingClass *>(pending) : 0;
            Unknown_006F8A20(placed);
            if (!Debug_Map) {
                CopyPendingObject = PendingObject;
                CopyPendingObjectPtr = PendingObjectPtr;
                CopyZoneCell = cell;
                CopyZoneOffset = ZoneOffset;
                CopyCursorSize = CursorSize;
                CopyPendingHouse = PendingHouse;
                if (PendingObject) Map.Set_Cursor_Shape(PendingObject->Occupy_List(true));
                Map.Unknown_00498F10(0);
                Map.PendingObjectPtr = 0;
                Map.PendingObject = 0;
                Map.PendingHouse = -1;
            }
        } else {
            Speech_Play_Name("EVA_CannotDeployHere", -1, -1);
        }
    } else {
        int continue_select = 0;
        if (IsRubberBand) {
            TacticalMap->IsToRedraw = true;
            if (!WWKeyboard->Down(0x10)) {
                if (TacticalMap->Rubber_Band_Contains_Selectable()) Clear_Selected_Objects_00482bc0();
                else continue_select = 1;
            }
            TacticalMap->Select_Rubber_Band(Unknown_0049C3F0);
            TechnoClass::Reset_Action_Line_Timer();
            IsRubberBand = false;
            Set_Default_Mouse(0, small_mouse);
            IsTentative = false;
            drag_select_aborted = true;
            if (!continue_select) { Unknown_Click_End_006F6770(); return; }
        }
        {
            if (action == 8) {
                if (!object || !CurrentObject.Count() || !CurrentObject[0]->Owning_House()->Is_Player_Control()) {
                    action = 7;
                } else if (object->IsSelected) {
                    if (Unknown_006F76B0()) Unknown_006F7020(object->Class_Of()->ID);
                    else object->Unselect();
                } else {
                    if (Unknown_006F76B0()) Unknown_006F71E0(object->Class_Of()->ID);
                    else object->unknown_slot80();
                }
            }
            if ((action == 7 && object) || (action == 0 && object && object->CanBeSelected() && !object->IsSelected)) {
                if (object->Kind_Of() != 6 || static_cast<BuildingClass *>(object)->House->Is_Player_Control() ||
                    static_cast<BuildingClass *>(object)->TranslucencyLevel != 15 || static_cast<TechnoClass *>(object)->unknown_slot191()) {
                    Clear_Selected_Objects_00482bc0();
                    if (Unknown_006F76B0()) Unknown_006F71E0(object->Class_Of()->ID);
                    else object->unknown_slot80();
                    Set_Default_Mouse(0, small_mouse);
                    TechnoClass::Reset_Action_Line_Timer();
                }
            }
            if (action == 0x3d) {
                CoordStruct point = cell.As_Coord(Map[cell].Height * unknown_height_step_852948);
                BeaconManager.Unknown_0042E740(point);
            } else if (action != 0 && action != 7) {
                bool allowed = true;
                if (action != 8 && action != 10 && action != 12 && action != 13 && action != 14 && action != 15 &&
                    action != 0x21 && action != 0x22 && action != 0x14 && action != 0x25 && action != 0x26 &&
                    action != 0x27 && action != 0x28 && action != 0x29 && action != 0x41 && action != 0x3a && action != 0x3c) {
                    bool execute = Unknown_Planning_00615DE0();
                    bool add = Planning_Can_Add();
                    allowed = execute && add;
                }
                if (allowed) {
                    Active_Click_0049E870(object, cell, action);
                    TechnoClass::Reset_Action_Line_Timer();
                    if (action == 0x21) {
                        if (object->Kind_Of() == 6) {
                            if (static_cast<BuildingClass *>(object)->IsOn)
                                OutList.Add(EventClass(PlayerPtr->ArrayIndex, 2, TargetClass(object)));
                            else OutList.Add(EventClass(PlayerPtr->ArrayIndex, 1, TargetClass(object)));
                        }
                    } else if (action == 0x3c) {
                        if (MuteSWLaunches == 0) {
                            CoordStruct point = cell.As_Coord(Map[cell].Height * unknown_height_step_852948);
                            BeaconManager.Unknown_0042E370(PlayerPtr->ArrayIndex, point, -1);
                        }
                        Map.Unknown_0049CAC0(0);
                        Set_Default_Mouse(0, small_mouse);
                    } else if (action == 10) {
                        if (object->Kind_Of() == 6) OutList.Add(EventClass(PlayerPtr->ArrayIndex, 0x15, TargetClass(object)));
                    } else if (action == 13) {
                        if (object) {
                            int kind = object->Kind_Of();
                            if (kind > 0 && kind <= 2) OutList.Add(EventClass(PlayerPtr->ArrayIndex, 0x16, TargetClass(object)));
                        }
                    } else if (action == 12) {
                        if (object) OutList.Add(EventClass(PlayerPtr->ArrayIndex, 0x16, TargetClass(object)));
                        else OutList.Add(EventClass(PlayerPtr->ArrayIndex, static_cast<unsigned char>(0x17), cell));
                    }
                    SuperWeaponTypeClass *type = SuperWeaponTypeClass::From_Action(action);
                    if (type) OutList.Add(EventClass(PlayerPtr->ArrayIndex, static_cast<unsigned char>(0x12), type->ArrayIndex, cell));
                }
            }
            IsTentative = false;
        }
    }
    Unknown_Click_End_006F6770();
}


// Not exact (RA2_NON_MATCHING): DisplayClass::unknown_slot46 at 0x0049B0F0, best score 60.15, 2026-11-13; differs: 751 instructions in the original, 928 in the draft (shape 85.77)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x49B0F0 FUN_0049b0f0
// entry 0x49B0F0; exact-match score 62.84 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout926 (build/class-work/Scout926/w/mu2.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// void DisplayClass::unknown_slot46(Cell const &, bool, ObjectClass *, int, bool) (0x0049B0F0)
// The mouse over the map with no button held (TibSun DisplayClass::Mouse_Left_Up).


extern ConvertClass *unknown_drawer_8323c8;     // 0x008323C8
// Two mouse shapes chosen by functions (0x006F6740 returns 0x16, 0x006F6730 0x47).
int Unknown_006F6740();
int Unknown_006F6740b();
int Unknown_006F6730();

void DisplayClass::unknown_slot46(Cell const &cell, bool shadow, ObjectClass *object, int action, bool small_mouse)
{
    IsTentative = false;

    AbstractClass *target = 0;
    if (object != 0) {
        target = object;
    } else {
        if (cell != unknown_geometry_k) {
            target = (AbstractClass *)&Map[cell];
        }
    }
    if (object != 0) {
        TechnoClass *techno = object->As_Techno();
        if (techno != 0 && (techno->Kind_Of() != 6 || !((BuildingClass *)techno)->Type->InvisibleInGame) && !Debug_Map) {
            techno->unknown_380 = true;
        }
    }

    if (IsWaypointMode && DraggedWaypoint != 0) {
        if (PlayerPtr->Waypoint_At(cell) == 0 && Map.Unknown_0055A660(cell, true)) {
            *DraggedWaypoint = Cell_Coord(cell);
            DraggedWaypoint->Z = Map.Get_Height_GL(*DraggedWaypoint) + ((Map[cell].unknown_140 & 0x100) ? unknown_geometry_j : 0);
        }
    }

    int waypoint_action = Action_To_Waypoint_Action(action, cell);

    if (WaypointColor == RGBClass(0, 0, 0)) {
        unsigned short color = ((unsigned short *)unknown_drawer_8323c8->Table)[1];
        WaypointColor = Unpack_Hicolor_Pixel(color);
    }

    switch (waypoint_action) {
    case 0x2A:
    case 0x2B:
    case 0x2F:
        Update_Waypoint_Color(PlayerPtr->SelectedPath);
        break;

    case 0x2C:
    case 0x2D:
    case 0x2E:
    case 0x31:
    case 0x32:
    case 0x33: {
        int path = -1;
        char id;
        WaypointClass *waypoint = PlayerPtr->Waypoint_At(cell);
        PlayerPtr->Fetch_Waypoint_Data(waypoint, &path, &id);
        if (!IsWaypointMode) {
            PlayerPtr->SelectedPath = path;
        }
        Update_Waypoint_Color(path);
        break;
    }

    default: {
        unsigned short *table = (unsigned short *)unknown_drawer_8323c8->Table;
        table[1] = Build_Hicolor_Pixel(WaypointColor.Red, WaypointColor.Green, WaypointColor.Blue);
        Update_Waypoint_Color(-1);
        if (!IsWaypointMode) {
            PlayerPtr->SelectedPath = -1;
        }
        break;
    }
    }

    waypoint_action = Unknown_006D8F60(waypoint_action, (int)&cell, (int)object);

    if (shadow) {
        switch (waypoint_action) {
        case 0x1F:
        case 0x24:
            Set_Default_Mouse(0x1A, small_mouse);
            break;
        case 0x1A:
            Set_Default_Mouse(Unknown_006F6740(), small_mouse);
            break;
        case 10:
        case 0xF:
        case 0x20:
            Set_Default_Mouse(0x23, small_mouse);
            break;
        case 0x32:
            Set_Default_Mouse(0x3C, small_mouse);
        case 0x3C:
            Set_Default_Mouse(0x4E, small_mouse);
            break;
        case 2:
            if (CurrentObject.Count() && CurrentObject[0] != 0 && CurrentObject[0]->AbstractFlags.IsTechno && CurrentObject[0]->Techno_Type_Class()->MoveToShroud) {
                Set_Default_Mouse(0x12, small_mouse);
                break;
            }
            Set_Default_Mouse(0x13, small_mouse);
            break;
        case 1:
        case 5:
            Set_Default_Mouse(0x12, small_mouse);
            break;
        case 0xC:
        case 0xD:
        case 0xE:
            Set_Default_Mouse(0x20, small_mouse);
            break;
        case 0x11:
        case 0x1B:
        case 0x21:
        case 0x22:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F:
        case 0x31:
        case 0x33:
            Set_Default_Mouse(0x3C, small_mouse);
            break;
        case 0x1E:
            Set_Default_Mouse(0x1C, small_mouse);
            break;
        case 0x39:
            Set_Default_Mouse(0x3B, small_mouse);
            break;
        case 0x3D:
            Set_Default_Mouse(0x11, small_mouse);
            break;
        case 0x14:
            Set_Default_Mouse(0x35, small_mouse);
            break;
        case 0x25:
            Set_Default_Mouse(0x39, small_mouse);
            break;
        case 0x26:
            Set_Default_Mouse(0x32, small_mouse);
            break;
        case 0x27:
        case 0x28:
            Set_Default_Mouse(0x3A, small_mouse);
            break;
        case 0x29:
        case 0x41:
            Set_Default_Mouse(0x2F, small_mouse);
            break;
        case 0x3E:
        case 0x3F:
            Set_Default_Mouse(Unknown_006F6730(), small_mouse);
            break;
        case 0:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
        case 0xB:
        case 0x10:
        case 0x1C:
        case 0x1D:
        case 0x23:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
        case 0x40:
            Set_Default_Mouse(0, small_mouse);
            break;
        default:
            Set_Default_Mouse(0, small_mouse);
            break;
        }
    } else {
        switch (waypoint_action) {
        case 0x11:
        case 0x1B:
        case 0x21:
        case 0x22:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F:
        case 0x31:
        case 0x32:
        case 0x33:
            Set_Default_Mouse(0x3C, small_mouse);
            break;
        case 0x1E:
            Set_Default_Mouse(0x1C, small_mouse);
            break;
        case 0x39:
            Set_Default_Mouse(0x3B, small_mouse);
            break;
        case 0x1D:
            Set_Default_Mouse(0x21, small_mouse);
            break;
        case 7:
        case 8:
        case 0x3D:
            Set_Default_Mouse(0x11, small_mouse);
            break;
        case 0x1A:
            Set_Default_Mouse(Unknown_006F6740b(), small_mouse);
            break;
        case 1:
            Set_Default_Mouse(0x12, small_mouse);
            break;
        case 5:
            if (target != 0 && CurrentObject.Count() == 1 && CurrentObject[0] != 0 && CurrentObject[0]->AbstractFlags.IsTechno && ((TechnoClass *)CurrentObject[0])->unknown_slot213(target)) {
                Set_Default_Mouse(0x14, small_mouse);
                break;
            }
            // fall into the next case
        case 6:
            Set_Default_Mouse(0x15, small_mouse);
            break;
        case 3:
        case 9:
        case 0xB:
        case 0x23:
            Set_Default_Mouse(0x19, small_mouse);
            break;
        case 0xE:
            Set_Default_Mouse(0x20, small_mouse);
            break;
        case 4:
        case 0x34:
            Set_Default_Mouse(0x1B, small_mouse);
            break;
        case 0x35:
            Set_Default_Mouse(0x26, small_mouse);
            break;
        case 2:
        case 0x36:
            Set_Default_Mouse(0x13, small_mouse);
            break;
        case 0x10:
        case 0x37:
        case 0x38:
        case 0x40:
            Set_Default_Mouse(0x34, small_mouse);
            break;
        case 10:
            Set_Default_Mouse(0x22, small_mouse);
            break;
        case 0xD:
            Set_Default_Mouse(0x1F, small_mouse);
            break;
        case 0xC:
            Set_Default_Mouse(0x1E, small_mouse);
            break;
        case 0xF:
        case 0x20:
            Set_Default_Mouse(0x23, small_mouse);
            break;
        case 0x1F:
            Set_Default_Mouse(0x1A, small_mouse);
            break;
        case 0x14:
            Set_Default_Mouse(0x35, small_mouse);
            break;
        case 0x25:
            Set_Default_Mouse(0x39, small_mouse);
            break;
        case 0x26:
            Set_Default_Mouse(0x32, small_mouse);
            break;
        case 0x27:
        case 0x28:
            Set_Default_Mouse(0x3A, small_mouse);
            break;
        case 0x29:
        case 0x41:
            Set_Default_Mouse(0x2F, small_mouse);
            break;
        case 0x3C:
            Set_Default_Mouse(0x4E, small_mouse);
            break;
        case 0x3E:
        case 0x3F:
            Set_Default_Mouse(Unknown_006F6730(), small_mouse);
            break;
        case 0x1C:
            break;
        default:
            Set_Default_Mouse(0, small_mouse);
            break;
        }
    }
}
#endif

// void __fastcall Unknown_0049C3F0(ObjectClass *object) (0x0049C3F0)
// The selection band's function for each object in it (DisplayClass slot 48
// passes it to Tactical 0x006AA610): selects one of the player's selectable
// objects; a building only when its type can undeploy (+0x3CC) and is not one
// of the rules' BuildConst types. A successful selection clears 0x007D88D6.

void __fastcall Unknown_0049C3F0(ObjectClass *object)
{
    HouseClass *house = object->Owning_House();
    BuildingClass *building = object->Kind_Of() == 6 ? (BuildingClass *)object : 0;
    if (house != 0 && house->Is_Player_Control() && object->CanBeSelected()) {
        if (building != 0) {
            BuildingTypeClass *type = building->Type;
            if (type->UndeploysInto == 0) {
                return;
            }
            for (int index = 0; index < Rules->BuildConst.Count(); index++) {
                if (Rules->BuildConst[index] == type) {
                    return;
                }
            }
        }
        if (!object->IsInLimbo && object->unknown_slot80()) {
            AllowVoice = false;
        }
    }
}


// void DisplayClass::Mouse_Left_Press(Point2D const &xy) (0x0049C490, slot 45)
// The left mouse button went down at xy (hypothesis: Westwood's Mouse_Left_Press):
// with no mode active (+0x11AC, +0x11AE, +0x11AD clear, +0x11B4 -1) and nothing
// waiting for placement (+0x11A4), a selection band is anchored there (+0x11CC,
// its start +0x11D0 and last point +0x11D8).

void DisplayClass::Mouse_Left_Press(Point2D const &xy)
{
    if (!IsRepairMode && !IsPowerMode && !IsSellMode && IsTargettingMode == -1 && PendingObject == 0) {
        IsTentative = true;
        unknown_11d0 = xy;
        unknown_11d8 = xy;
    }
}


// void DisplayClass::Mouse_Left_Held(Point2D const &xy) (0x0049C500, slot 47)
// The left mouse button held at xy (hypothesis: Westwood's Mouse_Left_Held):
// a band being dragged follows the point, clamped to the tactical area; a
// press that has moved more than 4 pixels from where it started begins the
// band. Neither happens in the waypoint mode (+0x11AF) or the mode +0x11B0.

void DisplayClass::Mouse_Left_Held(Point2D const &xy)
{
    if (IsRubberBand && !IsWaypointMode && !IsBeaconMode) {
        Rect area = TacticalRect;
        int x = xy.X;
        int y = xy.Y;
        if (x < 0) {
            x = 0;
        }
        if (x >= area.Width) {
            x = area.Width - 1;
        }
        if (y < 0) {
            y = 0;
        }
        if (y >= area.Height) {
            y = area.Height - 1;
        }
        Point2D point(x, y);
        if (point != unknown_11d8) {
            TacticalMap->IsToRedraw = true;
            TacticalMap->Modify_Rubber_Band(point);
        }
    } else if (IsTentative) {
        Point2D const &start = unknown_11d0;
        Point2D delta(xy.X - start.X, xy.Y - start.Y);
        if ((int)unknown_sqrt_4ba410((double)delta.X * delta.X + (double)delta.Y * delta.Y) > 4) {
            IsRubberBand = true;
            IsTentative = false;
            if (!IsWaypointMode && !IsBeaconMode) {
                TacticalMap->IsToRedraw = true;
                TacticalMap->Start_Rubber_Band(start);
                Map.Override_Mouse_Shape(0, false);
            }
        }
    }
}


void DisplayClass::Compute_Start_Pos()
{
    CoordStruct center(0,0,0);
    int count=0;
    for (int i=0;i<Technos.Count();i++) {
        TechnoClass *tech=Technos[i];
        if (!tech->IsInLimbo && tech->unknown_372) {
            center += Object_Coord(tech);
            count++;
        }
    }
    center.Z=0;
    if (count) center/=count;
    else {
        center.X=PlayerPtr->BaseCenter.X;
        center.Y=PlayerPtr->BaseCenter.Y;
    }
    unknown_id_source_a3d290->Views[0]=unknown_id_source_a3d290->Views[1]=unknown_id_source_a3d290->Views[2]=unknown_id_source_a3d290->Views[3]=Cell(center);
    unknown_id_source_a3d290->AltHomeCell=unknown_id_source_a3d290->HomeCell;
    if (TacticalMap) TacticalMap->Set_Tactical_Position_006a66b0(center);
}


// void DisplayClass::Unknown_0049C7C0(int control) (0x0049C7C0; hypothesis: Sell_Mode_Control)
// Switches the mode at +0x11AD (-1 toggles, 0 off, 1 on) unless +0x11A4
// is set: resets the mouse and the other modes, then enters the mode when
// the player's +0x230 is positive (0x00482BC0), else leaves it and
// restores the mouse shape.

void Unselect_All();

void DisplayClass::Unknown_0049C7C0(int control)
{
    bool mode = IsSellMode;
    switch (control) {
    case -1:
        mode = !IsSellMode;
        break;
    case 0:
        mode = false;
        break;
    case 1:
        mode = true;
        break;
    }
    if (mode != IsSellMode && !PendingObject) {
        Set_Default_Mouse(0, false);
        IsRepairMode = false;
        IsPowerMode = false;
        IsWaypointMode = false;
        IsBeaconMode = false;
        if (mode && PlayerPtr->unknown_230 > 0) {
            IsSellMode = true;
            Unselect_All();
        } else {
            IsSellMode = false;
            Revert_Mouse_Shape();
        }
    }
}


// void DisplayClass::Unknown_0049C860(int control, bool keep) (0x0049C860)
// The waypoint-mode switch (+0x11AF; -1 toggles, 0 off, 1 on) unless +0x11A4
// is set: clears the other modes, then enters waypoint mode with a free path
// (or, with keep, the player's current path, +0x154), else leaves it;
// leaving restores the coordinate at +0x11BC to where +0x11B8 points and
// shows the mouse again.

void Unselect_All();

void DisplayClass::Unknown_0049C860(int control, bool keep)
{
    bool mode = IsWaypointMode;
    switch (control) {
    case -1:
        mode = !mode;
        break;
    case 0:
        mode = false;
        break;
    case 1:
        mode = true;
        break;
    }
    if (mode != IsWaypointMode && !PendingObject) {
        IsRepairMode = false;
        IsSellMode = false;
        IsPowerMode = false;
        IsBeaconMode = false;
        int path = PlayerPtr->Unknown_004F30D0();
        if (mode) {
            if (path == -1 && (!keep || PlayerPtr->SelectedPath == -1)) {
                IsWaypointMode = false;
                Revert_Mouse_Shape();
                DraggedWaypoint = 0;
                return;
            }
            IsWaypointMode = true;
            if (!keep) {
                PlayerPtr->SelectedPath = path;
            }
            Unselect_All();
            return;
        }
        IsWaypointMode = false;
        Revert_Mouse_Shape();
        if (DraggedWaypoint) {
            *DraggedWaypoint = *(CoordStruct *)&unknown_11bc;
            DraggedWaypoint = 0;
            WWMouse->Show_Mouse();
        }
    }
}


// void DisplayClass::Unknown_0049C980(int control) (0x0049C980)
// Switches the mode at +0x11AE (-1 toggles, 0 off, 1 on) unless +0x11A4
// is set: resets the mouse and the other modes, then enters the mode when
// the player's +0x230 is positive (0x00482BC0), else leaves it and
// restores the mouse shape.

void Unselect_All();

void DisplayClass::Unknown_0049C980(int control)
{
    bool mode = IsPowerMode;
    switch (control) {
    case -1:
        mode = !IsPowerMode;
        break;
    case 0:
        mode = false;
        break;
    case 1:
        mode = true;
        break;
    }
    if (mode != IsPowerMode && !PendingObject) {
        Set_Default_Mouse(0, false);
        IsRepairMode = false;
        IsSellMode = false;
        IsWaypointMode = false;
        IsBeaconMode = false;
        if (mode && PlayerPtr->unknown_230 > 0) {
            IsPowerMode = true;
            Unselect_All();
        } else {
            IsPowerMode = false;
            Revert_Mouse_Shape();
        }
    }
}


// void DisplayClass::Unknown_0049CA20(int control) (0x0049CA20; hypothesis: Repair_Mode_Control)
// Switches the mode at +0x11AC (-1 toggles, 0 off, 1 on) unless +0x11A4
// is set: clears the other modes, resets the mouse, then enters the mode
// when the player's +0x230 is positive (0x00482BC0), else leaves it and
// restores the mouse shape.

void Unselect_All();

void DisplayClass::Unknown_0049CA20(int control)
{
    bool mode = IsRepairMode;
    switch (control) {
    case -1:
        mode = !IsRepairMode;
        break;
    case 0:
        mode = false;
        break;
    case 1:
        mode = true;
        break;
    }
    if (mode != IsRepairMode && !PendingObject) {
        IsSellMode = false;
        IsPowerMode = false;
        IsWaypointMode = false;
        IsBeaconMode = false;
        Set_Default_Mouse(0, false);
        if (mode && PlayerPtr->unknown_230 > 0) {
            IsRepairMode = true;
            Unselect_All();
        } else {
            IsRepairMode = false;
            Revert_Mouse_Shape();
        }
    }
}


// void DisplayClass::Unknown_0049CAC0(int control) (0x0049CAC0)
// The mode switch for +0x11B0 (-1 toggles, 0 off, 1 on) unless +0x11A4 is
// set. The requested state starts from +0x11AC (as written in the
// original); turning on also needs 0x0042E700 to allow the player's house.
// Clears the other modes and resets the mouse, then enters the mode
// (0x00482BC0) or leaves it and restores the mouse shape.

void Unselect_All();

void DisplayClass::Unknown_0049CAC0(int control)
{
    bool mode = IsRepairMode;
    switch (control) {
    case -1:
        mode = !mode;
        break;
    case 0:
        mode = false;
        break;
    case 1:
        mode = true;
        break;
    }
    if (mode != IsBeaconMode && !PendingObject) {
        if (mode && !BeaconManager.Unknown_0042E700(PlayerPtr->ArrayIndex)) {
            mode = false;
        }
        IsSellMode = false;
        IsPowerMode = false;
        IsWaypointMode = false;
        IsRepairMode = false;
        Set_Default_Mouse(0, false);
        if (mode) {
            IsBeaconMode = true;
            Unselect_All();
        } else {
            IsBeaconMode = false;
            Revert_Mouse_Shape();
        }
    }
}


// CoordStruct DisplayClass::Unknown_0049CB70(CoordStruct const &coord, bool any) (0x0049CB70)
// The free infantry spot of the map's cell at a coordinate; the last
// argument is set when the cell has the bridge flag (+0x140 bit 8) and the
// coordinate lies at or above the bridge deck (the cell's level plus four,
// scaled).

CoordStruct DisplayClass::Unknown_0049CB70(CoordStruct const &coord, bool any)
{
    CellClass &cell = Map[coord];
    return cell.Closest_Free_Spot(coord, any,
                                  (cell.unknown_140 & 0x100) && coord.Z >= (cell.Height + 4) * unknown_geometry_g ? 1 : 0);
}


// bool DisplayClass::Unknown_0049CBF0(CoordStruct const &coord, bool alt) (0x0049CBF0)
// Asks the map's cell at a coordinate whether the sub-cell spot of the
// coordinate is free (0x00476460), alt being its second argument.

int __fastcall Unknown_004763D0(CoordStruct const &coord);

bool DisplayClass::Unknown_0049CBF0(CoordStruct const &coord, bool alt)
{
    return Map[coord].Is_Spot_Free(Unknown_004763D0(coord), alt);
}


// void DisplayClass::Encroach_Shadow() (0x0049CC20)
// Over all 512 x 512 cells on the map: marks the visible ones that are not
// fully shrouded (+0x140 bit 5); then unmaps each marked cell through
// 0x0049CF00 (clearing the mark), runs 0x0049E110 (false, false) and redraws.

void DisplayClass::Encroach_Shadow()
{
    int x;
    int y;
    for (y = 0; y < 512; y++) {
        for (x = 0; x < 512; x++) {
            if (In_Radar(Cell(x, y))) {
                CellClass &cell = (*this)[Cell(x, y)];
                if (!(cell.unknown_12c & 0x10) && (cell.unknown_12c & 8)) {
                    cell.unknown_140 |= 0x20;
                }
            }
        }
    }
    for (y = 0; y < 512; y++) {
        for (x = 0; x < 512; x++) {
            Cell where(x, y);
            if (In_Radar(where) && ((*this)[where].unknown_140 & 0x20)) {
                (*this)[where].unknown_140 &= ~0x20;
                Unknown_0049CF00(where);
            }
        }
    }
    All_To_Look(false, false);
    Flag_To_Redraw(true);
}


// void DisplayClass::Encroach_Fog() (0x0049CD20)
// Re-maps the map: marks every mapped cell that is not fully shrouded
// (+0x140 bit 6), runs 0x0049E110 (false, true), then maps each marked cell
// again through 0x0049CDB0, clearing the mark.

void DisplayClass::Encroach_Fog()
{
    Reset_Iterator();
    for (CellClass *cell = Iterate(); cell != 0; cell = Iterate()) {
        if (!(cell->unknown_140 & 1) && (cell->unknown_140 & 2)) {
            cell->unknown_140 |= 0x40;
        }
    }
    All_To_Look(false, true);
    Reset_Iterator();
    for (CellClass *marked = Iterate(); marked != 0; marked = Iterate()) {
        if (marked->unknown_140 & 0x40) {
            marked->unknown_140 &= ~0x40;
            Cell where = marked->MapCoords;
            Unknown_0049CDB0(where);
        }
    }
}


// void DisplayClass::Unknown_0049CDB0(Cell const &cell) (0x0049CDB0)
// Unmaps a cell on the map for layer 1: clears its mapped bits, sets its
// layer-1 frame to 0xFE and redraws it when visible. Each neighbour the
// tactical view reports as -2 while its frame is not 0xFE is unmapped in
// turn; one that is fully shrouded or whose frame changed takes the new
// frame (when both are at least -1/0) and is marked mapped. A cell that was
// mapped then runs 0x0047BC80.

void DisplayClass::Unknown_0049CDB0(Cell const &cell)
{
    if (!In_Radar(cell)) {
        return;
    }
    CellClass &target = (*this)[cell];
    bool mapped = false;
    if (target.unknown_140 & 3) {
        mapped = true;
    }
    target.unknown_140 &= ~3;
    target.FogFrame = 0xFE;
    if (target.unknown_12c & 8) {
        TacticalMap->Flag_Cell(&target);
    }
    for (int facing = 0; facing < 8; facing++) {
        Cell adjacent = Adjacent_Cell(cell, facing);
        CellClass &next = (*this)[adjacent];
        Cell where = next.MapCoords;
        int frame = TacticalMap->Cell_Shadow(where, 1);
        if (frame == -2 && (char)next.FogFrame != (char)frame) {
            Unknown_0049CDB0(adjacent);
        } else if ((next.unknown_140 & 1) || frame != (char)next.FogFrame) {
            if (frame >= 0 && (char)next.FogFrame >= -1) {
                next.FogFrame = frame;
                next.unknown_140 = (next.unknown_140 & ~1) | 2;
                TacticalMap->Flag_Cell(&next);
            }
        }
    }
    if (mapped) {
        target.Fog_Cell();
    }
}


// void DisplayClass::Unknown_0049CF00(Cell const &cell) (0x0049CF00)
// Unmaps a cell on the map for layer 0: a visible cell loses its visible
// and fully-shrouded bits (+0x12C bits 3 and 4) and is redrawn, and its
// neighbours lose the fully-shrouded bit and are redrawn.

void DisplayClass::Unknown_0049CF00(Cell const &cell)
{
    if (!In_Radar(cell)) {
        return;
    }
    CellClass &target = (*this)[cell];
    if (!(target.unknown_12c & 8)) {
        return;
    }
    target.unknown_12c &= ~0x18;
    TacticalMap->Flag_Cell(&target);
    for (int facing = 0; facing < 8; facing++) {
        Cell adjacent = Adjacent_Cell(cell, facing);
        CellClass &next = (*this)[adjacent];
        if (adjacent != cell) {
            next.unknown_12c &= ~0x10;
            TacticalMap->Flag_Cell(&next);
        }
    }
}


// void DisplayClass::Read_INI_0049CFD0(CCINIClass &ini) (0x0049CFD0, slot 32)
// Reads the map's sections of a scenario INI (TibSun DisplayClass::Read_INI): [Map] Size,
// Level, Fill and Theater, the theater's type data, the waypoints and home view, [CellTags],
// the IsoMapPack..5 blocks through a 640x400 staging surface, then the tiles and LocalSize.

void __fastcall IsometricTileTypeClass_Read_Control_File(int theater, bool arg);
void Unknown_0052B650();
void __fastcall Unknown_0052B2E0(bool arg1, bool arg2);
extern "C" int __cdecl atoi(char const *string);

void DisplayClass::Read_INI_0049CFD0(CCINIClass &ini)
{
    char const * const name = "Map";

    INIRect defsize;
    defsize.X = 1;
    defsize.Y = 1;
    defsize.Width = 50;
    defsize.Height = 50;
    INIRect rect = ini.Get_Rect(name, "Size", defsize);

    unknown_slot24();

    Set_Map_Dimensions((Rect const *)&rect, true, false, true);

    int cell_height = ini.Get_Int(name, "Level", 0);

    char fill[32];
    int ittype = IsometricTileTypeClass_ClearTile;
    ini.Get_String(name, "Fill", "Clear", fill, sizeof(fill));
    if (Compare_Narrow_00781f20(fill, "Water") == 0) {
        ittype = IsometricTileTypeClass_WaterSet;
    }

    Reset_Iterator();
    CellClass *cptr = Iterate();
    while (cptr != 0) {
        cptr->IsoTileIndex = ittype;
        cptr->SubTile = 0;
        cptr->Height += cell_height;
        cptr = Iterate();
    }

    unknown_id_source_a3d290->Theater = ini.Get_TheaterType(name, "Theater", (TheaterType)0);

    Init(unknown_id_source_a3d290->Theater);

    unknown_session_a3d298.Update_Progress(63);
    Call_Back();

    long uniqueid = unknown_id_source_a3d290->UniqueID;
    TerrainTypeClass::Init(unknown_id_source_a3d290->Theater);
    if (unknown_id_source_a3d290->Theater != LastTheater) {
        IsometricTileTypeClass_Read_Control_File(unknown_id_source_a3d290->Theater, true);
    } else {
        Unknown_0052B650();
    }
    unknown_id_source_a3d290->UniqueID = uniqueid + 10000;
    OverlayTypeClass::Init(unknown_id_source_a3d290->Theater);
    BuildingTypeClass::Init(unknown_id_source_a3d290->Theater);
    AnimTypeClass::Init(unknown_id_source_a3d290->Theater);
    SmudgeTypeClass::Init(unknown_id_source_a3d290->Theater);
    VeinholeMonsterClass::Init(unknown_id_source_a3d290->Theater);

    unknown_session_a3d298.Update_Progress(65);
    Call_Back();

    unknown_id_source_a3d290->Read_Waypoints(ini);

    if (!unknown_id_source_a3d290->Is_Valid_Waypoint(unknown_id_source_a3d290->HomeCell)) {
        unknown_id_source_a3d290->Set_Waypoint_Location(unknown_id_source_a3d290->HomeCell,
            Cell((MapSize.Width + MapSize.Height) / 2, (MapSize.Width + MapSize.Height) / 2));
    }

    Cell homecell = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_id_source_a3d290->Variables[0].Value ? unknown_id_source_a3d290->AltHomeCell : unknown_id_source_a3d290->HomeCell);
    unknown_id_source_a3d290->Views[0] = unknown_id_source_a3d290->Views[1] = unknown_id_source_a3d290->Views[2] = unknown_id_source_a3d290->Views[3] = homecell;
    CoordStruct homecoord;
    homecoord.X = homecell.X * 256 + 128;
    homecoord.Y = homecell.Y * 256 + 128;
    homecoord.Z = 0;
    TacticalMap->Set_Tactical_Position_006a66b0(homecoord);

    char const * const celltags = "CellTags";
    int len = ini.Entry_Count(celltags);
    for (int index = 0; index < len; index++) {
        char const *cellentry = ini.Get_Entry(celltags, index);
        TagTypeClass *tp;
        Cell cell;

        char buffer[128];
        if (ini.Get_String(celltags, cellentry, "", buffer, sizeof(buffer))) {
            tp = TagTypeClass::Find_Or_Make(buffer);
        } else {
            tp = 0;
        }

        int val = atoi(cellentry);

        if (NewINIFormat >= 4) {
            cell = Cell(val % 1000, val / 1000);
        } else {
            cell = Cell(val % 128, val / 128);
        }

        if (tp != 0 && !(*this)[cell].Tag) {
            TagClass *tt = TagClass::Find_Or_Make(tp);
            if (tt) {
                tt->Set_Position(cell);
                (*this)[cell].Attach_Tag(tt);
                TaggedCells.Add(cell);
            }
        }
    }
    unknown_session_a3d298.Update_Progress(67);
    Call_Back();

    BSurface staging_buffer(640, 400, 2);

    int size = staging_buffer.Get_Width() * staging_buffer.Get_Height() * staging_buffer.Bytes_Per_Pixel();

    static char const * const ISOMAPPACK1 = "IsoMapPack";
    len = ini.Get_UUBlock(ISOMAPPACK1, staging_buffer.Lock(), size);
    if (len > 0) {
        BufferStraw bstraw(staging_buffer.Lock(), len);
        Map.Read_Binary_1(bstraw);
        staging_buffer.Unlock();
    }
    staging_buffer.Unlock();

    static char const * const ISOMAPPACK2 = "IsoMapPack2";
    len = ini.Get_UUBlock(ISOMAPPACK2, staging_buffer.Lock(), size);
    if (len > 0) {
        BufferStraw bstraw(staging_buffer.Lock(), len);
        Map.Unknown_0054DEA0(bstraw);
        staging_buffer.Unlock();
    }
    staging_buffer.Unlock();

    static char const * const ISOMAPPACK3 = "IsoMapPack3";
    len = ini.Get_UUBlock(ISOMAPPACK3, staging_buffer.Lock(), size);
    if (len > 0) {
        BufferStraw bstraw(staging_buffer.Lock(), len);
        Map.Read_Binary_3(bstraw);
        staging_buffer.Unlock();
    }
    staging_buffer.Unlock();

    static char const * const ISOMAPPACK4 = "IsoMapPack4";
    len = ini.Get_UUBlock(ISOMAPPACK4, staging_buffer.Lock(), size);
    if (len > 0) {
        BufferStraw bstraw(staging_buffer.Lock(), len);
        Map.Unknown_0054E0C0(bstraw);
        staging_buffer.Unlock();
    }
    staging_buffer.Unlock();

    static char const * const ISOMAPPACK5 = "IsoMapPack5";
    len = ini.Get_UUBlock(ISOMAPPACK5, staging_buffer.Lock(), size);
    if (len > 0) {
        BufferStraw bstraw(staging_buffer.Lock(), len);
        Map.Read_Binary_5(bstraw);
        staging_buffer.Unlock();
    }
    staging_buffer.Unlock();

    unknown_session_a3d298.Update_Progress(68);
    Call_Back();

    Unknown_0052B2E0(Debug_Map || Debug_ForceScenario, unknown_id_source_a3d290->IsRandom);

    unknown_session_a3d298.Update_Progress(69);
    Call_Back();

    (INIRect &)MapLocalSize = ini.Get_Rect(name, "LocalSize", (INIRect const &)MapSize);
    Map.Unknown_00630840(MapLocalSize);
    Map.Get_Max_Level();

    LastTheater = unknown_id_source_a3d290->Theater;
}


// void DisplayClass::Write_INI_0049D910(CCINIClass &ini) (0x0049D910)
// Writes the map's sections of a scenario INI (TibSun DisplayClass::Write_INI): [Map] Theater,
// Size and LocalSize, the waypoints, [CellTags] (one entry x + y * 1000 per tagged cell) and the
// IsoMapPack5 block packed through the surface 0x00839958's memory (IsoMapPack..4 are cleared).

void DisplayClass::Write_INI_0049D910(CCINIClass &ini)
{
    char entry[20];

    static char const * const NAME = "Map";
    ini.Clear(NAME);
    ini.Put_TheaterType(NAME, "Theater", (TheaterType)unknown_id_source_a3d290->Theater);
    ini.Put_Rect(NAME, "Size", (INIRect const &)MapSize);
    ini.Put_Rect(NAME, "LocalSize", (INIRect const &)MapLocalSize);

    unknown_id_source_a3d290->Write_Waypoints(ini);

    static char const * const CELLTRIG = "CellTags";
    ini.Clear(CELLTRIG);
    for (int y = 0; y < 512; y++) {
        for (int x = 0; x < 512; x++) {
            if ((*this)[Cell(x, y)].Tag != 0) {
                TagClass *tp = (*this)[Cell(x, y)].Tag;
                if (tp != 0 && tp->Class != 0) {
                    wsprintfA(entry, "%d", x + (y * 1000));
                    ini.Put_String(CELLTRIG, entry, tp->Class->ID);
                }
            }
        }
    }

    static char const * const MAPPACK1 = "IsoMapPack";
    static char const * const MAPPACK2 = "IsoMapPack2";
    static char const * const MAPPACK3 = "IsoMapPack3";
    static char const * const MAPPACK4 = "IsoMapPack4";
    static char const * const MAPPACK5 = "IsoMapPack5";
    BufferPipe bpipe(AlternateSurface->Lock(), AlternateSurface->Get_Height() * AlternateSurface->Get_Width() * AlternateSurface->Bytes_Per_Pixel());
    int len = Map.Write_Binary_5(bpipe);
    ini.Clear(MAPPACK1);
    ini.Clear(MAPPACK2);
    ini.Clear(MAPPACK3);
    ini.Clear(MAPPACK4);
    ini.Clear(MAPPACK5);
    if (len > 0) {
        ini.Put_UUBlock(MAPPACK5, AlternateSurface->Lock(), len);
        AlternateSurface->Unlock();
    }
    AlternateSurface->Unlock();
}


// int DisplayClass::Unknown_0049DB60(void *buffer, int arg2) (0x0049DB60)
// Packs the map's description into a buffer: the theater, the map and local
// sizes, the waypoints that are set (index and X + Y * 1000), then one
// 21-byte record per cell (coordinates, tile, three bytes, overlay index and
// data, one more byte and the tag's type). Returns the bytes written.

int DisplayClass::Unknown_0049DB60(void *buffer, int arg2)
{
    char *out = (char *)buffer;
    *(int *)out = unknown_id_source_a3d290->Theater;
    out += 4;
    *(Rect *)out = MapSize;
    out += sizeof(Rect);
    *(Rect *)out = MapLocalSize;
    out += sizeof(Rect);
    int *waypoint_count = (int *)out;
    out += 4;
    int waypoints = 0;
    for (int waypoint = 0; waypoint < 101; waypoint++) {
        if (unknown_id_source_a3d290->Is_Valid_Waypoint(waypoint)) {
            *(int *)out = waypoint;
            out += 4;
            *(int *)out = unknown_id_source_a3d290->Get_Waypoint_Location(waypoint).Y * 1000 +
                          unknown_id_source_a3d290->Get_Waypoint_Location(waypoint).X;
            out += 4;
            waypoints++;
        }
    }
    *waypoint_count = waypoints;
    int *cell_count = (int *)out;
    out += 4;
    int cells = 0;
    Reset_Iterator();
    CellClass *cell;
    while ((cell = Iterate()) != 0) {
        *(Cell *)out = cell->MapCoords;
        out += 4;
        *(int *)out = cell->IsoTileIndex;
        out += 4;
        *out = cell->SubTile;
        out += 1;
        *out = cell->Height;
        out += 1;
        *out = cell->Ramp;
        out += 1;
        *(int *)out = cell->OverlayIndex;
        out += 4;
        *out = cell->OverlayData;
        out += 1;
        *out = cell->IsIceGrowthAllowed;
        out += 1;
        int tag_class = 0;
        if (cell->Tag != 0 && cell->Tag->Class != 0) {
            tag_class = (int)cell->Tag->Class;
        }
        *(int *)out = tag_class;
        out += 4;
        cells++;
    }
    *cell_count = cells;
    return out - (char *)buffer;
}


// void DisplayClass::Unknown_0049DCD0(void *buffer) (0x0049DCD0)
// Unpacks what 0x0049DB60 packed: the theater, the map and local sizes, the
// waypoints and the cells' tile, bytes and overlay.

void DisplayClass::Unknown_0049DCD0(void *buffer)
{
    char *in = (char *)buffer;
    unknown_id_source_a3d290->Theater = *(int *)in;
    in += 4;
    MapSize = *(Rect *)in;
    in += sizeof(Rect);
    MapLocalSize = *(Rect *)in;
    in += sizeof(Rect);
    int waypoints = *(int *)in;
    in += 4;
    for (int count = waypoints; count > 0; count--) {
        int waypoint = *(int *)in;
        in += 4;
        int packed = *(int *)in;
        in += 4;
        Cell cell;
        cell.X = packed % 1000;
        cell.Y = packed / 1000;
        unknown_id_source_a3d290->Set_Waypoint_Location(waypoint, cell);
    }
    in += 4;
    Reset_Iterator();
    CellClass *cell;
    while ((cell = Iterate()) != 0) {
        in += 4;
        cell->IsoTileIndex = *(int *)in;
        in += 4;
        cell->SubTile = *in;
        in += 1;
        cell->Height = *in;
        in += 1;
        cell->Ramp = *in;
        in += 1;
        cell->OverlayIndex = *(int *)in;
        in += 4;
        cell->OverlayData = *in;
        in += 1;
        cell->IsIceGrowthAllowed = *in;
        in += 1;
        in += 4;
    }
}


// void DisplayClass::Reveal_Technos(bool skip_buildings, bool arg2) (0x0049DE00)
// Over every techno (Technos) out of limbo (buildings left out when asked):
// the counterpart of 0x0049E000 with slots 262 and 235. The player's own
// with +0x373 set get slot 262 (0, 0, false, 0), the sight around them
// redrawn (0x0054A550 with +0x1F4 + 3) and, as gap generators, slot 235;
// allied buildings, when the rules' +0x142B allows, get slot 262 (0, arg2,
// false, 0) and the redraw; other houses' RevealToAll buildings slot 262
// (0, 0, true, player) and the redraw; other gap generators slot 235.

void DisplayClass::Reveal_Technos(bool skip_buildings, bool arg2)
{
    int count = Technos.Count();
    for (int index = 0; index < count; index++) {
        TechnoClass *techno = Technos[index];
        if (techno == 0 || techno->IsInLimbo) {
            continue;
        }
        if (techno->Kind_Of() == 6 && skip_buildings) {
            continue;
        }
        if (techno->House->Is_Player_Control()) {
            if (techno->unknown_373) {
                techno->unknown_slot262(0, 0, false, 0);
                CoordStruct coord = techno->Coord;
                Map.Unknown_0054A550(coord, 0, techno->unknown_1f4 + 3, 0);
                if (techno->Techno_Type_Class()->GapGenerator) {
                    techno->unknown_slot235();
                }
            }
        } else if (techno->Kind_Of() == 6 && Rules->IsAllyReveal && techno->House->Is_Ally(PlayerPtr)) {
            techno->unknown_slot262(0, arg2, false, 0);
            CoordStruct coord = techno->Coord;
            Map.Unknown_0054A550(coord, 0, techno->unknown_1f4 + 3, 0);
        } else if (techno->Kind_Of() == 6 && techno->Techno_Type_Class()->RevealToAll && !techno->House->Is_Player_Control()) {
            techno->unknown_slot262(0, 0, true, PlayerPtr);
            CoordStruct coord = techno->Coord;
            Map.Unknown_0054A550(coord, 0, techno->unknown_1f4 + 3, 0);
        } else if (techno->Techno_Type_Class()->GapGenerator) {
            techno->unknown_slot235();
        }
    }
}


// void DisplayClass::Update_Technos_Visibility(bool skip_buildings, bool arg2) (0x0049E000)
// Over every techno (Technos) out of limbo (buildings left out when asked):
// the player's own with +0x373 set get slot 263 (0, 0, false, 0) and, as
// gap generators, slot 236; allied buildings, when the rules' +0x142B
// allows, get slot 263 (0, arg2, false, 0); other gap generators slot 236.

void DisplayClass::Update_Technos_Visibility(bool skip_buildings, bool arg2)
{
    int count = Technos.Count();
    for (int index = 0; index < count; index++) {
        TechnoClass *techno = Technos[index];
        if (techno == 0 || techno->IsInLimbo) {
            continue;
        }
        if (techno->Kind_Of() == 6 && skip_buildings) {
            continue;
        }
        if (techno->House->Is_Player_Control()) {
            if (techno->unknown_373) {
                techno->unknown_slot263(0, 0, false, 0);
                if (techno->Techno_Type_Class()->GapGenerator) {
                    techno->unknown_slot236();
                }
            }
        } else if (techno->Kind_Of() == 6 && Rules->IsAllyReveal && techno->House->Is_Ally(PlayerPtr)) {
            techno->unknown_slot263(0, arg2, false, 0);
        } else if (techno->Techno_Type_Class()->GapGenerator) {
            techno->unknown_slot236();
        }
    }
}


// void DisplayClass::All_To_Look(bool skip_buildings, bool arg2) (0x0049E110)
// Over the technos of layer 2 (buildings left out when asked): the player's
// own with +0x373 set get slot 70 (false, arg2); allied buildings, when the
// rules' +0x142B allows, get slot 70 (arg2, 0).

void DisplayClass::All_To_Look(bool skip_buildings, bool arg2)
{
    for (int index = 0; index < Layer[2].Count(); index++) {
        TechnoClass *techno = (TechnoClass *)Layer[2][index];
        if (techno == 0 || !techno->AbstractFlags.IsTechno) {
            continue;
        }
        if (techno->Kind_Of() == 6 && skip_buildings) {
            continue;
        }
        if (techno->House->Is_Player_Control()) {
            if (techno->unknown_373) {
                techno->Look(false, arg2);
            }
        } else if (techno->Kind_Of() == 6 && Rules->IsAllyReveal && techno->House->Is_Ally(PlayerPtr)) {
            techno->Look(arg2, 0);
        }
    }
}


// void DisplayClass::Constrained_Look(CoordStruct const &center, int distance) (0x0049E1D0)
// The player's discovered technos of layer 2 within their sight range plus
// distance of center look around (slot 70); with the rules' +0x142B allied
// buildings do too (TibSun DisplayClass::Constrained_Look).

void DisplayClass::Constrained_Look(CoordStruct const &center, int distance)
{
    for (int index = 0; index < Layer[2].Count(); index++) {
        TechnoClass *techno = Layer[2][index]->As_Techno();
        if (techno != 0) {
            if (techno->House->Is_Player_Control()) {
                if (techno->unknown_373 && Coord_Length(center - techno->Center_Coord()) <= techno->Techno_Type_Class()->Sight * 256 + distance) {
                    techno->Look(false, false);
                }
            } else {
                if (techno->Kind_Of() == 6 && Rules->IsAllyReveal && techno->House->Is_Ally(PlayerPtr)
                    && Coord_Distance(techno->Center_Coord(), center) <= techno->Techno_Type_Class()->Sight * 256 + distance) {
                    techno->Look(false, false);
                }
            }
        }
    }
}


// void DisplayClass::Unknown_0049E3B0() (0x0049E3B0)
// Centers the tactical view on the selected objects: their average
// coordinate, or with more than two of them the average of the others than
// the one farthest from it (TibSun DisplayClass::Center_Map).

void DisplayClass::Unknown_0049E3B0()
{
    int index;

    if (CurrentObject.Count() > 0) {
        CoordStruct center(0, 0, 0);
        for (index = 0; index < CurrentObject.Count(); index++) {
            CoordStruct position = CurrentObject[index]->Coord;
            center += position;
        }

        CoordStruct sum = center;

        center /= CurrentObject.Count();

        if (CurrentObject.Count() > 2) {
            CoordStruct farthest(0, 0, 0);
            int farthest_distance = 0;
            for (index = 0; index < CurrentObject.Count(); index++) {
                CoordStruct position = CurrentObject[index]->Coord;
                UnknownVector3 delta = UnknownVector3(center.X - position.X, center.Y - position.Y, center.Z - position.Z);
                int d = delta.Length();
                if (d > farthest_distance) {
                    farthest = CurrentObject[index]->Coord;
                    farthest_distance = d;
                }
            }

            sum -= farthest;
            center = sum / (CurrentObject.Count() - 1);
        }

        TacticalMap->Set_Tactical_Position_006a66b0(center);
    }
}


// void DisplayClass::Reinit_Cell_Drawers() (0x0049E570)
// After 0x00529720, runs 0x004790F0 (0, 0x10000, 0, 1000, 1000, 1000) on every
// cell of the map with 0x00A6FA2C cleared, then asks for a full redraw.

extern int unknown_int_a6fa2c;      // 0x00A6FA2C

void DisplayClass::Reinit_Cell_Drawers()
{
    Reset_Light_Converts_00529720();
    Map.Reset_Iterator();
    CellClass *cell = Map.Iterate();
    unknown_int_a6fa2c = 0;
    while (cell) {
        cell->Init_Drawer(0, 0x10000, 0, 1000, 1000, 1000);
        cell = Map.Iterate();
    }
    Flag_To_Redraw(true);
}


// void DisplayClass::Unknown_0049E5E0() (0x0049E5E0)
// Runs 0x004798F0 on every cell of the map.

void DisplayClass::Unknown_0049E5E0()
{
    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell; cell = Map.Iterate()) {
        cell->Recalc_Light();
    }
}


// wchar_t const *DisplayClass::Help_Text(int id) (0x0049E610, slot 33)
// Help text for the tactical area (ids 500..900, not while scrolling): the
// cell position when the cursor position display is on, the shadow text
// over unmapped cells, nothing for hidden enemy technos, otherwise the name
// of the object under the cursor (the cell's tiberium for non-technos)
// (TibSun DisplayClass::Help_Text).

wchar_t const *DisplayClass::Help_Text(int id)
{
    static wchar_t _buffer[100];        // 0x00852848 (size a hypothesis)

    if (id < 500 || id > 900 || Is_Scrolling()) {
        return 0;
    }

    Cell cell;
    CoordStruct coord;
    ObjectClass *object;
    bool fog, shadow;

    int left = TacticalRect.X;
    int top = TacticalRect.Y;
    Point2D xy = WWMouse->Get_Mouse_XY();
    Point2D point(xy.X - left, xy.Y - top);
    Map.Resolve_Point(point, cell, coord, object, fog, shadow);

    if (CursorCheat) {
        swprintf(_buffer, L"(%d,%d)", coord.X / 256, coord.Y / 256);
        return _buffer;
    }

    if (!(Map[coord].unknown_12c & 8) && unknown_b263d8) {
        return StringTable::Load_String("TXT_SHADOW", 0, "D:\\Ra2\\Display.CPP", 4976);
    }

    if (object == 0) {
        return 0;
    }
    TechnoClass *techno = object != 0 && object->AbstractFlags.IsTechno ? (TechnoClass *)object : 0;
    if (techno != 0) {
        if (!techno->unknown_372) {
            if (techno->unknown_1b8 == 2 && !Map[techno->Center_Coord()].Is_Sensed(PlayerPtr->ArrayIndex)) {
                return 0;
            }
            if (techno->Techno_Type_Class()->Invisible) {
                return 0;
            }
        }
    }

    if (object != 0) {
        wchar_t const *text = object->unknown_slot35();
        if (techno == 0) {
            text = (*this)[cell].unknown_47a200();
        }
        return text;
    }

    return 0;
}


// void DisplayClass::Reposition_Sidebar() (0x0049E7D0, slot 34)
// Closes the unused window when there is one (TibSun DisplayClass::Reposition_Sidebar
// and its UnusedWindow, win.h).


WIN32_IMPORT BOOL __stdcall CloseWindow(HWND window);
extern HWND UnusedWindow;   // 0x00B263DC

void DisplayClass::Reposition_Sidebar()
{
    if (UnusedWindow) {
        CloseWindow(UnusedWindow);
        UnusedWindow = 0;
    }
}


// HRESULT DisplayClass::Load(IStream *stream) (0x0049E810, slot 30)
// Loads the five display layers.

HRESULT DisplayClass::Load(IStream *stream)
{
    HRESULT result;
    for (int index = 0; index < 5; index++) {
        result = Layer[index].Load(stream);
    }
    return result;
}


// HRESULT DisplayClass::Save(IStream *stream) (0x0049E840, slot 31)
// Saves the five display layers, stopping at the first failure.

HRESULT DisplayClass::Save(IStream *stream)
{
    HRESULT result;
    for (int index = 0; index < 5; index++) {
        result = Layer[index].Save(stream);
        if (FAILED(result)) {
            break;
        }
    }
    return result;
}


// void DisplayClass::Active_Click_0049E870(ObjectClass *object, Cell cell, int action) (0x0049E870)
// TibSun DisplayClass::Active_Click: passes a click on to every selected object. The cell is clipped to the
// map; a waypoint in it sets every selected object's waypoint path and id (ObjectClass slot 99) and clears
// TechnoClass +0x37F for the foot objects. With an object, each selected object acts on it (slot 28 then
// slot 78); without one, actions 1 and 0x33 and action 2 pass the cell twice and every other action passes
// the cell and Cell(-1, -1) (slot 27 then slot 77). The voice flag 0x007D88D6 is cleared after each order
// and set again at the end. RA2 drops TibSun's group-move spreading.

// TibSun ObjectClass::Is_Foot: a byte result even for a null object.
static inline bool Is_Foot(ObjectClass const *object)
{
    bool foot = false;
    if (object) foot = object->AbstractFlags.IsFoot;
    return foot;
}

void DisplayClass::Active_Click_0049E870(ObjectClass *object, Cell cell, int action)
{
    int index;
    int waypoint_path = -1;
    char waypoint_id = 0;

    if (Debug_Map) {
        return;
    }

    cell = Map.Clip_To_Map(cell);
    WaypointClass *waypoint = PlayerPtr->Waypoint_At(cell);
    if (waypoint != 0) {
        PlayerPtr->Fetch_Waypoint_Data(waypoint, &waypoint_path, &waypoint_id);
    }

    for (index = 0; index < CurrentObject.Count(); index++) {
        CurrentObject[index]->Set_Waypoint_Path(waypoint_path, waypoint_id);
        if (Is_Foot(CurrentObject[index])) {
            TechnoClass *techno = (TechnoClass *)CurrentObject[index];
            techno->unknown_37f = false;
        }
    }

    if (object != 0) {
        for (index = 0; index < CurrentObject.Count(); index++) {
            CurrentObject[index]->unknown_slot78(CurrentObject[index]->unknown_slot28(object, false), object, false);
            AllowVoice = false;
        }
    } else if (action == 1 || action == 0x33) {
        for (index = 0; index < CurrentObject.Count(); index++) {
            CurrentObject[index]->unknown_slot77(CurrentObject[index]->What_Action(&cell, false, false), &cell, &cell, false);
            AllowVoice = false;
        }
    } else if (action == 2) {
        int count = CurrentObject.Count();
        for (index = 0; index < count; index++) {
            CurrentObject[index]->unknown_slot77(CurrentObject[index]->What_Action(&cell, false, false), &cell, &cell, false);
            AllowVoice = false;
        }
    } else {
        for (index = 0; index < CurrentObject.Count(); index++) {
            Cell none(-1, -1);
            CurrentObject[index]->unknown_slot77(CurrentObject[index]->What_Action(&cell, false, false), &cell, &none, false);
            AllowVoice = false;
        }
    }
    AllowVoice = true;
}


// int DisplayClass::Action_To_Waypoint_Action(int mouse, Cell const &cell) (0x0049EAD0)
// Over a cell holding one of the player's waypoints, some mouse shapes are
// replaced (1 and 2 by 45, 3, 9 and 11 by 50, 5 and 6 by 49).

int DisplayClass::Action_To_Waypoint_Action(int mouse, Cell const &cell)
{
    if (PlayerPtr->Waypoint_At(cell)) {
        switch (mouse) {
        case 1:
        case 2:
            return 45;
        case 5:
        case 6:
            return 49;
        case 3:
        case 9:
        case 11:
            return 50;
        }
    }
    return mouse;
}


// void DisplayClass::Update_Waypoint_Color(int color) (0x0049EB40)
// Loads eight colours of the palette at 0x00837810 (block color % 12, or
// the first block for -1) into entries 1-8 of the drawer 0x008323C8's
// surface value table, as screen pixels.

extern ConvertClass *unknown_drawer_8323c8;     // 0x008323C8
extern PaletteClass unknown_palette_837810;     // 0x00837810

void DisplayClass::Update_Waypoint_Color(int color)
{
    int base = color == -1 ? 0 : color % 12 * 8;
    unsigned short *table = (unsigned short *)unknown_drawer_8323c8->Table;
    for (int index = 0; index < 8; index++) {
        RGBClass rgb = unknown_palette_837810[index + base];
        table[index + 1] = (unsigned short)Build_Hicolor_Pixel(rgb.Red, rgb.Green, rgb.Blue);
    }
}


// void DisplayClass::Abort_Drag_Select() (0x0049EBF0, slot 35)
// Aborts a drag select (Westwood's Abort_Drag_Select): the tactical view
// drops its band, both band flags clear, the flag 0x00A40D45 is set and the
// default mouse shape returns.

// Set when a drag select is aborted (0x00A40D45; hypothesis).
extern bool drag_select_aborted;

void DisplayClass::Abort_Drag_Select()
{
    if (TacticalMap != 0) {
        TacticalMap->End_Rubber_Band();
    }
    IsRubberBand = false;
    IsTentative = false;
    drag_select_aborted = true;
    Set_Default_Mouse(0, false);
    Debug_Printf("Display: Abort_Drag_Select()\n");
}


// DisplayClass_Object_To_Follow (0x0049EC40): name from mischa85 names.csv

int __fastcall DisplayClass_Object_To_Follow(int display_address)
{
    int result_address;

    if ((*(char *)(display_address + 0x1198) == '\0') || (result_address = *(int *)(display_address + 0x119c), result_address == 0)) {
        result_address = (int)(0);
    }
    return result_address;
}



// void DisplayClass::Unknown_0049EC60(ObjectClass *object) (0x0049EC60; names.csv Set_To_Follow): stores the
// object (+0x119C) and whether there is one (+0x1198).

void DisplayClass::Unknown_0049EC60(ObjectClass *object)
{
    FollowingObjectPtr = object;
    FollowingObject = object != 0;
}


// LayerClass scalar deleting destructor (0x0049EC80, slot 0)
// Compiler generated with Layer[5]'s vtable: the implicit destructor
// (VectorClass<ObjectClass *>'s, its Clear inlined), then operator delete.


// unsigned GadgetClass::Get_ID() const (0x0049ECD0, slot 17)
// Inline in shared/gadget.hpp ({ return 0; }); the copy the linker kept is
// Display.CPP's, emitted with TacButton's vtable.


// DisplayClass::TacticalClass scalar deleting destructor (0x0049ECE0, slot 0)
// Compiler generated with TacButton's vtable: GadgetClass's destructor, then
// operator delete when bit 0 of the flags is set.


class Leaf_49ed00 { public: void get(unsigned int unused_argument_1); };
void Leaf_49ed00::get(unsigned int unused_argument_1) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


// bool MapClass::Is_Scrolling() (0x0049ED10, slot 26)
// Inline in shared/map.hpp ({ return false; }); the copy the linker kept is
// Display.CPP's COMDAT, emitted by this unit's DisplayClass vtable.


// DisplayClass scalar deleting destructor (0x0049ED20, slot 4)
// Compiler generated (implicit destructor): MapClass's destructor, then
// operator delete when bit 0 of the flags is set.


// Pipe scalar deleting destructor (0x0049EDB0, virtual slot 0)
// Compiler generated with the Pipe vtable; the copy the linker kept is this
// unit's (DisplayClass::Write_INI 0x0049D910 builds a BufferPipe).


// BufferPipe scalar deleting destructor (??_G) (0x0049EDD0)
// Compiler generated with the BufferPipe vtable; the copy the linker kept is
// this unit's (DisplayClass::Write_INI 0x0049D910 builds a BufferPipe).
