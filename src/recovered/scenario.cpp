// scenario.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/scenario.json carries the addresses and the rows that
// stay in their own files.


#define RA2_COORD_TPOINT3D
#define RA2_COORD_CELL_BASE_INIT
#define RA2_COORD_FROM_CELL_INLINE
#define RA2_SCENARIO_CLEAR_MAP23
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/id_source.hpp"
#include "shared/special.hpp"
#include "shared/tag.hpp"
#include "shared/abstract_type.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/crt_string.hpp"
#include "shared/type_list.hpp"
#include "shared/warhead_type.hpp"
#include "shared/string_table.hpp"
#include "shared/campaign.hpp"
#include "shared/session_state.hpp"
#include "shared/multi_mission.hpp"
#include "shared/cd.hpp"
#include "shared/theme.hpp"
#include "shared/mouse.hpp"
#include "shared/ccini_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/movie_globals.hpp"
#include "shared/options.hpp"
#include "shared/movie_list.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/win32_api.hpp"
#include "shared/debug_printf.hpp"
#include "shared/gscreen.hpp"
#include "shared/surface.hpp"
#include "shared/dsurface.hpp"
#include "shared/audio.hpp"
#include "shared/tool_tip.hpp"
#include "shared/map_view.hpp"
#include "shared/keyboard.hpp"
#include "shared/progress_screen.hpp"
#include "shared/unknown_map_generator.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/house_type.hpp"
#include "shared/rules.hpp"
#include "shared/connection.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/cd_timer.hpp"
#include "shared/owner_talk.hpp"
#include "shared/scenario_checkpoint.hpp"
#include "shared/building_class.hpp"
#include "shared/scenario_tags.hpp"
#include "shared/house.hpp"
#include "shared/tiberium.hpp"
#include "shared/cell.hpp"
#include "shared/wave.hpp"
#include "shared/particle_system.hpp"
#include "shared/anim.hpp"
#include "shared/random.hpp"
#include "shared/building_type_class.hpp"
#include "shared/veinhole.hpp"
#include "shared/path_estimator.hpp"
#include "shared/win32_window.hpp"
#include "shared/object.hpp"
#include "shared/tag_type.hpp"
#include "shared/isometric_tile.hpp"
#include "shared/light_source.hpp"
#include "shared/vector_class.hpp"
#include "shared/abstract.hpp"
#include "shared/empulse_class.hpp"
#include "shared/tactical.hpp"
#include "shared/logic.hpp"
#include "shared/kamikaze_tracker.hpp"
#include "shared/unit.hpp"
#include "shared/lightning_storm.hpp"
#include "shared/system_timer.hpp"
#include "shared/t_timer.hpp"
#include "shared/comm_buffer.hpp"
#include "shared/vox_class.hpp"
#include "shared/crt_memory.hpp"
#define Max(a, b) ((a) < (b) ? (b) : (a))
#include "shared/unknown_loadout.hpp"
#include "shared/shape_file.hpp"
#include "shared/team_type.hpp"
#include "shared/script.hpp"
#include "shared/task_force.hpp"
#include "shared/trigger_type.hpp"
#include "shared/tube.hpp"
#include "shared/ai_trigger_type.hpp"
#include "shared/terrain.hpp"
#include "shared/overlay.hpp"
#include "shared/smudge.hpp"
#define Min(a, b) ((a) < (b) ? (a) : (b))
#include "shared/raw_file_class.hpp"
#include "shared/map_preview.hpp"
#include "shared/unit_type.hpp"
#include "shared/infantry_type.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/wstring.hpp"
#include "shared/wide_string.hpp"
#include "shared/cell_class.hpp"
#include "shared/dir_type.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/cpu_info.hpp"
#include "shared/waypoint_string.hpp"


// Header-static dynamic initializers of the unit at 0x0065B2C0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x0065B2C0-0x0065B4A6); the other
// drafts of this plan are emitted by these includes.
// The unit's shape helpers fall back to this rectangle (shared/shape_file.hpp):
// the unit has no empty-rect initializer of its own.


// Dynamic initializer (0x0065B2F0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0065b2c0.cpp; no source of its own.


// Dynamic initializer (0x0065B310, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0065b2c0.cpp; no source of its own.


// Dynamic initializer (0x0065B330, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0065b2c0.cpp; no source of its own.


// Dynamic initializer (0x0065B350, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0065b2c0.cpp; no source of its own.


// Dynamic initializer (0x0065B370, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0065b2c0.cpp; no source of its own.


// Dynamic initializer (0x0065B390, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0065b2c0.cpp; no source of its own.


// Dynamic initializer (0x0065B3D0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0065b2c0.cpp; no source of its own.


// Dynamic initializer (0x0065B400, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0065b2c0.cpp; no source of its own.


// Dynamic initializer (0x0065B430, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0065b2c0.cpp; no source of its own.


// Dynamic initializer (0x0065B460, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0065b2c0.cpp; no source of its own.


// Dynamic initializer (0x0065B470, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0065b2c0.cpp; no source of its own.


// Dynamic initializer (0x0065B490, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_0065b2c0.cpp; no source of its own.


// 0x0065b4b0: function not yet identified
//
// Body: 37 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void unknown_timer_init_0065b4b0(void)
// Callees with accepted source:
//   unknown_timer_atexit_0065b4e0 -> src/recovered/0065b4e0.cpp
//   atexit_0078298a -> src/recovered/0078298a.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0065b4b0 {

extern void __cdecl unknown_timer_atexit_0065b4e0(void);
struct Callee_0069a430 { unsigned int SystemTimer_Value_0069a430(); };
extern unsigned int  atexit_0078298a(unsigned int);
extern "C" unsigned int unknown_system_timer_00ab6eac;
extern "C" unsigned int _unknown_timer_start_00ab6ea8;
extern "C" unsigned int _unknown_timer_accum_00ab6eb0;

void  unknown_timer_init_0065b4b0(void)
{
    _unknown_timer_start_00ab6ea8 = ((Callee_0069a430*)(&unknown_system_timer_00ab6eac))->SystemTimer_Value_0069a430();
    _unknown_timer_accum_00ab6eb0 = 0;
    atexit_0078298a((unsigned int)(unknown_timer_atexit_0065b4e0));
    return;
}

}  // namespace seed_0065b4b0


// 0x0065b4e0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void unknown_timer_atexit_0065b4e0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0065b4e0 {

void  unknown_timer_atexit_0065b4e0(void)
{
    return;
}

}  // namespace seed_0065b4e0


// ScenarioClass::ScenarioClass() (0x0065B4F0)
// Builds the scenario: generator seeded with 0, timers stopped at zero,
// empty loadout lists growing by 10, the defaults (Set_Defaults), the special
// options, tech level 1, no campaign, names, briefing, waypoints, variables
// or views.

ScenarioClass::ScenarioClass()
{
    Set_Defaults();
    unknown_22fc = false;
    unknown_223b = false;
    Special().Init();
    TechLevel = 1;
    unknown_224c = -1;
    NextScenario[0] = '\0';
    AltNextScenario[0] = '\0';
    for (int index = 0; index < 101; index++) {
        Waypoints[index] = unknown_geometry_k;
    }
    wcscpy(Name, unknown_empty_wide_839d4c);
    strcpy(FileName, unknown_string_83c55c);
    strcpy(Briefing, unknown_string_83c55c);
    strcpy(UIName, unknown_string_83c55c);
    wcscpy(BriefingText, unknown_empty_wide_839d4c);
    for (int variable = 0; variable < 50; variable++) {
        Variables[variable].unknown_00[0] = '\0';
        unknown_6612c0(variable, false);
    }
    for (int local = 0; local < 50; local++) {
        LocalVariables[local].unknown_00[0] = '\0';
        unknown_661560(local, false);
    }
    memset(Views, 0, sizeof(Views));
}


// Not exact (RA2_NON_MATCHING): ScenarioClass::ScenarioClass(NoInitClass const &) at 0x0065B780, best score 64.00, 2026-11-13; differs: 45 instructions in the original, 55 in the draft (shape 90.00)
#ifdef RA2_NON_MATCHING
// ScenarioClass::ScenarioClass(NoInitClass const &noinit) (0x0065B780)

// TibSun timer.h: TTimerClass(NoInitClass const & x) : T(x) {}
template <class T>
inline TTimerClass<T>::TTimerClass(NoInitClass const &)
{
}

ScenarioClass::ScenarioClass(NoInitClass const &noinit) :
    ElapsedTime(noinit),
    MissionTimer(noinit),
    MissionTimerLabel(noinit),
    unknown_7f8(noinit),
    unknown_804(noinit),
    unknown_810(noinit),
    unknown_81c(noinit),
    unknown_828(noinit),
    unknown_2240(noinit)
{
    if (MissionTimerLabel[0] != 0) {
        MissionTimerText = StringTable::Load_String(MissionTimerLabel, 0, "D:\\Ra2\\Scenario.CPP", 340);
    } else {
        MissionTimerText = unknown_empty_wide_839d4c;
    }
}
#endif

// void ScenarioClass::Set_Defaults() (0x0065B830)

void ScenarioClass::Set_Defaults()
{
    HomeCell = 98;
    AltHomeCell = 98;
    UniqueID = 1000000;
    Difficulty = 1;
    unknown_610 = 1;
    ElapsedTime = 0;
    ElapsedTime.Stop();
    unknown_620 = 0;
    unknown_620.Stop();
    unknown_62c = 0;
    unknown_630 = false;
    MissionTimer = 0;
    MissionTimer.Stop();
    MissionTimerText = unknown_empty_wide_839d4c;
    MissionTimerLabel = FixedString<32>(unknown_string_83c55c);
    unknown_7f8 = 0;
    unknown_804 = 0;
    unknown_810 = 0;
    unknown_81c = 0;
    unknown_828 = 0;
    Theater = -1;
    IntroMovie = -1;
    BriefMovie = -1;
    WinMovie = -1;
    LoseMovie = -1;
    ActionMovie = -1;
    PostScoreMovie = -1;
    PreMapSelectMovie = -1;
    Theme = -1;
    unknown_11fc = 0;
    CarryOverMoney = 0;
    unknown_1208 = 0;
    Percent = 0;
    unknown_2224 = 0;
    FreeRadar = false;
    unknown_2229 = false;
    unknown_222a = true;
    unknown_222b = true;
    unknown_222c = true;
    unknown_222d = false;
    unknown_222e = false;
    unknown_222f = false;
    EndOfGame = false;
    unknown_2231 = false;
    SkipScore = false;
    OneTimeOnly = false;
    SkipMapSelect = false;
    TruckCrate = false;
    FillSilos = false;
    unknown_2238 = false;
    unknown_2239 = true;
    MultiplayerOnly = false;
    unknown_223c = false;
    unknown_2240 = 0;
    StartingDropships = 0;
    unknown_22a8 = 100;
    unknown_22ac = 100;
    unknown_22b0 = 100;
    AmbientRed = 100;
    AmbientGreen = 100;
    AmbientBlue = 100;
    AmbientGround = 100;
    AmbientLevel = 16;
    IonAmbient = 100;
    IonRed = 0;
    IonGreen = 0;
    IonBlue = 0;
    IonGround = 100;
    IonLevel = 16;
    unknown_22e0 = 200;
    unknown_22e4 = 175;
    unknown_22e8 = 150;
    unknown_22ec = 125;
    unknown_22f0 = 100;
    unknown_22f4 = 100;
    NukeAmbientChangeRate = 1;
    InitTime = 10000;
    unknown_2304 = 0;
    unknown_2306 = false;
    TiberiumDeathToVisceroid = true;
    ParTimeEasy = 3600;
    ParTimeMedium = 3600;
    ParTimeHard = 3600;
    UnderParTitle[0] = 0;
    UnderParMessage[0] = 0;
    OverParTitle[0] = 0;
    OverParMessage[0] = 0;
    LSLoadMessage[0] = 0;
    LSLoadBriefing[0] = 0;
    LS640BriefLocX = 0;
    LS640BriefLocY = 0;
    LS800BriefLocX = 0;
    LS800BriefLocY = 0;
    LS640BkgdName[0] = 0;
    LS800BkgdName[0] = 0;
    AllowableUnits.Clear();
    AllowableUnitMaximums.Clear();
    AllowableUnitCounts.Clear();
    if (unknown_game_mode_a3d298 == 5) {
        wcscpy(unknown_id_source_a3d290->Name, StringTable::Load_String("GUI:SkirmishGame", 0, "D:\\Ra2\\Scenario.CPP", 486));
    }
}


// bool __fastcall Start_Scenario_0065bc70(char const *name, bool briefing, int campaign) (0x0065BC70)
// Westwood's Start_Scenario: an empty name takes the campaign's scenario;
// records the campaign and the file name, asks for the disk the campaign or
// the multiplayer map is on, plays the scenario's Intro (or Brief) movie,
// reads the scenario (0x0065C7D0), plays the dropship loadout and (briefing)
// the Action movie, resets the display, starts the elapsed time and the
// scenario's theme. False when the disk or the scenario fails. The Brief
// movie's file name is formatted into a buffer nobody reads (as in the
// original); the flag test compares with true (cmp byte ptr, 1).
// The inventory bound (584 bytes, to 0x0065BEB8) stops inside the function:
// the code continues by fall-through to its ret 4 at 0x0065C039 (972 bytes).

bool __fastcall Start_Scenario_0065bc70(char const *name, bool briefing, int campaign)
{
    if ((name == 0 || strlen(name) == 0) && campaign != -1) {
        if (unknown_flag_a40d05 == true) {
            name = unknown_scenario_name_a35ea8;
        } else {
            name = Campaigns[campaign]->Scenario;
        }
    }
    unknown_id_source_a3d290->unknown_224c = campaign;
    Debug_Printf("\n----- Starting scnenario: %s -----\n", name);
    Debug_Printf("Player Count: %d\n", unknown_players_a3fa7c.Count());
    strcpy(unknown_id_source_a3d290->FileName, name);
    _strupr(unknown_id_source_a3d290->FileName);

    CD::Set_Required_CD(-1);
    if (unknown_game_mode_a3d298 == 0) {
        if (unknown_id_source_a3d290->unknown_224c != -1) {
            CD::Set_Required_CD(Campaigns[unknown_id_source_a3d290->unknown_224c]->unknown_98);
        }
    } else if (unknown_session_a3d298.ScenarioIndex != -1) {
        MultiMission *mission = unknown_multi_missions_a3d8d8[unknown_session_a3d298.ScenarioIndex];
        if (!mission->Unknown_00671F00(Unknown_0046E4C0(60))) {
            CD::Set_Required_CD(mission->Unknown_00671F90());
        }
    }
    unknown_counter_a3fabc++;
    if (!CD().Force_Available(CD::RequiredCD)) {
        return false;
    }
    unknown_counter_a3fabc--;
    WWMouse->Hide_Mouse();

    {
        CCINIClass ini;
        CCFileClass file(name);
        if (ini.Load(file, true, false)) {
            int movie = ini.Get_Listed_Name("Basic", "Intro", -1);
            if (movie == -1) {
                movie = ini.Get_Listed_Name("Basic", "Brief", movie);
            }
            if (movie != -1) {
                Theme.Stop();
                Unknown_005A07F0(movie, -1, 1, 1, 1);
            }
        }
    }

    Debug_Printf("Reading scenario: %s\n", name);
    Theme.Play_Song(Theme.From_Name("LOADING"));
    if (!Unknown_0065c7d0(name)) {
        WWMouse->Show_Mouse();
        return false;
    }
    int brief = unknown_id_source_a3d290->BriefMovie;
    if (brief != -1) {
        char movie_name[28];
        wsprintfA(movie_name, "%s.VQA", unknown_names_a71050[brief]);
    }
    if (unknown_id_source_a3d290->StartingDropships > 0) {
        Unknown_004A6B90();
    }
    if (briefing) {
        Unknown_005A07F0(unknown_id_source_a3d290->ActionMovie, -1, 1, 1, 1);
    }
    Unknown_Free_Art_Set_006F30F0();
    Options.Set();
    unknown_surface_839954->Fill(0);
    Unknown_004E0C90(true, unknown_surface_839954, 0);
    Toggle_Display_Mode_0065c480(true);
    unknown_id_source_a3d290->ElapsedTime.Start();
    if (unknown_id_source_a3d290->Theme != -1) {
        Theme.Queue_Song(unknown_id_source_a3d290->Theme);
    } else {
        Theme.Stop(true);
    }
    unknown_flag_a40d04 = true;
    unknown_flag_a40380 = true;
    return true;
}


