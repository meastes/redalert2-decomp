// bool __fastcall Receive_Game_Options_005c3ff0(char const *text, char const *sender) (0x005C3FF0)
// Decodes a received game options string by its first letter: Z (NAT state, port, no preview), O (the
// observer slot), A (a player value), C (cooperative campaign info), L (slot names), G, K (map state),
// P (pings); anything else is the host's full options string (seed, speed, money, units, flags, map,
// digest, the players' colours/countries/addresses and the AI slots).
#include "shared/lobby.hpp"
#include "shared/gdlg_supp.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/session_state.hpp"
#include "shared/multiplayer_game_mode.hpp"
#include "shared/mplayer_score_type.hpp"
#include "shared/vector_class.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/crt_string.hpp"
#include "shared/crt_memory.hpp"
#include "shared/string_table.hpp"
#include "shared/wstring.hpp"
#include "shared/wide_string.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/ini_class.hpp"
#include "shared/ipx_address.hpp"
#include "shared/multi_mission.hpp"
#include "shared/options_dialog.hpp"
#include "shared/id_source.hpp"
#include "shared/cd_timer.hpp"
#include "shared/debug_printf.hpp"
#include "shared/options.hpp"
// @ref __strdup=0078d816
// @ref ??_C@_01KFMA@?$EA?$AA@=007e6eec
// @ref ??_C@_02EHJP@A0?$AA@=007e6ed0
// @ref ??_C@_02HFAJ@K1?$AA@=007e6f20
// @ref ??_C@_02IKLA@K2?$AA@=007e6f24
// @ref ??_C@_02MAKK@?0?3?$AA@=007e6ef0
// @ref ??_C@_02NPJO@K0?$AA@=007e6f40
// @ref ??_C@_0BA@BMJO@GUI?3JoinerNoMap?$AA@=007e6f44
// @ref ??_C@_0BB@NEO@TXT_COLOR_IN_USE?$AA@=007e6ed8
// @ref ??_C@_0BD@KADM@TXT_NAT_NO_PREVIEW?$AA@=007e7054
// @ref ??_C@_0BG@CLKC@Not?5same?5scenario?4?4?4?6?$AA@=007e6f54
// @ref ??_C@_0BG@MAHE@GUI?3JoinerMapTransfer?$AA@=007e6f28
// @ref ??_C@_0BK@MGDO@Decoding?5game?5options?5?$CFs?6?$AA@=007e6f90
// @ref ??_C@_0CC@IAJJ@Got?5Ping?5game?5options?5string?4?5?$CFs@=007e6fac
// @ref ??_C@_0CH@LDN@Setting?5Campaign?5info?5for?5Mode?5?$CF@=007e6ff8
// @ref ??_C@_0CI@PIJC@Handling?5Campaign?5info?5for?5Mode?5@=007e6fd0
// @ref ??_C@_0DE@OIF@?$EA?$EA?$EA?$EA?5Decoding?5cooperative?5campai@=007e7020
// @ref ??_C@_1CK@DKKA@?$AAG?$AAa?$AAm?$AAe?$AAo?$AAp?$AAt?$AA?5?$AAp?$AAa?$AAr?$AAs?$AAe?$AA?5?$AAf?$AAa?$AAi?$AAl?$AAe?$AAd@=007e6ef4
// @ref ?Set_Slot_Country_Color_005bccf0@@YIHPBDHH@Z=005bccf0
// @ref ?Unknown_0073bdb0@@YA_NXZ=0073bdb0
// @ref ?Unknown_0073d150@@YIXABVWstring@@H@Z=0073d150
// @ref ?Wide_From_7_Bit@@YI?AVWideString@@ABVWstring@@@Z=007716d0
// @ref ?unknown_a72d08@@3HA=00a72d08
// @ref ?unknown_old_2a_a72d53@@3_NA=00a72d53
// @ref ?unknown_old_2b_a72d54@@3_NA=00a72d54
// @ref ?unknown_old_2c_a72d55@@3_NA=00a72d55
// @ref ?unknown_old_a8_a72d56@@3_NA=00a72d56
// @ref ?unknown_old_crates_a72d57@@3_NA=00a72d57
// @ref ?unknown_old_money_7e6ddc@@3HA=007e6ddc
// @ref ?unknown_old_speed_7e6dd8@@3HA=007e6dd8
// @ref ?unknown_old_units_7e6de0@@3HA=007e6de0
// @ref ?unknown_scenario_name_a3d342@@3PAGA=00a3d342
// @ref ?unknown_slot_a3fc34@@3PAHA=00a3fc34
// @ref ?unknown_slot_a40238@@3PAHA=00a40238
// @ref ?unknown_slot_addresses_a3fbd4@@3PAVIPXAddressClass@@A=00a3fbd4
// @ref ?unknown_b2abd9@@3_NA=00b2abd9
// @ref ?unknown_b2ac40@@3HA=00b2ac40
// @ref _atoi=00782dfd
// @ref _atol=00782d72

