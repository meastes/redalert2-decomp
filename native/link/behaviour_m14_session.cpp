// Behaviour rows (Scout1119, native M14; NOT EXACT, native/behaviour_rewrites.json):
// session leaves the skirmish dialog reaches, from the pass-6a decompiles.
#define MULTI_MISSION_ACCESSORS
#include "shared/session_state.hpp"
#include "shared/multi_mission.hpp"
#include "shared/lobby.hpp"

extern "C" int __cdecl _stricmp(char const *a, char const *b);   // 0x00781F20

// 0x006720C0 (decompile: _stricmp(this + 0x58, "RandMap.Sed") == 0): whether the
// map is the random map generator's seed file rather than a map.
bool MultiMission::Unknown_006720C0()
{
    return _stricmp(unknown_58, "RandMap.Sed") == 0;
}

// 0x00672810 (decompile, on the object at 0x00A3D2B0 = the session + 0x18: clears
// +0x24 and sets the eight words at +0x4C and +0x6C to -1): no computer players,
// their countries and colours unset. Natively through the session's members
// (AICount +0x3C, AICountry +0x64, AIColor +0x84); the object is only ever
// unknown_lobby_state_a3d2b0.
void UnknownLobbyState::Unknown_00672810()
{
    UnknownSessionState &session = unknown_session_a3d298;
    session.AICount = 0;
    for (int i = 0; i < 8; ++i) {
        session.AICountry[i] = -1;
        session.AIColor[i] = -1;
    }
}

// The name of a game mode for unknown_set_dialog_wide_text 0x005C3450 (port/fixups.json):
// MultiplayerGameMode::unknown_name_20's buffer, or the empty wide string (0x00770430).
#include "shared/multiplayer_game_mode.hpp"
extern wchar_t const unknown_empty_wide_839d4c[];
unsigned short const *Native_Game_Mode_Name_005c3450(void const *mode)
{
    unsigned short const *text = ((MultiplayerGameMode const *)mode)->unknown_name_20.Get();
    return text ? text : (unsigned short const *)unknown_empty_wide_839d4c;
}
