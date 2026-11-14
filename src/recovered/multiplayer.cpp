// multiplayer.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/multiplayer.json carries the addresses and the rows that
// stay in their own files.


#define RA2_MPG_INITIALIZER_BASE_CONSTRUCTOR
#define RA2_MPGAME_START_OBJECTS
// Header-static dynamic initializers of the unit at 0x005B6A40:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x005B6A40-0x005B6C26); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x005B6A70, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_005b6a40.cpp; no source of its own.


// Dynamic initializer (0x005B6A90, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_005b6a40.cpp; no source of its own.


// Dynamic initializer (0x005B6AB0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_005b6a40.cpp; no source of its own.


// Dynamic initializer (0x005B6AD0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_005b6a40.cpp; no source of its own.


// Dynamic initializer (0x005B6AF0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_005b6a40.cpp; no source of its own.


// Dynamic initializer (0x005B6B10, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_005b6a40.cpp; no source of its own.


// Dynamic initializer (0x005B6B50, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_005b6a40.cpp; no source of its own.


// Dynamic initializer (0x005B6B80, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_005b6a40.cpp; no source of its own.


// Dynamic initializer (0x005B6BB0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_005b6a40.cpp; no source of its own.


// Dynamic initializer (0x005B6BE0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_005b6a40.cpp; no source of its own.


// Dynamic initializer (0x005B6BF0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_005b6a40.cpp; no source of its own.


// Dynamic initializer (0x005B6C10, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_005b6a40.cpp; no source of its own.


// DynamicVectorClass<MultiplayerGameMode *> &MultiplayerGameMode::unknown_modes_5b6c30() -- 0x005B6C30 (MultiplayerGameMode unit)
// The registered game modes: a function-local static list (guard bit 0 of
// 0x00A719E0, storage 0x00A71A10), destroyed at exit by 0x005B6C90.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#ifndef VECTOR_IMPLEMENT_DYNAMIC_INSERT_AFTER
#define VECTOR_IMPLEMENT_DYNAMIC_INSERT_AFTER
#endif
#include "shared/multiplayer_game_mode.hpp"
#include "shared/ccini_class.hpp"

DynamicVectorClass<MultiplayerGameMode *> &MultiplayerGameMode::unknown_modes_5b6c30()
{
    static DynamicVectorClass<MultiplayerGameMode *> modes;
    return modes;
}


// The atexit destructor of the game-mode list (0x005B6C90, _$E): emitted by
// the function-local static in MultiplayerGameMode_unknown_modes_005b6c30.cpp.


// MultiplayerGameMode::MultiplayerGameMode(...) (0x005B6CD0)
// A mode with its name and description, its INI file (read into a new
// CCINIClass when present) and id; [MultiplayerDialogSettings]
// WonlineTournamentAllowed of that INI sets unknown_3c (default 1). The INI comes from the
// accessor 0x005B7840, expanded here (its null test folds against the cleared member).
#include "shared/cc_file_class.hpp"

// The section name, a file-static pointer of the unit's data (0x007A7170).
static char const * const unknown_section_7a7170 = "MultiplayerDialogSettings";

MultiplayerGameMode::MultiplayerGameMode(WideString const &name, WideString const &description,
                                         Wstring const &ini_name, Wstring const &unknown_4, bool unknown_5,
                                         int id) :
    unknown_name_20(name),
    unknown_name_24(description),
    unknown_28(id),
    unknown_2c(ini_name),
    unknown_30(unknown_4),
    unknown_34(unknown_5),
    unknown_ini_38(0)
{
    unknown_04 = false;
    unknown_3c = 1;
    unknown_ini_5b7840();
    if (unknown_ini_38 != 0 && unknown_ini_38->Is_Present(unknown_section_7a7170)) {
        unknown_3c = unknown_ini_38->Get_Int(unknown_section_7a7170, "WonlineTournamentAllowed", unknown_3c);
    }
}


// MultiplayerGameMode::~MultiplayerGameMode() -- 0x005B6E20
// Deletes the teams (virtually, unknown_clear_teams_5b77e0 expanded) and the
// INI object, then the members.

MultiplayerGameMode::~MultiplayerGameMode()
{
    unknown_clear_teams_5b77e0();
    delete unknown_ini_38;
}


// int MultiplayerGameMode::unknown_slot10() -- 0x005B6EB0

int MultiplayerGameMode::unknown_slot10()
{
    return -2;
}


// MultiplayerGameMode *MultiplayerGameMode::unknown_first_mode_5b6ec0() -- 0x005B6EC0

MultiplayerGameMode *MultiplayerGameMode::unknown_first_mode_5b6ec0()
{
    if (unknown_modes_5b6c30().Count() == 0) {
        return 0;
    }
    return unknown_modes_5b6c30()[0];
}


// int MultiplayerGameMode::unknown_mode_count_5b6f80() -- 0x005B6F80

int MultiplayerGameMode::unknown_mode_count_5b6f80()
{
    return unknown_modes_5b6c30().Count();
}


// MultiplayerGameMode *MultiplayerGameMode::unknown_find_mode_5b6fe0(int id) -- 0x005B6FE0

MultiplayerGameMode *__fastcall MultiplayerGameMode::unknown_find_mode_5b6fe0(int id)
{
    for (int index = 0; index < unknown_mode_count_5b6f80(); index++) {
        MultiplayerGameMode *mode = unknown_modes_5b6c30()[index];
        if (mode->unknown_28 == id) {
            return mode;
        }
    }
    return 0;
}