extern "C" char *__cdecl _strdup(char const *string);
extern "C" long __cdecl atol(char const *string);
extern "C" int __cdecl atoi(char const *string);
extern "C" int __cdecl sscanf(char const *string, char const *format, ...);
extern bool unknown_b2abd9;
extern int unknown_a72d08;
extern int unknown_b2ac40;
extern IPXAddressClass unknown_slot_addresses_a3fbd4[8];
extern int unknown_slot_a3fc34[8];
extern int unknown_slot_a40238[8];
extern int unknown_old_speed_7e6dd8;
extern int unknown_old_money_7e6ddc;
extern int unknown_old_units_7e6de0;
extern bool unknown_old_2a_a72d53;
extern bool unknown_old_2b_a72d54;
extern bool unknown_old_2c_a72d55;
extern bool unknown_old_a8_a72d56;
extern bool unknown_old_crates_a72d57;
extern wchar_t unknown_scenario_name_a3d342[];
bool Local_User_Is_Channel_Owner_0074d960();
void __fastcall Register_Connection_State_0074e1f0(char const *sender, char const *receiver, int state);
void __fastcall Store_Player_Port_0074e2d0(char const *name, unsigned short port);
void __fastcall Leave_Channel_With_Message_0074c5f0(HWND dialog);
void __fastcall Store_User_Ping_0075b110(char const *name, int ping);
void __fastcall Unknown_0073d150(Wstring const &name, int value);
int __fastcall Set_Channel_Player_Options_0073d550(Wstring const &name, int country, int color);
int __fastcall Unknown_0073d3f0(Wstring const &name);
WideString __fastcall Wide_From_7_Bit(Wstring const &string);
bool Unknown_0073bdb0();