// void __fastcall Focus_Loss_0065c040() (0x0065C040)
// The game window lost the focus: pauses the sound and keeps the global
// volume (full volume meanwhile), stops the elapsed time in campaign and
// skirmish games, pauses the radar movie, turns tooltips off, aborts a drag
// select, drops a mouse override and redraws the screen with the mouse
// hidden (Westwood's Focus_Loss).

void __fastcall Focus_Loss_0065c040()
{
    Audio_Pause_All();
    unknown_saved_volume_7f1dec = Audio_Global_Levels[7]->Volume.Get_Target();
    Audio_Global_Levels[7]->Volume.Set(0x4000);
    if (unknown_game_mode_a3d298 == 0 || unknown_game_mode_a3d298 == 5) {
        Debug_Printf("Paused ElapsedTimer = %ld\n", unknown_id_source_a3d290->ElapsedTime.Time());
        unknown_id_source_a3d290->ElapsedTime.Stop();
    }
    Pause_Radar_Movie_005a0a90(true);
    if (ToolTips) {
        ToolTips->Enable(false);
    }
    Map.Abort_Drag_Select();
    Map.Override_Mouse_Shape(0, false);
    WWMouse->Hide_Mouse();
    Map.Flag_To_Redraw(2);
    Map.Render();
    WWMouse->Show_Mouse();
}


// void Focus_Restore_0065c140() (0x0065C140)
// The game window regained the focus: restores the mouse shape, restarts
// the elapsed time, restores the tooltips and the global volume, resumes the
// radar movie and the sound (Westwood's Focus_Restore).

void Focus_Restore_0065c140()
{
    Map.Revert_Mouse_Shape();
    unknown_id_source_a3d290->ElapsedTime.Start();
    Debug_Printf("Resume ElapsedTimer = %ld\n", unknown_id_source_a3d290->ElapsedTime.Time());
    if (ToolTips) {
        ToolTips->Enable(unknown_tooltips_enabled_a40b34);
    }
    Audio_Global_Levels[7]->Volume.Set(unknown_saved_volume_7f1dec);
    unknown_saved_volume_7f1dec = 0x4000;
    Pause_Radar_Movie_005a0a90(false);
    Audio_Resume_All();
}


// void __fastcall Pause_Count_0065c1f0(int seconds) (0x0065C1F0)
// Raises the pause count. A positive seconds (re)starts the +0x620 timer
// (seconds * 60 ticks) and does nothing more when one already ran. The first
// pause pauses the sound, stops the elapsed time in campaign and skirmish
// games and pauses the game unless it already was.

void __fastcall Pause_Count_0065c1f0(int seconds)
{
    if (seconds) {
        unknown_id_source_a3d290->unknown_620 = seconds * 60;
        if (unknown_id_source_a3d290->unknown_630) {
            return;
        }
        unknown_id_source_a3d290->unknown_630 = true;
    }
    if (unknown_id_source_a3d290->unknown_62c == 0) {
        Audio_Pause_All();
        if (unknown_game_mode_a3d298 == 0 || unknown_game_mode_a3d298 == 5) {
            unknown_id_source_a3d290->ElapsedTime.Stop();
        }
        unknown_was_paused_ab6ef4 = unknown_id_source_a3d290->unknown_2306;
        if (!unknown_was_paused_ab6ef4) {
            Pause_Game_0065c3d0();
        }
    }
    unknown_id_source_a3d290->unknown_62c++;
}


// void Release_Pause_0065c310() (0x0065C310)
// Lowers the pause count; the last release resumes the game (unless it was
// paused before the count rose), restarts the elapsed time and resumes the
// sound.

void Release_Pause_0065c310()
{
    if (unknown_id_source_a3d290->unknown_62c) {
        unknown_id_source_a3d290->unknown_62c--;
        if (unknown_id_source_a3d290->unknown_62c == 0) {
            if (!unknown_was_paused_ab6ef4) {
                Resume_Game_0065c420();
            }
            unknown_id_source_a3d290->ElapsedTime.Start();
            Audio_Resume_All();
        }
    }
}


// void Pause_Game_0065c3d0() (0x0065C3D0)
// Pauses the game: aborts a drag select, hides the mouse unless it is
// already hidden, marks the scenario paused and clears the keyboard.

void Pause_Game_0065c3d0()
{
    Map.Abort_Drag_Select();
    if (WWMouse->Get_Mouse_State() == 0) {
        WWMouse->Hide_Mouse();
    }
    unknown_id_source_a3d290->unknown_2306 = true;
    unknown_flag_a40d44 = true;
    WWKeyboard->Clear();
    unknown_ab9174 = 0;
}


// void Resume_Game_0065c420() (0x0065C420)
// Resumes a paused game: shows the mouse again, clears the paused mark and
// the keyboard.

void Resume_Game_0065c420()
{
    if (unknown_id_source_a3d290->unknown_2306 == true) {
        while (WWMouse->Get_Mouse_State() != 0) {
            WWMouse->Show_Mouse();
        }
    }
    unknown_id_source_a3d290->unknown_2306 = false;
    unknown_flag_a40d44 = false;
    WWKeyboard->Clear();
    unknown_ab9174 = 0;
}


// void __fastcall Toggle_Display_Mode_0065c480(bool preferred) (0x0065C480)
// With [Video] AllowModeToggle, switches to the RA2.INI screen size (when
// preferred and not already in it) or back to 640 x 480 (640 x 400 first)
// from a larger mode.

void __fastcall Toggle_Display_Mode_0065c480(bool preferred)
{
    if (!unknown_allow_mode_toggle_a40d0b) {
        return;
    }
    if (preferred) {
        if (unknown_int_839648 != Options.ScreenWidth || unknown_int_83964c != Options.ScreenHeight) {
            Debug_Printf("Toggle display mode to %d X %d\n", Options.ScreenWidth, Options.ScreenHeight);
            Set_Video_Mode_005434e0(Options.ScreenWidth, Options.ScreenHeight);
        }
    } else if (unknown_int_839648 != 640 && unknown_int_83964c > 480) {
        Debug_Printf("Toggling display mode back to 640 X 480 / 400\n");
        if (!Set_Video_Mode_005434e0(640, 400)) {
            Set_Video_Mode_005434e0(640, 480);
        }
    }
}


// Not exact (RA2_NON_MATCHING): Wait_For_Players_0065c520 at 0x0065C520, best score 96.97, 2026-11-13; differs: same instructions; registers, stack slots or operands differ
#ifdef RA2_NON_MATCHING
// bool __fastcall Wait_For_Players_0065c520() (0x0065C520)
// The multiplayer load wait (not in a normal or mode 5 game): services the game
// until the loading screen's mean progress reaches 0.9995, then reports 100
// (200 with the scenario's +0x223B flag) and returns true. Every 60 ticks
// (300 at first) the own progress is resent; when no progress was made for 3600
// ticks the players below 0.999 are dropped and it fails, as it does when the
// serial line loses its carrier. Name hypothesis.
#include "shared/win_modem.hpp"

char *__fastcall Wide_To_Narrow_006fa120(unsigned short const *string);

bool __fastcall Wait_For_Players_0065c520()
{
    if (unknown_game_mode_a3d298 == 0 || unknown_game_mode_a3d298 == 5) {
        return true;
    }
    CDTimerClass<SystemTimerClass> timeout;
    CDTimerClass<SystemTimerClass> resend(300);
    timeout = 3600;
    double last = ProgressScreen.Average_Progress();
    for (;;) {
        WWKeyboard->Check();
        Service_Game_IO_00481fb0();
        if (unknown_game_mode_a3d298 == 1 && SerialPort != 0
            && !(SerialPort->Get_Modem_Status() & 0x80)) {
            Debug_Printf("Carrier Detect is false. Aborting load wait\n");
            return false;
        }
        if (timeout.Value() <= 0) {
            for (int i = 1, player = 1; i < unknown_players_a3fa7c.Count(); player++, i++) {
                if (ProgressScreen.Get_Progress(player) < 0.999) {
                    NodeNameType *entry = unknown_players_a3fa7c[i];
                    char *name = Wide_To_Narrow_006fa120(entry->Name);
                    Debug_Printf("Player %s failed to make it into the game. Dropping %s\n", name, Wide_To_Narrow_006fa120(entry->Name));
                    int house = unknown_players_a3fa7c[i]->ID;
                    Destroy_Connection_005bb5d0(house, 1);
                    HouseClass *hp = Houses[house];
                    if (unknown_session_a3d298.Am_I_Master(hp)) {
                        unknown_session_a3d298.MasterPlayerID = -1;
                        memset(unknown_session_a3d298.MasterPlayerName, 0, sizeof(unknown_session_a3d298.MasterPlayerName));
                        i--;
                    } else {
                        i--;
                    }
                }
            }
            return false;
        }
        double progress = ProgressScreen.Average_Progress();
        if (progress >= 0.9995) {
            unknown_session_a3d298.Update_Progress(unknown_id_source_a3d290->unknown_223b ? 200 : 100);
            return true;
        }
        if (progress != last) {
            timeout = 3600;
            last = progress;
        }
        if (resend.Value() <= 0) {
            resend = 60;
            unknown_session_a3d298.Update_Progress(unknown_id_source_a3d290->unknown_223b ? 200 : 100);
        }
    }
}
#endif

// bool __fastcall Unknown_0065c7d0(char const *name) (0x0065C7D0)
// Start_Scenario's reader (Westwood's Read_Scenario shape; name hypothesis):
// copies the name, resets the frame counter, marks the load in progress
// (+0x22FC) and a ".SED" name as a random map (+0x223B). Unless 0x00A40D13
// is set, opens the progress screen (one player in a campaign or game mode 5;
// the game ID text in an online game), its listener 0x00535910, the side and
// the bar shape, and in an online game adds every other player's IP as a UDP
// broadcast address. Reads a random map through MapSeed (and its multiplayer
// setup) or the scenario file; on failure shows "unable to read" and returns
// false. Then places the message list, fills in the data, reports 99 percent
// and (online, forwarding) waits for the router, reports 100 (200 for a random
// map) and waits for the other players; when they are in, runs the progress
// bars to full. Clears the progress screen and its listener.

bool __fastcall Unknown_0065c7d0(char const *name)
{
    char filename[260];
    strcpy(filename, name);
    Frame = 0;
    unknown_id_source_a3d290->unknown_22fc = true;
    unknown_a40774++;
    char *extension = &filename[strlen(filename) - 4];
    if (_stricmp(extension, ".SED") == 0) {
        unknown_id_source_a3d290->unknown_223b = true;
        Debug_Printf("Scen->IsRandom = true\n");
    } else {
        unknown_id_source_a3d290->unknown_223b = false;
        Debug_Printf("Scen->IsRandom = false\n");
    }

    Unknown535910 *loader = 0;
    if (!Debug_Map) {
        int players = 1;
        if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) {
            players = unknown_players_a3fa7c.Count();
        }
        ProgressScreen.Init(100.0, players, 0);
        wchar_t const *game_id = 0;
        wchar_t text[130];
        if (unknown_game_mode_a3d298 == 4 && unknown_b2a7ec != 0) {
            swprintf(text, StringTable::Load_String("TXT_GAME_ID", 0, "D:\\Ra2\\Scenario.CPP", 1303), unknown_b2a7fc);
            game_id = text;
        }
        loader = Unknown_00535910();
        loader->Unknown_005359E0();
        ProgressScreen.Set_Listener(loader);
        int side = 0;
        if (unknown_game_mode_a3d298 == 0) {
            if (unknown_id_source_a3d290->unknown_224c != -1) {
                side = Campaigns[unknown_id_source_a3d290->unknown_224c]->unknown_98;
            }
        } else {
            int country = unknown_players_a3fa7c[0]->unknown_4b;
            if (country == -3) {
                country = 0;
            }
            side = HouseTypes[country]->Side;
        }
        unknown_id_source_a3d290->unknown_2239 = side == 0;
        ProgressScreen.Set_Side(side);
        Unknown_00535910()->Unknown_00535B90();
        if (unknown_game_mode_a3d298 == 0) {
            ProgressScreen.Load_Shape("SPLDBR.SHP", 0, (ConvertClass *)Unknown_Get_Art_6f2490());
        } else {
            ProgressScreen.Load_Shape("PROGBARM.SHP", 0, 0);
        }
        bool campaign = unknown_game_mode_a3d298 == 0;
        ProgressScreen.Set_Layout(loader->Unknown_00535AB0(), game_id, !campaign, !campaign);
        ProgressScreen.Set_Name_Width(loader->Unknown_00535B60());

        if (PacketTransport != 0 && unknown_game_mode_a3d298 == 4 && unknown_players_a3fa7c.Count() > 1) {
            Debug_Printf("Setting addresses for UDP broadcast\n");
            PacketTransport->Clear_Broadcast_Addresses();
            char address[32] = "xxx.xxx.xxx.xxx";
            for (int index = 1; index < unknown_players_a3fa7c.Count(); index++) {
                NetNumType net;
                NetNodeType node;
                unknown_players_a3fa7c[index]->Address.Get_Address(net, node);
                sprintf(address, "%d.%d.%d.%d", node[0], node[1], node[2], node[3]);
                Debug_Printf("Adding broadcast address %s\n", address);
                PacketTransport->Set_Broadcast_Address(address, unknown_node_ids_a402ba[index]);
            }
        }
    }

    bool ok;
    if (unknown_id_source_a3d290->unknown_223b) {
        ok = RandomMapGen.SeedData.Load_Or_Prompt_005791a0(filename);
        if (ok) {
            RandomMapGen.Unknown_0057A0D0(0, 0);
            Unknown_0065e8c0(true);
        }
        strcpy(unknown_id_source_a3d290->FileName, filename);
    } else {
        ok = Read_Scenario_0065e760(filename, true);
    }
    if (!Rules->Shroud) {
        Map.Unknown_0055A1F0();
    }
    if (!ok) {
        Debug_Printf("Error - Unable to read scenario: %s\n", filename);
        Unknown_004955D0(&unknown_palette_837e10, 7, unknown_481fb0);
        WWMessageBox(L" ").Process(
            StringTable::Load_String("TXT_UNABLE_READ_SCENARIO", 0, "D:\\Ra2\\Scenario.CPP", 1394),
            StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\Scenario.CPP", 1394), 0, 0, false);
        unknown_a40774--;
        unknown_id_source_a3d290->unknown_22fc = false;
        if (loader != 0) {
            ProgressScreen.Set_Listener(0);
            delete loader;
        }
        return false;
    }

    unknown_object_a3dc70.Unknown_005B4C60(unknown_int_839630 + 3, unknown_int_839634, 6, 0x62, 0xE, -1, -1, 0, 0x14, 0x62, unknown_int_839638 - 6);
    Fill_In_Data_0065cdd0();
    if (unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) {
        unknown_session_a3d298.Update_Progress(99);
        if (unknown_game_mode_a3d298 == 4 && unknown_flag_a3fba8) {
            unknown_OwnerTalkClass_00ab6d18.Wait_For_Router_Shutdown(15000, true);
        }
    }
    unknown_session_a3d298.Update_Progress(unknown_id_source_a3d290->unknown_223b ? 200 : 100);
    unknown_session_a3d298.unknown_639 = false;
    unknown_session_a3d298.unknown_63a = false;
    ok = Wait_For_Players_0065c520();
    if (unknown_game_mode_a3d298 == 4) {
        unknown_session_a3d298.Unknown_00672440();
    }
    unknown_a40774--;
    if (!Debug_Map && ok) {
        Service_Game_IO_00481fb0();
        while (ProgressScreen.Average_Progress() < 1.0) {
            for (int index = 0; index < unknown_players_a3fa7c.Count(); index++) {
                ProgressScreen.Set_Progress_Percent(index, 100.0, Point2D(-1, -1));
            }
        }
    }
    unknown_id_source_a3d290->unknown_22fc = false;
    ProgressScreen.End();
    if (loader != 0) {
        ProgressScreen.Set_Listener(0);
        loader->Unknown_00536E20();
    }
    return true;
}


// void __fastcall Fill_In_Data_0065cdd0() (0x0065CDD0)
// Completes a freshly read scenario: refreshes every building, centres the
// view on the home waypoint, makes the tags of the tag types that attach to
// the map, the world or a house, counts the house-owned tags, fills the
// silos, resets the map's cell overlays and zones, makes the gas cloud
// system and scatters ore twinkles (name from Westwood's Fill_In_Data;
// called after a scenario is read and by the map seed loader 0x0057B1FD).

