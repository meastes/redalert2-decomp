// bool __fastcall Select_Game(bool fade) -- 0x005135F0
// Body 0x005135F0-0x005146F4 (code) plus its five jump tables 0x005146F4-0x00514794 (40 entries):
// 4516 bytes, padding to 0x005147A0. Callers 0x00481D03 and 0x00481EDA (Main_Game) pass the fade flag in cl.
// The main menu loop (Red Alert's Select_Game): resets the per-game globals, runs the main menu and its
// sub-dialogs until a game is chosen, then starts the scenario (retrying from the top on failure).
// Its campaign picker and cheat reset are file statics that /Ob2 inlines, in the shape of ra2-decomp
// Choose_Campaign and Cheat_Disable beside Select_Game 0x0052D9A0 (YR fuzzy twin, src/init.cpp):
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Draft by codex4 (init_select_game_current, 99.92); helpers split out by Scout855.
// @ref ??_C@_05KMPM@INTRO?$AA@=007dbc90
// @ref ??_C@_06IFDN@GUI?3Ok?$AA@=007dbc48
// @ref ??_C@_0L@EJFF@GUI?3Cancel?$AA@=007d1604
// @ref ??_C@_0BA@PBEJ@IsTSpread?5?$DN?5?$CFd?6?$AA@=007dbb80
// @ref ??_C@_0BA@PLDN@IsTGrowth?5?$DN?5?$CFd?6?$AA@=007dbb90
// @ref ??_C@_0BD@LPLL@GUI?3ExitAreYouSure?$AA@=007dbc50
// @ref ??_C@_0BK@GHM@Init?4cpp?5?9?5GAME_INTERNET?6?$AA@=007dbc1c
// @ref ??_C@_0CB@HHBL@About?5to?5load?5a?5?$CFd?5player?5game?4?6@=007dbbcc
// @ref ??_C@_0CK@BGOB@Return?5from?5WOL?5to?5play?5a?5?$CFd?5pla@=007dbbf0
// @ref ??_C@_0N@DAHN@EMPRTRLR?4BIK?$AA@=007dbc38
// @ref ?Audio_Event_Handle_Clear@@YIXPAUAudioEventHandle@@@Z=00405bf0
// @ref ?Main_Options_Dialog@@YAXXZ=00542460
// @ref ?Save_Recording_Values@@YI_NAAVCCFileClass@@@Z=00516b00
// @ref ?Still_Playing@ThemeClass@@QBE_NXZ=006e82d0
// @ref ?Unknown_00513290@@YGHPAUHWND__@@IIJ@Z=00513290
// @ref ?Unknown_00514EE0@@YI_NXZ=00514ee0
// @ref ?Unknown_00516F00@@YIHH@Z=00516f00
// @ref ?Unknown_005A02D0@@YIXPBDHHHHH@Z=005a02d0
// @ref ?Unknown_005BD070@@YI_NXZ=005bd070
// @ref ?Unknown_00613E50@@YIXXZ=00613e50
// @ref ?Unknown_0066F3A0@@YIXPAUUnknownSessionState@@@Z=0066f3a0
// @ref ?Unknown_00683BF0@@YI_NXZ=00683bf0
// @ref ?Unknown_00737080@@YIXPAUHWND__@@0@Z=00737080
// @ref ?Unknown_0073A300@@YIH_N@Z=0073a300
// @ref ?unknown_7db55c@@3HA=007db55c
// @ref ?unknown_a3d23c@@3HA=00a3d23c
// @ref ?unknown_a3d258@@3HA=00a3d258
// @ref ?unknown_a3d570@@3HA=00a3d570
// @ref ?unknown_a3d574@@3HA=00a3d574
// @ref ?unknown_a41698@@3HA=00a41698
// @ref ?unknown_names_a71050@@3V?$DynamicVectorClass@PBD@@A=00a71050
// @ref ?unknown_cheat_codes_7db500@@3PAUUnknownCheatCode@@A=007db500
// @ref ?unknown_flag_7ce008@@3_NA=007ce008
// @ref ?unknown_flag_a35db0@@3DA=00a35db0
// @ref ?unknown_flag_a35db1@@3DA=00a35db1
// @ref ?unknown_flag_a40c78@@3DA=00a40c78
// @ref ?unknown_flag_a40d10@@3_NA=00a40d10
// @ref ?unknown_palette_837510@@3VPaletteClass@@A=00837510
#include "shared/win32_api.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/main_opt.hpp"
#include "shared/theme.hpp"
#include "shared/id_source.hpp"
#include "shared/session_state.hpp"
#include "shared/queue.hpp"
#include "shared/campaign_score.hpp"
#include "shared/win32_window.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/tool_tip.hpp"
#include "shared/keyboard.hpp"
#include "shared/mouse.hpp"
#include "shared/dsurface.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/ccini_class.hpp"
#include "shared/special.hpp"
#include "shared/options.hpp"
#include "shared/campaign.hpp"
#include "shared/cd.hpp"
#include "shared/string_table.hpp"
#include "shared/ww_message_box.hpp"
#include "shared/sound_play.hpp"
#include "shared/voc_class.hpp"
#include "shared/audio.hpp"
#include "shared/vox_class.hpp"
#include "shared/comm_buffer.hpp"
#include "shared/t_timer.hpp"
#include "shared/system_timer.hpp"
#include "shared/scenario_checkpoint.hpp"
#include "shared/map_seed.hpp"
#include "shared/modem_guest.hpp"
#include "shared/modem_host.hpp"
#include "shared/serial_progress.hpp"
#include "shared/null_modem.hpp"
#include "shared/ipx_address.hpp"
#include "shared/ipx_interface.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/connection.hpp"
#include "shared/owner_talk.hpp"
#include "shared/lobby.hpp"
#include "shared/movie_globals.hpp"
#include "shared/debug_printf.hpp"
#include "shared/movie_list.hpp"
#include "shared/campaign_choice.hpp"
#include "shared/unknown_globals.hpp"

