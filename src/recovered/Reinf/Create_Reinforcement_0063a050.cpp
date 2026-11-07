// bool __fastcall Create_Reinforcement(TeamTypeClass *teamtype, FootClass *object, Cell const &xcell,
// bool atwaypoint) -- 0x0063A050 (TS reinf.cpp)
// Places each object of a group at or near a cell: droppod groups are moved in from above, tunnel
// groups burrow up, others unlimbo (aircraft facing in); objects that find no cell are deleted.
// TibSun shapes: the droppod position is the cell converted to a coordinate (`PositionCoord = newcell`),
// the adjacent-cell searches count a FacingType through Adjacent_Cell (INCDEC.H ++).
#include "shared/reinforcement.hpp"
#include "shared/foot.hpp"
#include "shared/team_type.hpp"
#include "shared/cell.hpp"
#include "shared/aircraft.hpp"
#include "shared/map_view.hpp"
#include "shared/ilocomotion.hpp"
#include "shared/tunnel_locomotion.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/dir_type.hpp"
#include "shared/incdec.hpp"
#include "shared/house_adjacent.hpp"

inline bool _Can_Burrow(FootClass *object)
{
    while (object != 0) {
        TechnoTypeClass const *tclass = object->unknown_slot32();
        if (!(tclass->Locomotor == CLSID_TunnelLocomotionClass)) {
            return false;
        }
        object = (FootClass *)object->unknown_30;
    }
    return true;
}

bool __fastcall Create_Reinforcement(TeamTypeClass *teamtype, FootClass *object, Cell const &xcell, bool atwaypoint)
{
    bool drop_pod = false;
    if (teamtype->Droppod) {
        drop_pod = true;
    }

    bool burrow = _Can_Burrow(object);
    FacingType eface = FACING_N;
    Cell cell = xcell;

    if (!drop_pod && !burrow && !atwaypoint) {
        int source = teamtype->Owner->unknown_128;
        if (source == -1) {
            source = 0;
        }
        eface = (FacingType)(source << 1);
    } else {
        cell = Map.Unknown_005502E0(cell, object->unknown_slot32()->SpeedType, -1, 0, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
    }

    Cell newcell = cell;
    FootClass *o = (FootClass *)object->unknown_30;
    object->unknown_30 = 0;
    bool okvoice = false;

    if (newcell != unknown_geometry_k) {
        while (newcell != unknown_geometry_k && object != 0) {
            DirType desiredfacing(eface);
            if (object->Kind_Of() == 2) {
                desiredfacing = DirType(DirType(eface).As_Int() - DirType(FACING_SE).As_Int() - 1).Snap_To_8();
            }

            unknown_a40774++;

            bool placed = false;
            if (drop_pod) {
                object->unknown_slot103(CoordStruct(newcell));
                object->unknown_slot260((AbstractClass *)&Map[newcell], 1);
                object->Locomotor.Checked()->Move_To(Cell_Coord(newcell));
                object->unknown_slot262(false, false, false, 0);
                CoordStruct coord = object->Coord;
                Map.Unknown_0054A550(coord, 0, object->unknown_1f4 + 3, 0);
                placed = true;
            } else if (burrow) {
                CoordStruct coord = Map[newcell].unknown_475d60() - CoordStruct(0, 0, 400);
                placed = object->unknown_slot53(&coord, desiredfacing.As_Dir256());
                if (placed) {
                    object->unknown_slot103(object->Position_Coord_004f3f40() + CoordStruct(0, 0, -256 - object->unknown_slot110()));
                    object->unknown_slot260((AbstractClass *)&Map[newcell], 1);
                    object->unknown_slot306(1.0);
                    object->Locomotor.Checked()->Move_To(Cell_Coord(newcell));
                }
            } else {
                placed = object->unknown_slot53(&Map[newcell].unknown_475d60(), desiredfacing.As_Dir256());
            }

            if (placed) {
                okvoice = true;

                if (object->Kind_Of() != 2) {
                    object->unknown_slot116(5, false);
                    object->unknown_slot117();
                }

                if (drop_pod) {
                    FacingType adj;
                    for (adj = FACING_N; adj < FACING_COUNT; adj++) {
                        Cell trycell = Adjacent_Cell(newcell, adj);
                        if (Map.Unknown_0054AAB0(trycell)) {
                            newcell = trycell;
                            break;
                        }
                    }
                    if (adj >= FACING_COUNT) {
                        newcell = unknown_geometry_k;
                    }
                }
            } else {
                FacingType adj;
                for (adj = FACING_N; adj < FACING_COUNT; adj++) {
                    Cell trycell = Adjacent_Cell(newcell, adj);
                    if (!Map.Unknown_0054AAB0(trycell) && object->unknown_slot101(&Map[trycell], adj, -1, 0, 1) == 0) {
                        newcell = trycell;
                        break;
                    }
                }

                if (adj >= FACING_COUNT) {
                    newcell = unknown_geometry_k;
                } else if (newcell != unknown_geometry_k) {
                    unknown_a40774--;
                    continue;
                }

                delete object;
            }

            unknown_a40774--;
            object = o;
            if (object != 0) {
                o = (FootClass *)object->unknown_30;
                object->unknown_30 = 0;
            }
        }
    } else if (object != 0) {
        delete object;
    }

    if (o != 0) {
        while (o != 0) {
            FootClass *old = o;
            o = (FootClass *)o->unknown_30;
            old->unknown_30 = 0;
            delete old;
        }
    }

    return okvoice;
}