void __fastcall Fill_In_Data_0065cdd0()
{
    unknown_a40774++;
    for (int index = 0; index < unknown_BuildingClass_list_00a40af8.Count(); index++) {
        unknown_BuildingClass_list_00a40af8[index]->unknown_slot283();
    }

    Map.Flag_To_Redraw(2);

    if (!Debug_Map) {
        Map.unknown_slot54(1);
        if (unknown_game_mode_a3d298 == 0) {
            Cell cell = unknown_id_source_a3d290->Waypoints[unknown_scenario_checkpoint_00855a90.Variables[0] ? unknown_id_source_a3d290->AltHomeCell : unknown_id_source_a3d290->HomeCell];
            unknown_id_source_a3d290->Views[0] = unknown_id_source_a3d290->Views[1] =
                unknown_id_source_a3d290->Views[2] = unknown_id_source_a3d290->Views[3] = cell;
            CoordStruct coord = Cell_Center(cell);
            coord.Z = Map.Get_Height_GL(coord);
            Map.Set_Tactical_Position(coord);
        }
    }

    for (int tag_index = 0; tag_index < TagTypes.Count(); tag_index++) {
        TagTypeClass *tagtype = TagTypes[tag_index];
        if (tagtype->Attaches_To() & 4) {
            unknown_tags_866238.Add(TagClass::Find_Or_Make(tagtype));
        }
        if (tagtype->Attaches_To() & 0x10) {
            unknown_tags_866160.Add(TagClass::Find_Or_Make(tagtype));
        }
        if (tagtype->Attaches_To() & 8) {
            TagClass *tag = TagClass::Find_Or_Make(tagtype);
            HouseClass *house = HouseClass::As_Pointer(((HouseTypeClass *)tag->Class->FirstTrigger->House)->House);
            if (house != 0) {
                house->unknown_38.Add(tag);
            }
        }
    }
    unknown_a40774--;

    for (int count_index = 0; count_index < TagTypes.Count(); count_index++) {
        TagTypeClass *tagtype = TagTypes[count_index];
        if (tagtype->Is_Allow_Win()) {
            HouseClass *owner = HouseClass::As_Pointer(((HouseTypeClass *)tagtype->FirstTrigger->House)->ArrayIndex);
            owner->unknown_1c4++;
        }
    }

    if (unknown_id_source_a3d290->FillSilos) {
        for (int house_index = 0; house_index < Houses.Count(); house_index++) {
            HouseClass *house = Houses[house_index];
            if (house != 0) {
                int money = house->Available_Money();
                TiberiumClass *tiberium = unknown_TiberiumClass_list_00ac0b30[0];
                while (money > tiberium->Value && house->Available_Storage() > 0) {
                    house->Harvested(1.0f, 0);
                    money -= tiberium->Value;
                }
            }
        }
    }

    Map.Unknown_0049DE00(true, false);
    Initialize_Wave_Tables_00721bf0();
    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
        cell->Recalc_Attributes(-1);
    }
    Map.Compute_Zone_Connections();
    Map.Zone_Reset();
    Map.Reset_All_Subzones();
    Map.Set_WasUnderBridge_Flags();

    if (unknown_gas_cloud_system_a40d20 == 0) {
        unknown_gas_cloud_system_a40d20 = new ParticleSystemClass(
            ParticleSystemTypes[ParticleSystemTypeClass::Index_Or_Make("GasCloudSys")],
            CoordStruct(0xA80, 0xA80, 0), 0, 0, unknown_geometry_l);
    }

    if (Rules->OreTwinkle != 0) {
        Map.Reset_Iterator();
        for (CellClass *ore = Map.Iterate(); ore != 0; ore = Map.Iterate()) {
            if (ore->Get_Tiberium_Value() && Random_Pick(0, Rules->OreTwinkleChance - 1) == 0) {
                new AnimClass(Rules->OreTwinkle, ore->Center_Coord(), 0, 1, 0x600, 0);
            }
        }
    }

    unknown_set_frame_rate_0053d820(Rules->DetailMinFrameRateNormal);
    unknown_set_buffer_zone_0053d830(Rules->DetailBufferZoneWidth);
}


// void __fastcall Post_Load_Game_0065d2c0() (0x0065D2C0)
// Called once, right after a saved game is read (0x00656AD5): refreshes the
// building types, loads the veinhole shapes for the scenario's theater,
// rebuilds the radar, stops unknown_timer_ac0178 (keeping what remained) and
// restarts it from the current frame,
// sizes the path estimator for the map, runs 0x00520770 and the tiberium
// animation palettes, redraws the screen and resets the sidebar placement
// state (name hypothesis).

void __fastcall Post_Load_Game_0065d2c0()
{
    BuildingTypeClass::unknown_func_45e170();
    Unknown_006EA9B0();
    VeinholeMonsterClass::Init(unknown_id_source_a3d290->Theater);
    Map.Post_Load_Radar_Fixup();
    CDTimerClass<FrameTimerClass> &timer = unknown_timer_ac0178;
    timer.Stop();
    timer.Started = timer.Timer;
    Search.Update_Map_Dimensions(Map.MapSize);
    Unknown_00520770();
    Unknown_00423C70();
    Map.Flag_To_Redraw(2);
    InvalidateRect(MainWindow, 0, false);
    Unknown_006F89E0();
    Unknown_006F8A20(0);
    Unknown_0063C790();
}


// void Clear_Scenario_0065d370() (0x0065D370)


// Existing canonical callees; no new ABI aliases.
void __fastcall Read_Battle_Files();
void __fastcall Delete_All_Objects_00519960();
void RadarEventClass_unknown_reset_cells();
// Existing accepted operation 00663220, expanded here as allowed for a caller in another population.
namespace seed_00663220 {
extern "C" unsigned int _waypoint_none_00ab6e88;
inline void __fastcall Clear_All_Waypoints_00663220(int scenario)
{
    unsigned int *waypoint = (unsigned int *)(scenario + 0x632);
    int remaining = 101;
    do {
        *waypoint = _waypoint_none_00ab6e88;
        ++waypoint;
        --remaining;
    } while (remaining != 0);
}
}
// LayerClass virtual slot8 is the original 5-byte dispatch to virtual Clear atslot3.
// The name remains the existing shared/layer.hpp contract; no new wrapper or ABI alias.
inline void LayerClass::unknown_slot8() { Clear(); }

namespace seed_006e9670 { void Deinit_Tiberium_Spread_System_006e9670(); }
namespace seed_006ea130 { void Deinit_Tiberium_Growth_System_006ea130(); }
void Unknown_00714410();
void Unknown_00613E50();
void Unknown_0051FBB0();
extern HouseClass *unknown_player_a35db4;
extern BombListClass unknown_bomb_list_8322e0;
extern DynamicVectorClass<ObjectClass *> CurrentObjects;
extern DynamicVectorClass<int> unknown_map_list_83252c;
extern ParticleSystemClass *unknown_gas_cloud_system_a40d20;
extern DynamicVectorClass<TagClass *> unknown_tags_866160;
extern DynamicVectorClass<TagClass *> unknown_tags_866238;
extern Tactical *unknown_tactical_83996c;

void Clear_Scenario_0065d370()
{
    unknown_id_source_a3d290->UniqueID = 1000000;
    unknown_id_source_a3d290->RandomNumber.IsLocked = true;
    unknown_saved_volume_7f1dec = 0x4000;
    Audio_Global_Levels[7]->Volume.Set(0x4000);
    Unknown_00714410();
    unknown_player_a35db4 = 0;
    unknown_id_source_a3d290->Set_Defaults();
    Unknown_0051FBB0();
    for (int index = AbstractTypes.Count() - 1; index >= 0; index--) {
        if (AbstractTypes[index]->What_Am_I() == 0x12) {
            AbstractTypes.Delete(index);
        }
    }
    Delete_All_Objects_00519960();
    for (int tile = 0; tile < IsoTileTypes.Count(); tile++) {
        AbstractTypes.Add((AbstractTypeClass *)IsoTileTypes[tile]);
    }
    Map.MapClass::unknown_slot23();
    unknown_map_list_83252c.Clear();
    TagClass::Clear_All();
    // Expand the existing global-variable setter for the scenario reset loop.
    for (int variable = 0; variable < 50; ++variable) {
        ScenarioClass *scenario = unknown_id_source_a3d290;
        if (variable * sizeof(scenario->Variables[0]) < sizeof(scenario->Variables)) {
                UnknownScenarioVariable_004B5AB0 &state = scenario->Variables[variable];
                bool old_value = state.Value;
                if (old_value != false) {
                    state.Value = false;
                    scenario->unknown_222e = true;
                    TagClass::All_Timer_Global_Reset(variable);
                }
        }
    }
    delete unknown_tactical_83996c;
    unknown_tactical_83996c = new Tactical;
    unknown_tactical_83996c->Reset_Dirty_Rectangles();
    unknown_flag_7df630 = false;
    while (unknown_objects_a40368.Count()) {
        if (unknown_objects_a40368[0]->Kind_Of() == 8) {
            unknown_objects_a40368[0]->Release();
        } else {
            delete unknown_objects_a40368[0];
        }
    }
    unknown_flag_7df630 = true;
    while (TagTypes.Count()) {
        delete TagTypes[0];
    }
    unknown_tags_866238.Clear();
    unknown_tags_866160.Clear();
    unknown_id_source_a3d290->ParTimeEasy = 3600;
    unknown_id_source_a3d290->ParTimeMedium = unknown_id_source_a3d290->ParTimeEasy;
    unknown_id_source_a3d290->ParTimeHard = unknown_id_source_a3d290->ParTimeEasy;
    unknown_id_source_a3d290->UnderParTitle[0] = '\0';
    unknown_id_source_a3d290->UnderParMessage[0] = '\0';
    unknown_id_source_a3d290->OverParTitle[0] = '\0';
    unknown_id_source_a3d290->OverParMessage[0] = '\0';
    LightSourceClass::Clear_All();
    Init_Storm_0051F2F0();
    EMPulseClass::Delete_All_004B4DC0();
    VeinholeMonsterClass::Reset();
    seed_006e9670::Deinit_Tiberium_Spread_System_006e9670();
    seed_006ea130::Deinit_Tiberium_Growth_System_006ea130();
    unknown_bomb_list_8322e0.Clear();
    KamikazeTracker.Clear();
    unknown_tracker_839ea0.Clear_00413540();
    Unknown_00613E50();
    Map.MapSize = Rect(0, 0, 0, 0);
    Debug_Printf("Map.Init_Clear()\n");
    Map.MouseClass::Init_Clear();
    Debug_Printf("Logic.Init()\n");
    Logic.LayerClass::unknown_slot8();
    Debug_Printf("CurrentObjects.Clear()\n");
    CurrentObjects.Clear();
    Debug_Printf("Scen->Clear_All_Waypoints()\n");
    // The existing accepted seed takes the scenario pointer in ECX as a 32-bit integer.
    seed_00663220::Clear_All_Waypoints_00663220((int)unknown_id_source_a3d290);
    Debug_Printf("Init_Campaigns()\n");
    Read_Battle_Files();
    if (unknown_gas_cloud_system_a40d20 != 0) {
        unknown_gas_cloud_system_a40d20->Remove_All_Particles();
        delete unknown_gas_cloud_system_a40d20;
        unknown_gas_cloud_system_a40d20 = 0;
    }
    unknown_id_source_a3d290->RandomNumber.IsLocked = false;
    Unknown_006F89E0();
    Unknown_006F8A20(0);
    RadarEventClass_unknown_reset_cells();
    unknown_id_source_a3d290->UniqueID = 1000000;
}


// void __fastcall Do_Win_0065d7f0() (0x0065D7F0)
// The player won (Westwood's Do_Win, name hypothesis): stops the game as Do_Lose
// does; in a networked game shows the score and asks the session whether to go
// on; plays the Win movie and (unless option bit 1) the score, the PostScore and
// PreMapSelect movies, then ends the campaign or picks the next scenario (map
// selection or NextScenario/AltNextScenario by variable 1); checkpoints the
// scenario state, raises +0x834 and starts the next scenario.


void __fastcall Do_Win_0065d7f0()
{
    if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5 && !(unknown_flags_a3f600 & 2)) {
        Unknown_00624850();
        Unknown_0053F780();
    }
    unknown_flag_a40d04 = false;
    unknown_flag_a40380 = false;
    unknown_id_source_a3d290->ElapsedTime.Stop();
    Unknown_005A09E0();
    WWMouse->Erase_Mouse(unknown_surface_839954, false);
    WWMouse->Release_Mouse();
    Map.Set_Default_Mouse(0, false);
    WWMouse->Hide_Mouse();
    Audio_Level_Fade_To_Zero(&Audio_Global_Levels[7]->Volume);
    Audio_Wait_Drain(&Audio_Global_Levels[7]->Volume);
    long end = TickCount.Time() + 300;
    while (Speech_Is_Playing() && end > TickCount.Time()) {
        Service_Game_IO_00481fb0();
    }
    Speech_Stop(true);
    Unknown_007160D0();
    Audio_Stop();
    Unknown_00714410();
    Audio_Global_Levels[7]->Volume.Set(0x4000);

    Toggle_Display_Mode_0065c480(false);

    if (unknown_game_mode_a3d298 != 0) {
        bool next = false;
        unknown_host_name_a3f618[0] = 0;
        if (!(unknown_flags_a3f600 & 2)) {
            unknown_a3f58c++;
            if (unknown_session_a3d29c != 0) {
                if ((unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) && unknown_session_a3d29c->unknown_slot2()) {
                    unknown_5159b0(0, true);
                } else {
                    UnknownScore_005AAA20().Presentation_005AAA40();
                }
            } else {
                UnknownScore_005AAA20().Presentation_005AAA40();
            }
            if (unknown_session_a3d29c != 0) {
                next = unknown_session_a3d29c->unknown_slot36();
            }
            unknown_a3f598++;
            if (unknown_a3f598 >= 4) {
                unknown_a3f598 = 3;
            }
        }
        if (!next) {
            if (unknown_session_a3d29c != 0) {
                unknown_session_a3d29c->unknown_slot35(1);
            }
            unknown_flag_a40958 = false;
            return;
        }
        unknown_session_a3d29c->unknown_slot37();
    }
    Unknown_005A07F0(unknown_id_source_a3d290->WinMovie, -1, 1, 1, 1);
    if (!(unknown_flags_a3f600 & 2)) {
        WWKeyboard->Clear();
        if (!unknown_id_source_a3d290->SkipScore) {
            UnknownScore_0069A640().Presentation_0069A650();
        }
        if (unknown_id_source_a3d290->PostScoreMovie != -1) {
            Unknown_005A07F0(unknown_id_source_a3d290->PostScoreMovie, -1, 1, 1, 1);
        }
        if (unknown_id_source_a3d290->PreMapSelectMovie != -1) {
            Unknown_005A07F0(unknown_id_source_a3d290->PreMapSelectMovie, -1, 1, 1, 1);
        }
        if (!Unknown_00491A10() || unknown_id_source_a3d290->OneTimeOnly) {
            unknown_flag_a40958 = false;
            WWMouse->Show_Mouse();
            return;
        }
        if (unknown_id_source_a3d290->EndOfGame) {
            if (unknown_id_source_a3d290->unknown_224c != -1) {
                Unknown_005A07F0(Campaigns[unknown_id_source_a3d290->unknown_224c]->unknown_29c, -1, 1, 1, 1);
            }
            Unknown_004B37C0();
            unknown_flag_a40958 = false;
            WWMouse->Show_Mouse();
            return;
        }
        if (!((unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) && unknown_session_a3d29c != 0
              && unknown_session_a3d29c->unknown_slot2())) {
            if (unknown_id_source_a3d290->SkipMapSelect) {
                if (unknown_id_source_a3d290->Variables[1].Value) {
                    Unknown_0058F620(unknown_id_source_a3d290, unknown_id_source_a3d290->AltNextScenario);
                } else {
                    Unknown_0058F620(unknown_id_source_a3d290, unknown_id_source_a3d290->NextScenario);
                }
            } else {
                WWMouse->Show_Mouse();
                Unknown_0058F510(unknown_id_source_a3d290);
                WWMouse->Hide_Mouse();
            }
        }
        WWKeyboard->Clear();
    }
    WWMouse->Show_Mouse();
    unknown_scenario_checkpoint_00855a90.Capture_004B5AB0();
    unknown_id_source_a3d290->TechLevel++;
    if ((unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) && unknown_session_a3d29c != 0
        && unknown_session_a3d29c->unknown_slot2()) {
        if (unknown_game_mode_a3d298 == 3) {
            PacketTransport->Close_Socket();
            delete PacketTransport;
            PacketTransport = 0;
            PacketTransport = new IPXInterfaceClass;
        }
        PacketTransport->Init();
        int ai_count = unknown_session_a3d298.AICount;
        int colors[8];
        int countries[8];
        int difficulties[8];
        memcpy(colors, unknown_session_a3d298.AIColor, sizeof(colors));
        memcpy(countries, unknown_session_a3d298.AICountry, sizeof(countries));
        memcpy(difficulties, unknown_session_a3d298.AIDifficulty, sizeof(difficulties));
        if (!Unknown_005BC460()) {
            if (PacketTransport != 0) {
                delete PacketTransport;
                PacketTransport = 0;
            }
            unknown_flag_a40958 = false;
            return;
        }
        unknown_session_a3d298.AICount = ai_count;
        for (int index = 0; index < 8; index++) {
            unknown_session_a3d298.AIDifficulty[index] = difficulties[index];
            unknown_session_a3d298.AICountry[index] = countries[index];
            unknown_session_a3d298.AIColor[index] = colors[index];
        }
        WWKeyboard->Clear();
    }
    unknown_flag_a40958 = Unknown_00491A10();
    if (!unknown_flag_a40958) {
        WWMouse->Show_Mouse();
        return;
    }
    Start_Scenario_0065bc70(unknown_id_source_a3d290->FileName, true, unknown_id_source_a3d290->unknown_224c);
    if ((unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) && unknown_session_a3d29c != 0
        && unknown_session_a3d29c->unknown_slot2()) {
        unknown_session_a3d298.Create_Connections();
        if (unknown_game_mode_a3d298 == 3) {
            unknown_ipx_manager_a40978.Set_Timing(Max(unknown_ipx_manager_a40978.unknown_526c30() + 2, 15), -1, 600, true);
            unknown_ipx_manager_a40978.unknown_slot14(Max(unknown_ipx_manager_a40978.unknown_526c30() + 2, 60), -1, 600);
        } else if (unknown_game_mode_a3d298 == 4 && unknown_a3d580 == 0) {
            unknown_ipx_manager_a40978.Set_Timing(Max(unknown_ipx_manager_a40978.unknown_526c30() + 2, 60), -1, 600, true);
        }
    }
    unknown_scenario_checkpoint_00855a90.Restore_004B5B40();
    if (ToolTips != 0) {
        ToolTips->Enable(unknown_tooltips_enabled_a40b34);
    }
    Map.Render();
    Unknown_004955D0(&unknown_palette_837e10, 7, unknown_481fb0);
}


