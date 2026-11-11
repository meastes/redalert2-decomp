// NON-EXACT DRAFT 0x5C69E0 Lobby_Dialog_Proc (the Choose Map dialog's procedure)
// entry 0x5C69E0; exact-match score 93.62 (Scout1005 HEAD rescore of codex3's 94.87 form)
// origin lane: Scout1005 (build/class-work/Scout1005/keep/h5c69e0/best-9362-head.cpp), worker codex3
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1133 (native/drafts.json, M16). Not original bytes.
// @ref ??0?$DynamicVectorClass@PAVMultiMission@@@@QAE@IPBQAVMultiMission@@@Z=005cd770
// @ref ?Add@?$DynamicVectorClass@PAVMultiMission@@@@QAE_NABQAVMultiMission@@@Z=005cd880
// @ref ??0?$VectorClass@PAVMultiMission@@@@QAE@IPBQAVMultiMission@@@Z=005cdaf0
// The unit-local container tables are identified independently by MSVC RTTI.
// @ref ??_7?$VectorClass@PAVMultiMission@@@@6B@=007a7388
// @ref ??_7?$DynamicVectorClass@PAVMultiMission@@@@6B@=007a73a8
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// Adapted from TibSun code/netshare.cpp Scenario_DlgProc; RA2 mode-selection behavior from original CFG.
// EA GPLv3 Section 7 additional terms apply; see THIRD_PARTY.md.
#include "shared/lobby.hpp"
#include "shared/multi_mission.hpp"
#include "shared/multiplayer_game_mode.hpp"
#include "shared/map_preview.hpp"
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/session_state.hpp"
extern bool unknown_map_list_busy_a72d58;
extern bool unknown_map_mode_warning_a72d59;
// @ref ?unknown_map_list_busy_a72d58@@3_NA=00a72d58
// @ref ?unknown_map_mode_warning_a72d59@@3_NA=00a72d59
bool __fastcall End_Dialog_007357e0(HWND dialog, int result);
int __fastcall Make_Random_Map_005c84e0();
bool __fastcall Accept_Map_Selection_005c71f0(HWND dialog);
int __fastcall unknown_select_list_item_by_data_005c7160(HWND dialog, intptr_t data);   // NATIVE: pointer item data (port/fixups.json)
void __fastcall unknown_dialog_5e5f20(HWND dialog);
namespace OwnerDraw {
void __fastcall unknown_draw_dialog_back_5fee70(HWND window);
void __fastcall unknown_5ff380(unsigned int key);
}

// Gather missions separately from replacing the control's visible list.
static inline void Collect_Compatible_Maps(MultiplayerGameMode *mode, DynamicVectorClass<MultiMission *> &maps)
{
    int count = unknown_multi_missions_a3d8d8.Count();
    for (int index = 0; index < count; ++index) {
        MultiMission *map = unknown_multi_missions_a3d8d8[index];
        if (mode->unknown_5b7470(map)) maps.Add(map);
    }
}

// Rebuild the visible map list from the missions accepted by the selected mode.
static inline void Populate_Compatible_Map_List(HWND window, MultiplayerGameMode *mode)
{
    DynamicVectorClass<MultiMission *> maps;
    Collect_Compatible_Maps(mode, maps);
    HWND list = GetDlgItem(window, 0x553);
    mode->DrawTeamDropdown(list, &maps, 0);
}

// Refill the maps and update the random-map control for the new mode.
static inline void Refresh_Mode_Map_Controls(HWND window, MultiplayerGameMode *mode)
{
                    {
                        DynamicVectorClass<MultiMission *> maps;
                        int count = unknown_multi_missions_a3d8d8.Count();
                        for (int index = 0; index < count; ++index) {
                            MultiMission *map = unknown_multi_missions_a3d8d8[index];
                            if (mode->unknown_5b7470(map)) maps.Add(map);
                        }
                        HWND list = GetDlgItem(window, 0x553);
                        mode->DrawTeamDropdown(list, &maps, 0);
                    }
                    bool can_random = mode->IsAIAllowed();
                    HWND random_button = GetDlgItem(window, 0x583);
                    EnableWindow(random_button, can_random);
}

// Validation and refresh when the game-mode list changes.
static inline void Change_Lobby_Game_Mode(HWND window, unsigned wparam, wchar_t const *selected_name)
{
            if (!unknown_map_list_busy_a72d58 && (unsigned short)(wparam >> 16) == 1) {
                int selected = SendDlgItemMessageA(window, 0x6eb, 0x188, 0, 0);
                if (selected == -1) return;
                MultiplayerGameMode *mode = (MultiplayerGameMode *)SendDlgItemMessageA(window, 0x6eb, 0x199, selected, 0);
                if (!mode) return;
                WideString reason;
                if (mode->unknown_slot3(&reason)) {
                    mode->AssignTeams();
                    Refresh_Mode_Map_Controls(window, mode);
                    if (selected_name) {
                    int found = 0;
                    int count = SendDlgItemMessageA(window, 0x553, 0x18b, 0, 0);
                    for (int i = 0; i < count; ++i) {
                        wchar_t const *name = (wchar_t const *)SendDlgItemMessageA(window, 0x553, 0x199, i, 0);
                        if (Compare_Wide_007837d3(name, selected_name) == 0) found = i;
                    }
                    SendDlgItemMessageA(window, 0x553, 0x186, found, 0);
                    SendDlgItemMessageA(window, 0x553, 0x197, found, 0);
                    }
                    HWND dialog = unknown_lobby_dialog_a728e0;
                    selected = Selected_Campaign_005c67a0();
                    wchar_t const *name;
                    if (selected != -1 && (name = (wchar_t const *)SendDlgItemMessageA(dialog, 0x553, 0x199, selected, 0)) != 0) wcscpy(unknown_lobby_map_name_a72a68, name);
                    else unknown_lobby_map_name_a72a68[0] = 0;
                    unknown_map_mode_warning_a72d59 = false;
                } else {
                    if (!unknown_map_mode_warning_a72d59) {
                        Message_Box_005c3230(reason.Get(), 0, 0, false);
                        unknown_map_mode_warning_a72d59 = true;
                    }
                    SendDlgItemMessageA(window, 0x6eb, 0x186, 0, 0);
                }
            }
}

