// mpcoop.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mpcoop.json carries the addresses and the rows that
// stay in their own files.


#include "shared/multiplayer_game_mode.hpp"
#include "shared/coop_campaign.hpp"
#include "shared/unknown_description_list.hpp"
#include "shared/house_type.hpp"
#include "shared/session_state.hpp"
#include "shared/multi_mission.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/string_table.hpp"
#include "shared/lobby.hpp"
#include "shared/abstract_type.hpp"
#include "shared/debug_printf.hpp"
#include "shared/mplayer_score_type.hpp"
#include "shared/gdlg_supp.hpp"
#include "shared/id_source.hpp"
#include "shared/crt_string.hpp"
#include "shared/coord.hpp"
#include "shared/coord_distance.hpp"
#include "shared/house.hpp"
#include "shared/crt_memory.hpp"
#include "shared/rules.hpp"
#include "shared/uicmnds.hpp"
#include "shared/gadget.hpp"
#include "shared/display.hpp"
#include "shared/map_view.hpp"
#include "shared/campaign_score.hpp"
#include "shared/win32_api.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/wol_sink.hpp"
#include "shared/network_statistics.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/connection.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/owner_talk.hpp"
#include "shared/options.hpp"
#include "shared/tool_tip.hpp"
#include "shared/gscreen.hpp"
#include "shared/event.hpp"
#include "shared/wstring.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/ipx_conn.hpp"


// MPCooperative::MPCooperative(name, description, ini_name, unknown_4, unknown_5, id) -- 0x005A2960
// The base mode with unknown_5 false; makes the campaign records and one CoopCampaignClass per record.

MPCooperative::MPCooperative(WideString const &name, WideString const &description, Wstring const &ini_name,
    Wstring const &unknown_4, bool unknown_5, int id)
    : MultiplayerGameMode(name, description, ini_name, unknown_4, false, id)
{
    unknown_campaigns_40 = new UnknownDescriptionList_00490E00();
    unknown_campaign_44 = 0;
    unknown_48 = 0;
    unknown_340 = 0;
    unknown_score_54 = 0;
    unknown_04 = false;
    unknown_4c = false;
    unknown_4d = false;
    unknown_50 = 0;
    unknown_344 = true;
    for (int i = unknown_campaigns_328.Count() - 1; i >= 0; i--) {
        delete unknown_campaigns_328[i];
    }
    DynamicVectorClass<CoopCampaignClass *> &campaigns = unknown_campaigns_328;
    int length = campaigns.Length();
    campaigns.Clear();
    campaigns.Resize(length);
    int count;
    unknown_campaigns_40->Get_Count_00490C90(&count);
    for (i = 0; i < count; i++) {
        CoopCampaignClass *campaign = new CoopCampaignClass();
        campaign->Load_Campaign_0048f590(i);
        unknown_campaigns_328.Add(campaign);
    }
}


// bool MPCooperative::unknown_slot7(int country) -- 0x005A2CD0

bool MPCooperative::unknown_slot7(int country)
{
    if (unknown_campaigns_40 == 0) {
        return true;
    }
    UnknownDescriptionRecord_00490E00 *record;
    unknown_campaigns_40->Get_Record_00490C50(unknown_340, &record);
    if (record == 0) {
        return true;
    }
    bool found = false;
    if (country == -2) {
        if (record->Players.Count() > 1) {
            return true;
        }
    } else if (country == -3) {
        return true;
    }
    for (int i = 0; i < record->Players.Count(); i++) {
        if (Find_Country_Index_004904A0(record->Players[i]) == country) {
            found = true;
            break;
        }
    }
    return found;
}


// bool MPCooperative::AssignTeams() -- 0x005A2D70
// Rebuilds the campaign list: deletes the old campaigns and loads one per campaign record.

bool MPCooperative::AssignTeams()
{
    int i;
    for (i = unknown_campaigns_328.Count() - 1; i >= 0; i--) {
        CoopCampaignClass *campaign = unknown_campaigns_328[i];
        if (campaign != 0) {
            delete campaign;
        }
    }
    int capacity = unknown_campaigns_328.Length();
    unknown_campaigns_328.Clear();
    unknown_campaigns_328.Resize(capacity, 0);
    int count;
    unknown_campaigns_40->Get_Count_00490C90(&count);
    for (i = 0; i < count; i++) {
        CoopCampaignClass *campaign = new CoopCampaignClass();
        campaign->Load_Campaign_0048f590(i);
        unknown_campaigns_328.Add(campaign);
    }
    return true;
}


// int MPCooperative::Unknown_005A2E50() -- 0x005A2E50
// The last campaign record whose first map entry lists the session's multiplayer map, or -1.

int MPCooperative::Unknown_005A2E50()
{
    if (unknown_campaigns_40 == 0) {
        return -1;
    }
    int found = -1;
    for (int i = 0; i < unknown_campaigns_40->Count; i++) {
        UnknownDescriptionRecord_00490E00 *record;
        unknown_campaigns_40->Get_Record_00490C50(i, &record);
        for (int m = 0; m < 3; m++) {
            if (unknown_session_a3d298.ScenarioIndex != 0) {
                char const *scenario = unknown_multi_missions_a3d8d8[unknown_session_a3d298.ScenarioIndex]->unknown_58;
                if (_strnicmp(scenario, record->Maps + m * 260, strlen(record->Maps + m * 260)) == 0) {
                    found = i;
                }
            }
        }
    }
    return found;
}


// void MPCooperative::unknown_slot8() -- 0x005A2EF0
// Lobby set-up: drops the campaign when the lobby's map no longer belongs to it, makes and loads
// the campaign for the map (selecting the campaign's first map when the session's map is not one
// of its records), then refreshes the side combos and the campaign description.

void MPCooperative::unknown_slot8()
{
    if (unknown_campaigns_40 == 0) {
        return;
    }
    unknown_lobby_flag_7e6d90 = -1;
    Debug_Printf("Sending observer slot of 8");
    Send_Game_Options_005c3c60("O8");
    if (WS_Find_Dialog_00735940(0xC2) != 0 || WS_Find_Dialog_00735940(0xBC) != 0) {
        if (unknown_campaign_44 != 0) {
            int type;
            unknown_campaign_44->Get_Campaign_Type_0048e6b0(&type);
            if ((Unknown_005A2E50() != type && Current_Dialog_Id_00735b20() == 0x6B) || Current_Dialog_Id_00735b20() != 0x6B) {
                delete unknown_campaign_44;
                unknown_campaign_44 = 0;
            }
        }
        if (unknown_campaign_44 != 0) {
            if (unknown_players_a3fa7c.Count() == 2) {
                unknown_campaign_44->Set_Names(unknown_players_a3fa7c[0]->Name, unknown_players_a3fa7c[1]->Name);
            } else {
                unknown_campaign_44->Set_Names(unknown_empty_wide_839d4c, unknown_empty_wide_839d4c);
            }
        }
    }
    if (unknown_campaign_44 == 0) {
        unknown_campaign_44 = new CoopCampaignClass();
        if (unknown_campaign_44 == 0) {
            return;
        }
        unknown_campaign_44->Load_Campaign_0048f590(0);
        int index = Unknown_005A2E50();
        if (index < 0) {
            unknown_340 = 0;
            char *name = 0;
            unknown_campaign_44->Get_Map_Name_0048fb40(&name, 0);
            int i;
            for (i = 0; i < unknown_multi_missions_a3d8d8.Count(); i++) {
                char const *scenario = unknown_multi_missions_a3d8d8[i]->unknown_58;
                if (_strnicmp(scenario, name, strlen(name)) == 0) {
                    unknown_session_a3d298.ScenarioIndex = i;
                    Select_Scenario_005c7c30(i);
                    break;
                }
            }
            if (i == unknown_multi_missions_a3d8d8.Count()) {
                unknown_session_a3d298.ScenarioIndex = 0;
                Select_Scenario_005c7c30(0);
            }
        } else {
            unknown_340 = index;
            UnknownDescriptionRecord_00490E00 *record;
            unknown_campaigns_40->Get_Record_00490C50(index, &record);
            char *name = 0;
            unknown_campaign_44->Get_Map_Name_0048fb40(&name, 0);
            Debug_Printf("@@@ Looking for %s", unknown_multi_missions_a3d8d8[unknown_session_a3d298.ScenarioIndex]->unknown_58);
            char const *scenario = unknown_multi_missions_a3d8d8[unknown_session_a3d298.ScenarioIndex]->unknown_58;
            if (_strnicmp(scenario, name, strlen(name)) != 0) {
                int other = index == 0;
                do {
                    unknown_campaign_44->Load_Campaign_0048f590(other);
                    unknown_campaign_44->Load_Campaign_0048f590(index);
                    unknown_campaign_44->Get_Map_Name_0048fb40(&name, 0);
                    scenario = unknown_multi_missions_a3d8d8[unknown_session_a3d298.ScenarioIndex]->unknown_58;
                } while (_strnicmp(scenario, name, strlen(name)) != 0);
            }
        }
    }
    unknown_344 = true;
    int campaign;
    HWND lobby = Get_Lobby_Dialog_005c2c40();
    if (lobby != 0) {
        Unknown_005A3760(lobby);
        HWND item = GetDlgItem(lobby, 0x72F);
        if (item != 0 && unknown_campaign_44 != 0 && unknown_campaign_44->Get_Campaign_Type_0048e6b0(&campaign)) {
            char label[64];
            sprintf(label, "NAME:COOPDESC%d", campaign + 1);
            SendMessageA(item, 0x4B2, 0, (LPARAM)StringTable::Load_String(label, 0, "D:\\Ra2\\MPCoop.cpp", 0x157));
            ShowWindow(item, SW_SHOW);
        }
    }
}