// void __fastcall Do_Lose_0065dec0() (0x0065DEC0)
// The player lost (Westwood's Do_Lose, name hypothesis): stops the game time,
// closes the sidebar and the mouse, switches back to 640 x 480, fades out the
// sound and waits for the speech; in a networked game shows the score and asks
// the session whether to replay; in a campaign plays the Lose movie and asks
// "replay?". A replay restarts the scenario (re-creating the network transport
// for mode 3 with the computer players kept) and restores the checkpoint.


void __fastcall Do_Lose_0065dec0()
{
    if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5 && !(unknown_flags_a3f600 & 2)) {
        Unknown_00624850();
        Unknown_0053F780();
    }
    unknown_flag_a40d04 = false;
    unknown_flag_a40380 = false;
    unknown_id_source_a3d290->ElapsedTime.Stop();
    Unknown_005A09E0();
    WWMouse->Erase_Mouse(unknown_surface_839954, false);
    WWMouse->Release_Mouse();
    Map.Set_Default_Mouse(0, false);
    WWMouse->Hide_Mouse();
    Toggle_Display_Mode_0065c480(false);
    if (unknown_game_mode_a3d298 != 0) {
        unknown_host_name_a3f618[0] = 0;
    }
    Audio_Level_Fade_To_Zero(&Audio_Global_Levels[7]->Volume);
    Audio_Wait_Drain(&Audio_Global_Levels[7]->Volume);
    long end = TickCount.Time() + 300;
    while (Speech_Is_Playing() && end > TickCount.Time()) {
        Service_Game_IO_00481fb0();
    }
    Speech_Stop(true);
    Unknown_007160D0();
    Audio_Stop();
    Unknown_00714410();
    Audio_Global_Levels[7]->Volume.Set(0x4000);

    bool replay = false;
    if (unknown_game_mode_a3d298 != 0) {
        if (!(unknown_flags_a3f600 & 2)) {
            unknown_a3f58c++;
            if (unknown_session_a3d29c != 0) {
                if ((unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) && unknown_session_a3d29c->unknown_slot2()) {
                    unknown_5159b0(0, true);
                } else {
                    UnknownScore_005AAA20().Presentation_005AAA40();
                }
            } else {
                UnknownScore_005AAA20().Presentation_005AAA40();
            }
            if (unknown_session_a3d29c != 0) {
                replay = unknown_session_a3d29c->unknown_slot36();
            }
            unknown_a3f598++;
            if (unknown_a3f598 >= 4) {
                unknown_a3f598 = 3;
            }
        }
        if (!replay) {
            if (unknown_session_a3d29c != 0) {
                unknown_session_a3d29c->unknown_slot35(0);
            }
            unknown_flag_a40958 = false;
            return;
        }
        unknown_session_a3d29c->unknown_slot37();
    }
    Unknown_005A07F0(unknown_id_source_a3d290->LoseMovie, -1, 1, 1, 1);
    unknown_5159b0(0, true);
    if ((unknown_game_mode_a3d298 == 0 && !(unknown_flags_a3f600 & 2)
         && WWMessageBox(L" ").Process(
                StringTable::Load_String("TXT_TO_REPLAY", 0, "D:\\Ra2\\Scenario.CPP", 2612),
                StringTable::Load_String("TXT_YES", 0, "D:\\Ra2\\Scenario.CPP", 2612),
                StringTable::Load_String("TXT_NO", 0, "D:\\Ra2\\Scenario.CPP", 2612), 0, false) == 0)
        || replay) {
        if ((unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) && unknown_session_a3d29c != 0
            && unknown_session_a3d29c->unknown_slot2()) {
            if (unknown_game_mode_a3d298 == 3) {
                PacketTransport->Close_Socket();
                delete PacketTransport;
                PacketTransport = 0;
                PacketTransport = new IPXInterfaceClass;
            }
            PacketTransport->Init();
            int ai_count = unknown_session_a3d298.AICount;
            int colors[8];
            int countries[8];
            int difficulties[8];
            memcpy(colors, unknown_session_a3d298.AIColor, sizeof(colors));
            memcpy(countries, unknown_session_a3d298.AICountry, sizeof(countries));
            memcpy(difficulties, unknown_session_a3d298.AIDifficulty, sizeof(difficulties));
            if (!Unknown_005BC460()) {
                if (PacketTransport != 0) {
                    delete PacketTransport;
                    PacketTransport = 0;
                }
                unknown_flag_a40958 = false;
                return;
            }
            unknown_session_a3d298.AICount = ai_count;
            for (int index = 0; index < 8; index++) {
                unknown_session_a3d298.AIDifficulty[index] = difficulties[index];
                unknown_session_a3d298.AICountry[index] = countries[index];
                unknown_session_a3d298.AIColor[index] = colors[index];
            }
            WWKeyboard->Clear();
        }
        Start_Scenario_0065bc70(unknown_id_source_a3d290->FileName, false, unknown_id_source_a3d290->unknown_224c);
        if ((unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) && unknown_session_a3d29c != 0
            && unknown_session_a3d29c->unknown_slot2()) {
            unknown_session_a3d298.Create_Connections();
            if (unknown_game_mode_a3d298 == 3) {
                unknown_ipx_manager_a40978.Set_Timing(Max(unknown_ipx_manager_a40978.unknown_526c30() + 2, 15), -1, 600, true);
                unknown_ipx_manager_a40978.unknown_slot14(Max(unknown_ipx_manager_a40978.unknown_526c30() + 2, 60), -1, 600);
            } else if (unknown_game_mode_a3d298 == 4 && unknown_a3d580 == 0) {
                unknown_ipx_manager_a40978.Set_Timing(Max(unknown_ipx_manager_a40978.unknown_526c30() + 2, 60), -1, 600, true);
            }
        }
        unknown_scenario_checkpoint_00855a90.Restore_004B5B40();
        if (ToolTips != 0) {
            ToolTips->Enable(unknown_tooltips_enabled_a40b34);
        }
        Map.Render();
        return;
    }
    unknown_flag_a40958 = false;
}


// void __fastcall Do_Restart_0065e440() (0x0065E440)
// Restarts the current scenario (name hypothesis from Westwood's Do_Restart;
// reached only through a function pointer): disables the tool tips, stops the
// elapsed game time, closes the sidebar, stops the music, sounds and speech,
// restarts the scenario of the same name and campaign without a briefing,
// restores the checkpointed scenario state, waits out the rest of one second
// and redraws the screen.

void __fastcall Do_Restart_0065e440()
{
    unknown_flag_a40d04 = false;
    unknown_flag_a40380 = false;
    if (ToolTips != 0) {
        ToolTips->Enable(false);
    }
    unknown_id_source_a3d290->ElapsedTime.Stop();
    Unknown_005A09E0();
    CDTimerClass<SystemTimerClass> timer;
    timer = 60;
    Theme.Stop(true);
    Audio_Stop();
    Unknown_00714410();
    Speech_Stop(true);
    Unknown_007160D0();
    Map.Set_Default_Mouse(0, false);
    WWKeyboard->Clear();
    Start_Scenario_0065bc70(unknown_id_source_a3d290->FileName, false, unknown_id_source_a3d290->unknown_224c);
    unknown_scenario_checkpoint_00855a90.Restore_004B5B40();
    while (timer.Value() > 0) {
        Service_Game_IO_00481fb0();
    }
    WWKeyboard->Clear();
    if (ToolTips != 0) {
        ToolTips->Enable(unknown_tooltips_enabled_a40b34);
    }
    Map.Render();
}


// void __fastcall Unknown_0065e590() (0x0065E590)
// Ends the scenario on "battle control terminated" (reached only through a
// function pointer; Westwood's Do_Abort-like ending, name not recovered):
// stops the elapsed game time, sets +0x834 to 1, closes the sidebar, clears
// the keyboard, erases the mouse, stops the music, fades the global volume
// out, says EVA_BattleControlTerminated and waits for it (at most 300 ticks),
// stops the speech and sounds, restores the volume and switches back to
// 640 x 480 when the display mode was toggled.

void __fastcall Unknown_0065e590()
{
    unknown_flag_a40d04 = false;
    unknown_flag_a40380 = false;
    unknown_id_source_a3d290->ElapsedTime.Stop();
    unknown_id_source_a3d290->TechLevel = 1;
    Unknown_005A09E0();
    WWKeyboard->Clear();
    WWMouse->Erase_Mouse(unknown_surface_839954, false);
    WWMouse->Release_Mouse();
    Map.Set_Default_Mouse(0, false);
    Theme.Stop(true);
    Audio_Level_Fade_To_Zero(&Audio_Global_Levels[7]->Volume);
    Speech_Play_Name("EVA_BattleControlTerminated", 2, -1);
    Audio_Wait_Drain(&Audio_Global_Levels[7]->Volume);
    long end = TickCount.Time() + 300;
    while (Speech_Is_Playing() && end > TickCount.Time()) {
        Service_Game_IO_00481fb0();
    }
    Speech_Stop(true);
    Unknown_007160D0();
    Audio_Stop();
    Unknown_00714410();
    Audio_Global_Levels[7]->Volume.Set(0x4000);
    Toggle_Display_Mode_0065c480(false);
    unknown_flag_a40958 = false;
}


// bool __fastcall Read_Scenario_0065e760(char const *name, bool fresh) (0x0065E760)
// Reads the named scenario file (Westwood's Read_Scenario): makes the
// required disk available, resets the frame counter, loads the file into an
// INI, records the name as the scenario's file name and reads it with
// Read_Scenario_INI (unofficial); false when the disk or the file fails.

bool __fastcall Read_Scenario_0065e760(char const *name, bool fresh)
{
    if (!CD().Force_Available(CD::RequiredCD)) {
        return false;
    }
    Frame = 0;
    CCINIClass ini;
    CCFileClass file(name);
    Debug_Printf("Read_Scenario_INI - Filename is %s\n", name);
    if (!ini.Load(file, true, false)) {
        Debug_Printf("Scenario ini load failed!\n");
        return false;
    }
    strcpy(unknown_id_source_a3d290->FileName, name);
    bool result = Read_Scenario_INI(ini, false);
    return result;
}


// void __fastcall Unknown_0065e8c0(bool official) (0x0065E8C0)
// Multiplayer setup once a scenario is read (Read_Scenario_INI): with fewer
// players than Rules MaxPlayers removes the unused computer-less houses, makes
// the units (the game mode's own creation when there is one, else
// Create_Units), places the starting crates (between Rules CrateMinimum and
// CrateMaximum), then gives every active non-passive house its anger delay
// and starting money bonus, marks the passive houses' types, allies every
// house with the Special house and lets the game mode finish the setup.

void __fastcall Unknown_0065e8c0(bool official)
{
    if (unknown_players_a3fa7c.Count() + unknown_session_a3d298.AICount < Rules->MaxPlayers && !Debug_Map) {
        for (int index = 0; index < Houses.Count(); index++) {
            if (!Houses[index]->unknown_134 && !Houses[index]->Type->MultiplayPassive && unknown_session_a3d298.AICount < 1) {
                Houses[index]->unknown_func_4e7150();
            }
        }
    }
    Service_Game_IO_00481fb0();
    if (!Debug_Map) {
        int saved = unknown_a40774;
        unknown_a40774 = 0;
        if (unknown_session_a3d29c != 0) {
            unknown_session_a3d29c->unknown_slot27(official);
            unknown_session_a3d29c->Unknown_005B7CE0();
        } else {
            Create_Units_00660320(official);
        }
        unknown_a40774 = saved;
    }
    unknown_session_a3d298.Update_Progress(93);
    Service_Game_IO_00481fb0();
    if (unknown_flag_a3d2c1) {
        int crates = Rules->CrateMinimum > unknown_session_a3d55c ? Rules->CrateMinimum : unknown_session_a3d55c;
        crates = crates < Rules->CrateMaximum ? crates : Rules->CrateMaximum;
        for (int crate = 0; crate < crates; crate++) {
            Map.Unknown_0054E410();
        }
    }
    Service_Game_IO_00481fb0();
    Map.Unknown_0049C670();
    HouseClass *special = HouseClass::As_Pointer(HouseTypeClass::Find_Index("Special"));
    for (int house = 0; house < Houses.Count(); house++) {
        Houses[house]->unknown_55f0.House = Houses[house];
        if (!Houses[house]->unknown_134 && !Houses[house]->Type->MultiplayPassive) {
            Houses[house]->unknown_5530 = CDTimerClass<FrameTimerClass>(Rules->AIHateDelays[Houses[house]->unknown_c8]);
            int money = Houses[house]->Available_Money();
            Houses[house]->Refund_Money(money * (Rules->MultiplayerAICM[Houses[house]->unknown_c8] * 0.01));
        }
        if (Houses[house]->Type->MultiplayPassive) {
            Houses[house]->Type->WallOwner = false;
        } else {
            Houses[house]->Type->WallOwner = true;
        }
        if (Houses[house] != special) {
            Houses[house]->Make_Ally(special, false);
            special->Make_Ally(Houses[house], false);
        }
    }
    if (unknown_session_a3d29c != 0) {
        unknown_session_a3d29c->unknown_slot28();
        unknown_session_a3d29c->unknown_slot29();
    }
    memset(unknown_flags_a3f118, 1, sizeof(unknown_flags_a3f118));
}


// bool __fastcall Read_Scenario_INI(CCINIClass &ini, bool official) (0x0065EB40)
// Reads a scenario: the difficulty and fog of war from the session, the
// campaign's MISSION.INI texts (name, briefing, loading screen) and the
// scenario's own <name>.INI rules addition in a normal game, the theater,
// the rules sections, the global variables, the houses and [Basic]
// (Read_Basic), then every object list (teams, scripts, task forces,
// triggers, tags, AI triggers, the map, terrain, overlays, units, aircraft,
// infantry, buildings, smudges). Reports the loading progress along the
// way; false when the player side or [Basic] cannot be set up.