// void MultiplayerGameMode::unknown_fill_combo_5b70b0(HWND combo, int id) -- 0x005B70B0
// Fills a combo box with the registered modes' names (each mode as item data) and selects the mode
// whose id matches; in game mode 5 only the modes whose slot 12 agrees are listed.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/unknown_globals.hpp"

void __fastcall MultiplayerGameMode::unknown_fill_combo_5b70b0(HWND combo, int id)
{
    unknown_reset_combo_6007f0(combo);
    bool listed = unknown_game_mode_a3d298 == 5;
    for (int index = 0; index < unknown_mode_count_5b6f80(); index++) {
        MultiplayerGameMode *mode = unknown_modes_5b6c30()[index];
        if (!listed || mode->unknown_slot12()) {
            int item = SendMessageA(combo, 0x143, 0, (LPARAM)mode->Get_Name().unknown_text_770430());
            if (mode->Get_ID() == id) {
                SendMessageA(combo, 0x14E, item, 0);
            }
            SendMessageA(combo, 0x151, item, (LPARAM)mode);
        }
    }
}


// void MultiplayerGameMode::unknown_fill_list_5b71e0(HWND list, int id) -- 0x005B71E0
// The list-box twin of 0x005B70B0 with two more filters.
#include "shared/lobby.hpp"
#include "shared/network_tests.hpp"
#include "shared/unknown_player_entry.hpp"

void __fastcall MultiplayerGameMode::unknown_fill_list_5b71e0(HWND list, int id)
{
    SendMessageA(list, 0x184, 0, 0);
    bool listed = unknown_game_mode_a3d298 == 5;
    for (int index = 0; index < unknown_mode_count_5b6f80(); index++) {
        MultiplayerGameMode *mode = unknown_modes_5b6c30()[index];
        if (mode->unknown_3c == 0 && unknown_game_mode_a3d298 == 4 && unknown_73c370()) {
            continue;
        }
        if (unknown_lobby_flag_7e6d90 == 0 && unknown_player_slots_a3fa98[0]->Team == -1 && !mode->unknown_slot40()) {
            continue;
        }
        if (!listed || mode->unknown_slot12()) {
            int item = SendMessageA(list, 0x4CD, 0, (LPARAM)mode->Get_Name().unknown_text_770430());
            if (mode->Get_ID() == id) {
                SendMessageA(list, 0x186, item, 0);
            }
            SendMessageA(list, 0x19A, item, (LPARAM)mode);
        }
    }
}


// bool MultiplayerGameMode::AssignTeams() -- 0x005B7350
// Slot 4: every player gets a team of its own (team number = list index),
// except players for which 0x0066E290 answers -3 (no team, -1).

bool MultiplayerGameMode::AssignTeams()
{
    for (int index = 0; index < unknown_players_a3fa7c.Count(); index++) {
        if (unknown_players_a3fa7c[index]->unknown_66e290() == -3) {
            unknown_players_a3fa7c[index]->Team = -1;
        } else {
            unknown_players_a3fa7c[index]->Team = index;
        }
    }
    return true;
}


// bool MultiplayerGameMode::CheckStartConditions(WideString *message) -- 0x005B73A0

bool MultiplayerGameMode::CheckStartConditions(WideString *message)
{
    return true;
}


// bool MultiplayerGameMode::unknown_slot20() -- 0x005B73B0

bool MultiplayerGameMode::unknown_slot20()
{
    return true;
}


// bool MultiplayerGameMode::unknown_slot21() -- 0x005B73C0

bool MultiplayerGameMode::unknown_slot21()
{
    return true;
}


// bool MultiplayerGameMode::AssignTeamsAtStart() -- 0x005B73D0

bool MultiplayerGameMode::AssignTeamsAtStart()
{
    return true;
}


// bool MultiplayerGameMode::IsAIAllowed() -- 0x005B73E0

bool MultiplayerGameMode::IsAIAllowed()
{
    return unknown_34;
}


// bool MultiplayerGameMode::unknown_slot12() -- 0x005B73F0

bool MultiplayerGameMode::unknown_slot12()
{
    return false;
}


// int MultiplayerGameMode::FirstValidMapIndex() -- 0x005B7400
#include "shared/multi_mission.hpp"

int MultiplayerGameMode::FirstValidMapIndex()
{
    int count = unknown_multi_missions_a3d8d8.Count();
    for (int index = 0; index < count; index++) {
        if (unknown_5b7470(unknown_multi_missions_a3d8d8[index])) {
            return index;
        }
    }
    return -1;
}


// bool MultiplayerGameMode::unknown_5b7470(MultiMission *map) -- 0x005B7470
// A map with the fixed text at +0x58 follows slot 11; otherwise the map must
// allow this mode's name (unless +0x17C is clear and 0x0073C370 agrees).

bool MultiplayerGameMode::unknown_5b7470(MultiMission *map)
{
    if (map->Unknown_006720C0()) {
        return IsAIAllowed();
    }
    if (!map->unknown_17c && unknown_73c370()) {
        return false;
    }
    return map->Unknown_006720E0(unknown_30);
}


// int MultiplayerGameMode::RandomHumanCountryIndex() -- 0x005B74C0
#include "shared/session_state.hpp"

int MultiplayerGameMode::RandomHumanCountryIndex()
{
    return unknown_session_a3d298.Unknown_00672940();
}


// int MultiplayerGameMode::RandomAICountryIndex() -- 0x005B74D0

int MultiplayerGameMode::RandomAICountryIndex()
{
    return RandomHumanCountryIndex();
}


// void MultiplayerGameMode::PopulateTeamDropdown(HWND combo, DynamicVectorClass<int> const *items, int selected) -- 0x005B74E0
// Slot 14: refills a combo box (0x006007F0 resets it) with the given items
// (each added as the item text and kept as its item data) and selects the
// item equal to `selected`.


