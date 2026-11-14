// mpsiege.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mpsiege.json carries the addresses and the rows that
// stay in their own files.


// 0x005ab5a0: function not yet identified
//
// Owner: MultiplayerSiege, stores the MultiplayerSiege vtable (constructor or destructor) (from RTTI/vtables).
// Body: 52 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 * __thiscall FUN_005ab5a0(undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4, undefined4 param_5,undefined4 param_6,undefined4 param_7)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005ab5a0 {

struct Callee_005b6cd0 { void FUN_005b6cd0(unsigned int name_address, unsigned int description_address, unsigned int ini_name_address, unsigned int unknown_string_address, unsigned char unknown_flag, unsigned int mode_id); };
extern "C" void* PTR_FUN_007a6b8c;

struct Owner_005ab5a0 { unsigned int * FUN_005ab5a0(unsigned int name_address, unsigned int description_address, unsigned int ini_name_address, unsigned int unknown_string_address, unsigned int unknown_flag, unsigned int mode_id); };

unsigned int * Owner_005ab5a0::FUN_005ab5a0(unsigned int name_address, unsigned int description_address, unsigned int ini_name_address, unsigned int unknown_string_address, unsigned int unknown_flag, unsigned int mode_id)
{
    unsigned int * object_words = (unsigned int *)this;
    ((Callee_005b6cd0*)(object_words))->FUN_005b6cd0((unsigned int)(name_address), (unsigned int)(description_address), (unsigned int)(ini_name_address), (unsigned int)(unknown_string_address), (unsigned char)(unknown_flag), (unsigned int)(mode_id));
    *object_words = (unsigned int)(&PTR_FUN_007a6b8c);
    return object_words;
}

}  // namespace seed_005ab5a0


// 0x005ab5e0: function not yet identified
//
// Owner: MultiplayerSiege, stores the MultiplayerSiege vtable (constructor or destructor) (from RTTI/vtables).
// Body: 11 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_005ab5e0(undefined4 *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005ab5e0 {

extern void __fastcall FUN_005b6e20(void);
extern "C" void* PTR_FUN_007a6b8c;

void __fastcall FUN_005ab5e0(unsigned int *object_words)
{
    *object_words = (unsigned int)(&PTR_FUN_007a6b8c);
    FUN_005b6e20();
    return;
}

}  // namespace seed_005ab5e0


// bool MultiplayerSiege::AssignTeams() -- 0x005AB5F0 (the MultiplayerSiege unit)
// Slot 4: after the base step (0x005B7350) the first player defends (team
// 1); the loop over the rest assigns team 2 to the second entry only (as
// the original code does). False if the base step fails.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/multiplayer_game_mode.hpp"

bool MultiplayerSiege::AssignTeams()
{
    if (!MultiplayerGameMode::AssignTeams()) {
        return false;
    }
    if (unknown_players_a3fa7c.Count() > 0) {
        unknown_players_a3fa7c[0]->Team = 1;
        for (int index = 1; index < unknown_players_a3fa7c.Count(); index++) {
            unknown_players_a3fa7c[1]->Team = 2;
        }
    }
    return true;
}


// bool MultiplayerSiege::CheckStartConditions(WideString *message) -- 0x005AB640
// Slot 5 (the team check before a game): after the base check
// (0x005B73A0), requires exactly one defender (team 1), at least one
// attacker (team 2) and no other team numbers besides 0; otherwise sets the
// localized reason in *message and fails.
#include "shared/string_table.hpp"

bool MultiplayerSiege::CheckStartConditions(WideString *message)
{
    if (!MultiplayerGameMode::CheckStartConditions(message)) {
        return false;
    }
    bool defender = false;
    int attackers = 0;
    for (int index = 0; index < unknown_players_a3fa7c.Count(); index++) {
        switch (unknown_players_a3fa7c[index]->Team) {
        case 0:
            break;
        case 1:
            if (defender) {
                *message = StringTable::Load_String("MP:OnlyOneBeseiged", 0, "D:\\Ra2\\MPSiege.cpp", 0x33);
                return false;
            }
            defender = true;
            break;
        case 2:
            attackers++;
            break;
        default:
            *message = StringTable::Load_String("MP:IllegalTeam", 0, "D:\\Ra2\\MPSiege.cpp", 0x3c);
            return false;
        }
    }
    if (!defender) {
        *message = StringTable::Load_String("MP:NoDefender", 0, "D:\\Ra2\\MPSiege.cpp", 0x41);
        return false;
    }
    if (attackers < 1) {
        *message = StringTable::Load_String("MP:NoAttackers", 0, "D:\\Ra2\\MPSiege.cpp", 0x46);
        return false;
    }
    return true;
}


// bool MultiplayerSiege::AssignTeamsAtStart() -- 0x005AB740
// Slot 6: the base behaviour only (0x005B73D0).