class Leaf_5a31f0 { public: void get(); };
void Leaf_5a31f0::get() { *(unsigned char*)((char*)this + 0x344) = 1; }


// int MPCooperative::FirstValidMapIndex() -- 0x005A3200
// The multiplayer map index of the first cooperative campaign's first map (making the campaign
// records and the campaign on first use), or -1.

int MPCooperative::FirstValidMapIndex()
{
    int count = 0;
    char *name = 0;
    if (unknown_campaigns_40 == 0) {
        unknown_campaigns_40 = new UnknownDescriptionList_00490E00();
    }
    unknown_campaigns_40->Get_Count_00490C90(&count);
    if (count != 0) {
        if (unknown_campaign_44 == 0) {
            unknown_campaign_44 = new CoopCampaignClass();
        }
        unknown_campaign_44->Load_Campaign_0048f590(0);
        unknown_campaign_44->Get_Map_Name_0048fb40(&name, 0);
        for (int i = 0; i < unknown_multi_missions_a3d8d8.Count(); i++) {
            char const *scenario = unknown_multi_missions_a3d8d8[i]->unknown_58;
            if (_strnicmp(scenario, name, strlen(name)) == 0) {
                return i;
            }
        }
        AssignTeams();
    }
    return -1;
}


// bool MPCooperative::AssignTeamsAtStart() -- 0x005A3320
// Loads the campaign selected in the lobby list (the saved campaign past the last record), then
// refreshes the side combos and resets the country of every player slot 7 refuses.

bool MPCooperative::AssignTeamsAtStart()
{
    if (unknown_campaigns_40 == 0 || unknown_campaign_44 == 0) {
        return false;
    }
    int count;
    unknown_campaigns_40->Get_Count_00490C90(&count);
    int selected = Selected_Campaign_005c67a0();
    if (selected >= count) {
        if (unknown_players_a3fa7c.Count() == 2) {
            unknown_campaign_44->Set_Names(unknown_players_a3fa7c[0]->Name, unknown_players_a3fa7c[1]->Name);
        }
        if (!unknown_campaign_44->Load_Saved_Campaign_0048eb30()) {
            return false;
        }
        unknown_4c = true;
    } else if (Current_Dialog_Id_00735b20() == 0x6B) {
        delete unknown_campaign_44;
        unknown_campaign_44 = unknown_campaigns_328[selected];
        unknown_campaigns_328[selected] = new CoopCampaignClass();
        unknown_campaigns_328[selected]->Load_Campaign_0048f590(selected);
        unknown_50 = selected;
        if (unknown_players_a3fa7c.Count() == 2) {
            unknown_campaign_44->Set_Names(unknown_players_a3fa7c[0]->Name, unknown_players_a3fa7c[1]->Name);
        }
    }
    UnknownDescriptionRecord_00490E00 *record;
    unknown_campaigns_40->Get_Record_00490C50(unknown_340, &record);
    if (record == 0) {
        return false;
    }
    int players = record->Players.Count();
    if (players == 0) {
        return false;
    }
    Unknown_005A3760(Get_Lobby_Dialog_005c2c40());
    for (int i = 0; i < unknown_players_a3fa7c.Count(); i++) {
        if (!unknown_slot7(unknown_players_a3fa7c[i]->unknown_66e290())) {
            if (players > 1) {
                unknown_players_a3fa7c[i]->unknown_66e2d0(-2);
            } else {
                int house = Find_Country_Index_004904A0(record->Players[0]);
                unknown_players_a3fa7c[i]->unknown_66e2d0(house);
            }
        }
    }
    return true;
}


// int MPCooperative::unknown_slot10() -- 0x005A34B0
// The country of the selected campaign record's single player entry, or -2 (none, or more than one).

int MPCooperative::unknown_slot10()
{
    UnknownDescriptionRecord_00490E00 *record;
    unknown_campaigns_40->Get_Record_00490C50(unknown_340, &record);
    if (record == 0) {
        return -2;
    }
    if (record->Players.Count() == 0) {
        return -2;
    }
    if (record->Players.Count() > 1) {
        return -2;
    }
    return Find_Country_Index_004904A0(record->Players[0]);
}


// bool MPCooperative::unknown_slot18(int index, DynamicVectorClass<int> choices, char const *second_name, char const *first_name) -- 0x005A3500
// Makes the campaign when there is none, then stores the campaign information for the record index
// (a changed campaign type marks the side combos for a refresh).

bool MPCooperative::unknown_slot18(int index, DynamicVectorClass<int> choices, char const *second_name, char const *first_name)
{
    if (unknown_campaign_44 == 0) {
        Unknown_005A4F20();
        unknown_344 = true;
    }
    if (unknown_campaign_44 != 0 && unknown_campaigns_40 != 0) {
        int type;
        unknown_campaign_44->Get_Campaign_Type_0048e6b0(&type);
        if (type != index) {
            unknown_344 = true;
        }
        unknown_campaign_44->Set_Campaign_Info_0048f7d0(*unknown_campaigns_40, index, choices, second_name, first_name);
        unknown_340 = index;
        return true;
    }
    return false;
}


// bool MPCooperative::unknown_slot19(int index, int time, MPlayerScoreType *scores) -- 0x005A35D0
// Refreshes the side combos once after a campaign change and records the two players' scores
// for the campaign map.

bool MPCooperative::unknown_slot19(int index, int time, MPlayerScoreType *scores)
{
    if (unknown_campaign_44 == 0) {
        return false;
    }
    HWND lobby = Get_Lobby_Dialog_005c2c40();
    if (lobby != 0 && unknown_344) {
        Unknown_005A3760(lobby);
        unknown_344 = false;
    }
    return unknown_campaign_44->Add_Score_0048f730(index, time, scores[0], scores[1]);
}


// void MPCooperative::Unknown_005A3650(HWND dialog, int id) -- 0x005A3650
// Refills one side combo with the countries the selected campaign record lists ("random" first when
// it lists more than one), each with its HouseTypes index as item data.

void MPCooperative::Unknown_005A3650(HWND dialog, int id)
{
    int count = HouseTypes.Count();
    if (count != 0) {
        HWND combo = GetDlgItem(dialog, id);
        ShowWindow(combo, SW_HIDE);
        SendMessageA(combo, 0x14B, 0, 0);
        UnknownDescriptionRecord_00490E00 *record;
        unknown_campaigns_40->Get_Record_00490C50(unknown_340, &record);
        if (record != 0) {
            int players = record->Players.Count();
            if (players != 0) {
                if (players > 1) {
                    wchar_t const *name = StringTable::Load_String("GUI:Random", 0, "D:\\Ra2\\MPCoop.cpp", 0x225);
                    LRESULT index = SendMessageA(combo, 0x4C2, 0, (LPARAM)name);
                    SendMessageA(combo, 0x151, index, -2);
                }
                for (int i = 0; i < players; i++) {
                    int house = Find_Country_Index_004904A0(record->Players[i]);
                    if (house >= 0 && house < count) {
                        wchar_t const *name = HouseTypes[house]->UIName;
                        LRESULT index = SendMessageA(combo, 0x4C2, 0, (LPARAM)name);
                        SendMessageA(combo, 0x151, index, house);
                    }
                }
                SendMessageA(combo, 0x14E, 0, 0);
                ShowWindow(combo, SW_SHOW);
            }
        }
    }
}


// void MPCooperative::Unknown_005A3760(HWND dialog) -- 0x005A3760
// Fills each player row's side combo from the selected campaign record: "random" when the
// record lists more than one country, else its only country; then refreshes the rows and shows
// the campaign description (NAME:COOPDESC<n>) in the lobby.

void MPCooperative::Unknown_005A3760(HWND dialog)
{
    if (dialog != 0) {
        int count = unknown_int_a3db1c;
        UnknownDescriptionRecord_00490E00 *record;
        unknown_campaigns_40->Get_Record_00490C50(unknown_340, &record);
        if (record != 0) {
            int players = record->Players.Count();
            if (players != 0) {
                for (int i = 0; i < count; i++) {
                    int id = Player_Side_Combo_ID_4d1350(i);
                    Unknown_005A3650(dialog, id);
                    if (players > 1) {
                        Select_Side_Combo_4d1a90(dialog, id, -2);
                        unknown_player_slots_a3fa98[i]->unknown_66e2d0(-2);
                    } else {
                        int house = Find_Country_Index_004904A0(record->Players[0]);
                        Select_Side_Combo_4d1a90(dialog, id, house);
                        unknown_player_slots_a3fa98[i]->unknown_66e2d0(house);
                    }
                }
                Unknown_005cb170(dialog, count);
                HWND lobby = Get_Lobby_Dialog_005c2c40();
                int campaign;
                if (lobby != 0 && unknown_campaign_44 != 0 && unknown_campaign_44->Get_Campaign_Type_0048e6b0(&campaign)) {
                    char label[64];
                    sprintf(label, "NAME:COOPDESC%d", campaign + 1);
                    HWND item = GetDlgItem(lobby, 0x72F);
                    if (item != 0) {
                        SendMessageA(item, 0x4B2, 0, (LPARAM)StringTable::Load_String(label, 0, "D:\\Ra2\\MPCoop.cpp", 0x273));
                        ShowWindow(item, SW_SHOW);
                        InvalidateRect(item, 0, 0);
                    }
                }
            }
        }
    }
}