void MultiplayerGameMode::PopulateTeamDropdown(HWND combo, DynamicVectorClass<int> const *items, int selected)
{
    unknown_reset_combo_6007f0(combo);
    for (int index = 0; index < items->Count(); index++) {
        int value = (*items)[index];
        int item = SendMessageA(combo, 0x143, 0, value);      // CB_ADDSTRING
        if (value == selected) {
            SendMessageA(combo, 0x14e, item, 0);              // CB_SETCURSEL
        }
        SendMessageA(combo, 0x151, item, value);              // CB_SETITEMDATA
    }
}


// void MultiplayerGameMode::DrawTeamDropdown(HWND list, DynamicVectorClass<MultiMission *> const *maps, MultiMission *selected) -- 0x005B7550
// Slot 15: the list-box counterpart of slot 14: empties the list
// (LB_RESETCONTENT), adds every multiplayer map with the custom message 0x4CD, keeps
// the map as item data (LB_SETITEMDATA) and selects the item of `selected`
// (LB_SETCURSEL). The elements are maps: MPCooperative's override 0x005A4B70
// reads their file names (+0x58).

void MultiplayerGameMode::DrawTeamDropdown(HWND list, DynamicVectorClass<MultiMission *> const *maps, MultiMission *selected)
{
    SendMessageA(list, 0x184, 0, 0);                          // LB_RESETCONTENT
    for (int index = 0; index < maps->Count(); index++) {
        MultiMission *map = (*maps)[index];
        int item = SendMessageA(list, 0x4cd, 0, (LPARAM)map);
        if (map == selected) {
            SendMessageA(list, 0x186, item, 0);               // LB_SETCURSEL
        }
        SendMessageA(list, 0x19a, item, (LPARAM)map);         // LB_SETITEMDATA
    }
}


// void MultiplayerGameMode::PopulateTeamDropdownForPlayer(HWND combo, int player) -- 0x005B75D0
// Slot 16: rebuilds the teams (slot 41), then fills a team combo box (reset
// by 0x006007F0) with every team's name, keeps the team as item data and
// selects the team of the given player (a valid player list index).

void MultiplayerGameMode::PopulateTeamDropdownForPlayer(HWND combo, int player)
{
    unknown_reset_combo_6007f0(combo);
    for (int index = 0; index < Teams.Count(); index++) {
        delete Teams[index];
    }
    Teams.Clear();
    CreateMPTeams(&Teams);
    if (player >= 0 && player < unknown_players_a3fa7c.Count()) {
        player = unknown_players_a3fa7c[player]->Team;
        for (int team_index = 0; team_index < Teams.Count(); team_index++) {
            MultiplayerTeam *team = Teams[team_index];
            int item = SendMessageA(combo, 0x143, 0, (LPARAM)team->Name.unknown_text_770430());   // CB_ADDSTRING
            if (team->Number == player) {
                SendMessageA(combo, 0x14e, item, 0);                                       // CB_SETCURSEL
            }
            SendMessageA(combo, 0x151, item, (LPARAM)team);                                  // CB_SETITEMDATA
        }
    }
}


// MultiplayerTeam *MultiplayerGameMode::GetTeam(int number) -- 0x005B76B0
// The team with a number; the teams are (re)created through slot 41 first
// when fewer than three exist.

MultiplayerTeam *MultiplayerGameMode::GetTeam(int number)
{
    if (Teams.Count() < 3) {
        for (int index = 0; index < Teams.Count(); index++) {
            delete Teams[index];
        }
        Teams.Clear();
        CreateMPTeams(&Teams);
    }
    for (int index = 0; index < Teams.Count(); index++) {
        MultiplayerTeam *team = Teams[index];
        if (team != 0 && team->Number == number) {
            return team;
        }
    }
    return 0;
}


// void MultiplayerGameMode::CreateMPTeams(DynamicVectorClass<MultiplayerTeam *> *teams) -- 0x005B7720
// Slot 41: the default teams: the observers' team, then one battle team per
// team number 1..the team count (0x00A3D558) (0x005A2550).

extern int unknown_team_count_a3d558;
MultiplayerTeam *__fastcall unknown_create_battle_team_005a2550(int number);

void MultiplayerGameMode::CreateMPTeams(DynamicVectorClass<MultiplayerTeam *> *teams)
{
    teams->Add(new MultiplayerObserverTeam);
    for (int number = 1; number <= unknown_team_count_a3d558; number++) {
        teams->Add(unknown_create_battle_team_005a2550(number));
    }
}


// void MultiplayerGameMode::unknown_clear_teams_5b77e0() -- 0x005B77E0
// Deletes every team and empties the list.

void MultiplayerGameMode::unknown_clear_teams_5b77e0()
{
    for (int index = 0; index < Teams.Count(); index++) {
        delete Teams[index];
    }
    Teams.Clear();
}


// bool MultiplayerGameMode::UnfixAlliances() -- 0x005B7820
// Slot 26: clears flag 0x400 of the scenario (0x00A3D290); true.
#include "shared/abstract.hpp"

bool MultiplayerGameMode::UnfixAlliances()
{
    unknown_id_source_a3d290->unknown_000 &= ~0x400;
    return true;
}


// bool MultiplayerGameMode::unknown_slot7(int unknown) -- 0x005B7830

bool MultiplayerGameMode::unknown_slot7(int unknown)
{
    return true;
}