// Westwood-style macro: each operand is evaluated where it appears.


bool __fastcall Read_Scenario_INI(CCINIClass &ini, bool official)
{
    char buffer[32];
    unknown_a40774++;
    Debug_Printf("Clearing old scenario\n");
    Clear_Scenario_0065d370();
    if (unknown_game_mode_a3d298 == 0) {
        unknown_id_source_a3d290->Difficulty = unknown_difficulty_a40b1c;
        unknown_id_source_a3d290->unknown_610 = 2 - unknown_difficulty_a40b1c;
        unknown_id_source_a3d290->Special().FogOfWar = false;
        unknown_special_a40920.FogOfWar = false;
    } else {
        unknown_id_source_a3d290->Difficulty = unknown_session_difficulty_a3d2d8;
        unknown_id_source_a3d290->unknown_610 = 2 - unknown_id_source_a3d290->Difficulty;
        unknown_id_source_a3d290->Special().FogOfWar = unknown_session_fog_a3d33f;
        unknown_special_a40920.FogOfWar = unknown_session_fog_a3d33f;
    }
    unknown_id_source_a3d290->InitTime = ini.Get_Int("Basic", "InitTime", 10000);
    ini.Get_Bool("Basic", "Official", false);

    CCINIClass mission;
    CCFileClass file;
    if (unknown_game_mode_a3d298 == 0) {
        _splitpath(unknown_id_source_a3d290->FileName, 0, 0, buffer, 0);
        strcat(buffer, ".INI");
        file.Set_Name(buffer);
        if (file.Is_Available(0)) {
            mission.Load(file, false, false);
            Rules->Addition(mission);
        }
        file.Close();
        file.Set_Name("MISSION.INI");
        if (file.Is_Available(0)) {
            mission.Load(file, false, false);
            ScenarioClass *scen = unknown_id_source_a3d290;
            unknown_id_source_a3d290->Name[0] = 0;
            if (mission.Is_Present(unknown_id_source_a3d290->FileName, "Name")) {
                mission.Get_Unicode(unknown_id_source_a3d290->FileName, "Name", unknown_empty_wide_839d4c, unknown_id_source_a3d290->Name, 45);
            }
            unknown_id_source_a3d290->BriefingText[0] = 0;
            if (mission.Is_Present(unknown_id_source_a3d290->FileName, "Briefing")) {
                mission.Get_String(unknown_id_source_a3d290->FileName, "Briefing", unknown_string_83c55c, unknown_id_source_a3d290->Briefing, 32);
                if (strlen(unknown_id_source_a3d290->Briefing) != 0) {
                    wcsncpy(unknown_id_source_a3d290->BriefingText, StringTable::Load_String(unknown_id_source_a3d290->Briefing, 0, "D:\\Ra2\\Scenario.CPP", 3180), 0x400);
                    unknown_id_source_a3d290->BriefingText[0x3FF] = 0;
                }
            }
            unknown_id_source_a3d290->UIName[0] = 0;
            if (mission.Is_Present(unknown_id_source_a3d290->FileName, "UIName")) {
                mission.Get_String(unknown_id_source_a3d290->FileName, "UIName", unknown_string_83c55c, unknown_id_source_a3d290->UIName, 32);
                if (strlen(unknown_id_source_a3d290->UIName) != 0) {
                    wcsncpy(unknown_id_source_a3d290->Name, StringTable::Load_String(unknown_id_source_a3d290->UIName, 0, "D:\\Ra2\\Scenario.CPP", 3192), 45);
                    unknown_id_source_a3d290->Name[44] = 0;
                }
            }
            unknown_id_source_a3d290->LSLoadMessage[0] = 0;
            if (mission.Is_Present(unknown_id_source_a3d290->FileName, "LSLoadMessage")) {
                mission.Get_String(unknown_id_source_a3d290->FileName, "LSLoadMessage", unknown_string_83c55c, unknown_id_source_a3d290->LSLoadMessage, 31);
            }
            unknown_id_source_a3d290->LSLoadBriefing[0] = 0;
            if (mission.Is_Present(unknown_id_source_a3d290->FileName, "LSLoadBriefing")) {
                mission.Get_String(unknown_id_source_a3d290->FileName, "LSLoadBriefing", unknown_string_83c55c, unknown_id_source_a3d290->LSLoadBriefing, 31);
            }
            if (mission.Is_Present(unknown_id_source_a3d290->FileName, "LS640BriefLocX")) {
                unknown_id_source_a3d290->LS640BriefLocX = mission.Get_Int(unknown_id_source_a3d290->FileName, "LS640BriefLocX", unknown_id_source_a3d290->LS640BriefLocX);
            }
            if (mission.Is_Present(unknown_id_source_a3d290->FileName, "LS640BriefLocY")) {
                unknown_id_source_a3d290->LS640BriefLocY = mission.Get_Int(unknown_id_source_a3d290->FileName, "LS640BriefLocY", unknown_id_source_a3d290->LS640BriefLocY);
            }
            if (mission.Is_Present(unknown_id_source_a3d290->FileName, "LS800BriefLocX")) {
                unknown_id_source_a3d290->LS800BriefLocX = mission.Get_Int(unknown_id_source_a3d290->FileName, "LS800BriefLocX", unknown_id_source_a3d290->LS800BriefLocX);
            }
            if (mission.Is_Present(unknown_id_source_a3d290->FileName, "LS800BriefLocY")) {
                unknown_id_source_a3d290->LS800BriefLocY = mission.Get_Int(unknown_id_source_a3d290->FileName, "LS800BriefLocY", unknown_id_source_a3d290->LS800BriefLocY);
            }
            unknown_id_source_a3d290->LS640BkgdName[0] = 0;
            if (mission.Is_Present(unknown_id_source_a3d290->FileName, "LS640BkgdName")) {
                mission.Get_String(unknown_id_source_a3d290->FileName, "LS640BkgdName", unknown_string_83c55c, unknown_id_source_a3d290->LS640BkgdName, 64);
            }
            unknown_id_source_a3d290->LS800BkgdName[0] = 0;
            if (mission.Is_Present(unknown_id_source_a3d290->FileName, "LS800BkgdName")) {
                mission.Get_String(unknown_id_source_a3d290->FileName, "LS800BkgdName", unknown_string_83c55c, unknown_id_source_a3d290->LS800BkgdName, 64);
            }
        }
    }

    Unknown_00535910()->Unknown_00535BF0();
    unknown_session_a3d298.Update_Progress(3);
    Swizzler.Reset();
    delete unknown_tactical_83996c;
    unknown_tactical_83996c = new Tactical;
    unknown_tactical_83996c->Set_View_Dimensions_006a65a0(unknown_rect_839630);
    unknown_id_source_a3d290->Theater = ini.Get_TheaterType("Map", "Theater", (TheaterType)0);
    Init_Theater_00519e70(unknown_id_source_a3d290->Theater);
    unknown_session_a3d298.Update_Progress(30);
    Rules->Advanced_Command_Bar(unknown_ini_839860, unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5);
    unknown_session_a3d298.Update_Progress(31);
    Rules->Process(*unknown_ini_8396d0);
    unknown_session_a3d298.Update_Progress(35);
    Service_Game_IO_00481fb0();

    ScenarioClass *scenario = unknown_id_source_a3d290;
    CCINIClass &rules = *unknown_ini_8396d0;
    int count = Min(rules.Entry_Count("VariableNames"), 50);
    for (int index = 0; index < count; index++) {
        char const *entry = rules.Get_Entry("VariableNames", index);
        int slot = atoi(entry);
        rules.Get_String("VariableNames", entry, 0, scenario->Variables[slot].unknown_00, 40);
    }
    Service_Game_IO_00481fb0();
    Rules->Addition(ini);
    unknown_session_a3d298.Update_Progress(45);

    unknown_a40770 = 0;
    scenario = unknown_id_source_a3d290;
    int side = 0;
    if (unknown_game_mode_a3d298 == 0) {
        if (scenario->unknown_224c != -1) {
            side = Campaigns[scenario->unknown_224c]->unknown_98;
        }
    } else {
        int country = unknown_players_a3fa7c[0]->unknown_4b;
        if (country == -3) {
            country = 0;
        }
        side = HouseTypes[country]->Side;
    }
    bool allied = side == 0;
    if (unknown_game_mode_a3d298 == 0) {
        HouseClass::Read_Scenario_INI(ini);
        ini.Get_String("Basic", "Player", "Americans", buffer, sizeof(buffer));
        int house = HouseClass::Find_Index(buffer);
        if (house != -1 && house <= Houses.Count()) {
            allied = Houses[house]->Type->Side == 0;
        } else {
            allied = false;
        }
        unknown_id_source_a3d290->unknown_2239 = allied;
        if (!Unknown_0051a330(!allied)) {
            unknown_a40774--;
            return false;
        }
    } else {
        scenario->unknown_2239 = allied;
        if (!Unknown_0051a330(!allied)) {
            unknown_a40774--;
            return false;
        }
    }
    unknown_session_a3d298.Update_Progress(50);
    if (!unknown_id_source_a3d290->Read_Basic(ini)) {
        unknown_a40774--;
        return false;
    }
    unknown_session_a3d298.Update_Progress(58);
    if (unknown_game_mode_a3d298 == 0) {
        char player[32];
        ini.Get_String("Basic", "Player", "Americans", player, sizeof(player));
        int house = HouseClass::Find_Index(player);
        if (house != -1 && house <= Houses.Count()) {
            allied = Houses[house]->Type->Side == 0;
        } else {
            allied = false;
        }
        if (!Unknown_0051a330(!allied)) {
            unknown_a40774--;
            return false;
        }
        unknown_id_source_a3d290->unknown_2239 = allied;
    } else {
        unknown_id_source_a3d290->unknown_2239 = allied;
        if (unknown_session_a3d29c) {
            unknown_session_a3d29c->unknown_slot26();
        }
    }
    if (unknown_game_mode_a3d298 != 0 && !unknown_session_a3d2c0) {
        unknown_special_a40920.DestroyableBridges = false;
    }
    unknown_special_a40920.Unknown_0068B410();
    Service_Game_IO_00481fb0();

    TeamTypeClass::Read_Scenario_INI(unknown_ini_8397a0, 1);
    TeamTypeClass::Read_Scenario_INI(ini, 0);
    ScriptTypeClass::Read_Scenario_INI(unknown_ini_8397a0, 1);
    ScriptTypeClass::Read_Scenario_INI(ini, 0);
    TaskForceClass::Read_Scenario_INI(unknown_ini_8397a0, 1);
    TaskForceClass::Read_Scenario_INI(ini, 0);
    TriggerTypeClass::Read_Scenario_INI(ini);
    TagTypeClass::Read_Scenario_INI(ini);
    AITriggerTypeClass::Read_Scenario_INI(unknown_ini_8397a0, 1);
    AITriggerTypeClass::Read_Scenario_INI(ini, 0);
    Unknown_006c0220();
    unknown_session_a3d298.Update_Progress(60);
    Map.Read_INI_0049CFD0(ini);
    Service_Game_IO_00481fb0();
    TubeClass::Read_INI(ini);
    BuildingTypeClass::unknown_func_45ebd0();
    Map.Flag_To_Redraw(2);
    unknown_session_a3d298.Update_Progress(70);
    Service_Game_IO_00481fb0();
    OverlayClass::Read_INI(ini);
    Service_Game_IO_00481fb0();
    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell; cell = Map.Iterate()) {
        cell->Recalc_Attributes(-1);
    }
    OverlayClass::unknown_func_5dc180();
    TerrainClass::Read_INI(ini);
    Service_Game_IO_00481fb0();
    VeinholeMonsterClass::Init_Vein_Growth_System(true);
    Unknown_006e9fe0();
    Unknown_006e9520();
    unknown_session_a3d298.Update_Progress(72);
    Map.Compute_Radar_Image();
    UnitClass::Read_INI(ini);
    Service_Game_IO_00481fb0();
    unknown_session_a3d298.Update_Progress(74);
    AircraftClass::Read_INI_0041A840(ini);
    Service_Game_IO_00481fb0();
    InfantryClass::Read_INI(ini);
    Service_Game_IO_00481fb0();
    unknown_session_a3d298.Update_Progress(76);
    unknown_7df630 = false;
    BuildingClass::Read_INI(ini);
    Service_Game_IO_00481fb0();
    unknown_session_a3d298.Update_Progress(78);
    unknown_7df630 = true;
    Service_Game_IO_00481fb0();
    SmudgeClass::Read_INI(ini);
    Service_Game_IO_00481fb0();
    if (unknown_game_mode_a3d298 == 5 && unknown_a40d39) {
        file.Close();
        file.Set_Name("TMCJ4F.INI");
        if (file.Is_Available(0) == true) {
            mission.Load(file, false, false);
            Rules->Addition(mission);
        }
    }
    unknown_session_a3d298.Update_Progress(82);
    Service_Game_IO_00481fb0();
    Map.Overpass();
    unknown_session_a3d298.Update_Progress(86);
    Service_Game_IO_00481fb0();
    unknown_object_84e8d0.Unknown_0042E1A0();
    unknown_session_a3d298.Update_Progress(90);
    Service_Game_IO_00481fb0();
    if (unknown_game_mode_a3d298 != 0 && !official) {
        Unknown_0065e8c0(ini.Get_Bool("Basic", "Official", false));
    }
    Service_Game_IO_00481fb0();
    Swizzler.Reset();
    unknown_session_a3d298.Update_Progress(96);
    Service_Game_IO_00481fb0();
    Unknown_006ecf10();
    if (unknown_game_mode_a3d298 != 0) {
        unknown_id_source_a3d290->Special() = unknown_special_a40920;
    }
    unknown_a40774--;
    int init = unknown_a40774;
    unknown_a40774 = 0;
    BuildingClass::unknown_44df40();
    unknown_a40774 = init;
    unknown_session_a3d298.Update_Progress(98);
    Service_Game_IO_00481fb0();
    Map.Clear_Redraw_Cells();
    if (unknown_id_source_a3d290->unknown_000 & 0x1000) {
        Map.Unknown_005685F0();
    }
    RadarEventClass_unknown_delete_all_63c640();
    Map.Complete_Radar_Refresh();
    return true;
}


// void __fastcall Write_Scenario_INI_0065f980(char const *filename, bool flag) (0x0065F980)
// Writes the scenario INI file (Westwood's Write_Scenario_INI; called by the
// map saver 0x0057823B with flag set): starts from the existing file when
// Debug_Map is set, writes [Basic] and the rest of the scenario's
// sections, the preview (or the houses without flag), the map, the objects,
// the teams and AI triggers (unless flag and not Debug_Map), the triggers
// and tags, and saves the INI.

void __fastcall Write_Scenario_INI_0065f980(char const *filename, bool flag)
{
    CCINIClass ini;
    if (Debug_Map) {
        if (CCFileClass(filename).Is_Available()) {
            CCFileClass file(filename);
            ini.Load(file, true, false);
        }
    }
    unknown_id_source_a3d290->Write_Basic(ini, flag);
    if (flag) {
        if (unknown_preview_a6fee4 != 0 && unknown_preview_a6fee4->Is_Present()) {
            unknown_preview_a6fee4->Write_INI(ini);
        } else {
            MapPreviewClass preview;
            preview.Create_Preview();
            preview.Write_INI(ini);
        }
    } else {
        HouseClass::Write_Scenario_INI(ini);
    }
    Map.Write_INI_0049D910(ini);
    TerrainClass::Write_INI(ini);
    UnitClass::Write_INI(ini);
    InfantryClass::Write_INI(ini);
    AircraftClass::Write_INI_0041A6C0(ini);
    Building_Write_All_INI_0044b200(&ini);
    OverlayClass::Write_INI(ini);
    SmudgeClass::Write_INI(ini);
    TubeClass::Write_INI(ini);
    if (!flag || Debug_Map) {
        Unknown_00668EA0(ini, 0);
        TaskForceClass::Write_Scenario_INI(ini, 0);
        TeamTypeClass::Write_Scenario_INI(ini, 0);
        AITriggerTypeClass::Write_Scenario_INI(ini, 0);
    }
    TriggerTypeClass::Write_Scenario_INI(ini);
    TagTypeClass::Write_Scenario_INI(ini);
    RawFileClass file(filename);
    ini.Save(file, true);
}