// bool MPCooperative::unknown_slot20() -- 0x005A38D0
// Refreshes the side combos once after a campaign change and sends the campaign options.

bool MPCooperative::unknown_slot20()
{
    if (unknown_campaign_44 == 0) {
        return false;
    }
    unknown_campaign_44->Get_Campaign_Type_0048e6b0(&unknown_340);
    HWND lobby = Get_Lobby_Dialog_005c2c40();
    if (lobby != 0 && unknown_344) {
        Unknown_005A3760(lobby);
        unknown_344 = false;
    }
    return unknown_campaign_44->Send_Campaign_Options_00490150(*unknown_campaigns_40);
}


// void MPCooperative::unknown_player_colors_5a3920() -- 0x005A3920
// The session colours (0x00A71118, eight): -2 (none), then each session player's colour by its index.
// No entry references: slot 21 0x005A3960 expands it.

void MPCooperative::unknown_player_colors_5a3920()
{
    int i;
    for (i = 0; i < 8; i++) {
        unknown_colors_a71118[i] = -2;
    }
    int count = unknown_players_a3fa7c.Count();
    for (i = 0; i < count; i++) {
        unknown_colors_a71118[i] = unknown_players_a3fa7c[i]->Get_Color_0066e2b0();
    }
}


// bool MPCooperative::unknown_slot21() -- 0x005A3960 (expands unknown_player_colors_5a3920)
// Picks the computer players' houses and colours for every map of the selected campaign record
// (slot 23/24 houses; the players' colours first, random free colours for the rest) into
// unknown_1a8/unknown_268 and sends them as a "C1,<maps>,<house>,<colour>..." game options text.

bool MPCooperative::unknown_slot21()
{
    UnknownDescriptionRecord_00490E00 *record;
    unknown_campaigns_40->Get_Record_00490C50(unknown_340, &record);
    char message[400];
    sprintf(message, "C1,%d", record->MapCount);
    for (int list = 0; list < 8; list++) {
        int length = unknown_1a8[list].Length();
        unknown_1a8[list].Clear();
        unknown_1a8[list].Resize(length);
        length = unknown_268[list].Length();
        unknown_268[list].Clear();
        unknown_268[list].Resize(length);
    }
    int players = unknown_players_a3fa7c.Count();
    for (int map = 0; map < record->MapCount; map++) {
        unknown_player_colors_5a3920();
        for (int slot = 0; slot < 8; slot++) {
            if (slot < players) {
                unknown_1a8[slot].Add(RandomHumanCountryIndex());
                unknown_268[slot].Add(unknown_colors_a71118[slot]);
            } else {
                unknown_1a8[slot].Add(RandomAICountryIndex());
                int color;
                bool taken;
                do {
                    color = unknown_id_source_a3d290->RandomNumber(0, 7);
                    taken = false;
                    if (color != -2) {
                        for (int i = 0; i < 8; i++) {
                            if (unknown_colors_a71118[i] == color) {
                                taken = true;
                                break;
                            }
                        }
                    }
                } while (taken);
                unknown_colors_a71118[slot] = color;
                unknown_268[slot].Add(color);
            }
            char text[16];
            sprintf(text, ",%d,%d", unknown_1a8[slot][map], unknown_268[slot][map]);
            strcat(message, text);
        }
    }
    Debug_Printf("@@@@ Sending campaign game AI settings: %s\n", message);
    Send_Game_Options_005c3c60(message);
    Debug_Printf("@@@@ Sent\n");
    return true;
}


// void MPCooperative::unknown_slot25(int kind, char *text) -- 0x005A3CA0
// Kind 1 carries the campaign AI settings: a round count, then per round one comma-separated
// value per player for each of the two per-player lists (-1 where a value is missing).

void MPCooperative::unknown_slot25(int kind, char *text)
{
    if (kind == 1) {
        int rounds = atoi(strtok(text, ","));
        int i;
        for (i = 0; i < 8; i++) {
            int capacity = unknown_1a8[i].Length();
            unknown_1a8[i].Clear();
            unknown_1a8[i].Resize(capacity);
            capacity = unknown_268[i].Length();
            unknown_268[i].Clear();
            unknown_268[i].Resize(capacity);
        }
        for (int round = 0; round < rounds; round++) {
            for (i = 0; i < 8; i++) {
                char *token = strtok(0, ",");
                if (token != 0) {
                    unknown_1a8[i].Add(atoi(token));
                } else {
                    unknown_1a8[i].Add(-1);
                }
                token = strtok(0, ",");
                if (token != 0) {
                    unknown_268[i].Add(atoi(token));
                } else {
                    unknown_268[i].Add(-1);
                }
            }
        }
    }
}


// bool MPCooperative::UnfixAlliances() -- 0x005A3EE0
bool MPCooperative::UnfixAlliances()
{
    HWND dialog = Get_Lobby_Dialog_005c2c40();
    if (dialog != 0 && unknown_campaign_44 != 0) {
        int campaign;
        if (unknown_campaign_44->Get_Campaign_Type_0048e6b0(&campaign)) {
            char label[64];
            sprintf(label, "NAME:COOPDESC%d", campaign + 1);
            HWND item = GetDlgItem(dialog, 0x72F);
            if (item != 0) {
                SendMessageA(item, 0x4B2, 0, (LPARAM)StringTable::Load_String(label, 0, "D:\\Ra2\\MPCoop.cpp", 0x31E));
                ShowWindow(item, SW_SHOW);
            }
        }
    }
    return true;
}


class Target_5a3f70 { public: unsigned int PopulateTeamDropdownForPlayer_005b75d0(unsigned int target_window, unsigned int target_player_index); };
class Leaf_5a3f70 { public: unsigned int get(unsigned int window_address, unsigned int player_index); };
unsigned int Leaf_5a3f70::get(unsigned int window_address, unsigned int player_index) { return ((Target_5a3f70*)(this))->PopulateTeamDropdownForPlayer_005b75d0((unsigned int)(window_address), (unsigned int)(player_index)); }


// Cell MPCooperative::AssignStartingPositionsToHouse(int house, DynamicVectorClass<Cell> &cells, bool *taken) -- 0x005A3F90
// Picks a start cell for a house and marks it taken: a random one while none is taken, the one
// nearest the single taken cell, else the one farthest from all taken cells.

Cell MPCooperative::AssignStartingPositionsToHouse(int house, DynamicVectorClass<Cell> &cells, bool *taken)
{
    int count = 0;
    int i;
    for (i = 0; i < cells.Count(); i++) {
        if (taken[i]) {
            count++;
        }
    }
    int index;
    if (count == 0) {
        index = unknown_id_source_a3d290->RandomNumber(0, cells.Count() - 1);
        taken[index] = true;
        return cells[index];
    } else {
        int nearest = -1;
        int nearest_index = -1;
        if (count == 1) {
            for (i = 0; i < cells.Count(); i++) {
                if (!taken[i]) {
                    int total = 0;
                    for (int j = 0; j < cells.Count(); j++) {
                        if (taken[j]) {
                            Cell delta(cells[i].X - cells[j].X, cells[i].Y - cells[j].Y);
                            total += (short)Cell_Distance(delta.X, delta.Y);
                        }
                    }
                    if (total < nearest || nearest < 0) {
                        nearest = total;
                        nearest_index = i;
                    }
                }
            }
            taken[nearest_index] = true;
            return cells[nearest_index];
        } else {
            int farthest = -1;
            int farthest_index = -1;
            for (i = 0; i < cells.Count(); i++) {
                if (!taken[i]) {
                    int total = 0;
                    for (int j = 0; j < cells.Count(); j++) {
                        if (taken[j]) {
                            Cell delta(cells[i].X - cells[j].X, cells[i].Y - cells[j].Y);
                            total += (short)Cell_Distance(delta.X, delta.Y);
                        }
                    }
                    if (total > farthest) {
                        farthest = total;
                        farthest_index = i;
                    }
                }
            }
            index = farthest_index;
        }
    }
    taken[index] = true;
    return cells[index];
}


// bool MPCooperative::StartingPositionsToHouseBaseCells(bool official) -- 0x005A4170
// Gives every playing (not passive) house a start cell from the multiplayer start waypoints
// (slot 42 picks it) and centres its base there.

bool MPCooperative::StartingPositionsToHouseBaseCells(bool official)
{
    DynamicVectorClass<Cell> waypoints = Multiplayer_Start_Waypoints_0065fff0(official);
    bool taken[16];
    memset(taken, 0, sizeof(taken));
    for (int i = 0; i < Houses.Count(); i++) {
        HouseClass *house = Houses[i];
        if (!house->Type->MultiplayPassive) {
            Debug_Printf("Assigning Starting Points for house %d (%s)\n", i, (char const *)house->Type->ID);
            Cell cell = AssignStartingPositionsToHouse(i, waypoints, taken);
            house->BaseCenter = CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
        }
    }
    return true;
}


// bool MPCooperative::unknown_slot22() -- 0x005A4250

