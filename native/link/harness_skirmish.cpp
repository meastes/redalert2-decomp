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
#include "shared/main_loop.hpp"
#include "shared/cc_file_class.hpp"
#include <stdlib.h>
#include <stdio.h>

class TechnoClass;
extern DynamicVectorClass<TechnoClass *> Technos;   // 0x00A40C20 (shared/logic.hpp)

extern "C" {
const char *ra2port_skirmish_map(void);
int ra2port_skirmish_frames(void);
void ra2port_frame(int frame);
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
// one human player and one computer opponent (Russians), the session's saved options.
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
    // RA2_NATIVE_SKIRMISH_AI=<n> (1-7, default 1): n computer players in slots
    // 1..n, as the dialog's player combos store them; RA2_NATIVE_SKIRMISH_AI_LEVEL
    // (0 easy, 1 medium, 2 brutal) their difficulty (Scout1187, M23).
    int ai_count = 1;
    if (char const *env = getenv("RA2_NATIVE_SKIRMISH_AI")) {
        ai_count = To_Int(env);
        if (ai_count < 1) ai_count = 1;
        if (ai_count > 7) ai_count = 7;
    }
    int ai_level = 0;
    if (char const *env = getenv("RA2_NATIVE_SKIRMISH_AI_LEVEL")) {
        ai_level = To_Int(env);
        if (ai_level > 2) ai_level = 2;
    }
    if (getenv("RA2_NATIVE_LIST_MAPS")) {
        for (int i = 0; i < session.Scenarios.Count(); ++i) {
            char line[200] = "#";
            Append_Int(line, i, sizeof line);
            Append(line, " ", sizeof line);
            Append(line, session.Scenarios[i]->Get_Filename(), sizeof line);
            Append(line, " players ", sizeof line);
            Append_Int(line, session.Scenarios[i]->unknown_180, sizeof line);
            Append(line, "-", sizeof line);
            Append_Int(line, session.Scenarios[i]->unknown_184, sizeof line);
            fprintf(stderr, "harness-maplist %s\n", line);
        }
    }
    session.AICount = ai_count;
    for (int player = 0; player < 8; player++) {
        session.AIDifficulty[player] = 0;
        session.AICountry[player] = -1;
        session.AIColor[player] = -1;
    }
    // The opponent's country. The dialog resolves "random" (-3) before the game
    // starts: Assign_Houses 0x0065FBB0 makes no house for -1 or -3, so with -3 the
    // game had no computer player (Scout1111). The harness names one: Russians.
    static char const *const ai_countries[7] = {"Russians", "Arabs", "Germans", "Confederation", "British", "French", "Africans"};
    for (int player = 1; player <= ai_count; player++) {
        int country = HouseTypeClass::Find_Index(ai_countries[player - 1]);
        session.AIDifficulty[player] = ai_level;
        session.AICountry[player] = country >= 0 ? country : 1;
        session.AIColor[player] = player;
    }
    session.SkirmishOptions.GameMode = session.GameModeIndex;
    session.SkirmishOptions.ScenIndex = session.ScenarioIndex < 0 ? 0 : session.ScenarioIndex;
    for (int player = 1; player < 8; player++) {
        bool computer = player <= ai_count;
        session.SkirmishOptions.Slots[player][0] = computer ? 4 + ai_level : 1;   // computer (4 easy .. 6 brutal) / none (Skirmish_Command's values)
        session.SkirmishOptions.Slots[player][1] = computer ? session.AICountry[player] : -1;
        session.SkirmishOptions.Slots[player][2] = computer ? player : -1;
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
    // RA2_NATIVE_SKIRMISH_SPEED=<0-6> (Scout1191, M24; test scaffolding): the speed slider's value
    // (0 fastest: the port's 60 frames per second floor) for frame-time runs.
    if (char const *speed = getenv("RA2_NATIVE_SKIRMISH_SPEED")) unknown_a40b18 = session.GameSpeed_30 = session.SkirmishOptions.GameSpeed = atoi(speed);   // as Skirmish_Command 0x006829F0 stores it
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
    // unknown_id_source_a3d290->FileName. Debug_Map stays false: it is
    // Westwood's editor flag, under which the multiplayer setup makes no
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

void Native_Harness_Orders(int frame);   // harness_orders.cpp
void Native_Harness_Input(int frame);    // harness_input.cpp
void Native_Harness_Save(int frame);     // harness_save.cpp
void Native_Checkpoint();               // harness_save.cpp (host checkpoints)
void Native_Harness_Event_Test(int frame);   // harness_event_test.cpp (RA2_NATIVE_EVENT_TEST)
void Native_Harness_Theme_Test(int frame);   // harness_theme_test.cpp (RA2_NATIVE_THEME_TEST)
// RA2_NATIVE_FILE_DUMP="<name>:<out path>[;...]" (Scout1236, test scaffolding): at the
// first frame of a game each named game file is read through the game's own file layer
// (CCFileClass: loose files, then the mixes) and written to the out path, so a scripted
// campaign run can be planned from the mission's INI (its triggers and lose conditions).
static void Native_Harness_File_Dump(int frame)
{
    if (frame != 1) return;
    char const *env = getenv("RA2_NATIVE_FILE_DUMP");
    for (char const *p = env; p && *p;) {
        char name[64], out[512];
        int n = 0;
        while (*p && *p != ':' && n < 63) name[n++] = *p++;
        name[n] = 0;
        if (*p != ':') break;
        ++p;
        n = 0;
        while (*p && *p != ';' && n < 511) out[n++] = *p++;
        out[n] = 0;
        if (*p == ';') ++p;
        CCFileClass file(name);
        if (!file.Is_Available()) { fprintf(stderr, "file-dump: %s not found\n", name); continue; }
        long size = file.Size();
        char *buffer = (char *)malloc(size > 0 ? (size_t)size : 1);
        long got = file.Read(buffer, size);
        file.Close();
        FILE *f = fopen(out, "wb");
        if (f) { fwrite(buffer, 1, (size_t)(got > 0 ? got : 0), f); fclose(f); }
        fprintf(stderr, "file-dump: %s %ld bytes -> %s\n", name, got, out);
        free(buffer);
    }
}
void Native_Note_Frame();                   // native_event_data.cpp

// Native frame pacing (user report 2026-11-09, M20: real play "far too fast").
// The skirmish dialog's speed slider stores 6 - position in Options.GameSpeed
// (Skirmish_Command 0x006829F0); rules.ini [MultiplayerDialogSettings]
// GameSpeed=0 ("fastest") is the slider's default. Main_Loop then sets the
// frame timer 0x00839990 to 0 and Sync_Delay 0x00540960 does not wait: the
// original ran as fast as the machine drew (DirectDraw blits of its day; the
// CnCNet patches cap the same speed at 60 frames per second). Natively a frame
// takes about 10 ms, so speed 0 ran near 100 frames per second. The port keeps
// one SystemTimerClass tick (16 ms, timeGetTime() >> 4: about 62 frames per
// second) as the floor for a frame whose timer was 0, in single player and
// skirmish only (network games pace themselves). Not original code.
// RA2_NATIVE_UNCAPPED=1 restores the uncapped loop (test runs).
extern "C" uint64_t ra2port_monotonic_ns(void);
extern "C" void ra2port_sleep_until_ns(uint64_t deadline);
// Scout1191 (M24): the floor keeps a deadline 16 ms after the previous one (at the
// clock's nanosecond resolution) instead of sleeping whole milliseconds from the
// frame's end: the old floor overshot by about 1.5 ms a frame (57 frames per
// second measured with 3 AIs, not the intended 62). A frame that ran late starts
// a new deadline from now (no catch-up burst).
// The host asks before it pauses the game on focus loss (platform/sdl/host.cpp,
// Scout1247): not in a network session (lobby or game), which the other players'
// games pace; single player (0) and skirmish (5) may pause. Not original code.
extern "C" int ra2_native_network_game(void)
{
    int type = unknown_session_a3d298.Type;
    return type != 0 && type != 5;
}

static void Native_Frame_Floor()
{
    static int uncapped = -1;
    if (uncapped < 0) uncapped = getenv("RA2_NATIVE_UNCAPPED") ? 1 : 0;
    static uint64_t deadline = 0;
    uint64_t const period = 16000000u;
    int type = unknown_session_a3d298.Type;
    if (uncapped || unknown_a40b18 > 0 || (type != 0 && type != 5)) {
        deadline = 0;
        return;
    }
    uint64_t now = ra2port_monotonic_ns();
    if (deadline && now < deadline) {
        ra2port_sleep_until_ns(deadline);
        deadline += period;
    } else {
        deadline = now + period;
    }
}

// Frame work times (Scout1191, M24; test diagnostics): the wall time of one
// Main_Loop pass less the time its thread slept (Sync_Delay's pacing), summed per
// 1,000 frames into the frames-<n>k note: mean, 95th percentile and worst.
extern "C" thread_local uint64_t ra2port_slept_ns;
static unsigned frame_work_us[1000];
static int frame_work_count = 0;

bool Native_Main_Loop_Seam()
{
    uint64_t work_start = ra2port_monotonic_ns(), slept_start = ra2port_slept_ns;
    bool over = unknown_53fbd0();
    {
        uint64_t spent = ra2port_monotonic_ns() - work_start - (ra2port_slept_ns - slept_start);
        if (frame_work_count < 1000) frame_work_us[frame_work_count++] = (unsigned)(spent / 1000u);
    }
    Native_Frame_Floor();
    Native_Note_Frame();   // native_event_data.cpp: Sync_Delay's in-wait pass waits for a stalled frame
    // Every game counts its frames (a game started from the menus too:
    // --frames=N, scripted input by frame; Scout1119, M14).
    {
        char detail[16] = "";
        Append_Int(detail, ++frames_done, sizeof detail);
        ra2port_progress("frame", detail);
        Native_Harness_Orders(frames_done);
        Native_Harness_Input(frames_done);
        Native_Harness_Save(frames_done);
        Native_Checkpoint();
        Native_Harness_Event_Test(frames_done);
        Native_Harness_File_Dump(frames_done);
        Native_Harness_Theme_Test(frames_done);
        ra2port_frame(frames_done);     // scripted input due at the next frame (--input-script)
        if (frames_done >= ra2port_skirmish_frames()) {
            ra2port_progress("frames", detail);
        }
        // Every 1,000 frames a milestone with its time (frame rate), and the
        // end of the game as Main_Loop saw it (Scout1125, M15): the player won,
        // lost, restarted or aborted (Main_Loop_End 0x0053F840 then runs
        // Do_Win/Do_Lose: the score screen) or the game is no longer active.
        if (frames_done % 1000 == 0 && frames_done <= 60000) {
            static char names[60][16];
            char *name = names[frames_done / 1000 - 1];
            name[0] = 0;
            Append(name, "frames-", sizeof names[0]);
            Append_Int(name, frames_done / 1000, sizeof names[0]);
            Append(name, "k", sizeof names[0]);
            static char at_buffer[128];
            char *at = at_buffer;
            at[0] = 0;
            Append(at, "game frame ", 128);
            Append_Int(at, Frame, 128);
            {
                // mean / p95 / max work per frame over the last 1,000 frames, in tenths of a millisecond
                unsigned sorted[1000];
                int n = frame_work_count;
                unsigned long long sum = 0;
                for (int i = 0; i < n; ++i) { sorted[i] = frame_work_us[i]; sum += sorted[i]; }
                for (int i = 1; i < n; ++i) { unsigned v = sorted[i]; int j = i - 1; while (j >= 0 && sorted[j] > v) { sorted[j + 1] = sorted[j]; --j; } sorted[j + 1] = v; }
                static char work[3][16];
                unsigned values[3] = {n ? (unsigned)(sum / n) : 0u, n ? sorted[n * 95 / 100] : 0u, n ? sorted[n - 1] : 0u};
                static char const *labels[3] = {" work ms mean ", " p95 ", " max "};
                for (int k = 0; k < 3; ++k) {
                    Append(at, labels[k], 128);
                    Append_Int(at, (int)(values[k] / 1000u), 128);
                    Append(at, ".", 128);
                    Append_Int(at, (int)(values[k] / 100u % 10u), 128);
                }
                (void)work;
                frame_work_count = 0;
            }
            ra2port_progress(name, at);
        }
        static bool ended = false;
        static int games_ended = 0;
        bool ending = player_wins_a35db1 || player_loses_a40c78 || player_restarts_866250 || player_aborts_a35db0 || !unknown_flag_a40958;
        if (ended && !ending) ended = false;     // a later game (a loaded one) runs
        if (!ended && ending) {
            ended = true;
            ++games_ended;
            char why[96] = "";
            Append(why, player_wins_a35db1 ? "player wins" : player_loses_a40c78 ? "player loses" : player_restarts_866250 ? "restart"
                : player_aborts_a35db0 ? "abort" : "game no longer active", sizeof why);
            Append(why, " at frame ", sizeof why);
            Append_Int(why, frames_done, sizeof why);
            ra2port_progress(games_ended == 1 ? "game-over" : "game-over-2", why);
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
