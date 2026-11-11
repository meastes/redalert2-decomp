// void TeamClass::TMission_unknown_6bbbb0(ScriptMissionClass const &mission, bool unknown) (0x006BBBB0; Red Alert's Coordinate_Do)
// Conscripts every member; a live one without target and destination that strayed beyond twice the
// allowed distance (unless Data is mission 0xB) moves back to the team (+0x34), the cell beside it
// (0x004C8AF0) as destination; otherwise one without target and destination takes mission Data (except
// guard 5 over unloading 0x10).
#include "shared/unit.hpp"
#include "shared/foot.hpp"
#include "shared/team.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"

void TeamClass::TMission_unknown_6bbbb0(ScriptMissionClass const &mission, bool unknown)
{
    FootClass *unit = Member;
    int do_mission = mission.Data;
    while (unit != 0) {
        unknown_func_6ba7d0(unit);
        if (unknown_func_6bdd80(unit) && (unit->unknown_579 || unit->Kind_Of() == 2)) {
            if (unit->unknown_228 == 0 && unit->unknown_494 == 0
                && unit->unknown_5d4c00(unknown_34) > Unknown_006BE6A0() * 2 && do_mission != 0xB) {
                unit->QueueMission(2, false);
                unit->Assign_Destination(unknown_34, true);
                unit->QueueMission(2, false);
                Cell dest = unit->unknown_4c8af0(Coord_Cell(unknown_34->Center_Coord()));
                unit->Assign_Destination(&Map[dest], true);
            } else if (unit->unknown_228 == 0 && unit->unknown_494 == 0 && unit->Get_Mission() != do_mission
                && (do_mission != 5 || unit->Get_Mission() != 0x10)) {
                unit->unknown_6d6e50(0);
                unit->QueueMission(do_mission, false);
                unit->unknown_slot220(0);
                unit->Assign_Destination(0, true);
            }
        }
        unit = (FootClass *)unit->unknown_4c8;
    }
}
