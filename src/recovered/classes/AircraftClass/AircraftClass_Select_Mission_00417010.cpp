// Aircraft mission selection 0x00417010, slot261. Behavior labels are hypotheses.
#include "shared/aircraft.hpp"
#include "shared/house.hpp"
#include "shared/team.hpp"
// @ref ?unknown_slot261@FootClass@@UAE_N_N0@Z=004c6f10
// @ref ?Is_Human_Player@HouseClass@@QAE_NXZ=004f52f0
// @ref ?HasAnyLink@RadioClass@@QBE_NXZ=00636f10
// @ref ?unknown_func_6baa00@TeamClass@@QAE_NXZ=006baa00
// @ref ?Unknown_006B8FB0@TeamClass@@QAE_NPAVFootClass@@H_N@Z=006b8fb0
// @ref ?unknown_4198b0@AircraftClass@@QAEPAVAbstractClass@@XZ=004198b0
bool AircraftClass::unknown_slot261(bool first, bool second)
{
    if (Has_Suspended_Mission()) {
        Restore_Mission();
        if (Mission == 25) {
            unknown_a8 = 0;
            unknown_5ba = false;
        }
        return false;
    }
    int current_mission = Mission;
    bool mission_blocks_selection = current_mission == 4 || (current_mission > 25 && current_mission <= 27);
    if (mission_blocks_selection) return false;
    if (MissionQueue == 26) {
        Commence();
        return false;
    }
    bool result = FootClass::unknown_slot261(first, second);
    int mission = !House->Is_Human_Player() && !Team && unknown_slot163() ? 11 : 5;
    int landing_altitude = Landing_Altitude();
    if (In_Which_Layer() == 2) goto grounded;
    if (Get_Height_AGL() <= landing_altitude || Type->MissileSpawn) goto grounded;
    if (Cargo.Object) {
        if (!unknown_32c) goto select_destination;
        if (!Team) {
            mission = 16;
            Assign_Destination(unknown_4198b0(), true);
            goto choose_mission;
        }
        goto default_mission;
    } else {
        if (!unknown_32c) goto check_owner;
        if (!House->Is_Human_Player()) goto check_owner;
        goto check_team;
check_owner:
        if (!House->Is_Human_Player() && Type->Ammo == 0) goto check_team;
        goto check_weapon;
check_team:
        if (Team && Team->unknown_func_6baa00())
            Team->Unknown_006B8FB0(this, -1, 0);
check_weapon:
        if (unknown_slot231(0)->WeaponType) {
        if (unknown_32c) {
            if (!unknown_254) {
                if (Team) Team->Unknown_006B8FB0(this, -1, 0);
                unknown_slot220(0);
                Assign_Destination(0, true);
                mission = 4;
            } else if (!Team) mission = 15;
            goto choose_mission;
        }
        if ((unknown_254 && unknown_228 && Get_Mission() == 1) || MissionQueue == 1) {
            mission = 1;
            goto choose_mission;
        }
        if (!In_Air() || (unknown_494 && Mission == 7) || Type->Dock.Count() <= 0 ||
            (!unknown_32d && Team)) goto choose_mission;
        {
            TechnoClass *dock = unknown_slot299((DynamicVectorClass<TechnoTypeClass *> const *)&Type->Dock, 0, 0);
            Assign_Destination(0, true);
            if (dock && unknown_slot150(2, dock) == 1) {
                Assign_Destination(dock, true);
                mission = 7;
                goto choose_mission;
            }
        }
        if (Type->AirportBound) {
            unknown_slot224(0);
            return false;
        }
        goto select_destination;
        }
        if (Team) return false;
        goto select_destination;
    }
select_destination:
    Assign_Destination(unknown_4198b0(), true);
    mission = 2;
    goto choose_mission;
grounded:
    if (unknown_32c) {
        if (Cargo.Object) mission = Team ? 5 : 16;
        else if (!Team) {
            unknown_slot220(0);
            Assign_Destination(0, true);
            mission = 4;
        }
        goto choose_mission;
    }
    Assign_Destination(0, true);
    unknown_slot220(0);
    if (!House->Is_Human_Player() && !Team && unknown_slot163()) {
        mission = 11;
        goto choose_mission;
    }
default_mission:
    mission = 5;
choose_mission:
    if (!unknown_254 && unknown_slot163() && !HasAnyLink()) {
        TechnoClass *dock = unknown_slot299((DynamicVectorClass<TechnoTypeClass *> const *)&Type->Dock, 0, 0);
        if (dock) {
            Assign_Destination(dock, true);
            unknown_slot220(0);
            mission = 7;
        } else if (Type->AirportBound) {
            unknown_slot224(0);
            return false;
        }
    }
    if (HasAnyLink()) mission = 7;
    QueueMission(mission, false);
    if (unknown_slot122()) Commence();
    return result;
}