// CCINIClass *MultiplayerGameMode::unknown_ini_5b7840() -- 0x005B7840
// The mode's INI: made on first use and read from the mode's INI file when it is there. The
// three-argument CCINIClass::Load is called directly (the two-argument inline wrapper would
// spend the inline budget that expands the List/GenericList constructors here).

CCINIClass *MultiplayerGameMode::unknown_ini_5b7840()
{
    if (unknown_ini_38 == 0) {
        unknown_ini_38 = new CCINIClass;
        CCFileClass file(unknown_2c);
        if (file.Is_Available()) {
            unknown_ini_38->Load(file, false, false);
        }
    }
    return unknown_ini_38;
}


// Cell MultiplayerGameMode::AssignStartingPositionsToHouse(int house, DynamicVectorClass<Cell> &cells, bool *taken) -- 0x005B7920
// Picks a house's start cell and marks it taken: a random one while none is taken (an observer's
// is not marked and noted in unknown_flag_a71a3c), the observer's cell again for the next house
// once, an observer gets the first taken cell, else the free cell farthest from the taken ones.
#include "shared/id_source.hpp"
#include "shared/house.hpp"
#include "shared/coord.hpp"
#include "shared/coord_distance.hpp"
#include "shared/debug_printf.hpp"

Cell MultiplayerGameMode::AssignStartingPositionsToHouse(int house, DynamicVectorClass<Cell> &cells, bool *taken)
{
    int index;
    for (index = 0; index < cells.Count(); index++) {
        if (taken[index]) {
            break;
        }
    }
    if (index == cells.Count()) {
        unknown_flag_a71a3c = false;
        unknown_flag_a71a3d = false;
        bool observer = false;
        HouseClass *owner = Houses[house];
        if (owner != 0) {
            for (int p = 0; p < unknown_players_a3fa7c.Count(); p++) {
                if (owner->Is_Human_Player() && Compare_Wide_007837d3(owner->UIName, unknown_players_a3fa7c[p]->Name) == 0) {
                    observer = unknown_players_a3fa7c[p]->Team == -1;
                    break;
                }
            }
        }
        if (observer) {
            index = unknown_id_source_a3d290->RandomNumber(0, cells.Count() - 1);
            taken[index] = true;
            unknown_flag_a71a3c = true;
            Debug_Printf("Observer observing random cell: %d (%d,%d)\n", index, cells[index].X, cells[index].Y);
            return cells[index];
        }
        index = unknown_id_source_a3d290->RandomNumber(0, cells.Count() - 1);
        Debug_Printf("First cell taken: %d (%d,%d)\n", index, cells[index].X, cells[index].Y);
        taken[index] = true;
        return cells[index];
    }
    if (unknown_flag_a71a3c && !unknown_flag_a71a3d) {
        unknown_flag_a71a3d = true;
        return cells[index];
    }
    bool observer = false;
    HouseClass *owner = Houses[house];
    if (owner != 0) {
        for (int p = 0; p < unknown_players_a3fa7c.Count(); p++) {
            if (owner->Is_Human_Player() && Compare_Wide_007837d3(owner->UIName, unknown_players_a3fa7c[p]->Name) == 0) {
                observer = unknown_players_a3fa7c[p]->Team == -1;
                break;
            }
        }
    }
    if (observer) {
        unknown_flag_a71a3d = true;
        unknown_flag_a71a3c = true;
        Debug_Printf("Observer Observing cell: %d (%d,%d)\n", index, cells[index].X, cells[index].Y);
        return cells[index];
    }
    int farthest = -1;
    int farthest_index = -1;
    for (int i = 0; i < cells.Count(); i++) {
        if (!taken[i]) {
            int total = 0;
            for (int j = 0; j < cells.Count(); j++) {
                if (taken[j]) {
                    Cell delta(cells[i].X - cells[j].X, cells[i].Y - cells[j].Y);
                    total += (short)Cell_Distance(delta.Y, delta.X);
                }
            }
            if (total > farthest) {
                farthest = total;
                farthest_index = i;
            }
        }
    }
    index = farthest_index;
    taken[index] = true;
    Debug_Printf("Starting cell taken: %d (%d,%d)\n", index, cells[index].X, cells[index].Y);
    return cells[index];
}


// bool MultiplayerGameMode::StartingPositionsToHouseBaseCells(bool official) -- 0x005B7C00
// Gives every non-passive house a start cell (slot 42 over the map's start waypoints) as its
// BaseCenter.
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/house_type.hpp"
#include "shared/crt_string.hpp"

bool MultiplayerGameMode::StartingPositionsToHouseBaseCells(bool official)
{
    DynamicVectorClass<Cell> cells = Multiplayer_Start_Waypoints_0065fff0(official);
    bool taken[16];
    memset(taken, 0, sizeof(taken));
    for (int index = 0; index < Houses.Count(); index++) {
        HouseClass *house = Houses[index];
        if (!house->Type->MultiplayPassive) {
            Debug_Printf("Assigning Starting Points for house %d (%s)\n", index, (char const *)house->Type->ID);
            house->BaseCenter = Cell_Center(AssignStartingPositionsToHouse(index, cells, taken));
        }
    }
    return true;
}


