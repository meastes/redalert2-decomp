// Skirmish test harness (README "Skirmish harness"; ra2-native --skirmish=<map>
// --run-to=<milestone>). Exercises the skirmish path before the menu dialogs
// exist: Main_Game calls these seams (port/fixups.json) instead of Select_Game,
// Main_Loop and the interface-art load. Without --skirmish each seam calls the
// recovered function unchanged. Test scaffolding, never game behaviour: it is
// written from the recovered skirmish dialog (Skirmish_Command 0x006829F0, its
// 0x617 "start" branch); Select_Game skips its menu loop through a seam.
#define MULTI_MISSION_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS   // Count() for the harness-objects note
#include "shared/win32_api.hpp"
#include "shared/session_state.hpp"
#include "shared/id_source.hpp"
#include "shared/rules.hpp"
#include "shared/ccini_class.hpp"
#include "shared/house_type.hpp"
#include "shared/main_opt.hpp"
#include "shared/multi_mission.hpp"
#include "shared/multiplayer_game_mode.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/debug_printf.hpp"

class TechnoClass;
extern DynamicVectorClass<TechnoClass *> Technos;   // 0x00A40C20 (shared/logic.hpp)

extern "C" {
const char *ra2port_skirmish_map(void);
int ra2port_skirmish_frames(void);
void ra2port_progress(const char *milestone, const char *detail);
}
bool __fastcall Select_Game(bool fade);                 // 0x005135F0
void __fastcall Unknown_006F1B60();                     // 0x006F1B60 (interface art)
extern char const *unknown_name_7f86e8;                 // its first file name (unsourced data: null)

namespace {

// Small string helpers (the dialect's units see the game's CRT declarations,
// not the host libc headers).
int Lower(int c) { return c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c; }
bool Same_Name(char const *file, char const *name)
{
    // file equals name, or name plus ".map", ignoring case
    while (*name && Lower(*file) == Lower(*name)) { ++file; ++name; }
    if (*name) return false;
    if (!*file) return true;
    return Lower(file[0]) == '.' && Lower(file[1]) == 'm' && Lower(file[2]) == 'a' && Lower(file[3]) == 'p' && !file[4];
}
void Copy(char *to, char const *from, int size)
{
    int i = 0;
    for (; from[i] && i < size - 1; ++i) to[i] = from[i];
    to[i] = 0;
}
void Append(char *to, char const *from, int size)
{
    int n = 0;
    while (to[n]) ++n;
    Copy(to + n, from, size - n);
}
void Append_Int(char *to, int value, int size)
{
    char digits[16];
    int n = 0;
    unsigned v = value < 0 ? -value : value;
    do { digits[n++] = '0' + v % 10; v /= 10; } while (v);
    char text[18];
    int k = 0;
    if (value < 0) text[k++] = '-';
    while (n) text[k++] = digits[--n];
    text[k] = 0;
    Append(to, text, size);
}
int To_Int(char const *text)
{
    int v = 0;
    while (*text >= '0' && *text <= '9') v = v * 10 + (*text++ - '0');
    return v;
}

// The map: "#<n>" picks the n-th scenario description, a name matches a
// description's file name (case-insensitive, with or without ".map"), any
// other name is passed to Start_Scenario as is.
void Choose_Map(char const *map)
{
    UnknownSessionState &session = unknown_session_a3d298;
    int count = session.Scenarios.Count();
    int index = -1;
    if (map[0] == '#') {
        index = To_Int(map + 1);
        if (index < 0 || index >= count) index = -1;
    } else {
        for (int i = 0; i < count && index < 0; ++i) {
            char const *file = session.Scenarios[i]->Get_Filename();
            if (file && Same_Name(file, map)) {
                index = i;
            }
        }
    }
    char detail[400];
    if (index >= 0) {
        session.ScenarioIndex = index;
        Copy(unknown_id_source_a3d290->FileName, session.Scenarios[index]->Get_Filename(), sizeof(unknown_id_source_a3d290->FileName));
    } else {
        Copy(unknown_id_source_a3d290->FileName, map, sizeof(unknown_id_source_a3d290->FileName));
    }
    Copy(detail, unknown_id_source_a3d290->FileName, sizeof detail);
    Append(detail, " (", sizeof detail);
    Append_Int(detail, count, sizeof detail);
    Append(detail, index >= 0 ? " scenario descriptions)" : " scenario descriptions; not among them, passed as a file name)", sizeof detail);
    ra2port_progress("harness-map", detail);
}

// What the skirmish dialog's start button stores (Skirmish_Command 0x617):
// one human player and one computer opponent, the session's saved options.
void Set_Up_Skirmish(char const *map)
{
    UnknownSessionState &session = unknown_session_a3d298;
    session.Type = 5;
    Unknown_0066F3A0(&session);                     // Read_MultiPlayer_Settings (Select_Game, cases 11/16/17)
    for (int house = 0; house < HouseTypes.Count(); house++) {
        HouseTypes[house]->Read_INI(*unknown_ini_8396d0);
    }
    Rules->Countries(*unknown_ini_8396d0);          // Skirmish_Dialog 0x00683BF0
    Rules->Sides(*unknown_ini_8396d0);
    for (int house = 0; house < HouseTypes.Count(); house++) {
        HouseTypes[house]->Read_INI(*unknown_ini_8396d0);
    }
    if (session.Mode == 0) {
        session.Mode = MultiplayerGameMode::unknown_first_mode_5b6ec0();
    }
    Choose_Map(map);
    session.AICount = 1;
    for (int player = 0; player < 8; player++) {
        session.AIDifficulty[player] = 0;
        session.AICountry[player] = -1;
        session.AIColor[player] = -1;
    }
    session.AIDifficulty[1] = 0;
    session.AICountry[1] = -3;                      // random
    session.AIColor[1] = 1;
    session.SkirmishOptions.GameMode = session.GameModeIndex;
    session.SkirmishOptions.ScenIndex = session.ScenarioIndex < 0 ? 0 : session.ScenarioIndex;
    for (int player = 1; player < 8; player++) {
        session.SkirmishOptions.Slots[player][0] = player == 1 ? 4 : 1;   // computer / none (Skirmish_Command's values)
        session.SkirmishOptions.Slots[player][1] = player == 1 ? -3 : -1;
        session.SkirmishOptions.Slots[player][2] = player == 1 ? 1 : -1;
    }
    session.Set_Value_00672a30(0, true);           // the local side (first country)
    session.Set_Value_00672ae0(0, true);           // the local colour
    NodeNameType *node = new NodeNameType;
    if (node) {
        static char const name[] = "Native";
        for (int i = 0; i < 20; ++i) node->Name[i] = i < 6 ? (unsigned short)name[i] : 0;
        node->unknown_4b = session.unknown_124;
        node->unknown_53 = session.unknown_11c;
        node->unknown_63 = -1;
        session.Players.Add(node);
    }
    session.Unknown_00672BB0();
    session.SkirmishOptions.GameSpeed = session.GameSpeed_30;
    session.SkirmishOptions.Credits = session.unknown_24;
    session.SkirmishOptions.UnitCount = session.UnitCount;
    session.unknown_fog_a7 = false;
    session.unknown_28 = true;
    session.unknown_a5 = false;
    session.unknown_34 = false;
}

int frames_done = 0;
}
namespace {

}  // namespace

