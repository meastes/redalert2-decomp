// int RTacticalClass::Action(unsigned flags, unsigned &key) (0x0062FE30, slot 31)
// The radar's tactical gadget: a click on the radar picks the cell (and object) under it; with a
// pending action it shows or performs it on the map, otherwise a click recentres the tactical view
// on that cell, clamped to the playable diamond (TibSun RadarClass::RTacticalClass::Action; the
// ra2-decomp YR twin has the same four edge clamps). Draft: Scout704/Scout829, Scout836, Scout894.
// The nested press/cell tests through Cell's operator!= and the CoordStruct(Cell) conversion follow
// the twin (radar.cpp 0x006539D0, Coord(cell)): writing cell.X/cell.Y directly before the clamps makes
// VC6 regroup the last clamp's (X + Y) - bottom_edge as (Y - bottom_edge) + X.
#include "shared/radar.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/tactical.hpp"
#include "shared/techno.hpp"
#include "shared/keyboard.hpp"
#include "shared/mouse.hpp"
#include "shared/object.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/super.hpp"
#include "shared/blit.hpp"
// Declared in shared/scroll.hpp and shared/main_loop.hpp, which this unit cannot include
// (scroll.hpp refuses the light map_view; main_loop.hpp brings always.hpp's min next to the
// unit's own).
ObjectClass *__fastcall Unknown_0051A690(Cell const *cell, ObjectClass *object);
extern bool unknown_flag_a40d45;
extern int unknown_int_8325d0;
extern int unknown_int_8325d4;
// @ref ?unknown_pending_super_833694@@3HA=00833694
// @ref ?unknown_flag_a40d45@@3_NA=00a40d45
// @ref ?unknown_int_8325d0@@3HA=008325d0
// @ref ?unknown_int_8325d4@@3HA=008325d4
// @ref ?ABufferPtr@@3PAVABuffer@@A=00839c6c
// @ref ?unknown_475d60@UnknownCellClass@@QBE?AUCoordStruct@@XZ=00475d60
// @ref ?Unknown_0051A690@@YIPAVObjectClass@@PBVCell@@PAV1@@Z=0051a690

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
        coord.Z = Map.Unknown_0055A280(coord);
        shadow = Map.Unknown_00568140(coord) && unknown_b263d8;
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
                Map.unknown_slot48(CoordStruct(cell), cell, object, action, true);
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
                    unknown_tactical_83996c->Unknown_006A66B0(center);
                    Map.Flag_To_Redraw(1);
                    if (ABufferPtr != 0) {
                        ABufferPtr->MaxValue = 0x8000;
                    }
                }
            }
        }
    }
    GadgetClass::Action(0, key);
    return true;
}