bool MPCooperative::unknown_slot22()
{
    int players = Unknown_005c6600(unknown_session_a3d298.ScenarioIndex) - 2;
    Debug_Printf("@@@@ Adding %d AI players\n", players);
    if (players <= 0) {
        return false;
    }
    int added = 0;
    unknown_session_a3d298.AICount = players;
    UnknownDescriptionRecord_00490E00 *record;
    unknown_campaigns_40->Get_Record_00490C50(unknown_340, &record);
    if (record == 0) {
        return false;
    }
    if (record->Enemies.Count() == 0) {
        return false;
    }
    for (int slot = 2; slot < 8; slot++) {
        if (added < players) {
            Debug_Printf("@@@@ Setting slot %d for AI\n", slot);
            char *name = record->AIName;
            if (strcmp(name, "Normal") == 0) {
                unknown_session_a3d298.AIDifficulty[slot] = 1;
            } else if (strcmp(name, "Hard") == 0) {
                unknown_session_a3d298.AIDifficulty[slot] = 0;
            } else {
                unknown_session_a3d298.AIDifficulty[slot] = 2;
            }
            Debug_Printf("@@@@     AI difficulty: %d\n", unknown_session_a3d298.AIDifficulty[slot]);
            unknown_session_a3d298.AICountry[slot] = -2;
            unknown_session_a3d298.AIColor[slot] = -2;
            added++;
        } else {
            unknown_session_a3d298.AIDifficulty[slot] = -1;
            unknown_session_a3d298.AICountry[slot] = -1;
            unknown_session_a3d298.AIColor[slot] = -1;
        }
    }
    return true;
}


// bool MPCooperative::unknown_slot28() -- 0x005A43C0

bool MPCooperative::unknown_slot28()
{
    Debug_Printf("@@@@ Making Alliances\n");
    for (int i = 0; i < Houses.Count(); i++) {
        HouseClass *first = Houses[i];
        Debug_Printf("@@@@     House1 = %s\n", Wide_To_Narrow_006fa120(first->UIName));
        if (!first->Type->MultiplayPassive && first->unknown_134) {
            for (int j = i + 1; j < Houses.Count(); j++) {
                HouseClass *second = Houses[j];
                Debug_Printf("@@@@         House2 = %s\n", Wide_To_Narrow_006fa120(second->UIName));
                if (!second->Type->MultiplayPassive && second->unknown_134) {
                    char *second_name = Wide_To_Narrow_006fa120(second->UIName);
                    char *first_name = Wide_To_Narrow_006fa120(first->UIName);
                    Debug_Printf("@@@@ Making Allies of %s(%d/%d/%d) and %s(%d/%d/%d)\n",
                        first_name, first->Type->House, first->Type->Side, first->Type->Color,
                        second_name, second->Type->House, second->Type->Side, second->Type->Color);
                    first->Make_Ally(second, false);
                    second->Make_Ally(first, false);
                }
            }
        }
    }
    return true;
}


// int MPCooperative::RandomHumanCountryIndex() -- 0x005A4540

int MPCooperative::RandomHumanCountryIndex()
{
    int count = HouseTypes.Count();
    bool found = false;
    int campaign = 0;
    unknown_campaign_44->Get_Campaign_Type_0048e6b0(&campaign);
    UnknownDescriptionRecord_00490E00 *record;
    unknown_campaigns_40->Get_Record_00490C50(campaign, &record);
    int house;
    do {
        house = unknown_id_source_a3d290->Random()(0, count - 1);
        for (int i = 0; i < record->Players.Count(); i++) {
            if (house == Find_Country_Index_004904A0(record->Players[i])) {
                found = true;
            }
        }
    } while (!found);
    Debug_Printf("@@@@ Choosing a random player house: %d(%s)\n", house, Wide_To_Narrow_006fa120(HouseTypes[house]->UIName));
    return house;
}


// int MPCooperative::RandomAICountryIndex() -- 0x005A45F0

int MPCooperative::RandomAICountryIndex()
{
    int count = HouseTypes.Count();
    bool found = false;
    int campaign = 0;
    unknown_campaign_44->Get_Campaign_Type_0048e6b0(&campaign);
    UnknownDescriptionRecord_00490E00 *record;
    unknown_campaigns_40->Get_Record_00490C50(campaign, &record);
    int house;
    do {
        house = unknown_id_source_a3d290->Random()(0, count - 1);
        for (int i = 0; i < record->Enemies.Count(); i++) {
            if (house == Find_Country_Index_004904A0(record->Enemies[i])) {
                found = true;
            }
        }
    } while (!found);
    Debug_Printf("@@@@ Choosing a random AI house: %d(%s)\n", house, Wide_To_Narrow_006fa120(HouseTypes[house]->UIName));
    return house;
}


class Leaf_5a46b0 { public: bool get(); };
bool Leaf_5a46b0::get() { return false; }


// void MPCooperative::unknown_slot34(bool player_wins) -- 0x005A46C0
// The end of a cooperative map: when a human player is still in, advances and saves the campaign
// (announcing the result), marks the scenario to skip its score and map selection, saves the
// session's players into unknown_players_5c (unknown_58: the local one's row) and the session
// settings into +0x74..+0x1A8 for slot 37, and resets the per-game state.

extern int unknown_833680;    // 0x00833680
extern int unknown_833684;    // 0x00833684
extern int unknown_833688;    // 0x00833688

void MPCooperative::unknown_slot34(bool player_wins)
{
    bool win = false;
    for (int i = 0; i < Houses.Count(); i++) {
        if (Houses[i] != 0 && Houses[i]->Is_Human_Player() && !Houses[i]->unknown_13d) {
            win = true;
        }
    }
    Debug_Printf("@@@@ End-of-Game: PlayerWins:%d, win:%d\n", player_wins, win);
    if (win) {
        int scheme = Gadget_Color_Scheme();
        if (unknown_campaign_44 != 0) {
            unknown_campaign_44->Next_Map_0048e360();
            if (unknown_campaign_44->Save_Campaign_0048f040()) {
                unknown_messages_a3dc70.Add_Message(0, 0, StringTable::Load_String("TXT_COOP_SAVE_SUCCESS", 0, "D:\\Ra2\\MPCoop.cpp", 0x42E),
                    scheme, 0x16, (int)(Rules->MessageDelay * 900.0), 0);
            } else {
                unknown_messages_a3dc70.Add_Message(0, 0, StringTable::Load_String("TXT_COOP_SAVE_FAILED", 0, "D:\\Ra2\\MPCoop.cpp", 0x430),
                    scheme, 0x16, (int)(Rules->MessageDelay * 900.0), 0);
            }
        }
    }
    wcscpy(unknown_host_name_78, unknown_session_a3d298.HostName_2380);
    unknown_id_source_a3d290->SkipScore = true;
    unknown_id_source_a3d290->EndOfGame = false;
    unknown_id_source_a3d290->SkipMapSelect = true;
    unknown_id_source_a3d290->OneTimeOnly = false;
    Free_Pointers_005a7480(&unknown_players_5c);
    unknown_58 = 0;
    for (int p = 0; p < unknown_players_a3fa7c.Count(); p++) {
        NodeNameType *entry = new NodeNameType;
        wcscpy(entry->Name, unknown_players_a3fa7c[p]->Name);
        entry->unknown_6b = unknown_players_a3fa7c[p]->unknown_6b;
        entry->unknown_4b = unknown_players_a3fa7c[p]->unknown_4b;
        entry->unknown_53 = unknown_players_a3fa7c[p]->unknown_53;
        entry->ID = unknown_players_a3fa7c[p]->ID;
        strcpy(entry->unknown_34, unknown_players_a3fa7c[p]->unknown_34);
        entry->unknown_4f = unknown_players_a3fa7c[p]->unknown_4f;
        entry->unknown_57 = unknown_players_a3fa7c[p]->unknown_57;
        entry->Team = unknown_players_a3fa7c[p]->Team;
        entry->unknown_63 = unknown_players_a3fa7c[p]->unknown_63;
        entry->unknown_67 = unknown_players_a3fa7c[p]->unknown_67;
        entry->unknown_6b = unknown_players_a3fa7c[p]->unknown_6b;
        entry->Address = unknown_players_a3fa7c[p]->Address;
        unknown_players_5c.Add(entry);
        if (unknown_address_a3db2c == unknown_players_a3fa7c[p]->Address) {
            unknown_58 = p;
        }
    }
    unknown_c8 = unknown_session_a3d298.unknown_293c;
    unknown_128 = unknown_session_a3d298.unknown_299c;
    unknown_148 = unknown_session_a3d298.unknown_2fa0;
    unknown_198 = unknown_session_a3d298.unknown_3022;
    wcscpy(unknown_master_name_168, unknown_session_a3d298.MasterPlayerName);
    unknown_master_id_194 = unknown_session_a3d298.MasterPlayerID;
    unknown_74 = unknown_session_a3d298.unknown_124;
    unknown_a0 = Seed;
    unknown_a4 = unknown_house_setting_7d88d8;
    unknown_units_a8 = unknown_session_a3d298.UnitCount;
    unknown_credits_ac = unknown_session_a3d298.unknown_24;
    unknown_fog_b0 = unknown_session_a3d298.unknown_fog_a7;
    unknown_b1 = unknown_session_a3d298.unknown_28;
    unknown_crates_b2 = unknown_session_a3d298.unknown_crates_29;
    unknown_b3 = unknown_session_a3d298.unknown_a8;
    unknown_b4 = unknown_session_a3d298.unknown_a4;
    unknown_b5 = unknown_session_a3d298.unknown_a5;
    unknown_bases_b6 = unknown_session_a3d298.unknown_bases_20;
    unknown_b7 = unknown_session_a3d298.unknown_a6;
    unknown_ai_count_b8 = unknown_session_a3d298.AICount;
    unknown_difficulty_bc = unknown_session_a3d298.unknown_difficulty_40;
    unknown_c0 = unknown_session_a3d298.unknown_2a;
    unknown_c1 = unknown_session_a3d298.unknown_2b;
    unknown_c2 = unknown_session_a3d298.unknown_2c;
    unknown_c3 = unknown_session_a3d298.unknown_34;
    unknown_game_speed_c4 = unknown_session_a3d298.GameSpeed_30;
    unknown_session_a3d298.unknown_2b0 = 0;
    Map.Unknown_00498F10(0);
    unknown_833680 = 0;
    unknown_833684 = 0;
    unknown_833688 = -1;
    for (int row = 0; row < 4; row++) {
        unknown_session_a3d298.unknown_1fb4[row][unknown_session_a3d298.unknown_2300] = -1;
    }
}


