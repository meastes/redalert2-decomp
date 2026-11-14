// multiplayerteam.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/multiplayerteam.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x005B9930: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x005B9930-0x005B9B11); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x005B9950, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_005b9930.cpp; no source of its own.


// Dynamic initializer (0x005B9980, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_005b9930.cpp; no source of its own.


// Dynamic initializer (0x005B99A0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_005b9930.cpp; no source of its own.


// Dynamic initializer (0x005B99C0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_005b9930.cpp; no source of its own.


// Dynamic initializer (0x005B99E0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_005b9930.cpp; no source of its own.


// Dynamic initializer (0x005B9A00, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_005b9930.cpp; no source of its own.


// Dynamic initializer (0x005B9A20, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_005b9930.cpp; no source of its own.


// Dynamic initializer (0x005B9A60, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_005b9930.cpp; no source of its own.


// Dynamic initializer (0x005B9A90, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_005b9930.cpp; no source of its own.


// Dynamic initializer (0x005B9AC0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_005b9930.cpp; no source of its own.


// Dynamic initializer (0x005B9AF0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_005b9930.cpp; no source of its own.


// Dynamic initializer (0x005B9B00, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_005b9930.cpp; no source of its own.


// MultiplayerTeam::MultiplayerTeam(WideString const &name, int number) -- 0x005B9B20 (MultiplayerTeam unit)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/multiplayer_game_mode.hpp"

WIN32_IMPORT long __stdcall SendMessageA(HWND window, unsigned int message, unsigned int wparam, long lparam);

MultiplayerTeam::MultiplayerTeam(WideString const &name, int number) : Name(name), Number(number)
{
}


// MultiplayerTeam::~MultiplayerTeam() -- 0x005B9B50

MultiplayerTeam::~MultiplayerTeam()
{
}


// bool MultiplayerTeam::IsTeamIncluded(int player) -- 0x005B9B60
// Slot 1: true for a valid index into the player list (0x00A3FA7C).

bool MultiplayerTeam::IsTeamIncluded(int player)
{
    if (player < 0 || player >= unknown_players_a3fa7c.Count()) {
        return false;
    }
    return true;
}


// bool MultiplayerTeam::SetPlayerTeam(int player) -- 0x005B9B80
// Slot 2: puts a valid player (virtual slot 1) on this team.

bool MultiplayerTeam::SetPlayerTeam(int player)
{
    if (!IsTeamIncluded(player)) {
        return false;
    }
    unknown_players_a3fa7c[player]->Team = Number;
    return true;
}


// int MultiplayerTeam::CountMembers() const -- 0x005B9BB0
// The number of players whose team number is this team's.

int MultiplayerTeam::CountMembers() const
{
    int count = 0;
    for (int index = 0; index < unknown_players_a3fa7c.Count(); index++) {
        if (unknown_players_a3fa7c[index]->Team == Number) {
            count++;
        }
    }
    return count;
}


// int MultiplayerTeam::AddToList(HWND combo) -- 0x005B9BE0
// Adds the team name to a combo box (CB_ADDSTRING) with the team number as
// the item data (CB_SETITEMDATA); returns the new item's index.

int MultiplayerTeam::AddToList(HWND combo)
{
    int item = SendMessageA(combo, 0x143, 0, (LPARAM)Name.unknown_text_770430());   // CB_ADDSTRING
    SendMessageA(combo, 0x151, item, Number);                                     // CB_SETITEMDATA
    return item;
}


// MultiplayerTeam scalar deleting destructor (0x005B9C20, ??_G): emitted with the
// vtable; the destructor (destroying Name) is expanded inline.
