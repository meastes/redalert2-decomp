// mpfreeforall.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mpfreeforall.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x005A6E70: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x005A6E70-0x005A7051); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x005A6E90, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_005a6e70.cpp; no source of its own.


// Dynamic initializer (0x005A6EC0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_005a6e70.cpp; no source of its own.


// Dynamic initializer (0x005A6EE0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_005a6e70.cpp; no source of its own.


// Dynamic initializer (0x005A6F00, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_005a6e70.cpp; no source of its own.


// Dynamic initializer (0x005A6F20, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_005a6e70.cpp; no source of its own.


// Dynamic initializer (0x005A6F40, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_005a6e70.cpp; no source of its own.


// Dynamic initializer (0x005A6F60, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_005a6e70.cpp; no source of its own.


// Dynamic initializer (0x005A6FA0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_005a6e70.cpp; no source of its own.


// Dynamic initializer (0x005A6FD0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_005a6e70.cpp; no source of its own.


// Dynamic initializer (0x005A7000, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_005a6e70.cpp; no source of its own.


// Dynamic initializer (0x005A7030, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_005a6e70.cpp; no source of its own.


// Dynamic initializer (0x005A7040, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_005a6e70.cpp; no source of its own.


// FreeForAll::FreeForAll(...) -- 0x005A7060 (the FreeForAll unit)
// Forwards its six arguments to the MultiplayerGameMode constructor.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/multiplayer_game_mode.hpp"

FreeForAll::FreeForAll(WideString const &name, WideString const &description, Wstring const &ini_name,
    Wstring const &unknown_4, bool unknown_5, int id) :
    MultiplayerGameMode(name, description, ini_name, unknown_4, unknown_5, id)
{
}


// FreeForAll::~FreeForAll() -- 0x005A70A0

FreeForAll::~FreeForAll()
{
}


// bool FreeForAll::AssignTeams() -- 0x005A70B0
// Slot 4: the base behaviour only (0x005B7350).

bool FreeForAll::AssignTeams()
{
    return MultiplayerGameMode::AssignTeams();
}


// bool FreeForAll::CheckStartConditions(WideString *message) -- 0x005A70C0
// Slot 5 (the team check before a game): after the base check (0x005B73A0), every player
// with a team (not -1) gets a team of its own (its index in the session list).

bool FreeForAll::CheckStartConditions(WideString *message)
{
    if (!MultiplayerGameMode::CheckStartConditions(message)) {
        return false;
    }
    for (int index = 0; index < unknown_players_a3fa7c.Count(); index++) {
        NodeNameType *node = unknown_players_a3fa7c[index];
        if (node->Team != -1) {
            unknown_players_a3fa7c[index]->Team = index;
        }
    }
    return true;
}


// bool FreeForAll::AssignTeamsAtStart() -- 0x005A7110
// Slot 6: like slot 5 after the base slot 6 (0x005B73D0).

bool FreeForAll::AssignTeamsAtStart()
{
    if (!MultiplayerGameMode::AssignTeamsAtStart()) {
        return false;
    }
    for (int index = 0; index < unknown_players_a3fa7c.Count(); index++) {
        if (unknown_players_a3fa7c[index]->Team != -1) {
            unknown_players_a3fa7c[index]->Team = index;
        }
    }
    return true;
}


#include "shared/win32_api.hpp"


// void FreeForAll::PopulateTeamDropdownForPlayer(HWND combo, int team) -- 0x005A7150
// Slot 16: resets a team combo box (0x006007F0), lets team `team` and team
// -1 add their entries (0x005B9BE0) and selects the first entry.

WIN32_IMPORT long __stdcall SendMessageA(HWND window, unsigned int message, unsigned int wparam, long lparam);
void __fastcall unknown_reset_combo_6007f0(HWND combo);

void FreeForAll::PopulateTeamDropdownForPlayer(HWND combo, int team)
{
    unknown_reset_combo_6007f0(combo);
    MultiplayerTeam *entry = GetTeam(team);
    if (entry != 0) {
        entry->AddToList(combo);
    }
    entry = GetTeam(-1);
    if (entry != 0) {
        entry->AddToList(combo);
    }
    SendMessageA(combo, 0x14e, 0, 0);    // CB_SETCURSEL
}


// bool FreeForAll::unknown_slot12() -- 0x005A71B0
// Slot 12: always true (the base answers out of line, 0x005B73F0).

bool FreeForAll::unknown_slot12()
{
    return true;
}


// FreeForAll scalar deleting destructor (0x005A71C0, ??_G): emitted with the
// vtable; the destructor is expanded inline.