extern wchar_t const unknown_empty_wide_839d4c[];   // NATIVE: declared for the draft
bool __fastcall Decode_Pub_Gameopt_005c3ff0(char const *text, char const *sender)   // NATIVE: the name callers declare (draft: Receive_Game_Options_005c3ff0)
{
    bool changed = false;
    char *copy = _strdup(text);
    if (copy[0] == 'Z') {
        switch (copy[1]) {
        case '1': {
            for (int i = 0; i < 8; i++) {
                if (_stricmp(sender, Wide_To_Narrow_006fa120(unknown_player_slots_a3fa98[i]->Name)) == 0) {
                    char receiver[64];
                    strcpy(receiver, copy + 3);
                    Register_Connection_State_0074e1f0(sender, receiver, copy[2] - '0');
                }
            }
            break;
        }
        case '2':
            Store_Player_Port_0074e2d0(sender, (unsigned short)atol(copy + 2));
            free(copy);
            return false;
        case '3':
            if (!Local_User_Is_Channel_Owner_0074d960()) {
                HWND dialog = WS_Find_Dialog_00735940(0xC9);
                HWND preview = GetDlgItem(dialog, 0x3F2);
                if (preview != 0) {
                    ShowWindow(preview, SW_SHOWNORMAL);
                    ShowWindow(GetDlgItem(dialog, 0x468), SW_HIDE);
                    SendMessageA(preview, 0x4B2, 0, (LPARAM)StringTable::Load_String("TXT_NAT_NO_PREVIEW", 0, "D:\\Ra2\\netshare.cpp", 0x4BE));
                }
            }
            break;
        }
        free(copy);
        return false;
    }
    if (unknown_game_mode_a3d298 == 4 && (unknown_b2abd9 || unknown_b2ac40 != 0)) {
        free(copy);
        return false;
    }
    if (copy[0] == 'O') {
        HWND dialog = WS_Find_Dialog_00735940(0xC9);
        if (dialog != 0 || (dialog = WS_Find_Dialog_00735940(0xBD)) != 0) {
            int old = unknown_lobby_flag_7e6d90;
            unknown_lobby_flag_7e6d90 = copy[1] == '8' ? -1 : copy[1] - '0';
            if (unknown_lobby_flag_7e6d90 != old) {
                Unknown_005cafd0(dialog);
                wchar_t name[256];
                wcscpy(name, Narrow_To_Wide_006fa180(copy + 2));
                if (name[0] != 0) {
                    NodeNameType *entry;
                    {
                        WideString wide(name);
                        entry = Unknown_Find_Lobby_Entry_005cd1b0(wide);
                    }
                    if (entry == 0) {
                        entry = Unknown_Find_Lobby_Name_005c8b20(name);
                        if (entry == 0) {
                            free(copy);
                            return false;
                        }
                    }
                    if (entry == unknown_player_a72d44
                        || (entry != 0 && entry != unknown_open_entry_a72d2c && entry != unknown_closed_entry_a72d30
                            && entry != unknown_waiting_entry_a72d40 && entry != unknown_easy_ai_entry_a72d34
                            && entry != unknown_medium_ai_entry_a72d38 && entry != unknown_hard_ai_entry_a72d3c
                            && entry->Team == -1)) {
                        unknown_player_slots_a3fa98[unknown_lobby_flag_7e6d90] = entry;
                        Fill_Observer_Combo_4d1640(dialog, Player_Side_Combo_ID_4d1350(unknown_lobby_flag_7e6d90));
                        SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(unknown_lobby_flag_7e6d90)), 0x4F1, 0, 1);
                        int id = Player_Color_Combo_ID_4d1cb0(unknown_lobby_flag_7e6d90);
                        Fill_Random_Color_Combo_4d2250(dialog, id);
                        Select_Color_Combo_4d2480(dialog, id, 8);
                    } else {
                        Fill_Side_Combo_4d1560(dialog, Player_Side_Combo_ID_4d1350(unknown_lobby_flag_7e6d90));
                        SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(unknown_lobby_flag_7e6d90)), 0x4F1, 0, 0);
                        int id = Player_Color_Combo_ID_4d1cb0(unknown_lobby_flag_7e6d90);
                        Fill_Color_Combo_4d2080(dialog, id);
                        Select_Color_Combo_4d2480(dialog, id, -2);
                    }
                    unknown_player_slots_a3fa98[unknown_lobby_flag_7e6d90] = entry;
                }
            }
            if (unknown_lobby_flag_7e6d90 >= 0 && unknown_lobby_flag_7e6d90 != unknown_lobby_a3fa94) {
                Unknown_005c9b90(dialog, unknown_lobby_flag_7e6d90, false);
            }
        }
        free(copy);
        return false;
    }
    if (copy[0] == 'A') {
        int value = atol(copy + 1);
        if (unknown_chat_a72cfc != 0) {
            Unknown_0073d150(Wstring(sender), value);
            free(copy);
            return false;
        }
        Set_Player_Value_005bcbc0(sender, value);
        free(copy);
        return false;
    }
    if (copy[0] == 'C') {
        if (copy[1] == '0') {
            Debug_Printf("@@@@ Decoding cooperative campaign game options %s\n", text);
            int index = -1;
            DynamicVectorClass<int> list;
            int time = 0;
            int unknown = 0;
            MPlayerScoreType scores[2];
            char *token = strtok(copy + 2, ",");
            if (token != 0) {
                index = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                time = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                unknown = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[1].Kills = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[1].Built = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[1].Lost = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[1].Score = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[0].Kills = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[0].Built = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[0].Lost = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[0].Score = atol(token);
            }
            for (int i = 0; i < 10; i++) {
                token = strtok(0, ",");
                if (token == 0) {
                    break;
                }
                int value = atol(token);
                if (value < 0) {
                    break;
                }
                list.Add(value);
            }
            char *first = strtok(0, ",");
            char *second = strtok(0, ",");
            for (int mode = 0; mode <= MultiplayerGameMode::unknown_mode_count_5b6f80(); mode++) {
                if (MultiplayerGameMode::unknown_find_mode_5b6fe0(mode) != 0) {
                    Debug_Printf("Setting Campaign info for Mode %d: %s\n", mode,
                        Wide_To_Narrow_006fa120(MultiplayerGameMode::unknown_find_mode_5b6fe0(mode)->unknown_name_20.unknown_text_770430()));
                    if (MultiplayerGameMode::unknown_find_mode_5b6fe0(mode)->unknown_slot18(index, list, first, second)) {
                        MultiplayerGameMode::unknown_find_mode_5b6fe0(mode)->unknown_slot19(time, unknown, scores);
                    }
                }
            }
        } else {
            Debug_Printf("@@@@ Decoding cooperative campaign game options %s\n", text);
            for (int mode = 0; mode <= MultiplayerGameMode::unknown_mode_count_5b6f80(); mode++) {
                if (MultiplayerGameMode::unknown_find_mode_5b6fe0(mode) != 0) {
                    Debug_Printf("Handling Campaign info for Mode %d: %s\n", mode,
                        Wide_To_Narrow_006fa120(MultiplayerGameMode::unknown_find_mode_5b6fe0(mode)->unknown_name_20.unknown_text_770430()));
                    MultiplayerGameMode::unknown_find_mode_5b6fe0(mode)->unknown_slot25(copy[1] - '0', copy + 2);
                }
            }
        }
        free(copy);
        return false;
    }
    if (copy[0] == 'L') {
        WideString names = Wstring_To_Wide(Wstring(text + 1));
        Set_Slot_Names_005c8dd0(names.Get());
        HWND dialog = WS_Find_Dialog_00735940(0xC9);
        if (dialog != 0) {
            int slot = Find_First_Player_Slot_005c8c70();
            if (slot == -1) {
                Leave_Channel_With_Message_0074c5f0(dialog);
            } else if (slot != unknown_lobby_a3fa94) {
                int id = Player_Side_Combo_ID_4d1350(slot);
                if (unknown_player_slots_a3fa98[slot]->Team == -1) {
                    Fill_Observer_Combo_4d1640(dialog, id);
                    SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(slot)), 0x4F1, 0, 1);
                    int color = Player_Color_Combo_ID_4d1cb0(slot);
                    Fill_Random_Color_Combo_4d2250(dialog, color);
                    Select_Color_Combo_4d2480(dialog, color, 8);
                } else {
                    id = Player_Side_Combo_ID_4d1350(slot);
                    // NATIVE (Scout1274): the original (0x005C4A13) fills the side combo only when a game mode is set and its slot 1 is false
                    if (unknown_session_a3d298.Mode != 0 && !unknown_session_a3d298.Mode->unknown_slot1()) {
                        Fill_Side_Combo_4d1560(dialog, id);
                    }
                    SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(slot)), 0x4F1, 0, 0);
                    int color = Player_Color_Combo_ID_4d1cb0(slot);
                    Fill_Color_Combo_4d2080(dialog, color);
                    Select_Color_Combo_4d2480(dialog, color, -2);
                }
                slot = Find_First_Player_Slot_005c8c70();
                unknown_lobby_a3fa94 = slot;
                for (int i = 0; i < 8; i++) {
                    EnableWindow(GetDlgItem(dialog, Slot_Control_Id_005c97a0(i)), FALSE);
                    Unknown_005c9b90(dialog, i, false);
                }
                EnableWindow(GetDlgItem(dialog, Slot_Control_Id_005c97a0(slot)), TRUE);
                Unknown_005c9b90(dialog, slot, true);
                Select_Player_Side_4d1820(dialog, unknown_lobby_a3fa94);
                Select_Player_Color_4d23c0(dialog, unknown_lobby_a3fa94);
            }
        }
        free(copy);
        return false;
    }
    if (copy[0] == 'G') {
        if (WS_Find_Dialog_00735940(0xC9) != 0) {
            unknown_lobby_flag_a3fab8 = true;
            if (unknown_chat_a72cfc != 0) {
                Unknown_0075c980();
                free(copy);
                return false;
            }
            Unknown_005c28e0();
        }
        free(copy);
        return false;
    }
    if (copy[0] == 'K') {
        if (WS_Find_Dialog_00735940(0xC2) != 0 || WS_Find_Dialog_00735940(0xBC) != 0) {
            int state;
            switch (copy[1]) {
            case '1':
                state = 1;
                break;
            case '2':
                state = 2;
                break;
            default:
                state = 0;
                break;
            }
            Set_Player_Map_State_005c3f00(sender, state);
        }
        free(copy);
        return false;
    }
    if (copy[0] == 'P') {
        Debug_Printf("Got Ping game options string. %s\n", copy);
        char const *p = copy + 3;
        if (unknown_game_mode_a3d298 == 4) {
            while (*p != 0) {
                char name[64];
                char number[68];
                int i = 0;
                while (*p != 0 && *p != ',') {
                    name[i++] = *p++;
                }
                name[i] = 0;
                i = 0;
                if (*p != 0) {
                    p++;
                }
                while (*p != 0 && *p != ',') {
                    number[i++] = *p++;
                }
                number[i] = 0;
                int ping = atol(number);
                if (*p != 0) {
                    p++;
                }
                Store_User_Ping_0075b110(name, ping);
            }
        }
        free(copy);
        return false;
    }

    int dialog_id = Current_Dialog_Id_00735b20();
    if (dialog_id != 0xBC && dialog_id != 0xBD) {
        SendDlgItemMessageA(Find_Lobby_Window_005c2c40(), 0x53E, 0x49D, 0, 1);
    }
    Debug_Printf("Decoding game options %s\n", text);
    char *token = strtok(copy, ",");
    if (token != 0) {
        unknown_a40d3c = atol(token);
    }
    token = strtok(0, ",");
    if (token != 0) {
        *(int *)&Options = atol(token);
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.GameSpeed_30 = atol(token);
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_24 = atol(token);
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.UnitCount = atol(token);
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_2a = atol(token) != 0;
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_2b = atol(token) != 0;
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_2c = atol(token) != 0;
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_a8 = atoi(token) != 0;
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_crates_29 = atoi(token) != 0;
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.GameModeIndex = atol(token);
    }
    bool same = true;
    token = strtok(0, ",");
    WideString scenario_name(unknown_game_mode_a3d298 == 4 && Unknown_0073bdb0()
        ? WideString(unknown_empty_wide_839d4c) : Wide_From_7_Bit(Wstring(token)));
    int old_count = unknown_session_a3d298.unknown_884;
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_884 = atoi(token);
    }
    bool old_official = unknown_session_a3d298.ScenarioIsOfficial_880;
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.ScenarioIsOfficial_880 = atoi(token) != 0;
    }
    int old_length = unknown_session_a3d298.ScenarioFileLength_87c;
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.ScenarioFileLength_87c = atoi(token);
    }
    token = strtok(0, ",");
    if (token != 0) {
        if (_stricmp(unknown_session_a3d298.ScenarioFileName_658, token) != 0) {
            same = false;
        }
        strncpy(unknown_session_a3d298.ScenarioFileName_658, token, 0x202);
        strcpy(unknown_id_source_a3d290->FileName, unknown_session_a3d298.ScenarioFileName_658);
    }
    char *digest = strtok(0, ":");
    if (digest != 0) {
        if (strcmp(unknown_session_a3d298.ScenarioDigest_85a, digest) != 0) {
            same = false;
        }
        strncpy(unknown_session_a3d298.ScenarioDigest_85a, digest, 0x21);
    }
    MultiplayerGameMode *mode = MultiplayerGameMode::unknown_find_mode_5b6fe0(unknown_session_a3d298.GameModeIndex);
    if (dialog_id == 0xBD || dialog_id == 0xC9) {
        Change_Slot_Count_005cb300(Find_Lobby_Window_005c2c40(), old_count, unknown_session_a3d298.unknown_884,
            (UnknownSessionA3D29C *)mode);
    }
    bool found = true;
    if (!same || wcslen(unknown_scenario_name_a3d342) == 0) {
        found = false;
        same = false;
        for (int i = 0; i < unknown_session_a3d298.Scenarios.Count(); i++) {
            MultiMission *scenario = unknown_session_a3d298.Scenarios[i];
            if (_stricmp(unknown_session_a3d298.ScenarioFileName_658, scenario->unknown_58) == 0) {
                CCFileClass file(scenario->unknown_58);
                if (file.Is_Available(false) && file.Size() == unknown_session_a3d298.ScenarioFileLength_87c) {
                    INIClass ini;
                    ini.Load(file, false);
                    char buffer[32];
                    ini.Get_String("Digest", "1", "No digest here mate. Nope.", buffer, sizeof(buffer));
                    if (unknown_session_a3d298.ScenarioDigest_85a[0] == 0
                        || strcmp(unknown_session_a3d298.ScenarioDigest_85a, buffer) == 0) {
                        wcscpy(unknown_scenario_name_a3d342, scenario->Description);
                        found = true;
                    }
                }
            }
        }
        if (!found && scenario_name.Length() > 0) {
            wcscpy(unknown_scenario_name_a3d342, scenario_name.Get());
        }
        if (unknown_session_a3d298.Mode != 0) {
            unknown_session_a3d298.Mode->unknown_slot9();
        }
    }
    mode = MultiplayerGameMode::unknown_find_mode_5b6fe0(unknown_session_a3d298.GameModeIndex);
    if (mode != unknown_session_a3d298.Mode) {
        if (unknown_session_a3d298.Mode != 0) {
            unknown_session_a3d298.Mode->unknown_slot33();
        }
        if (mode != 0) {
            mode->unknown_slot8();
        }
    }
    unknown_session_a3d298.Mode = mode;
    if (mode != 0 && !same) {
        mode->UnfixAlliances();
    }
    HWND dialog = Find_Lobby_Window_005c2c40();
    SendMessageA(GetDlgItem(dialog, 0x6EC), 0x4B2, 0, (LPARAM)mode->unknown_name_20.unknown_text_770430());
    SendMessageA(GetDlgItem(dialog, 0x5A8), 0x4B2, 0, (LPARAM)unknown_scenario_name_a3d342);
    unknown_id_source_a3d290->TechLevel = -1;
    Frame = 0;
    unknown_session_a3d2ac = 2;
    if (old_official != unknown_session_a3d298.ScenarioIsOfficial_880
        || old_length != unknown_session_a3d298.ScenarioFileLength_87c) {
        same = false;
    }
    if (!same) {
        Debug_Printf("Not same scenario...\n");
        if (unknown_session_a3d298.ScenarioIsOfficial_880 && !found
            && _stricmp(unknown_session_a3d298.ScenarioFileName_658, "RandMap.Sed") != 0) {
            if (Find_Lobby_Window_005c2c40() != 0) {
                Unknown_005C2C80(-1, StringTable::Load_String("GUI:JoinerNoMap", 0, "D:\\Ra2\\netshare.cpp", 0x71A),
                    unknown_scenario_name_a3d342);
            }
            unknown_a72d52 = false;
            EnableWindow(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), FALSE);
            InvalidateRect(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), 0, FALSE);
            Send_Game_Options_005c3c60("K0");
        } else {
            unknown_a72d52 = true;
            EnableWindow(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), TRUE);
            InvalidateRect(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), 0, FALSE);
            if (!unknown_session_a3d298.ScenarioIsOfficial_880 && !found
                && _stricmp(unknown_session_a3d298.ScenarioFileName_658, "RandMap.Sed") != 0) {
                if (Find_Lobby_Window_005c2c40() != 0) {
                    Unknown_005C2C80(-1, StringTable::Load_String("GUI:JoinerMapTransfer", 0, "D:\\Ra2\\netshare.cpp", 0x72A),
                        unknown_scenario_name_a3d342);
                }
                Send_Game_Options_005c3c60("K2");
            } else {
                Send_Game_Options_005c3c60("K1");
            }
        }
        Unknown_005c7570(Find_Lobby_Window_005c2c40());
    }
    if (digest == 0) {
        Unknown_005C2C80(-1, L"Gameopt parse failed");
    }
    {
        unsigned int address = 0;
        for (char *name = strtok(0, ",:"); name != 0; name = strtok(0, ",:")) {
            if (strcmp(name, "@") == 0) {
                break;
            }
            token = strtok(0, ",:");
            if (token == 0) {
                break;
            }
            int country = atol(token);
            token = strtok(0, ",:");
            if (token == 0) {
                break;
            }
            int color = atol(token);
            token = strtok(0, ",:");
            if (token == 0) {
                break;
            }
            sscanf(token, "%X", &address);
            int i = 8;
            if (address != 0 && unknown_game_mode_a3d298 == 4) {
                for (i = 0; i < 8; i++) {
                    if (_stricmp(name, Wide_To_Narrow_006fa120(unknown_player_slots_a3fa98[i]->Name)) == 0) {
                        NetNumType net;
                        NetNodeType node;
                        memset(node, 0, sizeof(node));
                        memcpy(node, &address, sizeof(address));
                        memset(net, 0, sizeof(net));
                        unknown_slot_addresses_a3fbd4[i].Set_Address(net, node, 0);
                        token = strtok(0, ",:");
                        if (token != 0) {
                            int value;
                            sscanf(token, "%X", &value);
                            unknown_slot_a3fc34[i] = value;
                            token = strtok(0, ",:");
                            if (token != 0) {
                                int value2;
                                sscanf(token, "%X", &value2);
                                unknown_slot_a40238[i] = value2;
                            }
                        }
                        break;
                    }
                }
            }
            if (i == 8) {
                strtok(0, ",:");
                strtok(0, ",:");
            }
            unknown_a72d08 = 1;
            if (_stricmp(name, unknown_session_a3d298.Handle) == 0 && color != unknown_session_a3d298.unknown_12c) {
                if (unknown_a72d08 == 0) {
                    Unknown_005C2C80(-1, StringTable::Load_String("TXT_COLOR_IN_USE", 0, "D:\\Ra2\\netshare.cpp", 0x7A2));
                }
                unknown_session_a3d298.unknown_12c = color;
            }
            if (_stricmp(name, unknown_session_a3d298.Handle) == 0) {
                unknown_a72d08 = 0;
            }
            if (unknown_chat_a72cfc != 0 && Set_Channel_Player_Options_0073d550(Wstring(name), country, color) != 0) {
                changed = true;
            } else if (unknown_chat_a72cfc == 0 && Set_Slot_Country_Color_005bccf0(name, country, color) != 0) {
                changed = true;
            }
            address = 0;
        }
    }
    {
        char before[64];
        before[0] = 0;
        for (int i = 0; i < 8; i++) {
            char number[12];
            sprintf(number, "%d,", unknown_session_a3d298.AICountry[i]);
            strcat(before, number);
        }
        int count = 0;
        for (int j = 0; j < 8; j++) {
            token = strtok(0, ",:");
            if (token == 0) {
                break;
            }
            unknown_session_a3d298.AIDifficulty[j] = atol(token);
            token = strtok(0, ",:");
            if (token == 0) {
                break;
            }
            unknown_session_a3d298.AICountry[j] = atol(token);
            token = strtok(0, ",:");
            if (token == 0) {
                break;
            }
            unknown_session_a3d298.AIColor[j] = atol(token);
            if (unknown_session_a3d298.AICountry[j] != -1) {
                count++;
            }
        }
        unknown_session_a3d298.AICount = count;
        char after[64];
        after[0] = 0;
        for (int k = 0; k < 8; k++) {
            char number[12];
            sprintf(number, "%d,", unknown_session_a3d298.AICountry[k]);
            strcat(after, number);
        }
        if (strcmp(before, after) != 0) {
            changed = true;
        }
    }
    free(copy);
    if (unknown_game_mode_a3d298 != 4 || !Unknown_0073bdb0()) {
        Unknown_Fill_Options_Dialog_005c3550(Find_Lobby_Window_005c2c40(), 0);
    }
    if (unknown_old_speed_7e6dd8 != unknown_session_a3d298.GameSpeed_30) {
        changed = true;
    }
    if (unknown_old_money_7e6ddc != unknown_session_a3d298.unknown_24) {
        changed = true;
    }
    if (unknown_old_units_7e6de0 != unknown_session_a3d298.UnitCount) {
        changed = true;
    }
    if (unknown_old_2a_a72d53 != unknown_session_a3d298.unknown_2a) {
        changed = true;
    }
    if (unknown_old_2b_a72d54 != unknown_session_a3d298.unknown_2b) {
        changed = true;
    }
    if (unknown_old_2c_a72d55 != unknown_session_a3d298.unknown_2c) {
        changed = true;
    }
    if (unknown_old_a8_a72d56 != unknown_session_a3d298.unknown_a8) {
        changed = true;
    }
    if (unknown_old_crates_a72d57 != unknown_session_a3d298.unknown_crates_29) {
        changed = true;
    }
    if (!same || changed) {
        int result;
        if (unknown_chat_a72cfc != 0) {
            result = Unknown_0073d3f0(Wstring(0));
        } else {
            result = Unknown_Find_Player_By_Name_005bcc60(0);
        }
        unknown_lobby_flag_a3fab8 = false;
        if (unknown_chat_a72cfc != 0) {
            Unknown_0075ca00();
        } else {
            Unknown_005c2940();
        }
        if (result == 1) {
            if (unknown_chat_a72cfc != 0) {
                Unknown_0073d150(Wstring(0), 0);
            } else {
                Set_Player_Value_005bcbc0(0, 0);
            }
            char buffer[64];
            sprintf(buffer, "A0");
            Send_Game_Options_005c3c60(buffer);
            if (unknown_a72d52) {
                EnableWindow(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), TRUE);
            } else {
                EnableWindow(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), FALSE);
            }
            InvalidateRect(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), 0, FALSE);
        } else if (!IsWindowEnabled(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F)) && unknown_a72d52) {
            EnableWindow(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), TRUE);
            InvalidateRect(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), 0, FALSE);
        }
    }
    if (dialog_id != 0xBC && dialog_id != 0xBD) {
        SendDlgItemMessageA(Find_Lobby_Window_005c2c40(), 0x53E, 0x49D, 0, 0);
    }
    if (unknown_chat_a72cfc != 0) {
        unknown_call_73fee0_default();
    } else {
        Unknown_005bbdc0();
    }
    unknown_old_speed_7e6dd8 = unknown_session_a3d298.GameSpeed_30;
    unknown_old_money_7e6ddc = unknown_session_a3d298.unknown_24;
    unknown_old_units_7e6de0 = unknown_session_a3d298.UnitCount;
    unknown_old_2a_a72d53 = unknown_session_a3d298.unknown_2a;
    unknown_old_2b_a72d54 = unknown_session_a3d298.unknown_2b;
    unknown_old_2c_a72d55 = unknown_session_a3d298.unknown_2c;
    unknown_old_a8_a72d56 = unknown_session_a3d298.unknown_a8;
    unknown_old_crates_a72d57 = unknown_session_a3d298.unknown_crates_29;
    return !same;
}
