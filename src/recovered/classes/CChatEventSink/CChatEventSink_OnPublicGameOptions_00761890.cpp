#define RA2_CDTIMER_BASIC_BASE
#define RA2_BASIC_TIMER_VALUE
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Shape from TibSun wonline.cpp Handle_Public_Game_Options / OnPublicGameOptions.
// HRESULT __stdcall CChatEventSink::OnPublicGameOptions(HRESULT result, WOL::Channel *channel, WOL::User *user, char const *options) (0x00761890)
// Applies a game options text the host broadcasts. Online it answers "A1" when the host is known and the
// options start with a digit; otherwise it asks the host for a map preview when the scenario changed, when
// no preview arrived after 900 ticks, or when the scenario file is missing.
#include "shared/wol_sink.hpp"
#include "shared/session_state.hpp"
#include "shared/lobby.hpp"
#include "shared/map_preview.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/ini_class.hpp"
#include "shared/t_timer.hpp"
#include "shared/vector_class.hpp"
extern bool unknown_b2ac46;               // 0x00B2AC46
void Unknown_00750f60();                   // 0x00750F60
bool __fastcall Unknown_005c3ff0(char *options, char const *name);   // 0x005C3FF0
// @ref ??_C@_02EHJP@A0?$AA@=007e6ed0
// @ref ??_C@_02ONAI@A1?$AA@=007e6d74
// @ref ??_C@_03JMHL@P?$CFs?$AA@=007fdf40
// @ref ??_C@_0CH@NNIJ@Never?5recieved?5preview?$DL?5re?9reque@=007fdf44
// @ref ??_C@_0CJ@JFJN@New?5scenario?5?9?5requesting?5a?5map?5@=007fdf6c
// @ref ??_C@_0DP@DEPM@Sent?5private?5game?5option?5packet?5@=007fdf00
// @ref ?Unknown_005c3ff0@@YI_NPADPBD@Z=005c3ff0
// @ref ?Unknown_00750f60@@YAXXZ=00750f60
// @ref ?unknown_game_channel_b299c0@@3UChannel@WOL@@A=00b299c0
// @ref ?unknown_b2ac46@@3_NA=00b2ac46
// @ref _?$S8@?1??OnPublicGameOptions@CChatEventSink@@UAGJJPAUChannel@WOL@@PAUUser@4@PBD@Z@4EA=00b2b400
// @ref _?preview_timer@?1??OnPublicGameOptions@CChatEventSink@@UAGJJPAUChannel@WOL@@PAUUser@4@PBD@Z@4V?$BasicTimerClass@VSystemTimerClass@@@@A=00b2b408

// The game channel tests (0x0073C3C0, 0x0073C340, 0x0073C370; accepted out of line in the wonline unit).
inline bool __fastcall Unknown_0073c3c0(WOL::Channel const *channel)
{
    if (unknown_session_a3d298.Type != 4) return false;
    return (channel->unknown_20 >> 8) & 1;
}
inline bool __fastcall Unknown_0073c340(WOL::Channel const *channel)
{
    if (unknown_session_a3d298.Type != 4) return false;
    return channel->unknown_14 || Unknown_0073c3c0(channel);
}
inline bool Unknown_0073c370()
{
    return Unknown_0073c340(&unknown_game_channel_b299c0);
}

HRESULT __stdcall CChatEventSink::OnPublicGameOptions(HRESULT result, WOL::Channel *channel, WOL::User *user, char const *options)
{
    static BasicTimerClass<SystemTimerClass> preview_timer;
    if (result < 0) return S_OK;
    char *copy = _strdup(options);
    bool changed = Unknown_005c3ff0(copy, user->name);
    free(copy);
    if (unknown_b2abd9 || unknown_b2ac40 || *options == 'Z') return S_OK;
    bool request = false;
    WOL::User *host = Unknown_0074DA60();
    if (host && !_stricmp(host->name, unknown_wstring_b26cb8.Peek_Buffer())) return S_OK;
    if (Is_Online_Session()) {
        WideString name = Wstring_To_Wide(unknown_wstring_b26cb8);
        NodeNameType *node = Unknown_Find_Lobby_Name_005c8b20(name.Get());
        if (!node) return S_OK;
        if (node->unknown_66e290() < 0 || node->unknown_66e2a0() < 0 || *options < '0' || *options > '9') return S_OK;
        if (unknown_users_b2a728.Count == 2) unknown_chat_a72cfc->unknown_slot15("A1");
        return S_OK;
    }
    HWND dialog = Unknown_00735B10();
    if (!unknown_dialog_preview_a72ce8) {
        if (changed) {
            request = true;
            preview_timer = 0;
            Debug_Printf("New scenario - requesting a map preview\n");
        }
        if (preview_timer > 900 && (!unknown_dialog_preview_a72ce8 || !unknown_dialog_preview_a72ce8->Is_Present()) && !unknown_b2ac46) {
            if (unknown_session_a3d298.ScenarioFileName_658[0] && !Find_Local_Scenario_0066c5b0(unknown_session_a3d298.ScenarioFileName_658,
                    unknown_session_a3d298.ScenarioFileLength_87c, unknown_session_a3d298.ScenarioDigest_85a,
                    unknown_session_a3d298.ScenarioIsOfficial_880, false)) {
                request = true;
                Debug_Printf("Never recieved preview; re-requesting\n");
            } else {
                preview_timer = 0;
            }
        }
        if (!unknown_session_a3d298.Unknown_00672140()) {
            bool found = false;
            if (Find_Local_Scenario_0066c5b0(unknown_session_a3d298.ScenarioFileName_658, unknown_session_a3d298.ScenarioFileLength_87c,
                    unknown_session_a3d298.ScenarioDigest_85a, unknown_session_a3d298.ScenarioIsOfficial_880, false)) {
                {
                    RawFileClass file(unknown_session_a3d298.ScenarioFileName_658);
                    if (file.Is_Available()) found = true;
                }
                if (!found) {
                    request = false;
                    goto checked;
                }
            }
            if (Unknown_0073c370()) {
                NodeNameType *self = unknown_players_a3fa7c[0];
                if (self->unknown_67) {
                    self->unknown_67 = 0;
                    Send_Game_Options_005c3c60("A0");
                    EnableWindow(GetDlgItem(dialog, 0x59F), FALSE);
                }
            }
        checked:
            if (Current_Dialog_Id_00735b20() == 0xC9) InvalidateRect(Unknown_00735B10(), 0, FALSE);
        }
    } else {
        HWND item = GetDlgItem(dialog, 0x3F2);
        if (item) ShowWindow(item, SW_HIDE);
        ShowWindow(GetDlgItem(dialog, 0x468), SW_SHOWNORMAL);
    }
    if (request) {
        char text[64];
        sprintf(text, "P%s", unknown_session_a3d298.ScenarioFileName_658);
        WOL::User target;
        unknown_users_b2a728.Get_Copy(target, 0);
        unknown_chat_a72cfc->unknown_slot14(&target, text);
        Debug_Printf("Sent private game option packet requesting a preview download\n");
        Unknown_00750f60();
        return S_OK;
    }
    return S_OK;
}