// void __fastcall Assign_Houses_0065fbb0() (0x0065FBB0)
// Makes the houses of a multiplayer game: one per player, lowest colour
// first (the first player's house is the player's; a WOL game names it after
// the player), then one per computer player set in the session (country,
// colour and difficulty, one easier with CompEasyBonus), then Neutral and
// Special in light grey. Random countries are drawn first.

void __fastcall Assign_Houses_0065fbb0()
{
    int count = unknown_players_a3fa7c.Count();
    for (int index = 0; index < count; index++) {
        if (unknown_players_a3fa7c[index]->unknown_4b == -3) {
            unknown_players_a3fa7c[index]->unknown_66e2d0(0);
        }
    }

    bool taken[8];
    memset(taken, 0, sizeof(taken));
    unknown_house_a72d28 = 0;
    for (int house_count = 0; house_count < count; house_count++) {
        int pick = -1;
        int lowest = -1;
        for (int other = 0; other < count; other++) {
            if (!taken[other] && (lowest == -1 || unknown_players_a3fa7c[other]->unknown_53 < lowest)) {
                pick = other;
                lowest = unknown_players_a3fa7c[other]->unknown_53;
            }
        }
        NodeNameType *player = unknown_players_a3fa7c[pick];
        taken[pick] = true;
        HouseClass *house = new HouseClass(HouseTypes[player->unknown_4b]);
        if (unknown_game_mode_a3d298 == 4) {
            Wstring name = Narrow_Copy_00771280(player->Name);
            house->IniName = FixedString<21>(name.Get());
        } else {
            house->IniName = FixedString<21>("<human player>");
        }
        wcsncpy(house->UIName, player->Name, 21);
        house->UIName[20] = 0;
        house->unknown_134 = true;
        house->unknown_func_4e8300(player->unknown_53, player->unknown_4b, unknown_session_a3d298.unknown_24);
        house->ColorSchemeIndex = unknown_session_a3d298.Unknown_006715D0(player->unknown_53);
        house->Update_Color();
        house->Update_Laser_Color();
        if (pick == 0) {
            unknown_player_a35db4 = house;
            house->unknown_135 = true;
        }
        if (player->Team == -1) {
            unknown_house_a72d28 = house;
        }
        house->unknown_11c = unknown_house_setting_7d88d8;
        house->Assign_Handicap(1);
        player->ID = house->ArrayIndex;
    }

    int computers = unknown_session_a3d298.AICount;
    int made = 0;
    for (int slot = 0; slot < 8; slot++) {
        if (made < computers && unknown_session_a3d298.AICountry[slot] != -1 && unknown_session_a3d298.AICountry[slot] != -3) {
            int country = unknown_session_a3d298.AICountry[slot];
            int color = unknown_session_a3d298.AIColor[slot];
            made++;
            HouseClass *house = new HouseClass(HouseTypes[country]);
            house->unknown_11c = unknown_house_setting_7d88d8;
            house->unknown_134 = false;
            house->unknown_func_4e8300(color, country, unknown_session_a3d298.unknown_24);
            house->ColorSchemeIndex = unknown_session_a3d298.Unknown_006715D0(color);
            house->Update_Color();
            house->Update_Laser_Color();
            house->IniName = FixedString<21>("Computer");
            wcscpy(house->UIName, StringTable::Load_String("TXT_COMPUTER", 0, "D:\\Ra2\\Scenario.CPP", 3947));
            if (unknown_game_mode_a3d298 != 0) {
                house->unknown_194 = Rules->MaxIQ;
            }
            int difficulty = unknown_session_a3d298.AIDifficulty[slot];
            if (unknown_players_a3fa7c.Count() > 1 && Rules->CompEasyBonus && difficulty > 0) {
                difficulty--;
            }
            house->Assign_Handicap(difficulty);
        }
    }

    HouseClass *neutral = new HouseClass(HouseTypes[HouseTypeClass::Find_Index("Neutral")]);
    neutral->ColorSchemeIndex = unknown_color_scheme_index_663d90("LightGrey", 0x35);
    neutral->Update_Color();
    HouseClass *special = new HouseClass(HouseTypes[HouseTypeClass::Find_Index("Special")]);
    special->ColorSchemeIndex = unknown_color_scheme_index_663d90("LightGrey", 0x35);
    special->Update_Color();
}


// DynamicVectorClass<Cell> __fastcall Multiplayer_Start_Waypoints_0065fff0(bool official) (0x0065FFF0)
// The multiplayer start cells: the set waypoints among the first ones (one per
// player and computer house, or as many consecutive ones as are set), then
// random reachable cells inside the map until there are enough (Westwood's
// Build_Start_Waypoint_List; the official flag is not read).

DynamicVectorClass<Cell> __fastcall Multiplayer_Start_Waypoints_0065fff0(bool official)
{
    DynamicVectorClass<Cell> waypts;

    int num_waypts = 0;
    for (int i = 0; i < 8; i++) {
        if (unknown_id_source_a3d290->Unknown_00663240(i)) {
            num_waypts++;
        } else {
            break;
        }
    }
    int observers = 0;
    for (int player = 0; player < unknown_players_a3fa7c.Count(); player++) {
        if (unknown_players_a3fa7c[player]->Team == -1) {
            observers++;
        }
    }
    // Westwood's MAX macro: the house count expression is written twice.
    int look_for = num_waypts > unknown_players_a3fa7c.Count() - observers + unknown_session_a3d298.AICount
        ? num_waypts : unknown_players_a3fa7c.Count() - observers + unknown_session_a3d298.AICount;

    for (int waycount = 0; waycount < look_for; waycount++) {
        if (unknown_id_source_a3d290->Unknown_00663240(waycount)) {
            waypts.Add(unknown_id_source_a3d290->Get_Waypoint_Location(waycount));
            Debug_Printf("Multiplayer start waypoint found at cell %d,%d\n", unknown_id_source_a3d290->Get_Waypoint_Location(waycount).X, unknown_id_source_a3d290->Get_Waypoint_Location(waycount).Y);
        }
    }

    int deficiency = look_for - waypts.Count();
    if (deficiency > 0) {
        Debug_Printf("Multiplayer start waypoint deficiency - looking for more start positions\n");
        while (waypts.Count() < look_for) {
            Cell trycell = Cell(Map.unknown_120 + Random_Pick(10, Map.unknown_128 - 10), Map.unknown_124 + 10 + Random_Pick(0, Map.unknown_12c - 10));
            trycell = Map.Nearby_Location(trycell, 1, -1, 0, false, Point2D(8, 8), 0, 0, 0, 1, Cell(0, 0), 0);
            if (trycell != unknown_geometry_k) {
                waypts.Add(trycell);
                Debug_Printf("Random multiplayer start waypoint added at cell %d,%d\n", trycell.X, trycell.Y);
            }
        }
    }
    return waypts;
}


// void __fastcall Create_Units_00660320(bool official) (0x00660320)
// Westwood's Create_Units (name hypothesis): makes every multiplayer house's
// start units. The budget is the session's unit count (one less with bases)
// times the average Raw_Cost of the unit and infantry types allowed at the
// start (Rules BaseUnit excluded). Each house that is not passive gets a start
// cell: a random one first, then the free cell farthest from the taken ones.
// With bases it gets its base unit there (from Rules BaseUnit). Then it buys
// random units (while under two thirds of the budget) or infantry it may own
// at its tech level until the budget is spent, places each near the cell,
// makes them veterans with InitialVeteran and gives them mission 5 or 11.
// UnitClass is this unit's stand-in (shared/unit_type.hpp; the full class
// cannot be included after it), so the new base unit is used as TechnoClass.

void __fastcall Create_Units_00660320(bool official)
{
    DynamicVectorClass<TechnoClass *> objects;
    int tot_units = unknown_session_a3d298.UnitCount;
    if (unknown_flag_a3d2b8) {
        tot_units--;
    }
    Debug_Printf("Creating %d units - Random seed is %08x\n", tot_units, unknown_id_source_a3d290->RandomNumber);
    Debug_Printf("UniqueID is %08x\n", unknown_id_source_a3d290->UniqueID);

    int tot_cost = 0;
    int tot_count = 0;
    for (int index = 0; index < UnitTypes.Count(); index++) {
        UnitTypeClass *unit_type = UnitTypes[index];
        if (unit_type->AllowedToStartInMultiplayer && !Rules->BaseUnit.Is_Present(unit_type)) {
            tot_cost += unit_type->Raw_Cost();
            tot_count++;
        }
    }
    for (int infantry = 0; infantry < InfantryTypes.Count(); infantry++) {
        InfantryTypeClass *infantry_type = InfantryTypes[infantry];
        if (infantry_type->AllowedToStartInMultiplayer) {
            tot_cost += infantry_type->Raw_Cost();
            tot_count++;
        }
    }
    int budget = tot_cost / tot_count * tot_units;

    DynamicVectorClass<Cell> waypoints = Multiplayer_Start_Waypoints_0065fff0(official);
    bool taken[16];
    memset(taken, 0, sizeof(taken));
    int placed = 0;
    Cell cell;

    for (int house_index = 0; house_index < Houses.Count(); house_index++) {
        HouseClass *house = Houses[house_index];
        if (house->Type->MultiplayPassive) {
            continue;
        }
        Debug_Printf("Generating units for house %d (%s)\n", house_index, (char const *)house->Type->ID);
        DynamicVectorClass<UnitTypeClass *> unit_list;
        DynamicVectorClass<InfantryTypeClass *> infantry_list;
        int owner = 1 << house->Type->ArrayIndex;
        for (int u = 0; u < UnitTypes.Count(); u++) {
            UnitTypeClass *unit_type = UnitTypes[u];
            if (unit_type->AllowedToStartInMultiplayer && unit_type->TechLevel <= house->unknown_11c
                && (unit_type->Owner & owner) && !Rules->BaseUnit.Is_Present(unit_type)) {
                unit_list.Add(unit_type);
            }
        }
        for (int i = 0; i < InfantryTypes.Count(); i++) {
            InfantryTypeClass *infantry_type = InfantryTypes[i];
            if (infantry_type->AllowedToStartInMultiplayer && infantry_type->TechLevel <= house->unknown_11c
                && (infantry_type->Owner & owner)) {
                infantry_list.Add(infantry_type);
            }
        }

        if (placed == 0) {
            int pick = unknown_id_source_a3d290->Random()(0, waypoints.Count() - 1);
            cell = waypoints[pick];
            taken[pick] = true;
            placed = 1;
        } else {
            int distance[26];
            memset(distance, 0, sizeof(distance));
            for (int from = 0; from < waypoints.Count(); from++) {
                if (!taken[from]) {
                    for (int to = 0; to < waypoints.Count(); to++) {
                        if (taken[to]) {
                            distance[from] += Cell_Length(waypoints[from] - waypoints[to]);
                        }
                    }
                }
            }
            int best = 0;
            int best_distance = 0;
            for (int w = 0; w < waypoints.Count(); w++) {
                if (distance[w] > best_distance || best_distance == 0) {
                    best_distance = distance[w];
                    best = w;
                }
            }
            cell = waypoints[best];
            taken[best] = true;
            placed++;
        }
        house->BaseCenter = CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0);

        if (unknown_flag_a3d2b8) {
            TechnoClass *unit = (TechnoClass *)new UnitClass(house->Unknown_004EF6F0(Rules->BaseUnit), house);
            if (!unit->Unlimbo(&CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0), 0) && !Unknown_00660b60(unit, cell, 1)) {
                delete unit;
                unit = 0;
            }
            if (unit != 0) {
                house->unknown_530c = unknown_geometry_k;
                house->unknown_5308 = 0;
                if (unknown_id_source_a3d290->Special().unknown_bit_4) {
                    house->unknown_func_4e77a0(unit, true);
                }
            }
        }

        int cost = 0;
        objects.Clear();
        while (cost < budget) {
            TechnoTypeClass *type = 0;
            if (cost < budget * 2 / 3 && unit_list.Count() > 0) {
                type = unit_list[unknown_id_source_a3d290->Random()(0, unit_list.Count() - 1)];
            } else if (infantry_list.Count() > 0) {
                type = infantry_list[unknown_id_source_a3d290->Random()(0, infantry_list.Count() - 1)];
            }
            ObjectClass *object = type->Create_One_Of(house);
            TechnoClass *techno = object->As_Techno();
            if (!Unknown_00660b60(object, cell, 4)) {
                delete object;
            } else {
                Debug_Printf("House %s deployed object %s\n", (char const *)house->Type->ID, (char const *)type->ID);
                cost += type->Raw_Cost();
                objects.Add(techno);
                if (unknown_id_source_a3d290->Special().InitialVeteran) {
                    techno->unknown_118.Set_Elite(true);
                }
                if (!house->Is_Human_Player()) {
                    techno->Set_Mission(11);
                } else {
                    techno->Set_Mission(5);
                }
            }
        }
        objects.Clear();
    }
    Debug_Printf("Finished unit generation. Random number is %d\n", unknown_id_source_a3d290->Random()(0, 0xFFFF));
}


// int __fastcall Unknown_00660b60(ObjectClass *object, Cell &center, int distance) (0x00660B60)
// Places an object near a cell, scanning eight directions at increasing radii.
// Clip_Move/Clip_Scatter are file-local operation-boundary hypotheses from the reference twin.

static Cell const Placement_Clip_Move(Cell const &center,FacingType direction,int distance)
{
    int x=center.X;
    int y=center.Y;
    int left=Map.unknown_120;
    int right=left+Map.unknown_128-1;
    int top=Map.unknown_124;
    int bottom=top+Map.unknown_12c-1;
    switch(direction) {
    case 0: y-=distance; break;
    case 1: x+=distance; y-=distance; break;
    case 2: x+=distance; break;
    case 3: x+=distance; y+=distance; break;
    case 4: y+=distance; break;
    case 5: x-=distance; y+=distance; break;
    case 6: x-=distance; break;
    case 7: x-=distance; y-=distance; break;
    }
    if(x>right) x=right;
    if(x<left) x=left;
    if(y>bottom) y=bottom;
    if(y<top) y=top;
    return Cell(x,y);
}
static Cell const Placement_Clip_Scatter(Cell const &center,int radius)
{
    int x=center.X;
    int y=center.Y;
    int left=Map.unknown_120;
    int right=left+Map.unknown_128-1;
    int top=Map.unknown_124;
    int bottom=top+Map.unknown_12c-1;
    int xdistance=Random_Pick(0,radius);
    if(Random_Pick(0,99)<50) {
        x+=xdistance;
        if(x>right) x=right;
    } else {
        x-=xdistance;
        if(x<left) x=left;
    }
    int ydistance=Random_Pick(0,radius);
    if(Random_Pick(0,99)<50) {
        y+=ydistance;
        if(y>bottom) y=bottom;
    } else {
        y-=ydistance;
        if(y<top) y=top;
    }
    return Cell(x,y);
}
int __fastcall Unknown_00660b60(ObjectClass *object,Cell &center,int distance)
{
    if(Map.In_Radar(center)) {
        ObjectClass *occupant=Map[center].Cell_Techno(Point2D(0,0),false,0);
        if(!occupant || (occupant->Kind_Of()==15 && object->Kind_Of()==15)) {
            if(object->Unlimbo(&UnknownCoord434820(center,Map.Get_Height_GL(UnknownCoord434820(center))),0)) return 1;
        }
    }
    int pass;
    FacingType count;
    int skip;
    ObjectClass *occupant;
    Cell target;
    int range=distance;
    FacingType direction;
    for(;range<32;++range) {
        direction=Random_Pick(FACING_N,FACING_NW);
        for(pass=0;pass<2;++pass) {
            for(count=FACING_N;count<=FACING_NW;count=FacingType(count+1)) {
                skip=0;
                target=Placement_Clip_Move(center,direction,range);
                if(pass>0) target=Placement_Clip_Scatter(target,1);
                if(target.X==center.X && target.Y==center.Y) skip=1;
                if(!skip) {
                    occupant=Map[target].Cell_Techno(Point2D(0,0),false,0);
                    if(!occupant || (occupant->Kind_Of()==15 && object->Kind_Of()==15)) {
                        if(object->Unlimbo(&UnknownCoord434820(target,Map.Get_Height_GL(UnknownCoord434820(target))),0)) return 1;
                    }
                }
                direction=FacingType(direction+1);
                if(direction>FACING_NW) direction=FACING_N;
            }
        }
    }
    return 0;
}


