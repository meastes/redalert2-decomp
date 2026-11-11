// Behaviour rows (Scout1133, native M16; NOT EXACT, native/behaviour_rewrites.json):
// leaves the load game, the map choice and the campaign start reach that have
// no recovered body.
#include "shared/unknown_globals.hpp"

// 0x006EA9B0: the first call of Post_Load_Game 0x0065D2C0; the original body is
// a lone `ret` (c3, then int3/nop padding).
void Unknown_006EA9B0()
{
}

// The key functions of two game-mode vtables (clang emits a class's vtable with
// its first non-inline virtual; neither was recovered, so the vtables were
// link-closure placeholders and the Choose Map dialog's mode list,
// MultiplayerGameMode::unknown_fill_list_5b71e0, called through a null slot).
#include "shared/multiplayer_game_mode.hpp"

// 0x005A7500 (scalar deleting destructor, pass-6a decompile): sets the vtable
// and runs ~MultiplayerGameMode; the destructor itself adds nothing.
MultiplayerManBattle::~MultiplayerManBattle()
{
}

// 0x005AA800 (pass-6a decompile): forwards to MultiplayerTeam's slot 1.
bool MultiplayerObserverTeam::IsTeamIncluded(int unknown)
{
    return MultiplayerTeam::IsTeamIncluded(unknown);
}

// 0x005E8710 (pass-6a decompile; lobby.hpp): the Choose Map dialog's message
// 0x4EC visits every child control with OwnerDraw 0x005E86D0.
#include "shared/lobby.hpp"
#include "shared/owner_draw_dialog_control.hpp"
namespace OwnerDraw {
BOOL __stdcall unknown_5e86d0(HWND window, LPARAM);   // 0x005E86D0 (src/recovered/ownrdraw)
}
void __fastcall Unknown_005e8710(HWND dialog)
{
    EnumChildWindows(dialog, OwnerDraw::unknown_5e86d0, 0);
}

// 0x00453B70 (TechnoClass slot 177; bytes b8 02 00 00 00 c3): returns 2. A
// voxel building's draw (Render_Voxel_Object 0x006D24E0) passes it to the blit.
#include "shared/techno.hpp"
int TechnoClass::unknown_slot177()
{
    return 2;
}


// 0x006631A0 (pass-6a decompile): the map cell of a waypoint, Map[Waypoints[waypoint]]
// (TeamClass script missions; Allied 1's first team).
#include "shared/id_source.hpp"
#include "shared/map.hpp"
#include "shared/map_view.hpp"
CellClass *UnknownIDSource::unknown_6631a0(int waypoint)
{
    return &Map[Waypoints[waypoint]];
}

