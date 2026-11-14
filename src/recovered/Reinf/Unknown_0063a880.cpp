// Not exact (RA2_NON_MATCHING): FUN_0063a880 at 0x0063A880, best score 88.45, 2026-11-13; differs: same instruction count (485), shape 98.97: instruction choice and registers differ
#ifdef RA2_NON_MATCHING
// bool __fastcall Unknown_0063A880(TeamTypeClass *teamtype, int wp) -- 0x0063A880 (reinf.cpp)
// Chrono reinforcement: creates the team type's group and teleports each member in at the team's
// waypoint (or wp): a free spot near the waypoint cell, a teleport locomotor that piggybacks the
// member's own, the chrono delay; members that cannot be placed are deleted. Allied arrivals mark
// the radar event cell.
#include "shared/reinforcement.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/team.hpp"
#include "shared/team_type.hpp"
#include "shared/script.hpp"
#include "shared/foot.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/id_source.hpp"
#include "shared/radar_event_class.hpp"
#include "shared/rules.hpp"
#include "shared/teleport_locomotion.hpp"
#include "shared/unknown_geometry.hpp"

extern "C" GUID const unknown_uuid_IPiggyback_7cfc30;

FootClass * __fastcall _Create_Group(TeamTypeClass *teamtype);

template<class Interface, GUID const *iid>
inline HRESULT UnknownComPtr<Interface, iid>::Query(UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> const &other)
{
    HRESULT result;
    if (other) {
        Interface *pointer;
        result = other.Checked()->QueryInterface(*iid, (void **)&pointer);
        if (FAILED(result)) {
            Release_Held();
            Pointer = 0;
            return result;
        }
        Interface *attached = pointer;
        Release_Held();
        Pointer = attached;
    } else {
        operator=((Interface *)0);
        result = E_NOINTERFACE;
    }
    return result;
}
bool __fastcall Unknown_0063A880(TeamTypeClass *teamtype, int wp)
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

    FootClass *object = _Create_Group(teamtype);
    object->Team->IsTransient = false;

    if (!object) {
        return false;
    }

    Cell origin = teamtype->Waypoint_Location();
    if (wp != -1) {
        origin = unknown_id_source_a3d290->Get_Waypoint_Location(wp);
    }

    FootClass *o = (FootClass *)object->unknown_30;
    object->unknown_30 = 0;
    FootClass *list = 0;

    while (object != 0) {
        bool is_unit = false;
        if (object && object->Kind_Of() == 1) {
            is_unit = true;
        }

        CoordStruct coord = Map[origin].Center_Coord();
        if (!is_unit) {
            coord.X += ((object->unknown_254 & 3) - 2) * 256;
            coord.Y += ((object->unknown_254 & 0xC) - 4) * 64;
        }
        object->unknown_254--;

        CoordStruct result = unknown_geometry_l;
        CellClass *cellptr = &Map[coord];
        int tries = 0;
        while (result == unknown_geometry_l && tries < 255) {
            Cell newcell = Map.Nearby_Location(Coord_Cell(cellptr->Center_Coord()), object->Techno_Type_Class()->SpeedType, -1, 0, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
            cellptr = &Map[newcell];
            if (is_unit) {
                result = cellptr->Center_Coord();
                tries++;
            } else {
                result = cellptr->Closest_Free_Spot(cellptr->Center_Coord(), false, false);
                tries++;
            }
        }

        coord.X = result.X;
        coord.Y = result.Y;
        unknown_a40774++;
        int placed = object->Unlimbo(&coord, 0);
        unknown_a40774--;

        if (placed) {
            UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30> piggyback(object->Locomotor);
            UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> teleport(CLSID_TeleportLocomotionClass);
            teleport.Checked()->Link_To_Object(object);
            piggyback = UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>(teleport);
            if (piggyback) {
                piggyback->Begin_Piggyback(object->Locomotor);
                object->Locomotor = teleport;
            }
            object->unknown_21c = coord;
            object->Locomotor.Checked()->Move_To(CoordStruct(coord.X, coord.Y, coord.Z));
            object->unknown_205 = true;
            object->unknown_218 = Rules->ChronoReinfDelay;
            object->unknown_30 = list;
            list = object;

            object = o;
            if (object != 0) {
                o = (FootClass *)object->unknown_30;
                object->unknown_30 = 0;
            }
        } else {
            delete object;

            object = o;
            if (object != 0) {
                o = (FootClass *)object->unknown_30;
                object->unknown_30 = 0;
            }
        }
    }

    object = list;
    list = (FootClass *)object->unknown_30;
    object->unknown_30 = 0;
    while (object != 0) {
        object->Mark(0);
        object->unknown_214 = 3;
        object->Commence();

        object = list;
        if (object != 0) {
            list = (FootClass *)object->unknown_30;
            object->unknown_30 = 0;
        }
    }

    if (teamtype->Owner->Is_Ally(unknown_player_a35db4)) {
        RadarEventClass_unknown_remember_cell(origin);
    }
    return true;
}
#endif