long __stdcall Lobby_Dialog_Proc_005c69e0(HWND window, unsigned message, unsigned wparam, long lparam)
{
    switch (message) {
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0xf:
        OwnerDraw::unknown_draw_dialog_back_5fee70(window);
        if (unknown_dialog_preview_a72ce8) unknown_dialog_preview_a72ce8->Draw_0061D460(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window, 0);
        return 0;
    case 0x14:
        return 1;
    case 0x82:
        OwnerDraw::unknown_5ff380((unsigned int)window);
        return 0;
    case 0x111:
        switch ((unsigned short)wparam) {
        case 0x6c5:
            Accept_Map_Selection_005c71f0(window);
            return 0;
        case 0x583:
            {
                OwnerDraw::unknown_5e5fc0(window);
                ShowWindow(window, 0);
                int index = Make_Random_Map_005c84e0();
                if (index != -1) {
                    int selected = SendDlgItemMessageA(window, 0x6eb, 0x188, 0, 0);
                    if (selected != -1) {
                        MultiplayerGameMode *mode = (MultiplayerGameMode *)SendDlgItemMessageA(window, 0x6eb, 0x199, selected, 0);
                        if (mode) {
                            Populate_Compatible_Map_List(window, mode);
                        }
                    }
                    if (index < unknown_multi_missions_a3d8d8.Count()) unknown_select_list_item_by_data_005c7160(window, (intptr_t)unknown_multi_missions_a3d8d8[index]);
                    Select_Scenario_005c7c30(index);
                    if (!unknown_dialog_preview_a72ce8->unknown_00) Unknown_005c7570(window);
                    unknown_session_a3d2b4 = unknown_lobby_map_a72c80;
                    Select_Scenario_005c7c30(unknown_session_a3d2b4);
                    bool accepted = Accept_Map_Selection_005c71f0(window);
                    unknown_5159b0(0, true);
                    if (accepted) return 0;
                }
                OwnerDraw::unknown_show_dialog_5e61b0(window);
                ShowWindow(window, 5);
            }
            return 0;
        case 0x5c0:
            End_Dialog_007357e0(window, 2);
            return 0;
        case 0x6eb:
            Change_Lobby_Game_Mode(window, wparam, unknown_lobby_map_name_a72a68);
            return 0;
        }
        return 0;
    case 0x497:
        {
            unknown_map_list_busy_a72d58 = true;
            MultiplayerGameMode *mode = MultiplayerGameMode::unknown_find_mode_5b6fe0(unknown_session_a3d298.GameModeIndex);
            if (!mode) mode = MultiplayerGameMode::unknown_first_mode_5b6ec0();
            HWND mode_list = GetDlgItem(window, 0x6eb);
            MultiplayerGameMode::unknown_fill_list_5b71e0(mode_list, mode->Get_ID());
            if (mode) {
                Populate_Compatible_Map_List(window, mode);
            }
            bool can_random = mode->IsAIAllowed();
            HWND random_button = GetDlgItem(window, 0x583);
                    EnableWindow(random_button, can_random);
            if (unknown_session_a3d2b4 != -1 && unknown_session_a3d2b4 < unknown_multi_missions_a3d8d8.Count()) {
                MultiMission *map = unknown_multi_missions_a3d8d8[unknown_session_a3d2b4];
                if (map) {
                    int found = 0;
                    int count = SendDlgItemMessageA(window, 0x553, 0x18b, 0, 0);
                    for (int i = 0; i < count; ++i) {
                        if ((MultiMission *)SendDlgItemMessageA(window, 0x553, 0x199, i, 0) == map) found = i;
                    }
                    SendDlgItemMessageA(window, 0x553, 0x186, found, 0);
                    SendDlgItemMessageA(window, 0x553, 0x197, found, 0);
                }
            }
            unknown_lobby_map_a72c80 = unknown_session_a3d2b4;
            unknown_lobby_map_a728d0 = -1;
            unknown_map_list_busy_a72d58 = false;
        }
        return 0;
    case 0x4e9:
        {
            OwnerDraw::UnknownTextRequest *request = (OwnerDraw::UnknownTextRequest *)lparam;
            int control = GetDlgCtrlID((HWND)request->unknown_00);
            request->text = (unsigned short const *)0;
            if (control == 0x6eb && request->unknown_04 != -1) {
                MultiplayerGameMode *mode = (MultiplayerGameMode *)SendDlgItemMessageA(window, 0x6eb, 0x199, request->unknown_04, 0);
                if (mode) request->text = mode->unknown_name_24.unknown_text_770430();
            }
        }
        return 1;
    case 0x4ec:
        Unknown_005e8710(window);
        return 0;
    }
    return 0;
}