// void MPCooperative::DrawTeamDropdown(HWND list, DynamicVectorClass<MultiMission *> const *maps, MultiMission *selected) -- 0x005A4B70
// Fills the campaign list: makes the score screen and the campaign on first use, loads every
// campaign record, adds "<n>. <name>" for each campaign whose first map is in maps (item data: the
// map), then the saved-campaign row when there is a save, and selects the first row.

void MPCooperative::DrawTeamDropdown(HWND list, DynamicVectorClass<MultiMission *> const *maps, MultiMission *selected)
{
    unknown_04 = true;
    if (unknown_score_54 == 0) {
        unknown_score_54 = new CampaignScoreClass();
    }
    SendMessageA(list, 0x184, 0, 0);
    unknown_4d = false;
    if (unknown_campaign_44 == 0) {
        if (unknown_players_a3fa7c.Count() == 2) {
            unknown_campaign_44 = new CoopCampaignClass(unknown_players_a3fa7c[0]->Name, unknown_players_a3fa7c[1]->Name);
        }
    } else if (unknown_players_a3fa7c.Count() != 2) {
        unknown_campaign_44->Set_Names(unknown_empty_wide_839d4c, unknown_empty_wide_839d4c);
    }
    if (unknown_campaigns_328.Count() == 0) {
        int records;
        unknown_campaigns_40->Get_Count_00490C90(&records);
        for (int r = 0; r < records; r++) {
            CoopCampaignClass *campaign = new CoopCampaignClass();
            campaign->Load_Campaign_0048f590(r);
            unknown_campaigns_328.Add(campaign);
        }
    }
    DynamicVectorClass<wchar_t *> names;
    unknown_campaigns_40->Copy_Names(names);
    int count = names.Length();
    int i;
    for (i = 0; i < count; i++) {
        if (names[i] != 0) {
            char *map;
            unknown_campaigns_328[i]->Get_Map_Name_0048fb40(&map, -1);
            int map_count = maps->Count();
            char name[260];
            MultiMission *found = 0;
            sprintf(name, "%s.MAP", map);
            for (int k = 0; k < map_count; k++) {
                if (found != 0) {
                    break;
                }
                char const *scenario = (*maps)[k]->unknown_58;
                if (_stricmp(scenario, name) == 0) {
                    found = (*maps)[k];
                }
            }
            if (found != 0) {
                wchar_t text[260];
                swprintf(text, L"%d. %s", i + 1, names[i]);
                SendMessageA(list, 0x4CD, 0, (LPARAM)text);
                SendMessageA(list, 0x19A, i, (LPARAM)found);
            } else {
                Debug_Printf("@@@@ Could not find %s for %s\n", name, names[i]);
            }
            delete names[i];
        }
    }
    if (unknown_campaign_44 != 0 && unknown_campaign_44->Has_Saved_Campaign_0048fbf0() && !unknown_4d) {
        char name[260];
        unknown_campaign_44->Saved_Map_Is_Undefined_0048fe70(name);
        strcat(name, ".MAP");
        int map_count = maps->Count();
        MultiMission *found = 0;
        for (int k = 0; k < map_count; k++) {
            if (found != 0) {
                break;
            }
            char const *scenario = (*maps)[k]->unknown_58;
            if (_stricmp(scenario, name) == 0) {
                found = (*maps)[k];
            }
        }
        if (found != 0) {
            SendMessageA(list, 0x4CD, 0, (LPARAM)StringTable::Load_String("TXT_COOP_USE_SAVE", 0, "D:\\Ra2\\MPCoop.cpp", 0x528));
            SendMessageA(list, 0x19A, i, (LPARAM)found);
            unknown_4d = true;
        }
    }
    SendMessageA(list, 0x186, 0, 0);
}


// bool MPCooperative::Unknown_005A4F20() -- 0x005A4F20 (true extent 111 B; the inventory says 41)
// Replaces the selected campaign with a freshly loaded first campaign (named after the two players when there are two).

bool MPCooperative::Unknown_005A4F20()
{
    if (unknown_campaign_44 != 0) {
        delete unknown_campaign_44;
    }
    unknown_campaign_44 = 0;
    unknown_campaign_44 = new CoopCampaignClass();
    unknown_campaign_44->Load_Campaign_0048f590(0);
    if (unknown_players_a3fa7c.Count() == 2) {
        unknown_campaign_44->Set_Names(unknown_players_a3fa7c[0]->Name, unknown_players_a3fa7c[1]->Name);
    }
    return (unknown_campaign_44 != 0) ? true : false;
}


// void MPCooperative::unknown_slot33() -- 0x005A4F90
void MPCooperative::unknown_slot33()
{
    HWND dialog = Get_Lobby_Dialog_005c2c40();
    if (dialog != 0) {
        HWND item = GetDlgItem(dialog, 0x72F);
        if (item != 0) {
            SendMessageA(item, 0x4B2, 0, (LPARAM)unknown_text_839d4c);
            ShowWindow(item, SW_HIDE);
        }
        Fill_Side_Combos_4d16d0(dialog);
    }
    for (int i = 0; i < unknown_players_a3fa7c.Count(); i++) {
        unknown_players_a3fa7c[i]->Team = i;
    }
}


// void MPCooperative::unknown_slot30() -- 0x005A5000
// In the campaign dialog (0x6B) drops the lobby list's last row once; elsewhere reloads the chosen
// campaign and selects its current map, refreshes the lobby dialog and sends the lobby state. Then
// clears the campaign's player names.

void MPCooperative::unknown_slot30()
{
    if (Current_Dialog_Id_00735b20() == 0x6B) {
        if (unknown_4d) {
            HWND list = GetDlgItem(Unknown_00735B10(), 0x553);
            int count = SendMessageA(list, 0x18B, 0, 0);
            if (count == -1) {
                if (unknown_campaign_44 != 0) {
                    unknown_campaign_44->Set_Names(unknown_empty_wide_839d4c, unknown_empty_wide_839d4c);
                }
                return;
            }
            if (SendMessageA(list, 0x188, 0, 0) == count - 1) {
                SendMessageA(list, 0x186, count - 2, 0);
            }
            SendMessageA(list, 0x182, count - 1, 0);
            unknown_4d = false;
        }
    } else {
        if (unknown_campaign_44 != 0 && unknown_4c) {
            if (unknown_50 != 0) {
                unknown_campaign_44->Load_Campaign_0048f590(0);
                unknown_campaign_44->Load_Campaign_0048f590(unknown_50);
            } else {
                unknown_campaign_44->Load_Campaign_0048f590(1);
                unknown_campaign_44->Load_Campaign_0048f590(unknown_50);
            }
            char *map;
            unknown_campaign_44->Get_Map_Name_0048fb40(&map, -1);
            int count = unknown_multi_missions_a3d8d8.Count();
            char name[260];
            sprintf(name, "%s.MAP", map);
            int i;
            for (i = 0; i < count; i++) {
                char const *scenario = unknown_multi_missions_a3d8d8[i]->unknown_58;
                if (_stricmp(scenario, name) == 0) {
                    break;
                }
            }
            unknown_session_a3d298.ScenarioIndex = i;
            unknown_id_source_a3d290->TechLevel = i;
            Select_Scenario_005c7c30(i);
        }
        HWND dialog = Get_Lobby_Dialog_005c2c40();
        if (dialog != 0) {
            Unknown_005c3450(dialog, this);
            Unknown_005c34b0(dialog);
            Unknown_005c7570(dialog);
        }
        Unknown_005c3810(true, true);
    }
    if (unknown_campaign_44 != 0) {
        unknown_campaign_44->Set_Names(unknown_empty_wide_839d4c, unknown_empty_wide_839d4c);
    }
}


// void MPCooperative::unknown_slot31() -- 0x005A5190
// Makes and loads the campaign on first use and names it after the two players; when a saved
// campaign exists, offers it: in the campaign dialog (0x6B) adds a "use the save" row for the saved
// map to the lobby list once, elsewhere prints that a saved campaign is available.