#define Max(a, b) ((a) < (b) ? (b) : (a))

// The cheat codes the main menu types into (0x007DB500; 0x00516F00 matches them): a code whose
// unknown_c is clear has its typed text emptied before a multiplayer game.
#include "shared/init_cheat_codes.hpp"
// What the movie dialog (0x005134C0) returns: a movie's file name and the CD it is on.
struct UnknownMovieEntry {
    char const *Name;       // +0x00
    int unknown_4;          // +0x04
    int CD;                 // +0x08
};
extern char unknown_flag_a35db0;            // 0x00A35DB0
extern char unknown_flag_a35db1;            // 0x00A35DB1
extern char unknown_flag_a40c78;            // 0x00A40C78
extern bool unknown_flag_00866250;          // 0x00866250
extern int unknown_833680;                  // 0x00833680
extern int unknown_833684;                  // 0x00833684
extern int unknown_833688;                  // 0x00833688
extern int unknown_a3d570;                  // 0x00A3D570
extern int unknown_a3d574;                  // 0x00A3D574
extern int unknown_a3d258;                  // 0x00A3D258
extern int unknown_a3d23c;                  // 0x00A3D23C
extern bool unknown_flag_a40d10;            // 0x00A40D10
extern int unknown_a41698;                  // 0x00A41698
extern int unknown_7db55c;                  // 0x007DB55C
extern bool unknown_flag_7ce008;            // 0x007CE008
extern PaletteClass unknown_palette_837510; // 0x00837510
extern bool unknown_redraw_8523dc;