// void ScenarioClass::Save(IStream *stream) (0x00660F60)
// Writes the scenario to a saved game (called by the game save 0x006558B0):
// the whole object, then the three allowable-unit lists, with the elapsed
// time stopped while it is written and restarted afterwards.

void ScenarioClass::Save(IStream *stream)
{
    Debug_Printf("Scenario Save: ElapsedTimer = %ld\n", ElapsedTime);
    bool stopped = ElapsedTime.Started == -1;
    if (!stopped) {
        ElapsedTime.Stop();
    }
    stream->Write(this, sizeof(*this), 0);
    AllowableUnits.Save(stream);
    AllowableUnitMaximums.Save(stream);
    AllowableUnitCounts.Save(stream);
    if (!stopped) {
        ElapsedTime.Start();
    }
}


// void ScenarioClass::Load(IStream *stream) (0x006610C0)
// Reads the scenario back from a saved game (called after Clear_Scenario by
// the game load 0x00656B85): stops the elapsed time, reads the whole object
// and rebuilds it in place, reads the three allowable-unit lists, asks for
// the unit type pointers to be fixed up, fetches the mission timer text again
// and restarts the elapsed time.

void ScenarioClass::Load(IStream *stream)
{
    ElapsedTime.Stop();
    stream->Read(this, sizeof(*this), 0);
    new (this) ScenarioClass(NoInitClass());
    AllowableUnits.Load(stream);
    AllowableUnitMaximums.Load(stream);
    AllowableUnitCounts.Load(stream);
    for (int index = 0; index < AllowableUnits.Count(); index++) {
        Swizzle_Pointer(AllowableUnits[index]);
    }
    if (MissionTimerLabel[0] != 0) {
        MissionTimerText = StringTable::Load_String(MissionTimerLabel, 0, "D:\\Ra2\\Scenario.CPP", 6379);
    } else {
        MissionTimerText = unknown_empty_wide_839d4c;
    }
    ElapsedTime.Start();
    Debug_Printf("Scenario Load: ElapsedTimer = %ld\n", ElapsedTime);
}


// ScenarioClass's variable setters (0x006612C0, 0x00661560).

// Sets one of the 50 variables at +0x1210 (the global ones; TagClass
// 0x006B4220 is told of a change) and returns its old value; false for an
// index out of range.
bool ScenarioClass::unknown_6612c0(unsigned index, bool value)
{
    if (index < 50) {
        bool old = Variables[index].Value;
        if (old != value) {
            Variables[index].Value = value;
            unknown_222e = true;
            TagClass::All_Timer_Global_Reset(index);
        }
        return old;
    }
    return false;
}


// Sets the global variable of a name (true when there is none; 0x00661310).

bool ScenarioClass::Set_Global_Variable(char const *name, bool value)
{
    int index = Find_Global_Variable(name);
    if (index != -1) {
        return unknown_6612c0(index, value);
    }
    return true;
}


// The value of a global variable by index (false out of range; 0x006613B0).

bool ScenarioClass::Get_Global_Variable(int index, bool *value)
{
    if (index >= 0 && index < 50) {
        *value = Variables[index].Value;
        return true;
    }
    return false;
}


// The value of a global variable by name (false when there is none;
// 0x006613E0).

bool ScenarioClass::Get_Global_Variable(char const *name, bool *value)
{
    int index = Find_Global_Variable(name);
    if (index == -1) {
        return false;
    }
    return Get_Global_Variable(index, value);
}


// The index of the global variable of a name, or -1 (0x00661470).

int ScenarioClass::Find_Global_Variable(char const *name)
{
    for (int index = 0; index < 50; index++) {
        if (strcmp(name, Variables[index].unknown_00) == 0) {
            return index;
        }
    }
    return -1;
}


// bool ScenarioClass::Read_Global_Variables(CCINIClass &ini) (0x006614D0)
// Reads the global variable names from [VariableNames] ("index=name", at most
// 50 entries). Always true.

// Westwood-style macro: each operand is evaluated where it appears.

bool ScenarioClass::Read_Global_Variables(CCINIClass &ini)
{
    int count = Min(ini.Entry_Count("VariableNames"), 50);
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry("VariableNames", index);
        int slot = atoi(entry);
        ini.Get_String("VariableNames", entry, 0, Variables[slot].unknown_00, 40);
    }
    return true;
}


// Sets one of the 50 local variables at +0x1A12 (TagClass 0x006B4250 is told
// of a change) and returns its old value; false for an index out of range.
bool ScenarioClass::unknown_661560(unsigned index, bool value)
{
    if (index < 50) {
        bool old = LocalVariables[index].Value;
        if (old != value) {
            LocalVariables[index].Value = value;
            unknown_222e = true;
            TagClass::All_Timer_Local_Reset(index);
        }
        return old;
    }
    return false;
}


// Sets the local variable of a name (true when there is none; 0x006615B0).

bool ScenarioClass::Set_Local_Variable(char const *name, bool value)
{
    int index = Find_Local_Variable(name);
    if (index != -1) {
        return unknown_661560(index, value);
    }
    return true;
}


// The value of a local variable by index (false out of range; 0x00661650).

bool ScenarioClass::Get_Local_Variable(int index, bool *value)
{
    if (index >= 0 && index < 50) {
        *value = LocalVariables[index].Value;
        return true;
    }
    return false;
}


// The value of a local variable by name (false when there is none;
// 0x00661680).

bool ScenarioClass::Get_Local_Variable(char const *name, bool *value)
{
    int index = Find_Local_Variable(name);
    if (index == -1) {
        return false;
    }
    return Get_Local_Variable(index, value);
}


// The index of the local variable of a name, or -1 (0x00661710).

int ScenarioClass::Find_Local_Variable(char const *name)
{
    for (int index = 0; index < 50; index++) {
        if (strcmp(name, LocalVariables[index].unknown_00) == 0) {
            return index;
        }
    }
    return -1;
}


// bool ScenarioClass::Read_Variables(CCINIClass &ini) (0x00661770)
// Clears the 50 local variable names, then reads [VariableNames] (at most
// 50 entries, "index=name,value"): the name and, when given, the value.
// Always true.

// Westwood-style macro: each operand is evaluated where it appears.

bool ScenarioClass::Read_Variables(CCINIClass &ini)
{
    for (int index = 0; index < 50; index++) {
        LocalVariables[index].unknown_00[0] = 0;
    }
    int count = Min(ini.Entry_Count("VariableNames"), 50);
    for (index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry("VariableNames", index);
        int slot = atoi(entry);
        char buffer[128];
        ini.Get_String("VariableNames", entry, 0, buffer, sizeof(buffer));
        char const *name = strtok(buffer, ",");
        strcpy(LocalVariables[slot].unknown_00, name);
        char const *value = strtok(0, ",");
        if (value) {
            LocalVariables[slot].Value = atoi(value) != 0;
        }
    }
    return true;
}


// bool ScenarioClass::Write_Variables(CCINIClass &ini) (0x006618B0)
// Rewrites [VariableNames]: each named local variable as "index=name,value".
// Always true.

bool ScenarioClass::Write_Variables(CCINIClass &ini)
{
    ini.Clear(unknown_variable_names_section_7a9200);
    for (int index = 0; index < 50; index++) {
        UnknownScenarioVariable_004B5AB0 &variable = LocalVariables[index];
        if (variable.unknown_00[0] != 0) {
            char entry[12];
            char value[128];
            wsprintfA(entry, "%d", index);
            wsprintfA(value, "%s,%d", variable.unknown_00, variable.Value != 0);
            ini.Put_String(unknown_variable_names_section_7a9200, entry, value);
        }
    }
    return true;
}


// 0x00661940: function not yet identified
//
// Body: 26 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: int __fastcall Used_Local_Count_00661940(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00661940 {

int __fastcall Used_Local_Count_00661940(int scenario)
{
    int count;
    char *name;
    int remaining;

    count = (int)(0);
    name = (char *)(scenario + 0x1a12);
    remaining = (int)(0x32);
    do {
        if (*name != '\0') {
            count = (int)(count + 1);
        }
        name = (char *)(name + 0x29);
        remaining = (int)(remaining + -1);
    } while (remaining != 0);
    return count;
}

}  // namespace seed_00661940


// int __fastcall unknown_find_free_slot(UnknownSlotOwner_00661960 *) -- 0x00661960
// Purpose: returns the index of the first of 50 records (0x29 bytes each,
// starting at +0x1A12) whose first byte is zero, or -1 when all are in use.
// No direct callers (alignment-bounded entry). Owner type not identified.
struct UnknownSlot_00661960 {
    char unknown_00;
    char unknown_01[0x28];
};

struct UnknownSlotOwner_00661960 {
    char unknown_0000[0x1a12];
    UnknownSlot_00661960 unknown_slots_1a12[50];
};

int __fastcall unknown_find_free_slot_00661960(UnknownSlotOwner_00661960 *owner)
{
    for (int index = 0; index < 50; index++) {
        if (owner->unknown_slots_1a12[index].unknown_00 == '\0') {
            return index;
        }
    }
    return -1;
}


// bool ScenarioClass::Read_Basic(CCINIClass &ini) (0x00661980)
// Reads the scenario's [Basic] settings (the special options, the next
// scenarios, the movies, the flags, the carried money capped at 1.0, the
// loadout lists, whose limits and counts are padded to the type count), the
// local variables, the par times and texts ([Ranking]) and the lighting
// levels ([Lighting], stored in hundredths or thousandths; the ion levels
// default to the normal ones). Then the player's house: by [Basic] Player in
// a normal game, else through 0x0065FBB0. Reports the loading progress
// (55, 58, 60 percent); always true.


// Westwood-style macro: each operand is evaluated where it appears (the
// parenthesized operands are load-bearing, VC6_MATCHING_NOTES).

bool ScenarioClass::Read_Basic(CCINIClass &ini)
{
    Special().Read_INI(ini);
    ini.Get_String("Basic", "NextScenario", NextScenario, NextScenario, sizeof(NextScenario));
    ini.Get_String("Basic", "AltNextScenario", AltNextScenario, AltNextScenario, sizeof(AltNextScenario));
    IntroMovie = ini.Get_Listed_Name("Basic", "Intro", IntroMovie);
    BriefMovie = ini.Get_Listed_Name("Basic", "Brief", BriefMovie);
    WinMovie = ini.Get_Listed_Name("Basic", "Win", WinMovie);
    LoseMovie = ini.Get_Listed_Name("Basic", "Lose", LoseMovie);
    ActionMovie = ini.Get_Listed_Name("Basic", "Action", ActionMovie);
    PostScoreMovie = ini.Get_Listed_Name("Basic", "PostScore", PostScoreMovie);
    PreMapSelectMovie = ini.Get_Listed_Name("Basic", "PreMapSelect", PreMapSelectMovie);
    MultiplayerOnly = ini.Get_Bool("Basic", "MultiplayerOnly", MultiplayerOnly);
    unknown_2231 = ini.Get_Bool("Basic", "TimerInherit", unknown_2231);
    EndOfGame = ini.Get_Bool("Basic", "EndOfGame", EndOfGame);
    Theme = ini.Get_ThemeType("Basic", "Theme", Theme);
    unknown_ini_format_a40d24 = ini.Get_Int("Basic", "NewINIFormat", 0);
    CarryOverMoney = ini.Get_Double("Basic", "CarryOverMoney", CarryOverMoney);
    CarryOverMoney = Min(CarryOverMoney, 1.0);
    unknown_1208 = ini.Get_Int("Basic", "CarryOverCap", unknown_1208);
    SkipScore = ini.Get_Bool("Basic", "SkipScore", SkipScore);
    OneTimeOnly = ini.Get_Bool("Basic", "OneTimeOnly", OneTimeOnly);
    SkipMapSelect = ini.Get_Bool("Basic", "SkipMapSelect", SkipMapSelect);
    TruckCrate = ini.Get_Bool("Basic", "TruckCrate", TruckCrate);
    unknown_2229 = ini.Get_Bool("Basic", "TrainCrate", unknown_2229);
    FillSilos = ini.Get_Bool("Basic", "FillSilos", FillSilos);
    unknown_2238 = ini.Get_Bool("Basic", "IgnoreGlobalAITriggers", unknown_2238);
    Percent = ini.Get_Int("Basic", "Percent", Percent);
    StartingDropships = ini.Get_Int("Basic", "StartingDropships", StartingDropships);
    AllowableUnits = ini.Get_Techno_List("Basic", "AllowableUnits", AllowableUnits);
    AllowableUnitMaximums = ini.Get_Integer_List("Basic", "AllowableUnitMaximums", AllowableUnitMaximums);
    unknown_222a = ini.Get_Bool("Basic", "TiberiumGrowthEnabled", unknown_222a);
    unknown_222b = ini.Get_Bool("Basic", "VeinGrowthEnabled", unknown_222b);
    unknown_222c = ini.Get_Bool("Basic", "IceGrowthEnabled", unknown_222c);
    TiberiumDeathToVisceroid = ini.Get_Bool("Basic", "TiberiumDeathToVisceroid", TiberiumDeathToVisceroid);
    FreeRadar = ini.Get_Bool("Basic", "FreeRadar", FreeRadar);
    HomeCell = ini.Get_Int("Basic", "HomeCell", HomeCell);
    AltHomeCell = ini.Get_Int("Basic", "AltHomeCell", AltHomeCell);
    ini.Get_Bool("Basic", "CivEvac", false);

    int index;
    for (index = 0; index < AllowableUnits.Count() - AllowableUnitMaximums.Count(); index++) {
        AllowableUnitMaximums.Add(-1);
    }
    for (index = 0; index < AllowableUnitMaximums.Count(); index++) {
        AllowableUnitCounts.Add(0);
    }

    Read_Variables(ini);
    Service_Game_IO_00481fb0();

    ParTimeEasy = ini.Get_Time("Ranking", "ParTimeEasy", ParTimeEasy);
    ParTimeMedium = ini.Get_Time("Ranking", "ParTimeMedium", ParTimeMedium);
    ParTimeHard = ini.Get_Time("Ranking", "ParTimeHard", ParTimeHard);
    ini.Get_String("Ranking", "UnderParTitle", "GUI:NoUnderParTitle", UnderParTitle, sizeof(UnderParTitle));
    ini.Get_String("Ranking", "UnderParMessage", "GUI:NoUnderParMessage", UnderParMessage, sizeof(UnderParMessage));
    ini.Get_String("Ranking", "OverParTitle", "GUI:NoOverParTitle", OverParTitle, sizeof(OverParTitle));
    ini.Get_String("Ranking", "OverParMessage", "GUI:NoOverParMessage", OverParMessage, sizeof(OverParMessage));

    unknown_22b0 = unknown_22ac = unknown_22a8 = ini.Get_Double("Lighting", "Ambient", unknown_22a8 * 0.01) * 100.0;
    AmbientRed = ini.Get_Double("Lighting", "Red", AmbientRed * 0.01) * 100.0;
    AmbientGreen = ini.Get_Double("Lighting", "Green", AmbientGreen * 0.01) * 100.0;
    AmbientBlue = ini.Get_Double("Lighting", "Blue", AmbientBlue * 0.01) * 100.0;
    AmbientGround = ini.Get_Double("Lighting", "Ground", AmbientGround / 1000) * 1000.0;
    AmbientLevel = ini.Get_Double("Lighting", "Level", AmbientLevel / 1000) * 1000.0;
    IonAmbient = ini.Get_Double("Lighting", "IonAmbient", unknown_22a8 * 0.01) * 100.0;
    IonRed = ini.Get_Double("Lighting", "IonRed", AmbientRed * 0.01) * 100.0;
    IonGreen = ini.Get_Double("Lighting", "IonGreen", AmbientGreen * 0.01) * 100.0;
    IonBlue = ini.Get_Double("Lighting", "IonBlue", AmbientBlue * 0.01) * 100.0;
    IonGround = ini.Get_Double("Lighting", "IonGround", AmbientGround / 1000) * 1000.0;
    IonLevel = ini.Get_Double("Lighting", "IonLevel", AmbientLevel / 1000) * 1000.0;
    NukeAmbientChangeRate = ini.Get_Double("Lighting", "NukeAmbientChangeRate", NukeAmbientChangeRate);
    unknown_session_a3d298.Update_Progress(55);
    Service_Game_IO_00481fb0();

    if (unknown_game_mode_a3d298 == 0) {
        char player[20];
        ini.Get_String("Basic", "Player", unknown_string_83c55c, player, sizeof(player));
        int house = HouseClass::Find_Index(player);
        if (house == -1) {
            house = 0;
        }
        unknown_player_a35db4 = Houses[house];
        unknown_11fc = unknown_player_a35db4->Type->House;
    } else {
        Assign_Houses_0065fbb0();
    }
    unknown_player_a35db4->unknown_134 = true;
    unknown_player_a35db4->unknown_135 = true;
    unknown_player_a35db4->unknown_55e4 = 0;
    unknown_session_a3d298.Update_Progress(58);
    Service_Game_IO_00481fb0();
    if (Debug_Map) {
        Reset_Light_Converts_00529720();
    }
    unknown_session_a3d298.Update_Progress(60);
    Service_Game_IO_00481fb0();
    return true;
}