// Create each active house's starting objects from a common per-house unit budget.
#include "shared/unit_type.hpp"
#include "shared/infantry_type.hpp"
#include "shared/rules.hpp"
extern "C" char const multiplayer_seed_log_7e60bc[];
bool MultiplayerGameMode::Create_Starting_Objects_005b7ce0()
{
    int starting_money = 0;
    if (unknown_session_a3d298.UnitCount > 0) {
        unsigned int owners = 0;
        for (int house_index = 0; house_index < Houses.Count(); ++house_index) {
            HouseClass *house = Houses[house_index];
            if (!house->Type->MultiplayPassive)
                owners |= 1u << Houses[house_index]->Type->ArrayIndex;
        }
        int total_cost = 0;
        int type_count = 0;
        for (int unit_index = 0; unit_index < UnitTypes.Count(); ++unit_index) {
            UnitTypeClass *type = UnitTypes[unit_index];
            if (type->AllowedToStartInMultiplayer && (owners & type->Owner) && type->TechLevel <= unknown_house_setting_7d88d8) {
                if (!Rules->BaseUnit.Is_Present(type)) {
                    ++type_count;
                    total_cost += type->Raw_Cost();
                }
            }
        }
        for (int infantry_index = 0; infantry_index < InfantryTypes.Count(); ++infantry_index) {
            InfantryTypeClass *type = InfantryTypes[infantry_index];
            if (type->AllowedToStartInMultiplayer && (owners & type->Owner) && type->TechLevel <= unknown_house_setting_7d88d8) {
                ++type_count;
                total_cost += type->Raw_Cost();
            }
        }
        if (type_count == 0) type_count = 1;
        starting_money = ((type_count / 2 + total_cost) / type_count) * unknown_session_a3d298.UnitCount;
    }
    for (int house_index = 0; house_index < Houses.Count(); ++house_index) {
        HouseClass *house = Houses[house_index];
        if (!house->Type->MultiplayPassive) {
            if (house != 0) {
                for (int player_index = 0; player_index < unknown_session_a3d298.Players.Count(); ++player_index) {
                    if (house->Is_Human_Player() && Compare_Wide_007837d3(house->UIName, unknown_session_a3d298.Players[player_index]->Name) == 0) {
                        bool const &observer = unknown_session_a3d298.Players[player_index]->Team == -1;
                        if (observer) goto next_house;
                        break;
                    }
                }
            }
            int money = starting_money;
            if (!SpawnBaseUnits(house, &money) || !GenerateStartingUnits(house, &money)) return false;
            if (money > 0) house->Refund_Money(money);
            // These four 16-byte fields contain CounterClass objects, whose public base is VectorClass<int>.
            reinterpret_cast<VectorClass<int> *>(&house->unknown_54a0)->Clear();
            reinterpret_cast<VectorClass<int> *>(&house->unknown_54d0)->Clear();
            reinterpret_cast<VectorClass<int> *>(&house->unknown_54c0)->Clear();
            reinterpret_cast<VectorClass<int> *>(&house->unknown_54b0)->Clear();
        }
next_house:
        ;
    }
    int seed = Random_Pick(0, 0xffff);
    Debug_Printf(multiplayer_seed_log_7e60bc, seed);
    return true;
}


// bool MultiplayerGameMode::SpawnBaseUnits(HouseClass *house, int *money) -- 0x005B7F90
// With bases on, puts the house's base unit (from Rules BaseUnit) at its BaseCenter, or near
// it; false when there is none or it cannot be placed.
#include "shared/techno.hpp"

bool MultiplayerGameMode::SpawnBaseUnits(HouseClass *house, int *money)
{
    if (!unknown_session_a3d298.unknown_bases_20) {
        return true;
    }
    UnitTypeClass *type = house->Unknown_004EF6F0(Rules->BaseUnit);
    if (type != 0) {
        TechnoClass *unit = (TechnoClass *)new UnitClass(type, house);
        if (unit != 0) {
            CoordStruct &center = house->BaseCenter;
            if (unit->Unlimbo(&center, 0)) {
                return true;
            }
            Cell cell = Cell(center.X / 256, center.Y / 256);
            if (Unknown_00660b60(unit, cell, 1)) {
                return true;
            }
            delete unit;
        }
    }
    return false;
}


// bool MultiplayerGameMode::GenerateStartingUnits(HouseClass *house, int *money) -- 0x005B8060
// Spends the house's start money on random start units: units (Rules BaseUnit excluded) while
// more than a third of the money is left, else infantry, of the types the house may own at its
// tech level; each is placed near BaseCenter (given up after 20 failed placements), made a
// veteran with InitialVeteran and given mission 5 or 11. False when no type is available.
#include "shared/special.hpp"

bool MultiplayerGameMode::GenerateStartingUnits(HouseClass *house, int *money)
{
    if (*money > 0) {
        int start = *money;
        DynamicVectorClass<UnitTypeClass *> unit_list;
        DynamicVectorClass<InfantryTypeClass *> infantry_list;
        int owner = 1 << house->Type->ArrayIndex;
        for (int u = 0; u < UnitTypes.Count(); u++) {
            UnitTypeClass *unit_type = UnitTypes[u];
            if (unit_type->AllowedToStartInMultiplayer && (unit_type->Owner & owner)
                && unit_type->TechLevel <= house->unknown_11c && !Rules->BaseUnit.Is_Present(unit_type)) {
                unit_list.Add(unit_type);
            }
        }
        for (int i = 0; i < InfantryTypes.Count(); i++) {
            InfantryTypeClass *infantry_type = InfantryTypes[i];
            if (infantry_type->AllowedToStartInMultiplayer && (infantry_type->Owner & owner)
                && infantry_type->TechLevel <= house->unknown_11c) {
                infantry_list.Add(infantry_type);
            }
        }
        if (unit_list.Count() == 0 && infantry_list.Count() == 0) {
            return false;
        }
        int tries = 20;
        while (*money > 0 && tries > 0) {
            TechnoTypeClass *type;
            if (*money > start / 3 && unit_list.Count() != 0) {
                type = unit_list[unknown_id_source_a3d290->Random()(0, unit_list.Count() - 1)];
            } else {
                type = infantry_list[unknown_id_source_a3d290->Random()(0, infantry_list.Count() - 1)];
            }
            ObjectClass *object = type->Create_One_Of(house);
            TechnoClass *techno = dynamic_cast<TechnoClass *>(object);
            Cell cell = Cell(house->BaseCenter.X / 256, house->BaseCenter.Y / 256);
            if (!Unknown_00660b60(techno, cell, 4)) {
                delete techno;
                tries--;
            } else {
                *money -= type->Raw_Cost();
                if (unknown_id_source_a3d290->Special().InitialVeteran) {
                    techno->unknown_118.Set_Elite(true);
                }
                if (house->Is_Human_Player()) {
                    techno->Set_Mission(5);
                } else {
                    techno->Set_Mission(11);
                }
            }
        }
    }
    return true;
}