int __fastcall Unknown_00516F00(int timeout);
bool __fastcall Unknown_00514EE0();
int __stdcall Unknown_00513290(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
int __stdcall Unknown_005133E0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
int __stdcall Unknown_005134C0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
void __fastcall Read_Battle_Files();
bool __fastcall Load_Recording_Values(CCFileClass &file);
bool __fastcall Save_Recording_Values(CCFileClass &file);
void __fastcall Init_Random();
void __fastcall Unknown_005A02D0(char const *name, int theme, int unknown3, int unknown4, int unknown5, int unknown6);
void __fastcall Unknown_006F2C40();
void __fastcall Unknown_006F2D00();
bool __fastcall Unknown_00683BF0();
int __fastcall Unknown_0073A300(bool unknown1);
bool __fastcall Unknown_005BD070();
void __fastcall Unknown_00613E50();

void __fastcall Audio_Event_Handle_Clear(AudioEventHandle *handle);

static void Cheat_Disable()
{
    for (int code = 0; code < 4; code++) {
        if (!unknown_cheat_codes_7db500[code].AllowedInMultiplayer) {
            *unknown_cheat_codes_7db500[code].Flag = false;
        }
    }
}

static int Choose_Campaign()
{
    UnknownCampaignChoice choice;
    choice.Chosen = false;
    choice.Campaign = -1;
    if (Campaigns.Count() == 0) {
        Read_Battle_Files();
        if (Campaigns.Count() == 0) {
            return -1;
        }
    }
    Unknown_006F2C40();
    Audio_Event_Handle_Clear(&unknown_side_sound_a411e8);
    unknown_hover_time_a41680 = 0;
    unknown_hover_id_a41688 = 0;
    HWND window = unknown_create_dialog_5ff610(0x94, Unknown_005147A0, 0);
    if (window != 0) {
        SetWindowLongA(window, 8, (LONG)&choice);
        Unknown_00737080(window, MainWindow);
        unknown_dialog_5ff7c0(window);
        while (!choice.Chosen && unknown_6000d0() != true) {
            if (unknown_side_voc_7db4f8 != -1 && (unsigned __int64)unknown_hover_time_a41680 < (unsigned __int64)Audio_Now()) {
                Audio_Event_Handle_Stop(&unknown_side_sound_a411e8);
                Voc_Play(unknown_side_voc_7db4f8, 1.0f, 0x2000, &unknown_side_sound_a411e8);
                unknown_side_voc_7db4f8 = -1;
            }
            if (unknown_redraw_8523dc) {
                unknown_redraw_8523dc = false;
                unknown_5159b0(0, true);
                Unknown_004E0C90(false, unknown_surface_839954, 0);
            }
        }
        unknown_dialog_5ff6e0(window);
    }
    unknown_hover_time_a41680 = (unsigned __int64)Audio_Now() + 3000;
    while (Audio_Event_Handle_Event(&unknown_side_sound_a411e8) != 0 && (unsigned __int64)unknown_hover_time_a41680 > (unsigned __int64)Audio_Now()) {
        unknown_481fb0();
    }
    Unknown_006F2D00();
    return choice.Campaign;
}

bool __fastcall Select_Game(bool fade)
{
    Theme.Play_Song((ThemeType)Theme.From_Name("INTRO"));
top:
    {
        bool gameloaded = false;
        int selection;
        bool process = true;

        unknown_flag_a40958 = true;
        unknown_player_a35db4 = 0;
        DoList.Init();
        OutList.Init();
        Frame = 0;
        unknown_id_source_a3d290->MissionTimer = 0;
        unknown_id_source_a3d290->MissionTimer.Stop();
        unknown_id_source_a3d290->unknown_610 = 1;
        unknown_id_source_a3d290->Difficulty = 1;
        unknown_flag_a35db1 = 0;
        unknown_flag_a40c78 = 0;
        unknown_flag_00866250 = false;
        unknown_flag_a35db0 = 0;
        unknown_session_a3d298.unknown_2b0 = 0;
        Map.Unknown_00498F10(0);
        unknown_833680 = 0;
        unknown_833684 = 0;
        unknown_833688 = -1;
        unknown_a3d570 = 0;
        unknown_a3d574 = 0;
        unknown_a3d568 = 30;
        unknown_a3d258 = 0;
        unknown_a3d23c = 0;
        for (int i = 0; i < 4; i++) {
            unknown_session_a3d298.unknown_1fb4[i][unknown_session_a3d298.unknown_2300] = -1;
        }
        Map.Set_Default_Mouse(0, false);
        if (ToolTips != 0) {
            ToolTips->Enable(false);
        }
        if (unknown_session_a3d298.Type == 0) {
            selection = 0x12;
        } else {
            selection = (unknown_session_a3d298.Type == 4) ? 0x11 : 0x10;
        }
        if (!unknown_a40d13) {
            if (unknown_session_a3d298.Play && unknown_session_a3d298.RecordFile.Is_Available()) {
                if (unknown_session_a3d298.RecordFile.Open(1)) {
                    Load_Recording_Values(unknown_session_a3d298.RecordFile);
                    process = false;
                    Theme.Stop(true);
                } else {
                    unknown_session_a3d298.Play = false;
                }
            }
            while (process) {
                if (PacketTransport != 0) {
                    if (unknown_session_a3d298.ViaPacketRouter) {
                        if (unknown_connection_a409c4 != 0) {
                            unknown_OwnerTalkClass_00ab6d18.Unknown_0063FC90();
                        }
                        unknown_session_a3d298.ViaPacketRouter = false;
                    }
                    delete PacketTransport;
                    PacketTransport = 0;
                    Unknown_00514EE0();
                }
                if (selection == 0x12 && !unknown_flag_a40d05) {
                    selection = Unknown_00516F00(3600);
                }
                if (unknown_flag_a40d05) {
                    selection = 8;
                }
                if (selection != 7) {
                    Theme.Play_Song((ThemeType)Theme.From_Name("INTRO"));
                }
                unknown_481fb0();
                switch (selection) {
                case -1:
                    if (unknown_session_a3d298.Attract && unknown_session_a3d298.RecordFile.Is_Available()) {
                        unknown_session_a3d298.Play = true;
                        if (unknown_session_a3d298.RecordFile.Open(1)) {
                            Load_Recording_Values(unknown_session_a3d298.RecordFile);
                            process = false;
                            Theme.Stop(true);
                            break;
                        }
                        unknown_session_a3d298.Play = false;
                        selection = 0x12;
                    } else {
                        selection = 0x12;
                    }
                    break;
                case 1:
                    unknown_a72c68 = 0;
                    selection = unknown_dialog_box_5eab60(0x100, Unknown_00513290, 1);
                    break;
                case 2:
                    selection = 0x10;
                    unknown_session_a3d298.Type = 4;
                    IPXAddressClass::Set_Protocol(2);
                    break;
                case 3:
                    selection = 0x10;
                    unknown_session_a3d298.Type = 3;
                    IPXAddressClass::Set_Protocol(1);
                    break;
                case 4:
                    selection = unknown_dialog_box_5eab60(0x101, Unknown_005133E0, 1);
                    break;
                case 5:
                    selection = 0x12;
                    Main_Options_Dialog();
                    break;
                case 6: {
                    unsigned short const *text = StringTable::Load_String("GUI:ExitAreYouSure", 0, "D:\\Ra2\\Init.CPP", 0x96B);
                    unsigned short const *ok = StringTable::Load_String("GUI:Ok", 0, "D:\\Ra2\\Init.CPP", 0x96C);
                    unsigned short const *cancel = StringTable::Load_String("GUI:Cancel", 0, "D:\\Ra2\\Init.CPP", 0x96D);
                    unknown_5159b0(0, true);
                    if (WWMessageBox(L" ").Process(text, ok, cancel, 0, false) == 0) {
                        selection = 7;
                        Options.Save_Settings();
                    } else {
                        unknown_5159b0(0, true);
                        selection = 0x12;
                    }
                    break;
                }
                case 7: {
                    Theme.Stop(true);
                    long timeout = TickCount.Time() + 3000;
                    while (Theme.Still_Playing() && Speech_Is_Playing() && timeout > TickCount.Time()) {
                        unknown_481fb0();
                    }
                    Theme.Stop(false);
                    if (!unknown_flag_7ce008) {
                        Unknown_004955D0(&unknown_palette_837510, 30, 0);
                    }
                    return false;
                }
                case 13:
                    selection = 4;
                    if (unknown_flag_a40d10) {
                        if (unknown_a41698 <= 0) {
                            unknown_a41698 = unknown_names_a71050.Count();
                        }
                        if (unknown_a41698 == unknown_names_a71050.Count()) {
                            unknown_a41698--;
                        }
                        Unknown_005A07F0(unknown_a41698--, -1, 1, 1, 1);
                        unknown_5159b0(0, true);
                    } else {
                        Unknown_005A02D0("EMPRTRLR.BIK", -1, 1, 1, 1, 0);
                        unknown_5159b0(0, true);
                    }
                    break;
                case 14: {
                    UnknownMovieEntry *movie = (UnknownMovieEntry *)unknown_dialog_box_5eab60(0x129, Unknown_005134C0, 1);
                    if (movie != (UnknownMovieEntry *)-1 && movie != 0) {
                        if (!CD().Force_Available(movie->CD)) {
                            break;
                        }
                        Unknown_005A02D0(movie->Name, -1, 1, 1, 1, 0);
                        unknown_5159b0(0, true);
                    } else {
                        selection = 4;
                    }
                    break;
                }
                case 15:
                    selection = 4;
                    Unknown_004B37C0();
                    Theme.Queue_Song((ThemeType)Theme.From_Name("INTRO"));
                    unknown_5159b0(0, true);
                    break;
                case 8: {
                    new (&unknown_scenario_checkpoint_00855a90) ScenarioCheckpoint_004B5A70;
                    unknown_id_source_a3d290->unknown_224c = Choose_Campaign();
                    if (unknown_id_source_a3d290->unknown_224c == -1) {
                        process = true;
                        selection = 1;
                        break;
                    }
                    switch (unknown_difficulty_a40b1c) {
                    case 0:
                        unknown_id_source_a3d290->unknown_610 = 2;
                        unknown_id_source_a3d290->Difficulty = 0;
                        break;
                    case 1:
                        unknown_id_source_a3d290->unknown_610 = 2;
                        unknown_id_source_a3d290->Difficulty = 1;
                        break;
                    case 2:
                        unknown_id_source_a3d290->unknown_610 = 1;
                        unknown_id_source_a3d290->Difficulty = 1;
                        break;
                    case 3:
                        unknown_id_source_a3d290->unknown_610 = 0;
                        unknown_id_source_a3d290->Difficulty = 1;
                        break;
                    case 4:
                        unknown_id_source_a3d290->unknown_610 = 0;
                        unknown_id_source_a3d290->Difficulty = 2;
                        break;
                    }
                    unknown_session_a3d298.Type = 0;
                    process = false;
                    break;
                }
                case 9:
                    unknown_a40774++;
                    Read_Battle_Files();
                    unknown_a40774--;
                    if (LoadOptionsClass().Load_Dialog_0053b190()) {
                        gameloaded = true;
                        process = false;
                    } else {
                        selection = 1;
                    }
                    break;
                case 10:
                    selection = 1;
                    break;
                case 11:
                    unknown_session_a3d298.Type = 5;
                case 16:
                case 17: {
                    Unknown_0066F3A0(&unknown_session_a3d298);
                    for (int house = 0; house < HouseTypes.Count(); house++) {
                        HouseTypes[house]->Read_INI(*unknown_ini_8396d0);
                    }
                    unknown_counter_a3fabc = 0;
                    switch (unknown_session_a3d298.Type) {
                    case 0:
                        selection = 0x12;
                        continue;
                    case 5:
                        if (!Unknown_00683BF0()) {
                            unknown_session_a3d298.Type = 0;
                            selection = 1;
                        }
                        break;
                    case 1:
                    case 2: {
                        Cheat_Disable();
                        unknown_session_a3f616 = true;
                        if (serial_manager_00a40a28.Num_Connections()) {
                            serial_manager_00a40a28.Init_Send_Queue();
                            ModemGuest guest;
                            if ((unknown_session_a3d298.Type == 2 && serial_local_player_00a3fb38 == 0) || (unknown_session_a3d298.Type == 1 && serial_local_player_00a3fb38 == 2)) {
                                if (!ModemHost().Dialog_00598e10()) {
                                    unknown_session_a3d298.Type = 0;
                                    selection = 0x12;
                                }
                            } else {
                                if (!guest.Dialog_00596010()) {
                                    unknown_session_a3d298.Type = 0;
                                    selection = 0x12;
                                }
                            }
                        } else {
                            selection = 0x12;
                            unknown_session_a3d298.Type = 0;
                            continue;
                        }
                        break;
                    }
                    case 4: {
                        Cheat_Disable();
                        Debug_Printf("Init.cpp - GAME_INTERNET\n");
                        memset(&unknown_session_a3d298.unknown_299c, 0, sizeof(unknown_session_a3d298.unknown_299c));
                        unknown_session_a3d298.unknown_3018 = 0;
                        bool reset = Unknown_00514EE0();
                        int result = Unknown_0073A300(selection == 0x11);
                        selection = 0x10;
                        unknown_session_a3f616 = false;
                        switch (result) {
                        case -2:
                        case 0:
                            delete PacketTransport;
                            PacketTransport = 0;
                            selection = 0x12;
                            unknown_session_a3d298.Type = 0;
                            Theme.Play_Song((ThemeType)Theme.From_Name("INTRO"));
                            break;
                        case -1:
                            Theme.Stop(true);
                            if (!unknown_flag_7ce008) {
                                Unknown_004955D0(&unknown_palette_837510, 30, 0);
                            }
                            delete PacketTransport;
                            PacketTransport = 0;
                            return false;
                        case 1:
                            Debug_Printf("Return from WOL to play a %d player game\n", unknown_session_a3d298.Players.Count());
                            if (reset) {
                                process = false;
                                unknown_7db55c = 2;
                            } else {
                                unknown_session_a3d298.Type = 0;
                                selection = 0x12;
                                delete PacketTransport;
                                PacketTransport = 0;
                                unknown_7db55c = -1;
                            }
                            break;
                        case 2:
                            delete PacketTransport;
                            PacketTransport = 0;
                            unknown_session_a3d298.Type = 0;
                            selection = 11;
                            break;
                        }
                        break;
                    }
                    }
                    switch (unknown_session_a3d298.Type) {
                    case 3: {
                        Cheat_Disable();
                        Unknown_0066F3A0(&unknown_session_a3d298);
                        if (PacketTransport != 0) {
                            delete PacketTransport;
                            PacketTransport = 0;
                        }
                        for (int house = 0; house < HouseTypes.Count(); house++) {
                            HouseTypes[house]->Read_INI(*unknown_ini_8396d0);
                        }
                        unknown_session_a3d298.Type = 3;
                        unknown_session_a3d2ac = 2;
                        if (PacketTransport == 0) {
                            PacketTransport = new IPXInterfaceClass;
                            PacketTransport->Init();
                            unknown_5159b0(0, true);
                            if (Init_Network_005bc460() && Unknown_005BD070()) {
                                process = false;
                            } else {
                                unknown_session_a3d298.Type = 0;
                                selection = 0x12;
                                delete PacketTransport;
                                PacketTransport = 0;
                            }
                        }
                        break;
                    }
                    case 1:
                    case 2:
                    case 4:
                    case 5:
                        process = false;
                        break;
                    }
                    break;
                }
                }
            }
        }
        WWKeyboard->Clear();
        Init_Random();
        Unknown_00613E50();
        if (!gameloaded && unknown_a3d8c8 == 0) {
            Debug_Printf("About to load a %d player game.\n", unknown_session_a3d298.Players.Count());
            WWMouse->Hide_Mouse();
            if (selection != 8) {
                if (!unknown_flag_7ce008) {
                    Unknown_004955D0(&unknown_palette_837510, 15, unknown_481fb0);
                }
                unknown_surface_839954->Fill(0);
                Unknown_004E0C90(true, unknown_surface_839954, 0);
            }
            WWMouse->Show_Mouse();
            if (unknown_session_a3d298.Type == 4) {
                for (int player = 0; player < 8; player++) {
                    unknown_session_a3d298.unknown_2ff4[player] = TickCount.Time();
                }
            }
            if (unknown_session_a3d298.Type == 0 && !unknown_flag_a40d05 && !unknown_session_a3d298.Play) {
                if (!Start_Scenario_0065bc70(Campaigns[unknown_id_source_a3d290->unknown_224c]->Scenario, true, unknown_id_source_a3d290->unknown_224c)) {
                    if (unknown_a40d13) {
                        return false;
                    }
                    goto top;
                }
            } else if (!Start_Scenario_0065bc70(unknown_id_source_a3d290->FileName, true, -1)) {
                if (unknown_a40d13) {
                    return false;
                }
                goto top;
            }
            if (unknown_session_a3d298.Record) {
                if (unknown_session_a3d298.RecordFile.Open(2)) {
                    Save_Recording_Values(unknown_session_a3d298.RecordFile);
                } else {
                    unknown_session_a3d298.Record = false;
                }
            }
            unknown_special_a40920.IsFromInstall = false;
        }
        Debug_Printf("IsTGrowth = %d\n", unknown_special_a40920.TiberiumGrows);
        Debug_Printf("IsTSpread = %d\n", unknown_special_a40920.TiberiumSpreads);
        if (unknown_session_a3d298.Type != 0 && unknown_session_a3d298.Type != 5 && !unknown_session_a3d298.Play) {
            unknown_session_a3d298.Create_Connections();
            if (unknown_session_a3d298.Type == 3) {
                unknown_ipx_manager_a40978.Set_Timing(Max(unknown_ipx_manager_a40978.unknown_526c30() + 2, 15), -1, 600, true);
                unknown_ipx_manager_a40978.unknown_slot14(Max(unknown_ipx_manager_a40978.unknown_526c30() + 2, 60), -1, 600);
            } else if (unknown_session_a3d298.Type == 4 && unknown_a3d580 == 0) {
                unknown_ipx_manager_a40978.Set_Timing(Max(unknown_ipx_manager_a40978.unknown_526c30() + 2, 60), -1, 600, true);
            }
        }
        unknown_481fb0();
        WWMouse->Hide_Mouse();
        if (!unknown_flag_7ce008) {
            Unknown_004955D0(&unknown_palette_837510, 15, unknown_481fb0);
        }
        unknown_surface_839954->Fill(0);
        Unknown_004E0C90(true, unknown_surface_839954, 0);
        WWMouse->Show_Mouse();
        unknown_surface_83995c = unknown_surface_839954;
        Map.Override_Mouse_Shape(0x13, false);
        Map.Revert_Mouse_Shape();
        if (!unknown_a40d13) {
            Map.unknown_slot54(1);
        }
        Map.Flag_To_Redraw(false);
        unknown_481fb0();
        WWMouse->Hide_Mouse();
        return true;
    }
}

#undef Max
