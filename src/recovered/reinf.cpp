// reinf.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/reinf.json carries the addresses and the rows that
// stay in their own files.


#include "shared/reinforcement.hpp"
#include "shared/team.hpp"
#include "shared/team_type.hpp"
#include "shared/task_force.hpp"
#include "shared/script.hpp"
#include "shared/unit.hpp"
#include "shared/cell.hpp"
#include "shared/aircraft.hpp"
#include "shared/map_view.hpp"
#include "shared/id_source.hpp"
#include "shared/radar_event_class.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/foot.hpp"
#include "shared/ilocomotion.hpp"
#include "shared/tunnel_locomotion.hpp"
#include "shared/dir_type.hpp"
#include "shared/incdec.hpp"
#include "shared/house_adjacent.hpp"


// 0x00639960: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined1 unknown_return_false_00639960(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00639960 {

unsigned char  unknown_return_false_00639960(void)
{
    return 0;
}

}  // namespace seed_00639960


// bool __fastcall Do_Reinforcements(TeamTypeClass *teamtype, int wp) -- 0x00639970 (TS reinf.cpp)
// Creates the team type's group and brings it in at its waypoint (or wp): infantry next to a
// building or transport pop out of it, droppod teams arrive by paradrop plane, others through
// Create_Reinforcement. Allied arrivals mark the radar event cell.

FootClass * __fastcall _Create_Group(TeamTypeClass *teamtype);

static bool _Pop_Group_Out_Of_Object(FootClass *group, TechnoClass *object)
{
    int quantity = 0;

    while (group != 0) {
        TechnoClass *todo = group;
        group = (FootClass *)group->unknown_30;
        todo->unknown_30 = 0;

        switch (object->Kind_Of()) {
            case 6:
                if (object->Exit_Object(todo) != 2) {
                    delete todo;
                } else {
                    object->unknown_slot149(3);
                    ++quantity;
                }
                break;

            case 1:
            case 2:
                object->Cargo.Attach((FootClass *)todo);
                todo->Hidden();
                object->QueueMission(0x10, false);
                ++quantity;
                break;

            default:
                delete todo;
                break;
        }
    }

    return quantity != 0;
}

static bool _Consists_Only_Of_Infantry(FootClass const *first)
{
    while (first != 0) {
        if (first->Kind_Of() != 0xF) {
            return false;
        }
        first = (FootClass const *)first->unknown_30;
    }
    return true;
}

static TechnoClass *_Who_Can_Pop_Out_Of(Cell origin)
{
    CellClass *cellptr = &Map[origin];
    TechnoClass *candidate = 0;

    for (int f = -1; f < 8; f++) {
        CellClass *ptr = cellptr;
        if (f != -1) {
            ptr = &ptr->Adjacent_Cell(f);
        }

        TechnoClass *building = (TechnoClass *)ptr->Cell_Building();
        if (building && building->Strength > 0) {
            candidate = building;
        }

        UnitClass *unit = (UnitClass *)ptr->Cell_Unit();
        if (unit && unit->Strength && unit->Type->Passengers > 0) {
            return unit;
        }
    }
    return candidate;
}

static bool _Create_Paradrop(TeamTypeClass *teamtype, FootClass *object, Cell origin)
{
    Cell dest;
    TechnoTypeClass *ptype = AircraftTypes[AircraftTypeClass::From_Name("PDPLANE")];
    unknown_a40774++;
    AircraftClass *plane = (AircraftClass *)ptype->Create_One_Of(object->House);
    unknown_a40774--;
    if (plane) {
        plane->unknown_32c = true;

        if (teamtype->UseTransportOrigin) {
            dest = teamtype->Transport_Waypoint_Location();
        } else {
            int source = object->House->unknown_128;
            switch (source) {
                case 0:
                case 1:
                case 2:
                case 3:
                    break;

                default:
                    source = 0;
                    break;
            }
            dest = Map.Calculated_Cell(source, unknown_geometry_k, unknown_geometry_k, 4, 1, 0);
        }

        plane->QueueMission(0x1A, false);
        plane->Assign_Destination(0, true);
        plane->unknown_slot220((AbstractClass *)&Map[origin]);

        unknown_a40774++;
        int placed = plane->Unlimbo(&UnknownCoord434820(dest, 0), 0);
        unknown_a40774--;
        if (placed) {
            plane->unknown_5b1 = true;
            plane->Cargo.Attach(object);
            plane->Commence();
            return true;
        } else {
            delete plane;
        }
    }
    return false;
}

