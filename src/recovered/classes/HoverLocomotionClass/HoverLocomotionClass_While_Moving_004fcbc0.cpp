// int HoverLocomotionClass::While_Moving_004fcbc0(bool first_pass) (0x004FCBC0)
// TibSun hover.cpp While_Moving.
#include "shared/hover_locomotion.hpp"
#include "shared/cell_class.hpp"
#include "shared/rules.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/house.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/crt_memory.hpp"
#include "shared/infantry.hpp"
#include "shared/overlay_type.hpp"
#include "shared/tube.hpp"
// @ref _unknown_geometry_l=00a41080

inline CoordStruct Coord_Offset(CoordStruct const &coord, TPoint2D<int> const &offset)
{
    return *(CoordStruct *)&UnknownVector3(coord.X + offset.X, coord.Y + offset.Y, coord.Z);
}

inline CoordStruct Adjacent_Coord(CoordStruct const &coord, int facing)
{
    return Coord_Offset(coord, unknown_facing_offsets_851ac0[facing & 7]);
}

inline int Hover_Distance(CoordStruct const &from, CoordStruct const &to)
{
    UnknownVector3 difference = UnknownVector3(from.X - to.X, from.Y - to.Y, from.Z - to.Z);
    return difference.Length();
}

// The height step per tube direction; the count is read after the caller has built the coordinate.
inline int Tube_Height_Step(CoordStruct const &exit, TubeClass const *tube, int current_height)
{
    int count = tube->unknown_1c0;
    return (Map.Get_Height_GL(exit) - current_height) / count;
}