// bool MultiplayerGameMode::unknown_slot28() -- 0x005B8420

bool MultiplayerGameMode::unknown_slot28()
{
    return true;
}


// int MultiplayerGameMode::unknown_team_of_5b8430(int id) -- 0x005B8430
// The team number of the player with identifier `id` (+0x5F), or -1.

int MultiplayerGameMode::unknown_team_of_5b8430(int id)
{
    for (int index = 0; index < unknown_players_a3fa7c.Count(); index++) {
        if (unknown_players_a3fa7c[index]->ID == id) {
            return unknown_players_a3fa7c[index]->Team;
        }
    }
    return -1;
}


// bool MultiplayerGameMode::unknown_slot29() -- 0x005B8470

bool MultiplayerGameMode::unknown_slot29()
{
    return true;
}


// Constructor 0x005B8480: independent native body 9 bytes plus 7 NOPs.

MultiplayerGameMode::InitializerBase::InitializerBase()
{
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void MultiplayerGameMode::Read_Game_Modes_5b8490(Wstring const &section, InitializerBase &initializer) -- 0x005B8490
// From ra2-decomp 0x005D7590 (YR twin Read_Game_Modes, src/multiplayer.cpp), on Scout810's twin-shaped draft and
// LoadDlg485's r8490. The twin's direct MPModeIndex reads (unknown_28) in the insertion scan let the inlined
// mode-list accessor expand the list constructor at that site as in the original.
// Reads the [section] section of MPModes.ini: each entry is "id=title,description,ini,unknown_4[,flag]"
// (title and description are string-table labels, flag is true unless "false"); creates the mode
// through the initializer and inserts it into the mode list ordered by id.
#include "shared/ini_class.hpp"
#include "shared/string_table.hpp"

static inline void Trim_Trailing(Wstring &string)
{
    string.Trim_Trailing(" ");
}

void __fastcall MultiplayerGameMode::Read_Game_Modes_5b8490(Wstring const &section, InitializerBase &init)
{
    CCFileClass file("MPModes.ini");
    if (file.Is_Available()) {
        INIClass ini;
        if (ini.Load(file, false) > 0) {
            int count = ini.Entry_Count(section);
            for (int i = 0; i < count; i++) {
                Wstring entry = ini.Get_Entry(section, i);
                int index = entry.As_Integer();
                Wstring value;
                int length = ini.Get_String(section, entry, unknown_string_83c55c, value);
                if (length <= 0) continue;

                Wstring name;
                int pos = value.Get_Token(0, ",", name);
                if (!name.Length()) continue;
                name.Trim_Leading(" ");
                Trim_Trailing(name);
                WideString title((unsigned short const *)StringTable::Load_String(name.Get(), 0, "D:\\Ra2\\Multiplayer.cpp", 0x305));

                Wstring tip;
                pos = value.Get_Token(pos, ",", tip);
                if (!tip.Length()) continue;
                tip.Trim_Leading(" ");
                Trim_Trailing(tip);
                WideString tooltip((unsigned short const *)StringTable::Load_String(tip.Get(), 0, "D:\\Ra2\\Multiplayer.cpp", 0x310));

                Wstring inifile;
                pos = value.Get_Token(pos, ",", inifile);
                if (!inifile.Length()) continue;
                inifile.Trim_Leading(" ");
                Trim_Trailing(inifile);

                Wstring mapfilter;
                pos = value.Get_Token(pos, ",", mapfilter);
                mapfilter.Trim_Leading(" ");
                Trim_Trailing(mapfilter);

                Wstring ai;
                value.Get_Token(pos, ",", ai);
                ai.Trim_Leading(" ");
                Trim_Trailing(ai);
                bool aiallowed = true;
                if (ai.Length() > 0) {
                    aiallowed = Compare_Narrow_00781f20(ai.Get(), "false") != 0;
                }

                if (title.Length() <= 0 || inifile.Length() <= 0 || mapfilter.Length() <= 0) continue;

                MultiplayerGameMode *mode = init.Create(title, tooltip, inifile, mapfilter, aiallowed, index);
                int at;
                for (at = 0; at < unknown_modes_5b6c30().Count(); at++) {
                    if (unknown_modes_5b6c30()[at]->unknown_28 > mode->unknown_28) {
                        break;
                    }
                }
                if (at == 0) {
                    unknown_modes_5b6c30().Add_Head(mode);
                } else {
                    unknown_modes_5b6c30().Insert_After(mode, at - 1);
                }
            }
        }
    }
}


// 0x005b8bc0: function not yet identified
//
// Owner: MultiplayerGameMode::InitializerBase, stores the MultiplayerGameMode::InitializerBase vtable (constructor or destructor) (from RTTI/vtables).
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_005b8bc0(undefined4 *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005b8bc0 {

extern "C" void* PTR_FUN_007a72b0;

void __fastcall FUN_005b8bc0(unsigned int *object_words)
{
    *object_words = (unsigned int)(&PTR_FUN_007a72b0);
    return;
}

}  // namespace seed_005b8bc0


// void __fastcall MultiplayerGameMode::unknown_init_modes_5b8bd0() -- 0x005B8BD0
// Recreates the mode list: deletes the registered modes (0x005B8D40, expanded here) and registers
// the six modes by name with their Initializer<T> (whose Create and scalar deleting destructor
// copies this unit emits).

void MultiplayerGameMode::unknown_init_modes_5b8bd0()
{
    unknown_delete_modes_5b8d40();
    Initializer<MultiplayerBattle> battle;
    Read_Game_Modes_5b8490("Battle", battle);
    Initializer<MultiplayerManBattle> man_battle;
    Read_Game_Modes_5b8490("ManBattle", man_battle);
    Initializer<MultiplayerSiege> siege;
    Read_Game_Modes_5b8490("Siege", siege);
    Initializer<UnholyAlliance> unholy;
    Read_Game_Modes_5b8490("Unholy", unholy);
    Initializer<FreeForAll> free_for_all;
    Read_Game_Modes_5b8490("FreeForAll", free_for_all);
    Initializer<MPCooperative> cooperative;
    Read_Game_Modes_5b8490("Cooperative", cooperative);
}


// void MultiplayerGameMode::unknown_delete_modes_5b8d40() -- 0x005B8D40
// Deletes every registered game mode (the list accessor is expanded inline
// twice per iteration).

void MultiplayerGameMode::unknown_delete_modes_5b8d40()
{
    for (int index = 0; index < unknown_modes_5b6c30().Count(); index++) {
        delete unknown_modes_5b6c30()[index];
    }
}


// ??_GMultiplayerGameMode@@UAEPAXI@Z (0x005B8E10)
// Compiler generated (calls the destructor 0x005B6E20); emitted with the
// vtable by the constructor 0x005B6CD0.


// 0x005b8ec0: function not yet identified
//
// Owner: MultiplayerGameMode::InitializerBase, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 32 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_005b8ec0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005b8ec0 {

extern void  operator_delete_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_007a72b0;

struct Owner_005b8ec0 { unsigned int * FUN_005b8ec0(unsigned char deletion_flags); };

unsigned int * Owner_005b8ec0::FUN_005b8ec0(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_007a72b0);
    if ((deletion_flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_005b8ec0


// MultiplayerGameMode::Initializer<MultiplayerBattle> scalar deleting destructor (0x005B8EE0)
// Compiler-generated for the template class of shared/multiplayer_game_mode.hpp (Initializer<T>).
// No source of its own: emitted by the MultiplayerGameMode unit (unknown_init_modes_5b8bd0).


// MultiplayerGameMode::Initializer<MultiplayerManBattle> scalar deleting destructor (0x005B8F00)
// Compiler-generated for the template class of shared/multiplayer_game_mode.hpp (Initializer<T>).
// No source of its own: emitted by the MultiplayerGameMode unit (unknown_init_modes_5b8bd0).


// MultiplayerGameMode::Initializer<MultiplayerSiege> scalar deleting destructor (0x005B8F20)
// Compiler-generated for the template class of shared/multiplayer_game_mode.hpp (Initializer<T>).
// No source of its own: emitted by the MultiplayerGameMode unit (unknown_init_modes_5b8bd0).


// MultiplayerGameMode::Initializer<UnholyAlliance> scalar deleting destructor (0x005B8F40)
// Compiler-generated for the template class of shared/multiplayer_game_mode.hpp (Initializer<T>).
// No source of its own: emitted by the MultiplayerGameMode unit (unknown_init_modes_5b8bd0).


// MultiplayerGameMode::Initializer<FreeForAll> scalar deleting destructor (0x005B8F60)
// Compiler-generated for the template class of shared/multiplayer_game_mode.hpp (Initializer<T>).
// No source of its own: emitted by the MultiplayerGameMode unit (unknown_init_modes_5b8bd0).


// MultiplayerGameMode::Initializer<MPCooperative> scalar deleting destructor (0x005B8F80)
// Compiler-generated for the template class of shared/multiplayer_game_mode.hpp (Initializer<T>).
// No source of its own: emitted by the MultiplayerGameMode unit (unknown_init_modes_5b8bd0).


// 0x005b9000: function not yet identified
//
// Owner: DynamicVectorClass<MultiplayerGameMode *>, virtual slot 3 (vtable +0xc) (from RTTI/vtables).
// Body: 42 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void __fastcall Clear_005b9000(int param_1)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005b9000 {

extern void  operator_delete_00781d3b(unsigned int allocation_address);

void __fastcall Clear_005b9000(int vector_address)
{
    *(unsigned int *)(vector_address + 0x10) = 0;
    if ((*(int *)(vector_address + 4) != 0) && (*(char *)(vector_address + 0xd) != '\0')) {
        operator_delete_00781d3b((unsigned int)(*(int *)(vector_address + 4)));
        *(unsigned int *)(vector_address + 4) = 0;
    }
    *(unsigned char *)(vector_address + 0xd) = 0;
    *(unsigned int *)(vector_address + 8) = 0;
    return;
}

}  // namespace seed_005b9000


// DynamicVectorClass<MultiplayerGameMode *>::ID(MultiplayerGameMode * const *) (0x005B9030)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (population-2401).


// MultiplayerGameMode::Initializer<MultiplayerBattle>::Create(...) (0x005B9060)
// Template member instantiated from shared/multiplayer_game_mode.hpp (Initializer<T>).
// No source of its own: emitted by the MultiplayerGameMode unit (unknown_init_modes_5b8bd0).


// MultiplayerGameMode::Initializer<MultiplayerManBattle>::Create(...) (0x005B90A0)
// Template member instantiated from shared/multiplayer_game_mode.hpp (Initializer<T>).
// No source of its own: emitted by the MultiplayerGameMode unit (unknown_init_modes_5b8bd0).


// MultiplayerGameMode::Initializer<MultiplayerSiege>::Create(...) (0x005B90E0)
// Template member instantiated from shared/multiplayer_game_mode.hpp (Initializer<T>).
// No source of its own: emitted by the MultiplayerGameMode unit (unknown_init_modes_5b8bd0).


// MultiplayerGameMode::Initializer<UnholyAlliance>::Create(...) (0x005B9120)
// Template member instantiated from shared/multiplayer_game_mode.hpp (Initializer<T>).
// No source of its own: emitted by the MultiplayerGameMode unit (unknown_init_modes_5b8bd0).


// MultiplayerGameMode::Initializer<FreeForAll>::Create(...) (0x005B9160)
// Template member instantiated from shared/multiplayer_game_mode.hpp (Initializer<T>).
// No source of its own: emitted by the MultiplayerGameMode unit (unknown_init_modes_5b8bd0).


// MultiplayerGameMode::Initializer<MPCooperative>::Create(...) (0x005B91A0)
// Template member instantiated from shared/multiplayer_game_mode.hpp (Initializer<T>).
// No source of its own: emitted by the MultiplayerGameMode unit (unknown_init_modes_5b8bd0).


// Original region: 0x005b95c0 (93 body bytes, 3 independently frozen NOP bytes).
// Exact target SHA256: e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f.
// Purpose: initialize an inferred four-byte-element storage wrapper.
// Class/API identity and the three unknown field purposes remain unresolved.

extern "C" void* __cdecl allocate_storage_005b95c0(unsigned int byte_count);
extern "C" unsigned char base_dispatch_005b95c0[];
extern "C" unsigned char derived_dispatch_005b95c0[];

class WordStorage_005b95c0 {
    unsigned char* dispatch_table;
    unsigned int* storage;
    unsigned int capacity;
    unsigned char unknown_0x0c;
    unsigned char owns_storage;
    // Bytes 0x0e..0x0f are alignment padding and are not written.
    unsigned int unknown_0x10;
    unsigned int unknown_0x14;

public:
    WordStorage_005b95c0* initialize(unsigned int requested_capacity,
                                unsigned int* supplied_storage);
};

typedef char WordStorage_005b95c0_size_check[(sizeof(WordStorage_005b95c0) == 24) ? 1 : -1];

WordStorage_005b95c0* WordStorage_005b95c0::initialize(
    unsigned int requested_capacity,
    unsigned int* supplied_storage)
{
    storage = 0;
    capacity = requested_capacity;
    unknown_0x0c = 1;
    owns_storage = 0;
    dispatch_table = base_dispatch_005b95c0;

    // A zero capacity ignores even a nonnull supplied pointer.
    if (requested_capacity != 0) {
        if (supplied_storage != 0) {
            storage = supplied_storage;
        } else {
            // Preserve 32-bit wrapping and ownership even on a null result.
            storage = (unsigned int*)allocate_storage_005b95c0(requested_capacity << 2);
            owns_storage = 1;
        }
    }

    dispatch_table = derived_dispatch_005b95c0;
    unknown_0x14 = 10;
    unknown_0x10 = 0;
    return this;
}


extern "C" unsigned char vtable_5b9700[]; extern "C" void __cdecl release_buffer_5b9700(void*); extern "C" void __cdecl release_object_5b9700(void*);
class Leaf_5b9700 { public: void* get(unsigned int flags); };
void* Leaf_5b9700::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5b9700; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5b9700((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5b9700(this); return this; }


extern "C" unsigned char vtable_5b9750[]; extern "C" void __cdecl release_buffer_5b9750(void*); extern "C" void __cdecl release_object_5b9750(void*);
class Leaf_5b9750 { public: void* get(unsigned int flags); };
void* Leaf_5b9750::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5b9750; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5b9750((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5b9750(this); return this; }


extern "C" unsigned char vtable_5b97a0[]; extern "C" void __cdecl release_buffer_5b97a0(void*); extern "C" void __cdecl release_object_5b97a0(void*);
class Leaf_5b97a0 { public: void* get(unsigned int flags); };
void* Leaf_5b97a0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5b97a0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5b97a0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5b97a0(this); return this; }


extern "C" unsigned char vtable_5b97f0[]; extern "C" void __cdecl release_buffer_5b97f0(void*); extern "C" void __cdecl release_object_5b97f0(void*);
class Leaf_5b97f0 { public: void* get(unsigned int flags); };
void* Leaf_5b97f0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5b97f0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5b97f0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5b97f0(this); return this; }


extern "C" unsigned char table_5b9840[]; extern "C" void* __cdecl storage_5b9840(unsigned int);
class Storage_5b9840 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_5b9840::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_5b9840; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_5b9840(requested << 2); owns=1; } return this; }


extern "C" unsigned char table_5b9890[]; extern "C" void* __cdecl storage_5b9890(unsigned int);
class Storage_5b9890 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_5b9890::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_5b9890; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_5b9890(requested << 2); owns=1; } return this; }


extern "C" unsigned char table_5b98e0[]; extern "C" void* __cdecl storage_5b98e0(unsigned int);
class Storage_5b98e0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_5b98e0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_5b98e0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_5b98e0(requested << 2); owns=1; } return this; }