bool __fastcall Do_Reinforcements(TeamTypeClass *teamtype, int wp)
{
    if (!teamtype || !teamtype->TaskForce->CountEntries) return false;

    if (teamtype->Script == 0 || teamtype->Script->MissionCount == 0) {
        ScriptMissionClass tmission(5, 0);
        if (teamtype->Script == 0) {
            teamtype->Script = new ScriptTypeClass(0);
        }
        ScriptTypeClass *script = teamtype->Script;
        int count = script->MissionCount;
        if (count < 50) {
            script->MissionList[count] = tmission;
            script->MissionCount++;
        }
    }

    bool atwaypoint = false;
    bool drop_pod = false;
    if (teamtype->Droppod) {
        drop_pod = true;
    }

    FootClass *object = _Create_Group(teamtype);
    object->Team->IsTransient = false;

    if (!object) {
        return false;
    }

    atwaypoint = wp != -1;
    Cell origin;
    if (atwaypoint) {
        origin = unknown_id_source_a3d290->Get_Waypoint_Location(wp);
    } else {
        origin = teamtype->Waypoint_Location();
    }

    if (!drop_pod && origin != unknown_geometry_k && _Consists_Only_Of_Infantry(object)) {
        TechnoClass *candidate = _Who_Can_Pop_Out_Of(origin);
        if (candidate != 0) {
            return _Pop_Group_Out_Of_Object(object, candidate);
        }
    }

    bool onmap = atwaypoint && Map.Unknown_0055A660(origin, true);

    bool ok;
    if (!drop_pod) {
        ok = Create_Reinforcement(teamtype, object, origin, onmap);
    } else {
        ok = _Create_Paradrop(teamtype, object, origin);
    }

    if (ok && teamtype->Owner->Is_Ally(unknown_player_a35db4)) {
        RadarEventClass_unknown_remember_cell(origin);
    }
    return true;
}


// static FootClass * __fastcall _Create_Group(TeamTypeClass *teamtype) -- 0x00639DA0 (TS reinf.cpp)
// Creates a team of the type and its task force's objects; returns the first transport with the
// others loaded into it, or the chain of objects when there is no transport.

FootClass * __fastcall _Create_Group(TeamTypeClass *teamtype)
{
    TeamClass *team = new TeamClass(teamtype, teamtype->Owner, 0);
    if (team != 0) {
        team->IsForcedActive = true;
        team->IsUnderStrength = false;
    }

    bool hasunload = false;
    ScriptTypeClass *script = teamtype->Script;
    int count = script->MissionCount;
    for (int tm = 0; tm < count; tm++) {
        ScriptMissionClass mission = script->MissionList[tm];
        if (mission.Mission == 8) {
            hasunload = true;
            break;
        }
    }

    bool has_air_transport = team->unknown_func_6bd810();

    FootClass *transport = 0;
    FootClass *object = 0;
    for (int index = 0; index < teamtype->TaskForce->CountEntries; index++) {
        TechnoTypeClass *tclass = teamtype->TaskForce->Entries[index].Type;

        for (int sub = 0; sub < teamtype->TaskForce->Entries[index].Amount; sub++) {
            unknown_a40774++;
            FootClass *temp = (FootClass *)tclass->Create_One_Of(teamtype->Owner);
            unknown_a40774--;

            if (temp != 0) {

                if (tclass->Storage > 0 && teamtype->Full) {
                    temp->unknown_294.Add_Amount(tclass->Storage, 0);
                }

                switch (teamtype->VeteranLevel) {
                    case 0:
                        temp->unknown_118.Set_Dumbass(true);
                        break;
                    case 1:
                        break;
                    case 2:
                        temp->unknown_118.Set_Veteran(true);
                        break;
                    case 3:
                        temp->unknown_118.Set_Elite(true);
                        break;
                }

                if (team != 0) {
                    unknown_a40774++;
                    team->Add(temp);
                    unknown_a40774--;
                    temp->unknown_579 = true;
                }

                bool transporter = false;
                if (tclass->Passengers > 0 && teamtype->Full) {
                    if (has_air_transport) {
                        if (temp->Kind_Of() == 2) {
                            transporter = true;
                        } else {
                            transporter = false;
                        }
                    } else {
                        if (temp->Kind_Of() == 1) {
                            transporter = true;
                        } else {
                            transporter = false;
                        }
                    }
                }

                if (transporter) {
                    temp->unknown_30 = transport;
                    transport = temp;
                } else {
                    temp->unknown_30 = object;
                    object = temp;
                }
            }
        }
    }

    if (transport != 0 && object != 0 && teamtype->Full) {
        transport->Cargo.Attach(object);

        if (hasunload && transport->Kind_Of() == 2) {
            transport->unknown_32c = true;
        }
    }

    if (transport == 0 && object == 0) {
        if (team != 0) delete team;
        return 0;
    }

    if (transport == 0) {
        return object;
    }

    return transport;
}