int HoverLocomotionClass::While_Moving_004fcbc0(bool first_pass)
{
    if (unknown_24 != unknown_geometry_l) {
        LinkedTo->unknown_slot59(unknown_24);
        unknown_24 = unknown_geometry_l;
    }

    if (LinkedTo->unknown_slot202()) {
        return 7;
    }

    if (!Is_Moving() || unknown_18 == unknown_geometry_l) {
        return 0;
    }

    int nextface = LinkedTo->Path[0];

    if (nextface == 8) {
        int tubenum = Map[Object_Coord(LinkedTo)].unknown_116;
        if (tubenum >= 0 && tubenum < unknown_TubeClass_list_008661c8.Count()) {
            LinkedTo->Mark(0);
            LinkedTo->unknown_slot59(Object_Coord(LinkedTo));
            TubeClass *tube = unknown_TubeClass_list_008661c8[tubenum];
            Cell exit = tube->ExitCell;
            unknown_24 = exit.As_Coord();
            memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);
            LinkedTo->unknown_450 = unknown_24.As_Cell();
            LinkedTo->Path[23] = -1;
            LinkedTo->unknown_574 = tubenum;
            LinkedTo->unknown_575 = 0;
            TPoint2D<short> const &enter = tube->EnterCell;
            CoordStruct diff = Map[Adjacent_Cell(Cell(enter), tube->Directions[0])].unknown_475d60() - CoordStruct(Cell(enter));
            LinkedTo->unknown_45c = diff + Object_Coord(LinkedTo);
            int height = LinkedTo->unknown_slot110();
            int current_height = Map.Get_Height_GL(Object_Coord(LinkedTo));
            Cell exit2 = tube->ExitCell;
            int z = height + Tube_Height_Step(exit2, tube, current_height);
            LinkedTo->unknown_45c.Z = z;
        } else {
            LinkedTo->Path[0] = -1;
            unknown_24 = unknown_geometry_l;
            Stop();
            LinkedTo->Assign_Destination(0, true);
        }
        return 7;
    }

    if (nextface != -1) {
        unknown_24 = Cell(LinkedTo->unknown_slot104() + AdjacentCell[nextface & 7]).As_Coord();
        unknown_24.Z = Map.Get_Height_GL(unknown_24);
        if (Object_Coord(LinkedTo).Z >= unknown_24.Z + 2 * unknown_geometry_g + unknown_geometry_g) {
            unknown_24.Z += unknown_geometry_j;
        }

        if (!Map[unknown_24].Goodie_Check(LinkedTo) && !LinkedTo->unknown_76) {
            if (LinkedTo->unknown_7d && !LinkedTo->unknown_7a) {
                LinkedTo->Path[0] = -1;
                unknown_24 = unknown_geometry_l;
                Stop();
                return 7;
            }
            return 7;
        }

        if (!LinkedTo->unknown_7d || LinkedTo->unknown_76 || LinkedTo->unknown_7a) {
            return 7;
        }

        CellClass &adjacent = Map[Adjacent_Coord(Object_Coord(LinkedTo), nextface)];
        if (adjacent.IsUnderBridge ^ LinkedTo->unknown_79) {
            LinkedTo->unknown_57b = true;
        }

        Cell c = unknown_24.As_Cell();
        int ok = LinkedTo->unknown_slot101(&Map[c], nextface, LinkedTo->unknown_5d47d0(), 0, 1);

        switch (ok) {
        case 7:
        case 1:
            Map[unknown_24].unknown_478740();
            if (first_pass) {
                unknown_4fda80();
                LinkedTo->unknown_530 = 0;
                LinkedTo->Path[0] = -1;
                Start_Of_Move_004fe0f0(0);
                ok = While_Moving_004fcbc0(false);
            }
            if (ok != 0) {
                unknown_24 = unknown_geometry_l;
                unknown_4fda80();
            }
            return ok;

        case 3:
            unknown_24 = unknown_geometry_l;
            Map.Unknown_0055ACD0(LinkedTo, c);
            unknown_4fda80();
            break;

        case 6:
            if (first_pass) {
                unknown_24 = unknown_geometry_l;
                unknown_4fda80();
                LinkedTo->unknown_530 = 0;
                LinkedTo->Path[0] = -1;
                Start_Of_Move_004fe0f0(0);
                ok = While_Moving_004fcbc0(false);
                break;
            } else {
                if (Hover_Distance(LinkedTo->Center_Coord(), unknown_18) < Rules->unknown_1380
                    && abs(unknown_18.Z - Object_Coord(LinkedTo).Z) < 2 * unknown_geometry_g
                    && Map[Object_Coord(LinkedTo)].Land_Type() != 10) {
                    Stop_Moving();
                    LinkedTo->Assign_Destination(0, true);
                    return 7;
                }
                bool bridge;
                if (!Map[unknown_24].IsUnderBridge || abs(Object_Coord(LinkedTo).Z / unknown_geometry_g - Map[unknown_24].unknown_11b) <= 2) {
                    bridge = false;
                } else {
                    bridge = true;
                }
                Map[unknown_24].Incoming(&unknown_geometry_l, true, true, bridge);
                unknown_24 = unknown_geometry_l;
                unknown_4fda80();
                break;
            }

        case 0:
            LinkedTo->unknown_slot93(2);
            if (!LinkedTo->unknown_7d || LinkedTo->unknown_76 || LinkedTo->unknown_7a) {
                return 7;
            }
            if (LinkedTo->unknown_32d) {
                LinkedTo->unknown_slot58(unknown_24);
            }
            memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);
            LinkedTo->Path[23] = -1;
            LinkedTo->unknown_450 = unknown_24.As_Cell();
            break;

        case 4:
        case 5:
            unknown_24 = unknown_geometry_l;
            if (first_pass) {
                unknown_4fda80();
                LinkedTo->unknown_530 = 0;
                LinkedTo->Path[0] = -1;
                Start_Of_Move_004fe0f0(0);
                ok = While_Moving_004fcbc0(false);
                break;
            }
            if (Map[c].Cell_Object(Point2D(0, 0), false) != 0) {
                if (!LinkedTo->House->Is_Ally((TechnoClass *)Map[c].Cell_Object(Point2D(0, 0), false))) {
                    LinkedTo->Override_Mission(1, Map[c].Cell_Object(Point2D(0, 0), false), 0);
                }
            } else {
                if (Map[c].OverlayIndex != -1 && OverlayTypes[Map[c].OverlayIndex]->Wall) {
                    LinkedTo->Override_Mission(1, &Map[c], 0);
                }
            }
            break;

        case 2:
            unknown_24 = unknown_geometry_l;
            unknown_4fda80();
            if (!LinkedTo->unknown_5a1) {
                LinkedTo->unknown_5a1 = true;
                LinkedTo->unknown_558 = Rules->unknown_13d0;
            }
            if (LinkedTo->unknown_530 == 0) {
                LinkedTo->Path[0] = -1;
            }
            bool blocked = LinkedTo->unknown_5a1 && LinkedTo->unknown_558 == 0;
            Start_Of_Move_004fe0f0((blocked != 0) + 1);
            break;
        }
        return ok;
    }

    Start_Of_Move_004fe0f0(0);
    return 0;
}