// bool ScenarioClass::Write_Basic(CCINIClass &ini, bool skip_player) (0x006625B0)
// Writes the scenario's [Basic] settings (the player, the movies, the carried
// money and the home cells only when not skip_player), its local variables
// ([VariableNames], "name,value" by index), the par times ([Ranking]) and the
// lighting levels ([Lighting], stored in hundredths or thousandths); always
// true. ScenarioClass is the scenario object (ScenarioClass in TS).
// The unit's header statics (0x00663DF0-0x00663FD1) come from these two.

bool ScenarioClass::Write_Basic(CCINIClass &ini, bool skip_player)
{
    ini.Clear(unknown_basic_section_7a9204);
    ini.Clear(unknown_basic_section_7a9204, "CivEvac");
    if (strlen(unknown_id_source_a3d290->Briefing) != 0) {
        ini.Put_String(unknown_basic_section_7a9204, "Briefing", unknown_id_source_a3d290->Briefing);
    }
    if (strlen(unknown_id_source_a3d290->UIName) != 0) {
        ini.Put_String(unknown_basic_section_7a9204, "UIName", unknown_id_source_a3d290->UIName);
    }
    Special().Write_INI(ini);
    ini.Put_String(unknown_basic_section_7a9204, "NextScenario", NextScenario);
    ini.Put_String(unknown_basic_section_7a9204, "AltNextScenario", AltNextScenario);
    ini.Put_Unicode(unknown_basic_section_7a9204, "Name", Name);
    ini.Put_Int(unknown_basic_section_7a9204, "NewINIFormat", 4);
    ini.Put_Int(unknown_basic_section_7a9204, "CarryOverCap", unknown_1208 / 100);
    ini.Put_Bool(unknown_basic_section_7a9204, "EndOfGame", EndOfGame);
    ini.Put_Bool(unknown_basic_section_7a9204, "SkipScore", SkipScore);
    ini.Put_Bool(unknown_basic_section_7a9204, "OneTimeOnly", OneTimeOnly);
    ini.Put_Bool(unknown_basic_section_7a9204, "SkipMapSelect", SkipMapSelect);
    ini.Put_Bool(unknown_basic_section_7a9204, "Official", true);
    ini.Put_Bool(unknown_basic_section_7a9204, "IgnoreGlobalAITriggers", unknown_2238);
    ini.Put_Bool(unknown_basic_section_7a9204, "TruckCrate", TruckCrate);
    ini.Put_Bool(unknown_basic_section_7a9204, "TrainCrate", unknown_2229);
    ini.Put_Int(unknown_basic_section_7a9204, "Percent", Percent);
    ini.Put_Techno_List(unknown_basic_section_7a9204, "AllowableUnits", AllowableUnits);
    ini.Put_Integer_List(unknown_basic_section_7a9204, "AllowableUnitMaximums", AllowableUnitMaximums);
    ini.Put_ThemeType(unknown_basic_section_7a9204, "Theme", Theme);

    if (!skip_player) {
        char player[32];
        strcpy(player, unknown_player_a35db4->IniName);
        ini.Put_String(unknown_basic_section_7a9204, "Player", player);
        ini.Put_Listed_Name(unknown_basic_section_7a9204, "Intro", IntroMovie);
        ini.Put_Listed_Name(unknown_basic_section_7a9204, "Brief", BriefMovie);
        ini.Put_Listed_Name(unknown_basic_section_7a9204, "Win", WinMovie);
        ini.Put_Listed_Name(unknown_basic_section_7a9204, "Lose", LoseMovie);
        ini.Put_Listed_Name(unknown_basic_section_7a9204, "Action", ActionMovie);
        ini.Put_Listed_Name(unknown_basic_section_7a9204, "PostScore", PostScoreMovie);
        ini.Put_Listed_Name(unknown_basic_section_7a9204, "PreMapSelect", PreMapSelectMovie);
        ini.Put_Double(unknown_basic_section_7a9204, "CarryOverMoney", CarryOverMoney);
        ini.Put_Bool(unknown_basic_section_7a9204, "TimerInherit", unknown_2231);
        ini.Put_Bool(unknown_basic_section_7a9204, "FillSilos", FillSilos);
        ini.Put_Int(unknown_basic_section_7a9204, "StartingDropships", StartingDropships);
        ini.Put_Int(unknown_basic_section_7a9204, "HomeCell", HomeCell);
        ini.Put_Int(unknown_basic_section_7a9204, "AltHomeCell", AltHomeCell);
    }
    ini.Put_Int(unknown_basic_section_7a9204, "MultiplayerOnly", MultiplayerOnly);
    ini.Put_Bool(unknown_basic_section_7a9204, "TiberiumGrowthEnabled", unknown_222a);
    ini.Put_Bool(unknown_basic_section_7a9204, "VeinGrowthEnabled", unknown_222b);
    ini.Put_Bool(unknown_basic_section_7a9204, "IceGrowthEnabled", unknown_222c);
    ini.Put_Bool(unknown_basic_section_7a9204, "TiberiumDeathToVisceroid", TiberiumDeathToVisceroid);
    ini.Put_Bool(unknown_basic_section_7a9204, "FreeRadar", FreeRadar);
    ini.Put_Int(unknown_basic_section_7a9204, "InitTime", InitTime);

    ini.Clear(unknown_variable_names_section_7a9200);
    for (int index = 0; index < 50; index++) {
        UnknownScenarioVariable_004B5AB0 &variable = LocalVariables[index];
        if (variable.unknown_00[0] != 0) {
            char entry[12];
            char value[128];
            wsprintfA(entry, "%d", index);
            wsprintfA(value, "%s,%d", variable.unknown_00, variable.Value != 0);
            ini.Put_String(unknown_variable_names_section_7a9200, entry, value);
        }
    }

    ini.Put_Time("Ranking", "ParTimeEasy", ParTimeEasy);
    ini.Put_Time("Ranking", "ParTimeMedium", ParTimeMedium);
    ini.Put_Time("Ranking", "ParTimeHard", ParTimeHard);
    ini.Put_String("Ranking", "UnderParTitle", UnderParTitle);
    ini.Put_String("Ranking", "UnderParMessage", UnderParMessage);
    ini.Put_String("Ranking", "OverParTitle", OverParTitle);
    ini.Put_String("Ranking", "OverParMessage", OverParMessage);

    ini.Clear("Lighting");
    double ambient = unknown_22a8 * 0.01;
    ini.Put_Double("Lighting", "Ambient", ambient);
    double red = AmbientRed * 0.01;
    ini.Put_Double("Lighting", "Red", red);
    double green = AmbientGreen * 0.01;
    ini.Put_Double("Lighting", "Green", green);
    double blue = AmbientBlue * 0.01;
    ini.Put_Double("Lighting", "Blue", blue);
    double ground = AmbientGround * 0.001;
    ini.Put_Double("Lighting", "Ground", ground);
    double level = AmbientLevel * 0.001;
    ini.Put_Double("Lighting", "Level", level);
    double ionambient = IonAmbient * 0.01;
    ini.Put_Double("Lighting", "IonAmbient", ionambient);
    double ionred = IonRed * 0.01;
    ini.Put_Double("Lighting", "IonRed", ionred);
    double iongreen = IonGreen * 0.01;
    ini.Put_Double("Lighting", "IonGreen", iongreen);
    double ionblue = IonBlue * 0.01;
    ini.Put_Double("Lighting", "IonBlue", ionblue);
    double ionground = IonGround * 0.001;
    ini.Put_Double("Lighting", "IonGround", ionground);
    double ionlevel = IonLevel * 0.001;
    ini.Put_Double("Lighting", "IonLevel", ionlevel);
    return true;
}


// int __fastcall CPU_Scaled_Delay_00662d60(int delay) (0x00662D60)
// Scales a delay to the machine: 80% on a Pentium Pro or later (CPU family
// above 5), then delay * 200 / the performance counter frequency in MHz
// (name hypothesis).

int __fastcall CPU_Scaled_Delay_00662d60(int delay)
{
    int family;
    unsigned char has_cpuid;
    int mhz = 0;
    Get_CPU_Info(&family, &has_cpuid, 0, 0);
    if (family > 5) {
        delay = delay * unknown_cpu_delay_scale_7a9208;
    }
    unsigned int high;
    unsigned int low = Get_CPU_Rate(high);
    if (low != 0 || high != 0) {
        mhz = (high * 4294967296.0 + low) * 0.000001;
    }
    return delay * unknown_cpu_delay_factor_7a9210 / mhz;
}


// The next object ID (0x00663170).
long ScenarioClass::Next_ID()
{
    return ++UniqueID;
}


// The cell of a waypoint (0x00663180).
Cell ScenarioClass::Get_Waypoint_Location(int waypoint) const
{
    return Waypoints[waypoint];
}


// The map cell of a waypoint (0x006631A0).
CellClass *ScenarioClass::unknown_6631a0(int waypoint)
{
    return &Map[Waypoints[waypoint]];
}


// The centre of a waypoint's cell, raised by the bridge height on a bridge
// cell (0x006631C0).

CoordStruct ScenarioClass::Waypoint_Coord(int waypoint) const
{
    CellClass &cell = Map[Waypoints[waypoint]];
    CoordStruct coord = cell.Center_Coord();
    if (cell.unknown_140 & 0x500) {
        coord.Z += unknown_geometry_j;
    }
    return coord;
}


// Whether a waypoint is in range and set (0x00663240).
bool ScenarioClass::Unknown_00663240(int waypoint) const
{
    if (waypoint < 101 && waypoint >= 0 && Waypoints[waypoint] != unknown_geometry_k) {
        return true;
    }
    return false;
}


// Reads [Waypoints] ("index=x+y*1000"; missing entries are the empty cell)
// and marks the cells of the set waypoints (0x00663280).

void ScenarioClass::Read_Waypoints(CCINIClass &ini)
{
    for (int index = 0; index < 101; index++) {
        char buffer[20];
        wsprintfA(buffer, "%d", index);
        int value = ini.Get_Int("Waypoints", buffer, 0);
        if (value == 0) {
            Waypoints[index] = unknown_geometry_k;
        } else {
            Waypoints[index] = Cell(value % 1000, value / 1000);
        }
        if (Unknown_00663240(index)) {
            Map[Waypoints[index]].unknown_140 |= 4;
        }
    }
}


// Rewrites [Waypoints] from the set waypoints (0x00663350).

void ScenarioClass::Write_Waypoints(CCINIClass &ini)
{
    ini.Clear(unknown_waypoints_section_7a9218);
    for (int index = 0; index < 101; index++) {
        if (Waypoints[index] != unknown_geometry_k) {
            char buffer[32];
            wsprintfA(buffer, "%d", index);
            ini.Put_Int(unknown_waypoints_section_7a9218, buffer, Waypoints[index].X + Waypoints[index].Y * 1000);
        }
    }
}


// 0x006633e0: function not yet identified
//
// Body: 20 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __thiscall Clear_Waypoint_006633e0(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006633e0 {

extern "C" unsigned int waypoint_none_00ab6e88;

struct Owner_006633e0 { void Clear_Waypoint_006633e0(int index); };

void Owner_006633e0::Clear_Waypoint_006633e0(int index)
{
    int self = (int)this;
    *(unsigned int *)(self + 0x632 + index * 4) = waypoint_none_00ab6e88;
    return;
}

}  // namespace seed_006633e0


// Places a waypoint (0x00663400).
void ScenarioClass::Set_Waypoint_Location(int waypoint, Cell cell)
{
    Waypoints[waypoint] = cell;
}


// 0x00663420: function not yet identified
//
// Body: 25 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __thiscall Get_Waypoint_CellClass_00663420(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00663420 {

struct Callee_00548070 { unsigned int Cell_At_00548070(short *); };
extern "C" unsigned int Map_008324e0;

struct Owner_00663420 { void Get_Waypoint_CellClass_00663420(int index); };

void Owner_00663420::Get_Waypoint_CellClass_00663420(int index)
{
    int self = (int)this;
    ((Callee_00548070*)(&Map_008324e0))->Cell_At_00548070((short *)(self + 0x632 + index * 4));
    return;
}

}  // namespace seed_00663420


// The name of the first waypoint that is not set ("" when all are;
// 0x00663440).

char const *ScenarioClass::Free_Waypoint_Name() const
{
    for (int index = 0; index < 101; index++) {
        if (!Unknown_00663240(index)) {
            return Waypoint_As_String(index);
        }
    }
    return unknown_string_83c55c;
}


// 0x00663490: function not yet identified
//
// Body: 79 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 unknown_player_value_00663490(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00663490 {

extern "C" unsigned int unknown_list_00a35d0c;
extern "C" unsigned int unknown_list_00a35d64;
extern "C" unsigned int Scen_00a3d290;
extern "C" unsigned int Session_00a3d298;
extern "C" unsigned int * unknown_lobby_players_00a3fa80;

unsigned int  unknown_player_value_00663490(void)
{
    unsigned int result;
    int index;

    result = (unsigned int)(0);
    if (Session_00a3d298 == 0) {
        if (*(int *)(Scen_00a3d290 + 0x224c) != -1) {
            return *(unsigned int *)(*(int *)(unknown_list_00a35d64 + *(int *)(Scen_00a3d290 + 0x224c) * 4) + 0x98);
        }
    }
    else {
        index = (int)(*(int *)(*unknown_lobby_players_00a3fa80 + 0x4b));
        if (index == -3) {
            index = (int)(0);
        }
        result = (unsigned int)(*(unsigned int *)(*(int *)(unknown_list_00a35d0c + index * 4) + 0xbc));
    }
    return result;
}

}  // namespace seed_00663490


// Original region: 0x006634e0 (93 body bytes, 3 independently frozen NOP bytes).
// Exact target SHA256: e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f.
// Purpose: initialize an inferred four-byte-element storage wrapper.
// Class/API identity and the three unknown field purposes remain unresolved.

extern "C" void* __cdecl allocate_storage_006634e0(unsigned int byte_count);
extern "C" unsigned char base_dispatch_006634e0[];
extern "C" unsigned char derived_dispatch_006634e0[];

class WordStorage_006634e0 {
    unsigned char* dispatch_table;
    unsigned int* storage;
    unsigned int capacity;
    unsigned char unknown_0x0c;
    unsigned char owns_storage;
    // Bytes 0x0e..0x0f are alignment padding and are not written.
    unsigned int unknown_0x10;
    unsigned int unknown_0x14;

public:
    WordStorage_006634e0* initialize(unsigned int requested_capacity,
                                unsigned int* supplied_storage);
};

typedef char WordStorage_006634e0_size_check[(sizeof(WordStorage_006634e0) == 24) ? 1 : -1];

WordStorage_006634e0* WordStorage_006634e0::initialize(
    unsigned int requested_capacity,
    unsigned int* supplied_storage)
{
    storage = 0;
    capacity = requested_capacity;
    unknown_0x0c = 1;
    owns_storage = 0;
    dispatch_table = base_dispatch_006634e0;

    // A zero capacity ignores even a nonnull supplied pointer.
    if (requested_capacity != 0) {
        if (supplied_storage != 0) {
            storage = supplied_storage;
        } else {
            // Preserve 32-bit wrapping and ownership even on a null result.
            storage = (unsigned int*)allocate_storage_006634e0(requested_capacity << 2);
            owns_storage = 1;
        }
    }

    dispatch_table = derived_dispatch_006634e0;
    unknown_0x14 = 10;
    unknown_0x10 = 0;
    return this;
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<TechnoTypeClass *>::Add (0x00663540): emitted by this unit's use of its list.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<Cell>::operator=(VectorClass<Cell> const &) (0x00663590).
// Replaces the contents with an owned copy (shared/vector_class.hpp); the copy kept out of
// line in the Scenario unit (Multiplayer_Start_Waypoints 0x0065FFF0 calls it).