// bool __fastcall Create_Reinforcement(TeamTypeClass *teamtype, FootClass *object, Cell const &xcell,
// bool atwaypoint) -- 0x0063A050 (TS reinf.cpp)
// Places each object of a group at or near a cell: droppod groups are moved in from above, tunnel
// groups burrow up, others unlimbo (aircraft facing in); objects that find no cell are deleted.
// TibSun shapes: the droppod position is the cell converted to a coordinate (`PositionCoord = newcell`),
// the adjacent-cell searches count a FacingType through Adjacent_Cell (INCDEC.H ++).

inline bool _Can_Burrow(FootClass *object)
{
    while (object != 0) {
        TechnoTypeClass const *tclass = object->Techno_Type_Class();
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
        cell = Map.Nearby_Location(cell, object->Techno_Type_Class()->SpeedType, -1, 0, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
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
                object->Set_Coord(CoordStruct(newcell));
                object->Assign_Destination((AbstractClass *)&Map[newcell], true);
                object->Locomotor.Checked()->Move_To(Cell_Coord(newcell));
                object->unknown_slot262(false, false, false, 0);
                CoordStruct coord = object->Coord;
                Map.Unknown_0054A550(coord, 0, object->unknown_1f4 + 3, 0);
                placed = true;
            } else if (burrow) {
                CoordStruct coord = Map[newcell].unknown_475d60() - CoordStruct(0, 0, 400);
                placed = object->Unlimbo(&coord, desiredfacing.As_Dir256());
                if (placed) {
                    object->Set_Coord(object->Position_Coord_004f3f40() + CoordStruct(0, 0, -256 - object->Get_Height()));
                    object->Assign_Destination((AbstractClass *)&Map[newcell], true);
                    object->unknown_slot306(1.0);
                    object->Locomotor.Checked()->Move_To(Cell_Coord(newcell));
                }
            } else {
                placed = object->Unlimbo(&Map[newcell].unknown_475d60(), desiredfacing.As_Dir256());
            }

            if (placed) {
                okvoice = true;

                if (object->Kind_Of() != 2) {
                    object->QueueMission(5, false);
                    object->Commence();
                }

                if (drop_pod) {
                    FacingType adj;
                    for (adj = FACING_N; adj < FACING_COUNT; adj++) {
                        Cell trycell = Adjacent_Cell(newcell, adj);
                        if (Map.In_Radar(trycell)) {
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
                    if (!Map.In_Radar(trycell) && object->Can_Enter_Cell(&Map[trycell], adj, -1, 0, 1) == 0) {
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


// 0x0063a680: function not yet identified
//
// Body: 5 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined1 unknown_return_false_0063a680(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0063a680 {

unsigned char __stdcall unknown_return_false_0063a680(int unused_0, int unused_1, int unused_2)
{
    return 0;
}

}  // namespace seed_0063a680