void MPCooperative::unknown_slot31()
{
    if (unknown_campaign_44 == 0) {
        unknown_campaign_44 = new CoopCampaignClass();
        unknown_campaign_44->Load_Campaign_0048f590(0);
    }
    if (unknown_campaign_44 != 0) {
        if (unknown_players_a3fa7c.Count() == 2) {
            unknown_campaign_44->Set_Names(unknown_players_a3fa7c[0]->Name, unknown_players_a3fa7c[1]->Name);
        }
        unknown_slot20();
        if (unknown_campaign_44->Has_Saved_Campaign_0048fbf0()) {
            if (Current_Dialog_Id_00735b20() == 0x6B) {
                if (!unknown_4d) {
                    char name[260];
                    unknown_campaign_44->Saved_Map_Is_Undefined_0048fe70(name);
                    strcat(name, ".MAP");
                    MultiMission *map = 0;
                    int count = unknown_multi_missions_a3d8d8.Count();
                    for (int i = 0; i < count; i++) {
                        if (map != 0) {
                            break;
                        }
                        char const *scenario = unknown_multi_missions_a3d8d8[i]->unknown_58;
                        if (_stricmp(scenario, name) == 0) {
                            map = unknown_multi_missions_a3d8d8[i];
                        }
                    }
                    if (map != 0) {
                        HWND list = GetDlgItem(Unknown_00735B10(), 0x553);
                        int rows = SendMessageA(list, 0x18B, 0, 0);
                        if (rows != -1) {
                            SendMessageA(list, 0x4CD, 0, (LPARAM)StringTable::Load_String("TXT_COOP_USE_SAVE", 0, "D:\\Ra2\\MPCoop.cpp", 0x5E1));
                            SendMessageA(list, 0x19A, rows, (LPARAM)map);
                            unknown_4d = true;
                        }
                    }
                }
            } else {
                Unknown_005C2C80(-1, StringTable::Load_String("TXT_SAVE_CAMP_AVAILABLE", 0, "D:\\Ra2\\MPCoop.cpp", 0x5E7));
            }
        }
    }
}


class Leaf_5a5340 { public: unsigned int get(); };
unsigned int Leaf_5a5340::get() { return 2; }


// bool MPCooperative::unknown_slot36() -- 0x005A5350
// Shows the score screen for the map just played (making it on first use); returns what the
// screen left in unknown_score_850504.

bool MPCooperative::unknown_slot36()
{
    if (unknown_score_54 == 0) {
        unknown_score_54 = new CampaignScoreClass();
    }
    if (unknown_campaign_44 == 0) {
        return false;
    }
    if (unknown_campaigns_40 == 0) {
        return false;
    }
    DynamicVectorClass<MPlayerScoreType *> scores;
    int time;
    unknown_campaign_44->Get_Player_Scores_0048e710(scores, &time);
    UnknownDescriptionRecord_00490E00 *record = 0;
    int type;
    if (!unknown_campaign_44->Get_Campaign_Type_0048e6b0(&type)) {
        type = -1;
    } else {
        unknown_campaigns_40->Get_Record_00490C50(type, &record);
    }
    int difficulty = -1;
    if (type == 0 || type == 1) {
        difficulty = 0;
    } else if (type == 2 || type == 3) {
        difficulty = 1;
    } else if (type == 4) {
        difficulty = 2;
    }
    int count;
    unknown_campaign_44->Get_Map_Count_0048e6d0(&count);
    int current;
    unknown_campaign_44->Get_Current_Map_0048e6f0(&current);
    if (record != 0) {
        unknown_score_54->Presentation(scores, unknown_id_source_a3d290->ElapsedTime.Time(), current < count, difficulty, record->Name);
    } else {
        unknown_score_54->Presentation(scores, unknown_id_source_a3d290->ElapsedTime.Time(), current < count, difficulty, unknown_empty_wide_839d4c);
    }
    while (scores.Count() != 0) {
        if (scores[0] != 0) {
            delete scores[0];
        }
        scores[0] = 0;
        scores.Delete(0);
    }
    bool result = unknown_score_850504;
    unknown_score_850504 = false;
    return result;
}


// void MPCooperative::unknown_slot35(bool unknown) -- 0x005A5570
// After the last map of the campaign (when asked to), shows the campaign end score screen.

void MPCooperative::unknown_slot35(bool unknown)
{
    if (unknown_campaign_44 != 0) {
        int count;
        unknown_campaign_44->Get_Map_Count_0048e6d0(&count);
        int current;
        unknown_campaign_44->Get_Current_Map_0048e6f0(&current);
        if (unknown && current >= count) {
            CampaignEndScoreClass *score = new CampaignEndScoreClass();
            if (score != 0) {
                DynamicVectorClass<MPlayerScoreType *> scores;
                int time;
                unknown_campaign_44->Get_Player_Scores_0048e710(scores, &time);
                UnknownDescriptionRecord_00490E00 *record = 0;
                int type;
                if (!unknown_campaign_44->Get_Campaign_Type_0048e6b0(&type)) {
                    type = -1;
                } else {
                    unknown_campaigns_40->Get_Record_00490C50(type, &record);
                }
                int difficulty = -1;
                if (type == 0 || type == 1) {
                    difficulty = 0;
                } else if (type == 2 || type == 3) {
                    difficulty = 1;
                } else if (type == 4) {
                    difficulty = 2;
                }
                score->Presentation(scores, time, difficulty, type + 1, record->Name);
                delete score;
                while (scores.Count() != 0) {
                    if (scores[0] != 0) {
                        delete scores[0];
                    }
                    scores[0] = 0;
                    scores.Delete(0);
                }
            }
        }
    }
}


// void MPCooperative::unknown_slot37() -- 0x005A5720
// The next cooperative map: points the scenario at the campaign's current map, restores the
// session's players and settings slot 34 saved, sets the computer players from the campaign
// record, restarts the network timing from the players' pings and drops the connections.

extern "C" int __cdecl strncmp(char const *left, char const *right, unsigned int count);
extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
extern Wstring unknown_wstring_b26cb8;
extern unsigned int unknown_protocol_7fd370;
extern char unknown_b2a6d8[];
extern unsigned int unknown_b2ab84;
extern int unknown_pings_b2a734[];
extern IPXManagerClass unknown_ipx_manager_a40978;
extern OwnerTalkClass unknown_OwnerTalkClass_00ab6d18;
void Unknown_Pregame_Setup_005c74f0();
extern CCToolTip *ToolTips;