bool Native_Select_Game_Seam(bool fade)
{
    char const *map = ra2port_skirmish_map();
    if (map == 0) {
        return Select_Game(fade);
    }
    static bool started = false;
    ra2port_progress("init", 0);
    if (started) {
        return false;           // one game per run
    }
    started = true;
    Set_Up_Skirmish(map);
    // Select_Game resets the per-game state, skips its menu loop (the
    // Native_Skirmish_Menu_Bypass seam) and starts the scenario in
    // unknown_id_source_a3d290->FileName. unknown_a40d13 stays false: it is
    // Debug_Map (the editor flag), under which the multiplayer setup makes no
    // computer houses or start units (0x0065E8C0) and the view runs in editor mode.
    bool started_game = Select_Game(fade);
    if (started_game) {
        ra2port_progress("scenario", unknown_id_source_a3d290->FileName);
        // What the scenario start made: the technos (0x00A40C20) and the
        // session's start-unit settings (Create_Units 0x00660320 / the game mode).
        char made[96] = "technos ";
        Append_Int(made, Technos.Count(), sizeof made);
        Append(made, ", UnitCount ", sizeof made);
        Append_Int(made, unknown_session_a3d298.UnitCount, sizeof made);
        Append(made, ", bases ", sizeof made);
        Append_Int(made, unknown_session_a3d298.unknown_bases_20 ? 1 : 0, sizeof made);
        Append(made, unknown_session_a3d298.Mode ? ", game mode set" : ", no game mode", sizeof made);
        ra2port_progress("harness-objects", made);
        // The host's cursor starts at (0, 0), the tactical view's top-left
        // edge, where the game scrolls the map every frame (edge scrolling).
        // A test run has no mouse: park the cursor in the tactical area.
        SetCursorPos(400, 300);
    } else {
        ra2port_progress("harness-stop", "Select_Game returned false: Start_Scenario failed");
    }
    return started_game;
}

// Select_Game's menu-loop seam (port/fixups.json): skipped under --skirmish.
bool Native_Skirmish_Menu_Bypass()
{
    return ra2port_skirmish_map() != 0;
}

bool Native_Main_Loop_Seam()
{
    bool over = unknown_53fbd0();
    if (ra2port_skirmish_map() != 0) {
        char detail[16] = "";
        Append_Int(detail, ++frames_done, sizeof detail);
        ra2port_progress("frame", detail);
        if (frames_done >= ra2port_skirmish_frames()) {
            ra2port_progress("frames", detail);
        }
    }
    return over;
}

void Native_Interface_Art_Seam()
{
    if (ra2port_skirmish_map() != 0 && unknown_name_7f86e8 == 0) {
        // The 19 interface-art file names (0x007F86E8..) are unsourced data
        // (a user decision is pending); the art is the menus' and dialogs'.
        ra2port_progress("harness-art", "interface art skipped (its 19 file names are unsourced)");
        return;
    }
    Unknown_006F1B60();
}