bool MultiplayerSiege::AssignTeamsAtStart()
{
    return MultiplayerGameMode::AssignTeamsAtStart();
}


class Leaf_5ab750 { public: bool get(); };
bool Leaf_5ab750::get() { return false; }


// bool MultiplayerSiege::UnfixAlliances() -- 0x005AB760
// Slot 26: the base behaviour only (0x005B7820).

bool MultiplayerSiege::UnfixAlliances()
{
    return MultiplayerGameMode::UnfixAlliances();
}


// bool MultiplayerSiege::StartingPositionsToHouseBaseCells(bool official) -- 0x005AB770
// Gives the defending house (the session player on team 1) the first start cell and every
// other non-passive house a start cell from slot 42; false without a valid defender.
#include "shared/id_source.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/coord.hpp"
#include "shared/debug_printf.hpp"
#include "shared/crt_string.hpp"

bool MultiplayerSiege::StartingPositionsToHouseBaseCells(bool official)
{
    DynamicVectorClass<Cell> cells = Multiplayer_Start_Waypoints_0065fff0(official);
    bool taken[16];
    memset(taken, 0, sizeof(taken));
    int index;
    for (index = 0; index < unknown_players_a3fa7c.Count(); index++) {
        if (unknown_players_a3fa7c[index]->Team == 1) {
            break;
        }
    }
    if (index >= unknown_players_a3fa7c.Count()) {
        return false;
    }
    int id = unknown_players_a3fa7c[index]->ID;
    if (id < 0 || id >= Houses.Count()) {
        return false;
    }
    HouseClass *defender = Houses[id];
    if (defender->Type->MultiplayPassive) {
        return false;
    }
    Debug_Printf("Assigning Starting Points for Defending house %d (%s)\n", id, (char const *)defender->Type->ID);
    Cell start = cells[0];
    taken[0] = true;
    defender->BaseCenter = Cell_Center(start);
    for (int house = 0; house < Houses.Count(); house++) {
        HouseClass *attacker = Houses[house];
        if (!attacker->Type->MultiplayPassive && attacker != defender) {
            Debug_Printf("Assigning Starting Points for Attacking house %d (%s)\n", house, (char const *)attacker->Type->ID);
            attacker->BaseCenter = Cell_Center(AssignStartingPositionsToHouse(house, cells, taken));
        }
    }
    return true;
}


// void MultiplayerSiege::CreateMPTeams(DynamicVectorClass<MultiplayerTeam *> *teams) -- 0x005AB970
// Slot 41: the siege teams: observers, defenders, attackers.

void MultiplayerSiege::CreateMPTeams(DynamicVectorClass<MultiplayerTeam *> *teams)
{
    teams->Add(new MultiplayerObserverTeam);
    teams->Add(new MultiplayerSiegeDefenderTeam);
    teams->Add(new MultiplayerSiegeAttackerTeam);
}


// bool MultiplayerSiege::SpawnBaseUnits(HouseClass *house, int *money) -- 0x005ABA80
// The defenders (session team 1) get a construction yard (the first type of Rules +0x7C8
// the house may build) at their BaseCenter, or near it; every other player gets the
// base behaviour. A house without a session player gets the base behaviour and false.
#include "shared/building_class.hpp"
#include "shared/rules.hpp"

bool MultiplayerSiege::SpawnBaseUnits(HouseClass *house, int *money)
{
    int index;
    for (index = 0; index < unknown_players_a3fa7c.Count(); index++) {
        if (unknown_players_a3fa7c[index]->ID == house->ArrayIndex) {
            break;
        }
    }
    if (index >= unknown_players_a3fa7c.Count()) {
        MultiplayerGameMode::SpawnBaseUnits(house, money);
    } else {
        if (unknown_players_a3fa7c[index]->Team != 1) {
            return MultiplayerGameMode::SpawnBaseUnits(house, money);
        }
        BuildingTypeClass *type = house->unknown_func_4ef600(Rules->unknown_7c8);
        if (type != 0) {
            TechnoClass *building = (TechnoClass *)new BuildingClass(type, house);
            if (building != 0) {
                CoordStruct &center = house->BaseCenter;
                if (!building->Unlimbo(&center, 0)) {
                    Cell cell = Cell(center.X / 256, center.Y / 256);
                    if (!Unknown_00660b60(building, cell, 1)) {
                        delete building;
                        return false;
                    }
                }
                return true;
            }
        }
    }
    return false;
}


// 0x005abba0: function not yet identified
//
// Owner: MultiplayerSiege, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_005abba0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005abba0 {

extern void __fastcall FUN_005b6e20(void);
extern void  FUN_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_007a6b8c;

struct Owner_005abba0 { unsigned int * FUN_005abba0(unsigned char deletion_flags); };

unsigned int * Owner_005abba0::FUN_005abba0(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_007a6b8c);
    FUN_005b6e20();
    if ((deletion_flags & 1) != 0) {
        FUN_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_005abba0