void MPCooperative::unknown_slot37()
{
    NetNumType net;
    NetNodeType node;
    unsigned short port;
    char *map;
    unknown_campaign_44->Get_Map_Name_0048fb40(&map, -1);
    char name[260];
    sprintf(name, "%s.MAP", map);
    strcpy(unknown_id_source_a3d290->NextScenario, name);
    unknown_id_source_a3d290->unknown_224c = -1;
    unknown_id_source_a3d290->SkipScore = true;
    int count = unknown_multi_missions_a3d8d8.Count();
    sprintf(name, "%s.MAP", map);
    for (int i = 0; i < count; i++) {
        if (strncmp(name, unknown_multi_missions_a3d8d8[i]->unknown_58, 0x104) == 0) {
            unknown_session_a3d298.ScenarioIndex = i;
            break;
        }
    }
    unknown_b2a7fc = 0;
    unknown_b2ab84 = unknown_protocol_7fd370;
    unknown_b2a7ec = 0;
    strcpy(unknown_b2a6d8, unknown_wstring_b26cb8.Get());
    wcscpy(unknown_session_a3d298.HostName_2380, unknown_host_name_78);
    Free_Pointers_005a7480(&unknown_players_a3fa7c);
    unknown_session_a3d55c = 0;
    for (int p = 0; p < unknown_players_5c.Count(); p++) {
        NodeNameType *entry = new NodeNameType;
        wcscpy(entry->Name, unknown_players_5c[p]->Name);
        entry->unknown_6b = unknown_players_5c[p]->unknown_6b;
        entry->unknown_4b = unknown_players_5c[p]->unknown_4b;
        entry->unknown_53 = unknown_players_5c[p]->unknown_53;
        entry->ID = unknown_players_5c[p]->ID;
        strcpy(entry->unknown_34, unknown_players_5c[p]->unknown_34);
        entry->unknown_4f = unknown_players_5c[p]->unknown_4f;
        entry->unknown_57 = unknown_players_5c[p]->unknown_57;
        entry->Team = unknown_players_5c[p]->Team;
        entry->unknown_63 = unknown_players_5c[p]->unknown_63;
        entry->unknown_67 = unknown_players_5c[p]->unknown_67;
        entry->unknown_6b = unknown_players_5c[p]->unknown_6b;
        entry->Address = unknown_players_5c[p]->Address;
        unknown_players_a3fa7c.Add(entry);
        unknown_session_a3d55c++;
    }
    int computers = unknown_multi_missions_a3d8d8[unknown_session_a3d298.ScenarioIndex]->unknown_184 - 2;
    if (computers > 0) {
        unknown_session_a3d298.AICount = computers;
        int used = 0;
        UnknownDescriptionRecord_00490E00 *record;
        unknown_campaigns_40->Get_Record_00490C50(unknown_340, &record);
        int current;
        unknown_campaign_44->Get_Current_Map_0048e6f0(&current);
        for (int slot = 2; slot < 8; slot++) {
            if (used < computers) {
                if (strcmp(record->AIName, "Normal") == 0) {
                    unknown_session_a3d298.AIDifficulty[slot] = 1;
                } else if (strcmp(record->AIName, "Hard") == 0) {
                    unknown_session_a3d298.AIDifficulty[slot] = 0;
                } else {
                    unknown_session_a3d298.AIDifficulty[slot] = 2;
                }
                unknown_session_a3d298.AICountry[slot] = unknown_1a8[slot][current];
                unknown_session_a3d298.AIColor[slot] = unknown_268[slot][current];
                used++;
            } else {
                unknown_session_a3d298.AICountry[slot] = -1;
                unknown_session_a3d298.AIColor[slot] = -1;
            }
        }
    }
    unknown_address_a3db2c = unknown_players_a3fa7c[unknown_58]->Address;
    wcscpy(unknown_session_a3d298.MasterPlayerName, unknown_players_a3fa7c[unknown_58]->Name);
    unknown_session_a3d298.MasterPlayerID = -1;
    unknown_session_a3d298.unknown_124 = unknown_74;
    PacketTransport->Clear_Broadcast_Addresses();
    char address[32] = "xxx.xxx.xxx.xxx";
    for (int n = 1; n < unknown_players_a3fa7c.Count(); n++) {
        unknown_players_a3fa7c[n]->Address.Get_Address(net, node, &port);
        PacketTransport->Set_Broadcast_Address(address, port);
    }
    unknown_session_a3d298.unknown_293c = unknown_c8;
    unknown_session_a3d298.unknown_299c = unknown_128;
    unknown_session_a3d298.unknown_2fa0 = unknown_148;
    unknown_session_a3d298.unknown_3022 = unknown_198;
    wcscpy(unknown_session_a3d298.MasterPlayerName, unknown_master_name_168);
    unknown_session_a3d298.MasterPlayerID = unknown_master_id_194;
    Seed = unknown_a0;
    unknown_house_setting_7d88d8 = unknown_a4;
    unknown_session_a3d298.UnitCount = unknown_units_a8;
    unknown_session_a3d298.unknown_24 = unknown_credits_ac;
    unknown_session_a3d298.unknown_fog_a7 = unknown_fog_b0;
    unknown_session_a3d298.unknown_28 = unknown_b1;
    unknown_session_a3d298.unknown_crates_29 = unknown_crates_b2;
    unknown_session_a3d298.unknown_a8 = unknown_b3;
    unknown_session_a3d298.unknown_a4 = unknown_b4;
    unknown_session_a3d298.unknown_a5 = unknown_b5;
    unknown_session_a3d298.unknown_bases_20 = unknown_bases_b6;
    unknown_session_a3d298.unknown_a6 = unknown_b7;
    unknown_session_a3d298.unknown_difficulty_40 = unknown_difficulty_bc;
    unknown_session_a3d298.unknown_2a = unknown_c0;
    unknown_session_a3d298.unknown_2b = unknown_c1;
    unknown_session_a3d298.unknown_2c = unknown_c2;
    unknown_session_a3d298.unknown_34 = unknown_c3;
    unknown_session_a3d298.GameSpeed_30 = unknown_game_speed_c4;
    Select_Scenario_005c7c30(unknown_session_a3d298.ScenarioIndex);
    Options.GameSpeed = unknown_session_a3d298.GameSpeed_30;
    unknown_id_source_a3d290->TechLevel = unknown_session_a3d298.ScenarioIndex;
    switch (unknown_session_a3d298.GameSpeed_30) {
    case 0:
        unknown_session_a3d298.MaxAhead_2c8 = 40;
        unknown_session_a3d298.unknown_2e8 = 60;
        unknown_session_a3d298.unknown_2cc = 10;
        break;
    case 1:
        unknown_session_a3d298.MaxAhead_2c8 = 40;
        unknown_session_a3d298.unknown_2e8 = 45;
        unknown_session_a3d298.unknown_2cc = 10;
        break;
    case 2:
        unknown_session_a3d298.MaxAhead_2c8 = 30;
        unknown_session_a3d298.unknown_2e8 = 30;
        break;
    case 3:
        unknown_session_a3d298.MaxAhead_2c8 = 20;
        unknown_session_a3d298.unknown_2e8 = 20;
        break;
    case 4:
        unknown_session_a3d298.MaxAhead_2c8 = 20;
        unknown_session_a3d298.unknown_2e8 = 15;
        break;
    case 5:
        unknown_session_a3d298.MaxAhead_2c8 = 20;
        unknown_session_a3d298.unknown_2e8 = 12;
        break;
    default:
        unknown_session_a3d298.MaxAhead_2c8 = 10;
        unknown_session_a3d298.unknown_2e8 = 10;
        break;
    }
    unknown_session_a3d298.unknown_2e4 = 0;
    if (!unknown_04) {
        unknown_session_a3d298.unknown_2e8 = 0;
    }
    int ping = 0;
    for (int k = 0; k < unknown_players_a3fa7c.Count(); k++) {
        if (unknown_pings_b2a734[k] > ping) {
            ping = unknown_pings_b2a734[k];
        }
    }
    unknown_session_a3d298.unknown_290c = 1;
    if (ping == 0 || ping == 1000) {
        unknown_session_a3d298.unknown_2e8 = 0;
        Debug_Printf("Ping times not available\n");
    } else {
        Debug_Printf("Max ping is %d ms\n", ping);
        if (ping > 1000) {
            unknown_session_a3d298.unknown_290c = 2;
        } else if (ping < 600) {
            unknown_session_a3d298.unknown_290c = 1;
        }
        int retry = ping * 60 / 1000;
        retry += retry * 120 / 100;
        unknown_ipx_manager_a40978.Set_Timing(retry, -1, retry * 8, true);
        if (unknown_04) {
            unknown_ipx_manager_a40978.unknown_slot18(5, -1, 120);
            if (unknown_04) {
                switch (unknown_session_a3d298.unknown_290c) {
                case 0:
                case 1:
                    unknown_session_a3d298.unknown_2e4 = unknown_session_a3d298.MaxAhead_2c8;
                    break;
                case 2:
                    unknown_session_a3d298.unknown_2e4 = unknown_session_a3d298.MaxAhead_2c8 / 2 + unknown_session_a3d298.MaxAhead_2c8;
                    break;
                case 3:
                    unknown_session_a3d298.unknown_2e4 = unknown_session_a3d298.MaxAhead_2c8 * 2;
                    break;
                }
                unknown_session_a3d298.unknown_2e4 = unknown_session_a3d298.unknown_2e4 / 10 * 10;
            }
        }
    }
    unknown_session_a3d298.unknown_2e0 = unknown_session_a3d298.MaxAhead_2c8;
    memset(player_statistics_00a3d584, 0, sizeof(player_statistics_00a3d584));
    unknown_squad_1_b2a820[0] = '\0';
    Options.GameSpeed = unknown_session_a3d298.GameSpeed_30;
    unknown_squad_2_b293d8[0] = '\0';
    Unknown_Pregame_Setup_005c74f0();
    DoList.Count = 0;
    DoList.Head = 0;
    DoList.Tail = 0;
    OutList.Count = 0;
    OutList.Head = 0;
    OutList.Tail = 0;
    ToolTips->Enable(false);
    Debug_Printf("@@@@ Before destroying connections players.count() = %d\n", unknown_players_a3fa7c.Count());
    while (unknown_ipx_manager_a40978.Num_Connections()) {
        unknown_ipx_manager_a40978.Delete_Connection(unknown_ipx_manager_a40978.Connection_ID(0));
    }
    if (unknown_flag_a3fba8) {
        if (unknown_connection_a409c4) {
            unknown_OwnerTalkClass_00ab6d18.Unknown_0063FC90();
        }
        unknown_flag_a3fba8 = false;
    }
    Debug_Printf("@@@@ After destroying connections players.count() = %d\n", unknown_players_a3fa7c.Count());
}


// void MPCooperative::unknown_slot38() -- 0x005A6040
// Rebuilds the session player list from the players slot 34 saved, keeping those whose name
// is a house's UIName (each saved player's ID becomes that house's index).

void MPCooperative::unknown_slot38()
{
    Free_Pointers_005a7480(&unknown_players_a3fa7c);
    unknown_session_a3d55c = 0;
    for (int i = 0; i < unknown_players_5c.Count(); i++) {
        for (int h = 0; h < Houses.Count(); h++) {
            if (_wcsicmp(Houses[h]->UIName, unknown_players_5c[i]->Name) == 0) {
                unknown_players_5c[i]->ID = h;
                NodeNameType *entry = new NodeNameType;
                wcscpy(entry->Name, unknown_players_5c[i]->Name);
                entry->unknown_6b = unknown_players_5c[i]->unknown_6b;
                entry->unknown_4b = unknown_players_5c[i]->unknown_4b;
                entry->unknown_53 = unknown_players_5c[i]->unknown_53;
                strcpy(entry->unknown_34, unknown_players_5c[i]->unknown_34);
                entry->unknown_4f = unknown_players_5c[i]->unknown_4f;
                entry->unknown_57 = unknown_players_5c[i]->unknown_57;
                entry->Team = unknown_players_5c[i]->Team;
                entry->unknown_63 = unknown_players_5c[i]->unknown_63;
                entry->unknown_67 = unknown_players_5c[i]->unknown_67;
                entry->unknown_6b = unknown_players_5c[i]->unknown_6b;
                NetNumType net;
                NetNodeType node;
                unsigned short socket;
                unknown_players_5c[i]->Address.Get_Address(net, node, &socket);
                entry->Address.Set_Address(net, node, socket);
                unknown_players_a3fa7c.Add(entry);
                unknown_session_a3d55c++;
                break;
            }
        }
    }
}


// bool MPCooperative::unknown_slot39(DynamicVectorClass<int> unknown_list, int *unknown_result) -- 0x005A6230
// Clears the result; true while a campaign exists.

bool MPCooperative::unknown_slot39(DynamicVectorClass<int> unknown_list, int *unknown_result)
{
    *unknown_result = 0;
    if (unknown_campaign_44 != 0) {
        return true;
    }
    return false;
}


// 0x005a6280: function not yet identified
//
// Owner: MPCooperative, virtual slot 1 (vtable +0x4) (from RTTI/vtables).
// Body: 3 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined1 FUN_005a6280(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005a6280 {

unsigned char  FUN_005a6280(void)
{
    return 1;
}

}  // namespace seed_005a6280


// bool MPCooperative::unknown_slot2() -- 0x005A6290

bool MPCooperative::unknown_slot2()
{
    return true;
}


class Leaf_5a62a0 { public: bool get(); };
bool Leaf_5a62a0::get() { return false; }


extern "C" void __cdecl release_5a62b0(void*);
class Leaf_5a62b0 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_5a62b0::get(unsigned int flags) { cleanup(); if (flags & 1) release_5a62b0(this); return this; }


extern "C" unsigned char vtable_5a6360[]; extern "C" void __cdecl release_buffer_5a6360(void*); extern "C" void __cdecl release_object_5a6360(void*);
class Leaf_5a6360 { public: void* get(unsigned int flags); };
void* Leaf_5a6360::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5a6360; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5a6360((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5a6360(this); return this; }


extern "C" unsigned char vtable_5a6bf0[]; extern "C" void __cdecl release_buffer_5a6bf0(void*); extern "C" void __cdecl release_object_5a6bf0(void*);
class Leaf_5a6bf0 { public: void* get(unsigned int flags); };
void* Leaf_5a6bf0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5a6bf0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5a6bf0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5a6bf0(this); return this; }


extern "C" unsigned char vtable_5a6c40[]; extern "C" void __cdecl release_buffer_5a6c40(void*); extern "C" void __cdecl release_object_5a6c40(void*);
class Leaf_5a6c40 { public: void* get(unsigned int flags); };
void* Leaf_5a6c40::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5a6c40; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5a6c40((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5a6c40(this); return this; }


extern "C" unsigned char vtable_5a6c90[]; extern "C" void __cdecl release_buffer_5a6c90(void*); extern "C" void __cdecl release_object_5a6c90(void*);
class Leaf_5a6c90 { public: void* get(unsigned int flags); };
void* Leaf_5a6c90::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5a6c90; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5a6c90((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5a6c90(this); return this; }


extern "C" unsigned char vtable_5a6ce0[]; extern "C" void __cdecl release_buffer_5a6ce0(void*); extern "C" void __cdecl release_object_5a6ce0(void*);
class Leaf_5a6ce0 { public: void* get(unsigned int flags); };
void* Leaf_5a6ce0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5a6ce0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5a6ce0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5a6ce0(this); return this; }


extern "C" unsigned char vtable_5a6d30[]; extern "C" void __cdecl release_buffer_5a6d30(void*); extern "C" void __cdecl release_object_5a6d30(void*);
class Leaf_5a6d30 { public: void* get(unsigned int flags); };
void* Leaf_5a6d30::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5a6d30; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5a6d30((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5a6d30(this); return this; }


extern "C" unsigned char table_5a6d80[]; extern "C" void* __cdecl storage_5a6d80(unsigned int);
class Storage_5a6d80 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_5a6d80::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_5a6d80; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_5a6d80(requested << 2); owns=1; } return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<MPlayerScoreType *>::Clear (0x005A63B0; DynamicVectorClass<MPlayerScoreType *> vtable 0x007a68e4 slot 3).
// DynamicVectorClass<MPlayerScoreType *>::ID_pointer (0x005A63E0; DynamicVectorClass<MPlayerScoreType *> vtable 0x007a68e4 slot 5).
// VectorClass<MPlayerScoreType *>::Fetch (0x005A6400; VectorClass<MPlayerScoreType *> vtable 0x007a68c4 slot 6).
// DynamicVectorClass<NodeNameType *>::Clear (0x005A6410; DynamicVectorClass<NodeNameType *> vtable 0x007a6864 slot 3).
// DynamicVectorClass<NodeNameType *>::ID_pointer (0x005A6440; DynamicVectorClass<NodeNameType *> vtable 0x007a6864 slot 5).
// VectorClass<NodeNameType *>::Fetch (0x005A6460; VectorClass<NodeNameType *> vtable 0x007a6884 slot 6).
// DynamicVectorClass<CoopCampaignClass *>::Clear (0x005A6470; DynamicVectorClass<CoopCampaignClass *> vtable 0x007a6844 slot 3).
// DynamicVectorClass<CoopCampaignClass *>::ID_pointer (0x005A64A0; DynamicVectorClass<CoopCampaignClass *> vtable 0x007a6844 slot 5).
// VectorClass<CoopCampaignClass *>::Fetch (0x005A64C0; VectorClass<CoopCampaignClass *> vtable 0x007a68a4 slot 6).
// VectorClass<MPlayerScoreType *>::operator equal (0x005A64D0; VectorClass<MPlayerScoreType *> vtable 0x007a68c4 slot 1).
// VectorClass<MPlayerScoreType *>::Resize (0x005A6510; VectorClass<MPlayerScoreType *> vtable 0x007a68c4 slot 2).
// VectorClass<MPlayerScoreType *>::Clear (0x005A65C0; VectorClass<MPlayerScoreType *> vtable 0x007a68c4 slot 3).
// VectorClass<MPlayerScoreType *>::ID_object (0x005A65F0; VectorClass<MPlayerScoreType *> vtable 0x007a68c4 slot 4).
// VectorClass<MPlayerScoreType *>::ID_pointer (0x005A6630; VectorClass<MPlayerScoreType *> vtable 0x007a68c4 slot 5).
// DynamicVectorClass<MPlayerScoreType *>::Resize (0x005A6650; DynamicVectorClass<MPlayerScoreType *> vtable 0x007a68e4 slot 2).
// DynamicVectorClass<MPlayerScoreType *>::ID_object (0x005A6700; DynamicVectorClass<MPlayerScoreType *> vtable 0x007a68e4 slot 4).
// VectorClass<NodeNameType *>::operator equal (0x005A6730; VectorClass<NodeNameType *> vtable 0x007a6884 slot 1).
// VectorClass<NodeNameType *>::Resize (0x005A6770; VectorClass<NodeNameType *> vtable 0x007a6884 slot 2).
// VectorClass<NodeNameType *>::Clear (0x005A6820; VectorClass<NodeNameType *> vtable 0x007a6884 slot 3).
// VectorClass<NodeNameType *>::ID_object (0x005A6850; VectorClass<NodeNameType *> vtable 0x007a6884 slot 4).
// VectorClass<NodeNameType *>::ID_pointer (0x005A6890; VectorClass<NodeNameType *> vtable 0x007a6884 slot 5).
// DynamicVectorClass<NodeNameType *>::Resize (0x005A68B0; DynamicVectorClass<NodeNameType *> vtable 0x007a6864 slot 2).
// DynamicVectorClass<NodeNameType *>::ID_object (0x005A6960; DynamicVectorClass<NodeNameType *> vtable 0x007a6864 slot 4).
// VectorClass<CoopCampaignClass *>::operator equal (0x005A6990; VectorClass<CoopCampaignClass *> vtable 0x007a68a4 slot 1).
// VectorClass<CoopCampaignClass *>::Resize (0x005A69D0; VectorClass<CoopCampaignClass *> vtable 0x007a68a4 slot 2).
// VectorClass<CoopCampaignClass *>::Clear (0x005A6A80; VectorClass<CoopCampaignClass *> vtable 0x007a68a4 slot 3).
// VectorClass<CoopCampaignClass *>::ID_object (0x005A6AB0; VectorClass<CoopCampaignClass *> vtable 0x007a68a4 slot 4).
// VectorClass<CoopCampaignClass *>::ID_pointer (0x005A6AF0; VectorClass<CoopCampaignClass *> vtable 0x007a68a4 slot 5).
// DynamicVectorClass<CoopCampaignClass *>::Resize (0x005A6B10; DynamicVectorClass<CoopCampaignClass *> vtable 0x007a6844 slot 2).
// DynamicVectorClass<CoopCampaignClass *>::ID_object (0x005A6BC0).
// DynamicVectorClass<CoopCampaignClass *>::ID_object (0x005A6BC0; DynamicVectorClass<CoopCampaignClass *> vtable 0x007a6844 slot 4).
// VectorClass<NodeNameType *>::VectorClass(unsigned size, NodeNameType * const *array) (0x005A6DD0; stores the
// VectorClass<NodeNameType *> vtable 0x007a6884, whose slot 0 is the deleting destructor 0x005A6C40). Layout and
// behaviour: shared/vector_class.hpp.


// VectorClass<CoopCampaignClass *>::VectorClass(unsigned int, CoopCampaignClass * const *) (0x005A6E20)
// Template constructor instantiated from shared/vector_class.hpp.
// No source of its own: emitted by the MultiplayerGameMode/MPCooperative unit (the MPCooperative constructor calls it).
