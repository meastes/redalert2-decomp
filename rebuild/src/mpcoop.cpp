/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the cooperative game mode of the multiplayer lobby: two players play the cooperative
**	campaigns of CoopCamp.ini together. TibSun has no such file; the code is the executable's
**	from 0x005A2610 (the static initializers of the geometry constants) to 0x005A6DD0.
*/

#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: the includes this file needs
#include "vector.h"
#include "mpcoop.h"
#include "ccini.h"
#include "ccfile.h"
#include "stringtable.h"
#include "session.h"
#include "globals.h"
#include "houstype.h"
#include "scenario.h"
#include "gadget.h"
#include "_rules.h"
#include "rules.h"
#include "mplayer.h"
#include "mplayerscore.h"
#include "mouse.h"
#include "wonline.h"
#include "ipxmgr.h"
#include "_WSProto.h"
#include "wsproto.h"
#include "goptions.h"
#include "_tooltip.h"
#include "tooltip.h"
#include "cctooltip.h"
#include "ownertalk.h"
#include "connect.h"
#include "queue.h"
#include "_map.h"
#include "house.h"
#include "netshare.h"
#include "windlg.h"
#include "dbgprint.h"
#include <windows.h>
#undef LoadString
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rect.h"


void __fastcall Fill_Side_Combos(HWND dialog); // RA2: 0x004D16D0 (netdlg2.cpp)
int __fastcall Player_Side_Combo_ID(int player); // RA2: 0x004D1350 (netdlg2.cpp)
void __fastcall Select_Side_Combo(HWND dialog, int id, int side); // RA2: 0x004D1A90 (netshare.cpp)
void __fastcall Hide_Slot_Controls(HWND dialog, int first); // RA2: 0x005CB170 (netshare.cpp), hides the controls of the slots from the first one on
char * __fastcall Wide_To_Narrow(wchar_t const * text); // RA2: 0x006FA120 (a static buffer)
void __fastcall Set_Item_Text(HWND dialog, MultiplayerGameMode * mode); // RA2: 0x005C3450 (netshare.cpp)
void __fastcall Set_Skirmish_Dialog_Text(HWND dialog); // RA2: 0x005C34B0 (netshare.cpp)
int Get_Selected_Scenario_Row(void); // RA2: 0x005C67A0 (netshare.cpp), the highlighted row of the map list

const UINT CB_ADDITEM = 0x4C2; // RA2: WM_USER + 194, the game's own combo box message that adds an item (which the owner-drawn lists answer)
const UINT LB_ADDITEM = 0x4CD; // RA2: WM_USER + 205, the game's own list box message that adds an item (which the owner-drawn lists answer)
const int IDC_SCENARIO_LIST = 0x553; // RA2: the list of maps of the lobby dialog that the game opens first
const UINT ST_SETTEXT = 0x4B2; // RA2: WM_USER + 178, the game's own message that sets the text of an owner-drawn control
static int PlayerColors[8]; // RA2: 0x00A71118, the colors taken while the colors of the players are picked
const int IDC_CAMPAIGN_DESCRIPTION = 0x72F; // RA2: the lobby's control that shows the description of the campaign

/// <summary>
/// Counts the players a map takes: the waypoints 0 to 7 it sets, else the players of a random map.
/// </summary>
int __fastcall Map_Player_Count(char const * filename) // RA2: 0x005A2800
{
	if (filename == NULL) {
		return(0);
	}
	CCFileClass file(filename);
	INIClass ini;
	if (!ini.Load(file, false)) {
		return(0);
	}
	int count = 0;
	for (int i = 0; i < 8; i++) {
		char key[32];
		sprintf(key, "%d", i);
		if (ini.Get_Int("Waypoints", key, -1) != -1) {
			count++;
		}
	}
	if (count == 0) {
		count = ini.Get_Int("RandomMap", "NumPlayers", 0);
	}
	return(count);
}

/// <summary>
/// Makes the mode: it reads CoopCamp.ini and makes a campaign object for every campaign in it.
/// </summary>
MPCooperative::MPCooperative(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex) : // RA2: 0x005A2960
	MultiplayerGameMode(title, tooltip, inifile, mapfilter, false, modeindex) // RA2: the computer players are never allowed
{
	Campaigns = new CoopCampListClass; // RA2: 0x00490510
	Campaign = NULL;
	Field48 = 0;
	CampaignIndex = 0;
	ScoreScreen = NULL;
	unknown_4 = false;
	SavedCampaignChosen = false;
	SaveListed = false;
	LoadedRecord = 0;
	PlayersChanged = true;
	for (int j = CampaignStates.Count() - 1; j >= 0; j--) {
		CoopCampaignClass * state = CampaignStates[j];
		if (state) {
			delete state;
		}
	}
	DynamicVectorClass<CoopCampaignClass *> * states = &CampaignStates; // RA2: the vector is reached through a pointer
	int max = states->Length();
	states->Clear();
	states->Resize(max, NULL);
	int count;
	Campaigns->Get_Count(&count);
	for (int i = 0; i < count; i++) {
		CoopCampaignClass * state = new CoopCampaignClass;
		state->Load_Campaign(i);
		states->Add(state);
	}
}

/// <summary>
/// Destroys the campaign list, the campaign and the campaign objects, the copies of the players and the score screen.
/// </summary>
MPCooperative::~MPCooperative(void) // RA2: 0x005A2B50
{
	if (Campaigns) {
		delete Campaigns;
		Campaigns = NULL;
	}
	if (Campaign) {
		delete Campaign;
		Campaign = NULL;
	}
	for (int j = CampaignStates.Count() - 1; j >= 0; j--) {
		CoopCampaignClass * state = CampaignStates[j];
		if (state) {
			delete state;
		}
	}
	CampaignStates.Clear();
	for (int i = SavedPlayers.Count() - 1; i >= 0; i--) {
		delete SavedPlayers[i];
	}
	int max = SavedPlayers.Length();
	SavedPlayers.Clear();
	SavedPlayers.Resize(max, NULL);
	if (ScoreScreen) {
		delete ScoreScreen;
		ScoreScreen = NULL;
	}
	unknown_4 = false;
}

/// <summary>
/// Tells whether the campaign lets a player pick a house: the houses the campaign lists are the allowed ones, and the random and observer choices are when it lists more than one.
/// </summary>
bool MPCooperative::IsCountryAllowed(int country) // RA2: 0x005A2CD0
{
	if (Campaigns == NULL) {
		return(true);
	}
	CoopCampEntry * record;
	Campaigns->Get_Record(CampaignIndex, &record);
	if (record == NULL) {
		return(true);
	}
	bool found = false;
	if (country == -2) {
		if (record->Players.Count() > 1) {
			return(true);
		}
	} else if (country == -3) {
		return(true);
	}
	for (int i = 0; i < record->Players.Count(); i++) {
		if (Find_Country_Index(record->Players[i]) == country) {
			found = true;
			break;
		}
	}
	return(found);
}

/// <summary>
/// Reads the campaign objects again: one for every campaign of CoopCamp.ini.
/// </summary>
bool MPCooperative::AssignTeams(void) // RA2: 0x005A2D70
{
	for (int j = CampaignStates.Count() - 1; j >= 0; j--) {
		CoopCampaignClass * state = CampaignStates[j];
		if (state) {
			delete state;
		}
	}
	DynamicVectorClass<CoopCampaignClass *> * states = &CampaignStates; // RA2: as in the constructor
	int max = states->Length();
	states->Clear();
	states->Resize(max, NULL);
	int count;
	Campaigns->Get_Count(&count);
	for (int i = 0; i < count; i++) {
		CoopCampaignClass * state = new CoopCampaignClass;
		state->Load_Campaign(i);
		states->Add(state);
	}
	return(true);
}

/// <summary>
/// Finds the campaign whose first map is the selected scenario.
/// </summary>
int MPCooperative::Find_Campaign_Index(void) // RA2: 0x005A2E50
{
	if (Campaigns == NULL) {
		return(-1);
	}
	int found = -1;
	for (int i = 0; i < Campaigns->Count; i++) {
		CoopCampEntry * record;
		Campaigns->Get_Record(i, &record);
		for (int map = 0; map < 3; map++) {
			if (Session.Options.ScenarioIndex != 0) {
				if (strnicmp(Session.Scenarios[Session.Options.ScenarioIndex]->Get_Filename(), &record->Maps[map * _MAX_PATH], strlen(&record->Maps[map * _MAX_PATH])) == 0) {
					found = i;
				}
			}
		}
	}
	return(found);
}

/// <summary>
/// Called when the lobby opens: tells the host there is no observer (O8), keeps or makes the campaign object and selects its campaign and first map.
/// </summary>
void MPCooperative::vt_entry_20(void) // RA2: 0x005A2EF0
{
	if (Campaigns == NULL) {
		return;
	}
	LobbyObserverSlot = -1;
	DebugString("Sending observer slot of 8");
	Send_Host_Option("O8");
	if (WS_Find_Dialog(0xC2) != NULL || WS_Find_Dialog(0xBC) != NULL) {
		if (Campaign != NULL) {
			int type;
			Campaign->Get_Campaign_Type(&type);
			if ((Find_Campaign_Index() != type && WS_Top_Window_ID() == 0x6B) || WS_Top_Window_ID() != 0x6B) {
				delete Campaign;
				Campaign = NULL;
			}
		}
		if (Campaign != NULL) {
			if (Session.Players.Count() == 2) {
				Campaign->Set_Names(Session.Players[0]->Name, Session.Players[1]->Name);
			} else {
				Campaign->Set_Names(L"", L"");
			}
		}
	}
	if (Campaign == NULL) {
		Campaign = new CoopCampaignClass;
		if (Campaign == NULL) {
			return;
		}
		Campaign->Load_Campaign(0);
		int index = Find_Campaign_Index();
		if (index < 0) {
			CampaignIndex = 0;
			char * name = NULL;
			Campaign->Get_Map_Name(&name, 0);
			int i;
			for (i = 0; i < Session.Scenarios.Count(); i++) {
				if (strnicmp(Session.Scenarios[i]->Get_Filename(), name, strlen(name)) == 0) {
					Session.Options.ScenarioIndex = i;
					Set_Scenario_Info_From_Index(i);
					break;
				}
			}
			if (i == Session.Scenarios.Count()) {
				Session.Options.ScenarioIndex = 0;
				Set_Scenario_Info_From_Index(0);
			}
		} else {
			CampaignIndex = index;
			CoopCampEntry * record;
			Campaigns->Get_Record(index, &record);
			char * name = NULL;
			Campaign->Get_Map_Name(&name, 0);
			DebugString("@@@ Looking for %s", Session.Scenarios[Session.Options.ScenarioIndex]->Get_Filename());
			if (strnicmp(Session.Scenarios[Session.Options.ScenarioIndex]->Get_Filename(), name, strlen(name)) != 0) {
				int other = (index == 0);
				do {
					Campaign->Load_Campaign(other);
					Campaign->Load_Campaign(index);
					Campaign->Get_Map_Name(&name, 0);
				} while (strnicmp(Session.Scenarios[Session.Options.ScenarioIndex]->Get_Filename(), name, strlen(name)) != 0);
			}
		}
	}
	PlayersChanged = true;
	int type;
	HWND dialog = GameoptWindow();
	if (dialog != NULL) {
		Refresh_Player_Sides(dialog);
		HWND item = GetDlgItem(dialog, IDC_CAMPAIGN_DESCRIPTION);
		if (item != NULL && Campaign != NULL && Campaign->Get_Campaign_Type(&type)) {
			char label[64];
			sprintf(label, "NAME:COOPDESC%d", type + 1);
			SendMessageA(item, ST_SETTEXT, 0, (LPARAM)StringTable::LoadString(label, NULL, "D:\\Ra2\\MPCoop.cpp", 0x157));
			ShowWindow(item, SW_SHOW);
		}
	}
}

/// <summary>
/// Marks the lobby's player section as changed.
/// </summary>
void MPCooperative::vt_entry_24(void) // RA2: 0x005A31F0
{
	PlayersChanged = true;
}

/// <summary>
/// Makes the campaign object for the first campaign and returns the index of the scenario that is its first map.
/// </summary>
int MPCooperative::FirstValidMapIndex(void) // RA2: 0x005A3200
{
	int count = 0;
	char * name = NULL;
	if (Campaigns == NULL) {
		Campaigns = new CoopCampListClass;
	}
	Campaigns->Get_Count(&count);
	if (count != 0) {
		if (Campaign == NULL) {
			Campaign = new CoopCampaignClass;
		}
		Campaign->Load_Campaign(0);
		Campaign->Get_Map_Name(&name, 0);
		for (int i = 0; i < Session.Scenarios.Count(); i++) {
			if (strnicmp(Session.Scenarios[i]->Get_Filename(), name, strlen(name)) == 0) {
				return(i);
			}
		}
		AssignTeams();
	}
	return(-1);
}

/// <summary>
/// Gives the campaign the names of the two players, then lets the base class check.
/// </summary>
bool MPCooperative::CheckStartConditions(WideWstring * message) // RA2: 0x005A32E0
{
	if (Session.Players.Count() == 2 && Campaign != NULL) {
		Campaign->Set_Names(Session.Players[0]->Name, Session.Players[1]->Name);
	}
	return(MultiplayerGameMode::CheckStartConditions(message));
}

/// <summary>
/// Picks the campaign (or the saved one) the host selected and gives it the players; a player whose house the campaign does not allow gets a random or the only allowed one.
/// </summary>
bool MPCooperative::AssignTeamsAtStart(void) // RA2: 0x005A3320
{
	if (Campaigns == NULL || Campaign == NULL) {
		return(false);
	}
	int count;
	Campaigns->Get_Count(&count);
	int selected = Get_Selected_Scenario_Row();
	if (selected >= count) {
		if (Session.Players.Count() == 2) {
			Campaign->Set_Names(Session.Players[0]->Name, Session.Players[1]->Name);
		}
		if (!Campaign->Load_Saved_Campaign()) {
			return(false);
		}
		SavedCampaignChosen = true;
	} else if (WS_Top_Window_ID() == 0x6B) {
		delete Campaign;
		Campaign = CampaignStates[selected];
		CampaignStates[selected] = new CoopCampaignClass;
		CampaignStates[selected]->Load_Campaign(selected);
		LoadedRecord = selected;
		if (Session.Players.Count() == 2) {
			Campaign->Set_Names(Session.Players[0]->Name, Session.Players[1]->Name);
		}
	}
	CoopCampEntry * record;
	Campaigns->Get_Record(CampaignIndex, &record);
	if (record == NULL) {
		return(false);
	}
	int players = record->Players.Count();
	if (players == 0) {
		return(false);
	}
	Refresh_Player_Sides(GameoptWindow());
	for (int i = 0; i < Session.Players.Count(); i++) {
		if (!IsCountryAllowed(Session.Players[i]->Get_Country())) {
			if (players > 1) {
				Session.Players[i]->Set_Country(-2);
			} else {
				int house = Find_Country_Index(record->Players[0]);
				Session.Players[i]->Set_Country(house);
			}
		}
	}
	return(true);
}

/// <summary>
/// The house a player starts with: the only one the campaign lists, else random.
/// </summary>
int MPCooperative::DefaultCountry(void) // RA2: 0x005A34B0
{
	CoopCampEntry * record;
	Campaigns->Get_Record(CampaignIndex, &record);
	if (record == NULL) {
		return(-2);
	}
	if (record->Players.Count() == 0) {
		return(-2);
	}
	if (record->Players.Count() > 1) {
		return(-2);
	}
	return(Find_Country_Index(record->Players[0]));
}

/// <summary>
/// Takes the campaign settings the host sent: the campaign, the choices of houses and colors, and the names of the players.
/// </summary>
bool MPCooperative::vt_entry_48(int index, DynamicVectorClass<int> picks, char const * map, char const * name) // RA2: 0x005A3500
{
	if (Campaign == NULL) {
		Recreate_Campaign();
		PlayersChanged = true;
	}
	if (Campaign != NULL && Campaigns != NULL) {
		int type;
		Campaign->Get_Campaign_Type(&type);
		if (type != index) {
			PlayersChanged = true;
		}
		Campaign->Set_Campaign_Info(*Campaigns, index, picks, map, name);
		CampaignIndex = index;
		return(true);
	}
	return(false);
}

/// <summary>
/// Adds the scores of the two players to the campaign's.
/// </summary>
bool MPCooperative::vt_entry_4C(unsigned index, unsigned time, MPlayerScoreType * scores) // RA2: 0x005A35D0
{
	if (Campaign == NULL) {
		return(false);
	}
	HWND lobby = GameoptWindow();
	if (lobby != NULL && PlayersChanged) {
		Refresh_Player_Sides(lobby);
		PlayersChanged = false;
	}
	return(Campaign->Add_Score(index, time, scores[0], scores[1]));
}

/// <summary>
/// Fills a side combo box with the houses the campaign lets the players pick (and the random choice when there is more than one).
/// </summary>
void MPCooperative::Fill_House_Combo(HWND dialog, int id) // RA2: 0x005A3650
{
	int count = HouseTypes.Count();
	if (count != 0) {
		HWND combo = GetDlgItem(dialog, id);
		ShowWindow(combo, SW_HIDE);
		SendMessageA(combo, CB_RESETCONTENT, 0, 0);
		CoopCampEntry * record;
		Campaigns->Get_Record(CampaignIndex, &record);
		if (record != NULL) {
			int players = record->Players.Count();
			if (players != 0) {
				if (players > 1) {
					wchar_t const * name = StringTable::LoadString("GUI:Random", NULL, "D:\\Ra2\\MPCoop.cpp", 0x225);
					LRESULT index = SendMessageA(combo, CB_ADDITEM, 0, (LPARAM)name);
					SendMessageA(combo, CB_SETITEMDATA, index, -2);
				}
				for (int i = 0; i < players; i++) {
					int house = Find_Country_Index(record->Players[i]);
					if (house >= 0 && house < count) {
						wchar_t const * name = HouseTypes[house]->UIName;
						LRESULT index = SendMessageA(combo, CB_ADDITEM, 0, (LPARAM)name);
						SendMessageA(combo, CB_SETITEMDATA, index, house);
					}
				}
				SendMessageA(combo, CB_SETCURSEL, 0, 0);
				ShowWindow(combo, SW_SHOW);
			}
		}
	}
}

/// <summary>
/// Fills the side combo boxes of the player slots with the campaign's houses and selects the only house, or the random choice; hides the slots that are not used and shows the campaign's description.
/// </summary>
void MPCooperative::Refresh_Player_Sides(HWND dialog) // RA2: 0x005A3760
{
	if (dialog != NULL) {
		int count = Session.ScenarioMaxPlayers;
		CoopCampEntry * record;
		Campaigns->Get_Record(CampaignIndex, &record);
		if (record != NULL) {
			int players = record->Players.Count();
			if (players != 0) {
				for (int i = 0; i < count; i++) {
					int id = Player_Side_Combo_ID(i);
					Fill_House_Combo(dialog, id);
					if (players > 1) {
						Select_Side_Combo(dialog, id, -2);
						Session.Slots[i]->Set_Country(-2);
					} else {
						int house = Find_Country_Index(record->Players[0]);
						Select_Side_Combo(dialog, id, house);
						Session.Slots[i]->Set_Country(house);
					}
				}
				Hide_Slot_Controls(dialog, count);
				HWND lobby = GameoptWindow();
				int type;
				if (lobby != NULL && Campaign != NULL && Campaign->Get_Campaign_Type(&type)) {
					char label[64];
					sprintf(label, "NAME:COOPDESC%d", type + 1);
					HWND item = GetDlgItem(lobby, IDC_CAMPAIGN_DESCRIPTION);
					if (item != NULL) {
						SendMessageA(item, ST_SETTEXT, 0, (LPARAM)StringTable::LoadString(label, NULL, "D:\\Ra2\\MPCoop.cpp", 0x273));
						ShowWindow(item, SW_SHOW);
						InvalidateRect(item, NULL, FALSE);
					}
				}
			}
		}
	}
}

/// <summary>
/// Sends the campaign's options to the other player: the selected campaign is taken from the campaign object and the player section is filled again when it changed.
/// </summary>
bool MPCooperative::vt_entry_50(void) // RA2: 0x005A38D0
{
	if (Campaign == NULL) {
		return(false);
	}
	Campaign->Get_Campaign_Type(&CampaignIndex);
	HWND lobby = GameoptWindow();
	if (lobby != NULL && PlayersChanged) {
		Refresh_Player_Sides(lobby);
		PlayersChanged = false;
	}
	return(Campaign->Send_Campaign_Options(*Campaigns));
}

/// <summary>
/// Sets the colors of the eight slots to random (-2) and then the colors the players of the session chose.
/// </summary>
void MPCooperative::Fill_Player_Colors(void) // RA2: 0x005A3920; the executable expands it in vt_entry_54 and keeps this copy
{
	int i;
	for (i = 0; i < 8; i++) {
		PlayerColors[i] = -2;
	}
	int count = Session.Players.Count();
	for (i = 0; i < count; i++) {
		PlayerColors[i] = Session.Players[i]->Get_Color_Or_Random();
	}
}

/// <summary>
/// Chooses the houses and colors of the players and of the computer players for every map of the campaign and sends them to the other player as the game option C1.
/// </summary>
bool MPCooperative::vt_entry_54(void) // RA2: 0x005A3960
{
	CoopCampEntry * record;
	Campaigns->Get_Record(CampaignIndex, &record);
	char message[400];
	sprintf(message, "C1,%d", record->MapCount);
	for (int list = 0; list < 8; list++) {
		int length = HouseChoices[list].Length();
		HouseChoices[list].Clear();
		HouseChoices[list].Resize(length);
		length = ColorChoices[list].Length();
		ColorChoices[list].Clear();
		ColorChoices[list].Resize(length);
	}
	int players = Session.Players.Count();
	for (int map = 0; map < record->MapCount; map++) {
		Fill_Player_Colors();
		for (int slot = 0; slot < 8; slot++) {
			if (slot < players) {
				HouseChoices[slot].Add(RandomHumanCountryIndex());
				ColorChoices[slot].Add(PlayerColors[slot]);
			} else {
				HouseChoices[slot].Add(RandomAICountryIndex());
				int color;
				bool taken;
				do {
					color = Scen->RandomNumber(0, 7);
					taken = false;
					if (color != -2) {
						for (int i = 0; i < 8; i++) {
							if (PlayerColors[i] == color) {
								taken = true;
								break;
							}
						}
					}
				} while (taken);
				PlayerColors[slot] = color;
				ColorChoices[slot].Add(color);
			}
			char text[16];
			sprintf(text, ",%d,%d", HouseChoices[slot][map], ColorChoices[slot][map]);
			strcat(message, text);
		}
	}
	DebugString("@@@@ Sending campaign game AI settings: %s\n", message);
	Send_Host_Option(message);
	DebugString("@@@@ Sent\n");
	return(true);
}

/// <summary>
/// Takes the game option C1 the host sent: the number of maps and the house and color of every slot for each of them.
/// </summary>
/// <param name="kind">The digit after the C of the option.</param>
/// <param name="text">The rest of the option, split at the commas.</param>
void MPCooperative::vt_entry_64(unsigned kind, char * text) // RA2: 0x005A3CA0
{
	if (kind == 1) {
		int rounds = atol(strtok(text, ","));
		int i;
		for (i = 0; i < 8; i++) {
			int capacity = HouseChoices[i].Length();
			HouseChoices[i].Clear();
			HouseChoices[i].Resize(capacity);
			capacity = ColorChoices[i].Length();
			ColorChoices[i].Clear();
			ColorChoices[i].Resize(capacity);
		}
		for (int round = 0; round < rounds; round++) {
			for (i = 0; i < 8; i++) {
				char * token = strtok(NULL, ",");
				if (token != NULL) {
					HouseChoices[i].Add(atol(token));
				} else {
					HouseChoices[i].Add(-1);
				}
				token = strtok(NULL, ",");
				if (token != NULL) {
					ColorChoices[i].Add(atol(token));
				} else {
					ColorChoices[i].Add(-1);
				}
			}
		}
	}
}

/// <summary>
/// Shows the description of the selected campaign in the lobby.
/// </summary>
bool MPCooperative::UnfixAlliances(void) // RA2: 0x005A3EE0
{
	HWND dialog = GameoptWindow();
	if (dialog != NULL && Campaign != NULL) {
		int type;
		if (Campaign->Get_Campaign_Type(&type)) {
			char label[64];
			sprintf(label, "NAME:COOPDESC%d", type + 1);
			HWND item = GetDlgItem(dialog, IDC_CAMPAIGN_DESCRIPTION);
			if (item != NULL) {
				SendMessageA(item, ST_SETTEXT, 0, (LPARAM)StringTable::LoadString(label, NULL, "D:\\Ra2\\MPCoop.cpp", 0x31E));
				ShowWindow(item, SW_SHOW);
			}
		}
	}
	return(true);
}

/// <summary>
/// The teams of the dropdown are the base class's.
/// </summary>
void MPCooperative::PopulateTeamDropdownForPlayer(HWND hwnd, int index) // RA2: 0x005A3F70
{
	MultiplayerGameMode::PopulateTeamDropdownForPlayer(hwnd, index);
}

/// <summary>
/// Picks the starting cell of a house and marks it taken: the first house gets a random cell, the second the free cell nearest to the first, and every other the free cell farthest from the taken ones.
/// </summary>
/// <param name="result">Where the cell is returned.</param>
/// <param name="house">The index of the house.</param>
/// <param name="coords">The starting cells.</param>
/// <param name="satisfied">One flag per starting cell: taken or not.</param>
Cell * MPCooperative::AssignStartingPositionsToHouse(Cell * result, int house, DynamicVectorClass<Cell> * coords, unsigned char * satisfied) // RA2: 0x005A3F90
{
	int count = 0;
	int i;
	for (i = 0; i < coords->Count(); i++) {
		if (satisfied[i]) {
			count++;
		}
	}
	int index;
	if (count == 0) {
		index = Scen->RandomNumber(0, coords->Count() - 1);
		satisfied[index] = true;
		*result = (*coords)[index];
		return(result);
	} else {
		int nearest = -1;
		int nearest_index = -1;
		if (count == 1) {
			for (i = 0; i < coords->Count(); i++) {
				if (!satisfied[i]) {
					int sum = 0;
					for (int j = 0; j < coords->Count(); j++) {
						if (satisfied[j]) {
							sum += (*coords)[i].Distance_To((*coords)[j]);
						}
					}
					if (sum < nearest || nearest < 0) {
						nearest = sum;
						nearest_index = i;
					}
				}
			}
			satisfied[nearest_index] = true;
			*result = (*coords)[nearest_index];
			return(result);
		} else {
			int farthest = -1;
			int farthest_index = -1;
			for (i = 0; i < coords->Count(); i++) {
				if (!satisfied[i]) {
					int sum = 0;
					for (int j = 0; j < coords->Count(); j++) {
						if (satisfied[j]) {
							sum += (*coords)[i].Distance_To((*coords)[j]);
						}
					}
					if (sum > farthest) {
						farthest = sum;
						farthest_index = i;
					}
				}
			}
			index = farthest_index;
		}
	}
	satisfied[index] = true;
	*result = (*coords)[index];
	return(result);
}

/// <summary>
/// Gives every house that takes part a starting cell, which becomes the center of its base.
/// </summary>
bool MPCooperative::StartingPositionsToHouseBaseCells(char unused) // RA2: 0x005A4170
{
	DynamicVectorClass<Cell> cells = Build_Start_Waypoint_List(unused);
	unsigned char taken[16];
	memset(taken, 0, sizeof(taken));
	for (int i = 0; i < Houses.Count(); i++) {
		HouseClass * house = Houses[i];
		if (!house->Class->IsMultiplayPassive) {
			DebugString("Assigning Starting Points for house %d (%s)\n", i, house->Class->Name());
			Cell cell;
			Cell chosen = *AssignStartingPositionsToHouse(&cell, i, &cells, taken);
			house->Center = Coord(chosen);
		}
	}
	return(true);
}

/// <summary>
/// Sets up the computer players of the selected map: all the slots from the third on that the map has players for, with the difficulty the campaign names.
/// </summary>
bool MPCooperative::vt_entry_58(void) // RA2: 0x005A4250
{
	int players = RandomMapWaypointCount(Session.Options.ScenarioIndex) - 2;
	DebugString("@@@@ Adding %d AI players\n", players);
	if (players <= 0) {
		return(false);
	}
	int added = 0;
	Session.Options.AIPlayers = players;
	CoopCampEntry * record;
	Campaigns->Get_Record(CampaignIndex, &record);
	if (record == NULL) {
		return(false);
	}
	if (record->Enemies.Count() == 0) {
		return(false);
	}
	for (int slot = 2; slot < 8; slot++) {
		if (added < players) {
			DebugString("@@@@ Setting slot %d for AI\n", slot);
			char * name = record->AIName;
			if (strcmp(name, "Normal") == 0) {
				Session.Options.AISlots.Difficulties[slot] = 1;
			} else if (strcmp(name, "Hard") == 0) {
				Session.Options.AISlots.Difficulties[slot] = 0;
			} else {
				Session.Options.AISlots.Difficulties[slot] = 2;
			}
			DebugString("@@@@     AI difficulty: %d\n", Session.Options.AISlots.Difficulties[slot]);
			Session.Options.AISlots.Countries[slot] = -2;
			Session.Options.AISlots.Colors[slot] = -2;
			added++;
		} else {
			Session.Options.AISlots.Difficulties[slot] = -1;
			Session.Options.AISlots.Countries[slot] = -1;
			Session.Options.AISlots.Colors[slot] = -1;
		}
	}
	return(true);
}

/// <summary>
/// Makes allies of all the human houses.
/// </summary>
bool MPCooperative::AllyTeams(void) // RA2: 0x005A43C0
{
	DebugString("@@@@ Making Alliances\n");
	for (int i = 0; i < Houses.Count(); i++) {
		HouseClass * first = Houses[i];
		DebugString("@@@@     House1 = %s\n", Wide_To_Narrow(first->UIName));
		if (!first->Class->IsMultiplayPassive && first->IsHuman) {
			for (int j = i + 1; j < Houses.Count(); j++) {
				HouseClass * second = Houses[j];
				DebugString("@@@@         House2 = %s\n", Wide_To_Narrow(second->UIName));
				if (!second->Class->IsMultiplayPassive && second->IsHuman) {
					char * second_name = Wide_To_Narrow(second->UIName);
					char * first_name = Wide_To_Narrow(first->UIName);
					DebugString("@@@@ Making Allies of %s(%d/%d/%d) and %s(%d/%d/%d)\n",
						first_name, first->Class->House, first->Class->Side, first->Class->Scheme,
						second_name, second->Class->House, second->Class->Side, second->Class->Scheme);
					first->Make_Ally(second, false);
					second->Make_Ally(first, false);
				}
			}
		}
	}
	return(true);
}

/// <summary>
/// Picks a random house of the ones the campaign lists for the players.
/// </summary>
int MPCooperative::RandomHumanCountryIndex(void) // RA2: 0x005A4540
{
	int count = HouseTypes.Count();
	bool found = false;
	int campaign = 0;
	Campaign->Get_Campaign_Type(&campaign);
	CoopCampEntry * record;
	Campaigns->Get_Record(campaign, &record);
	int house;
	do {
		house = Scen->RandomNumber(0, count - 1);
		for (int i = 0; i < record->Players.Count(); i++) {
			if (house == Find_Country_Index(record->Players[i])) {
				found = true;
			}
		}
	} while (!found);
	DebugString("@@@@ Choosing a random player house: %d(%s)\n", house, Wide_To_Narrow(HouseTypes[house]->UIName));
	return(house);
}

/// <summary>
/// Picks a random house of the ones the campaign lists for the computer players.
/// </summary>
int MPCooperative::RandomAICountryIndex(void) // RA2: 0x005A45F0
{
	int count = HouseTypes.Count();
	bool found = false;
	int campaign = 0;
	Campaign->Get_Campaign_Type(&campaign);
	CoopCampEntry * record;
	Campaigns->Get_Record(campaign, &record);
	int house;
	do {
		house = Scen->RandomNumber(0, count - 1);
		for (int i = 0; i < record->Enemies.Count(); i++) {
			if (house == Find_Country_Index(record->Enemies[i])) {
				found = true;
			}
		}
	} while (!found);
	DebugString("@@@@ Choosing a random AI house: %d(%s)\n", house, Wide_To_Narrow(HouseTypes[house]->UIName));
	return(house);
}

/// <summary>
/// The start of the game is the base class's.
/// </summary>
bool MPCooperative::OnGameStart(void) // RA2: 0x005A46A0
{
	return(MultiplayerGameMode::OnGameStart());
}

/// <summary>
/// The mode never allows computer players (it sets up its own).
/// </summary>
bool MPCooperative::IsAIAllowed(void) // RA2: 0x005A46B0
{
	return(false);
}

/// <summary>
/// Called when a map is over: when a human player is still alive the campaign goes on to its next map and is saved; the players and the settings of the lobby are kept so that the lobby can be made again.
/// </summary>
/// <param name="player_wins">Whether the local player won.</param>
void MPCooperative::vt_entry_88(bool player_wins) // RA2: 0x005A46C0
{
	bool win = false;
	for (int i = 0; i < Houses.Count(); i++) {
		if (Houses[i] != NULL && Houses[i]->Is_Human_Player() && !Houses[i]->IsDefeated) {
			win = true;
		}
	}
	DebugString("@@@@ End-of-Game: PlayerWins:%d, win:%d\n", player_wins, win);
	if (win) {
		int scheme = GadgetClass::Get_Color_Scheme();
		if (Campaign != NULL) {
			Campaign->Next_Map();
			if (Campaign->Save_Campaign()) {
				Session.Messages.Add_Message(NULL, 0, StringTable::LoadString("TXT_COOP_SAVE_SUCCESS", NULL, "D:\\Ra2\\MPCoop.cpp", 0x42E), scheme, TextPrintType(TPF_6PT_GRAD | TPF_NOSHADOW), (int)(Rule->MessageDelay * 900.0), false);
			} else {
				Session.Messages.Add_Message(NULL, 0, StringTable::LoadString("TXT_COOP_SAVE_FAILED", NULL, "D:\\Ra2\\MPCoop.cpp", 0x430), scheme, TextPrintType(TPF_6PT_GRAD | TPF_NOSHADOW), (int)(Rule->MessageDelay * 900.0), false);
			}
		}
	}
	wcscpy(HostName, Session.GameName);
	Scen->IsSkipScore = true;
	Scen->IsEndOfGame = false;
	Scen->IsNoMapSel = true;
	Scen->IsOneTimeOnly = false;
	Clear_Vector(&SavedPlayers);
	LocalPlayer = 0;
	for (int p = 0; p < Session.Players.Count(); p++) {
		NodeNameType * entry = new NodeNameType;
		wcscpy(entry->Name, Session.Players[p]->Name);
		entry->Slot.Clan = Session.Players[p]->Slot.Clan;
		entry->Slot.Country = Session.Players[p]->Slot.Country;
		entry->Slot.ColorIdx = Session.Players[p]->Slot.ColorIdx;
		entry->Slot.HouseIndex = Session.Players[p]->Slot.HouseIndex;
		strcpy(entry->Player.Serial, Session.Players[p]->Player.Serial);
		entry->Slot.InitialCountry = Session.Players[p]->Slot.InitialCountry;
		entry->Slot.InitialColor = Session.Players[p]->Slot.InitialColor;
		entry->Slot.SpectatorFlag = Session.Players[p]->Slot.SpectatorFlag;
		entry->Slot.Time = Session.Players[p]->Slot.Time;
		entry->Slot.Unknown77 = Session.Players[p]->Slot.Unknown77;
		entry->Slot.Clan = Session.Players[p]->Slot.Clan;
		entry->Address = Session.Players[p]->Address;
		SavedPlayers.Add(entry);
		if (Session.HostAddress == Session.Players[p]->Address) {
			LocalPlayer = p;
		}
	}
	memcpy(Addresses, Session.SlotAddresses, sizeof(Addresses));
	memcpy(PlayerStates, Session.WolPlayerState, sizeof(PlayerStates));
	memcpy(PlayerPortBases, Session.PlayerPortBase, sizeof(PlayerPortBases));
	memcpy(PlayerPorts, Session.PlayerPorts, sizeof(PlayerPorts));
	wcscpy(MasterName, Session.MasterPlayerName);
	MasterID = Session.MasterPlayerID;
	SavedField74 = Session.House;
	Seed = ::Seed;
	TechLevel = BuildLevel;
	UnitCount = Session.Options.UnitCount;
	Credits = Session.Options.Credits;
	FogOfWar = Session.Options.FogOfWar;
	BridgeDestruction = Session.Options.BridgeDestruction;
	Goodies = Session.Options.Goodies;
	MCVRedeploy = Session.Options.MCVRedeploy;
	AlliesAllowed = Session.Options.AlliesAllowed;
	HarvTruce = Session.Options.HarvTruce;
	Bases = Session.Options.Bases;
	CTF = Session.Options.CTF;
	AIPlayers = Session.Options.AIPlayers;
	AIDifficulty = Session.Options.AIDifficulty;
	ShortGame = Session.Options.ShortGame;
	SWAllowed = Session.Options.SWAllowed;
	BuildOffAlly = Session.Options.BuildOffAlly;
	CrapEngineers = Session.Options.CrapEngineers;
	GameSpeed = Session.Options.GameSpeed;
	Session.MultiplayerObserver = false;
	Map.Set_Cursor_Shape(NULL);
	Map.PendingObjectPtr = 0; // RA2: nothing is being placed any more
	Map.PendingObject = 0;
	Map.PendingHouse = HOUSE_NONE;
	for (int row = 0; row < 4; row++) {
		Session.Score[row].Kills[Session.CurGame] = -1; // RA2: the first four score records have no kills for the game that follows
	}
}

/// <summary>
/// Shows the lobby's list of campaigns and the campaign object of the two players. The map list holds one row for each campaign, and one for the saved campaign when there is one.
/// </summary>
/// <param name="list">The map list box.</param>
/// <param name="maps">The scenarios the list is made of.</param>
/// <param name="map">The selected scenario.</param>
void MPCooperative::DrawTeamDropdown(HWND list, DynamicVectorClass<MultiMission const *> * maps, MultiMission const * map) // RA2: 0x005A4B70
{
	unknown_4 = true;
	if (ScoreScreen == NULL) {
		ScoreScreen = new CampaignScoreClass;
	}
	SendMessageA(list, LB_RESETCONTENT, 0, 0);
	SaveListed = false;
	if (Campaign == NULL) {
		if (Session.Players.Count() == 2) {
			Campaign = new CoopCampaignClass(Session.Players[0]->Name, Session.Players[1]->Name);
		}
	} else if (Session.Players.Count() != 2) {
		Campaign->Set_Names(L"", L"");
	}
	if (CampaignStates.Count() == 0) {
		int records;
		Campaigns->Get_Count(&records);
		for (int r = 0; r < records; r++) {
			CoopCampaignClass * state = new CoopCampaignClass;
			state->Load_Campaign(r);
			CampaignStates.Add(state);
		}
	}
	DynamicVectorClass<wchar_t *> names;
	Campaigns->Copy_Names(names);
	int count = names.Length();
	int i;
	for (i = 0; i < count; i++) {
		if (names[i] != NULL) {
			char * mapname;
			CampaignStates[i]->Get_Map_Name(&mapname, -1);
			int map_count = maps->Count();
			char name[260];
			MultiMission const * found = NULL;
			sprintf(name, "%s.MAP", mapname);
			for (int k = 0; k < map_count; k++) {
				if (found != NULL) {
					break;
				}
				char const * scenario = (*maps)[k]->Get_Filename();
				if (stricmp(scenario, name) == 0) {
					found = (*maps)[k];
				}
			}
			if (found != NULL) {
				wchar_t text[260];
				swprintf(text, L"%d. %s", i + 1, names[i]);
				SendMessageA(list, LB_ADDITEM, 0, (LPARAM)text);
				SendMessageA(list, LB_SETITEMDATA, i, (LPARAM)found);
			} else {
				DebugString("@@@@ Could not find %s for %s\n", name, names[i]);
			}
			delete names[i];
		}
	}
	if (Campaign != NULL && Campaign->Has_Saved_Campaign() && !SaveListed) {
		char name[260];
		Campaign->Saved_Map_Is_Undefined(name);
		strcat(name, ".MAP");
		int map_count = maps->Count();
		MultiMission const * found = NULL;
		for (int k = 0; k < map_count; k++) {
			if (found != NULL) {
				break;
			}
			char const * scenario = (*maps)[k]->Get_Filename();
			if (stricmp(scenario, name) == 0) {
				found = (*maps)[k];
			}
		}
		if (found != NULL) {
			SendMessageA(list, LB_ADDITEM, 0, (LPARAM)StringTable::LoadString("TXT_COOP_USE_SAVE", NULL, "D:\\Ra2\\MPCoop.cpp", 0x528));
			SendMessageA(list, LB_SETITEMDATA, i, (LPARAM)found);
			SaveListed = true;
		}
	}
	SendMessageA(list, LB_SETCURSEL, 0, 0);
}

/// <summary>
/// Makes the campaign object again: the first campaign, named after the two players when there are two.
/// </summary>
bool MPCooperative::Recreate_Campaign(void) // RA2: 0x005A4F20
{
	if (Campaign != NULL) {
		delete Campaign;
	}
	Campaign = NULL;
	Campaign = new CoopCampaignClass;
	Campaign->Load_Campaign(0);
	if (Session.Players.Count() == 2) {
		Campaign->Set_Names(Session.Players[0]->Name, Session.Players[1]->Name);
	}
	return((Campaign != NULL) ? true : false);
}

/// <summary>
/// Leaves the lobby: hides the description of the campaign and puts every player on his own team.
/// </summary>
void MPCooperative::vt_entry_84(void) // RA2: 0x005A4F90
{
	HWND dialog = GameoptWindow();
	if (dialog != NULL) {
		HWND item = GetDlgItem(dialog, IDC_CAMPAIGN_DESCRIPTION);
		if (item != NULL) {
			SendMessageA(item, ST_SETTEXT, 0, (LPARAM)L"");
			ShowWindow(item, SW_HIDE);
		}
		Fill_Side_Combos(dialog);
	}
	for (int i = 0; i < Session.Players.Count(); i++) {
		Session.Players[i]->Slot.SpectatorFlag = i;
	}
}

/// <summary>
/// Called when the lobby is left or entered again: in the map dialog it takes the row of the saved campaign off the list; in the lobby it loads the campaign again when the saved one was chosen, selects its map, and sends the options.
/// </summary>
void MPCooperative::vt_entry_78(void) // RA2: 0x005A5000
{
	if (WS_Top_Window_ID() == 0x6B) {
		if (SaveListed) {
			HWND list = GetDlgItem(WS_Top_Window(), IDC_SCENARIO_LIST);
			int count = SendMessageA(list, LB_GETCOUNT, 0, 0);
			if (count == -1) {
				if (Campaign != NULL) {
					Campaign->Set_Names(L"", L"");
				}
				return;
			}
			if (SendMessageA(list, LB_GETCURSEL, 0, 0) == count - 1) {
				SendMessageA(list, LB_SETCURSEL, count - 2, 0);
			}
			SendMessageA(list, LB_DELETESTRING, count - 1, 0);
			SaveListed = false;
		}
	} else {
		if (Campaign != NULL && SavedCampaignChosen) {
			if (LoadedRecord != 0) {
				Campaign->Load_Campaign(0);
				Campaign->Load_Campaign(LoadedRecord);
			} else {
				Campaign->Load_Campaign(1);
				Campaign->Load_Campaign(LoadedRecord);
			}
			char * map;
			Campaign->Get_Map_Name(&map, -1);
			int count = Session.Scenarios.Count();
			char name[260];
			sprintf(name, "%s.MAP", map);
			int i;
			for (i = 0; i < count; i++) {
				char const * scenario = Session.Scenarios[i]->Get_Filename();
				if (stricmp(scenario, name) == 0) {
					break;
				}
			}
			Session.Options.ScenarioIndex = i;
			Scen->Scenario = i;
			Set_Scenario_Info_From_Index(i);
		}
		HWND dialog = GameoptWindow();
		if (dialog != NULL) {
			Set_Item_Text(dialog, this);
			Set_Skirmish_Dialog_Text(dialog);
			Update_Network_Dialog_Preview(dialog);
		}
		Send_Lobby_Options(true, true);
	}
	if (Campaign != NULL) {
		Campaign->Set_Names(L"", L"");
	}
}

/// <summary>
/// Makes the campaign object when there is none and, when the saved campaign is there, offers it: as a row in the list of maps, or as a lobby message.
/// </summary>
void MPCooperative::vt_entry_7C(void) // RA2: 0x005A5190
{
	if (Campaign == NULL) {
		Campaign = new CoopCampaignClass;
		Campaign->Load_Campaign(0);
	}
	if (Campaign != NULL) {
		if (Session.Players.Count() == 2) {
			Campaign->Set_Names(Session.Players[0]->Name, Session.Players[1]->Name);
		}
		vt_entry_50();
		if (Campaign->Has_Saved_Campaign()) {
			if (WS_Top_Window_ID() == 0x6B) {
				if (!SaveListed) {
					char name[260];
					Campaign->Saved_Map_Is_Undefined(name);
					strcat(name, ".MAP");
					MultiMission * map = NULL;
					int count = Session.Scenarios.Count();
					for (int i = 0; i < count; i++) {
						if (map != NULL) {
							break;
						}
						char const * scenario = Session.Scenarios[i]->Get_Filename();
						if (stricmp(scenario, name) == 0) {
							map = Session.Scenarios[i];
						}
					}
					if (map != NULL) {
						HWND list = GetDlgItem(WS_Top_Window(), IDC_SCENARIO_LIST);
						int rows = SendMessageA(list, LB_GETCOUNT, 0, 0);
						if (rows != -1) {
							SendMessageA(list, LB_ADDITEM, 0, (LPARAM)StringTable::LoadString("TXT_COOP_USE_SAVE", NULL, "D:\\Ra2\\MPCoop.cpp", 0x5E1));
							SendMessageA(list, LB_SETITEMDATA, rows, (LPARAM)map);
							SaveListed = true;
						}
					}
				}
			} else {
				Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_SAVE_CAMP_AVAILABLE", NULL, "D:\\Ra2\\MPCoop.cpp", 0x5E7));
			}
		}
	}
}

/// <summary>
/// Returns 2 (the base class returns -1).
/// </summary>
int MPCooperative::vt_entry_80(void) // RA2: 0x005A5340
{
	return(2);
}

/// <summary>
/// Shows the score screen of the map that was played and returns whether the players are done with it.
/// </summary>
bool MPCooperative::vt_entry_90(void) // RA2: 0x005A5350
{
	if (ScoreScreen == NULL) {
		ScoreScreen = new CampaignScoreClass;
	}
	if (Campaign == NULL) {
		return(false);
	}
	if (Campaigns == NULL) {
		return(false);
	}
	DynamicVectorClass<MPlayerScoreType *> scores;
	int time;
	Campaign->Get_Player_Scores(scores, &time);
	CoopCampEntry * record = NULL;
	int type;
	if (!Campaign->Get_Campaign_Type(&type)) {
		type = -1;
	} else {
		Campaigns->Get_Record(type, &record);
	}
	int difficulty = -1;
	if (type == 0 || type == 1) {
		difficulty = DIFF_EASY;
	} else if (type == 2 || type == 3) {
		difficulty = DIFF_NORMAL;
	} else if (type == 4) {
		difficulty = DIFF_HARD;
	}
	int count;
	Campaign->Get_Map_Count(&count);
	int current;
	Campaign->Get_Current_Map(&current);
	if (record != NULL) {
		ScoreScreen->Presentation(scores, Scen->ElapsedTimer(), current < count, difficulty, record->Name);
	} else {
		ScoreScreen->Presentation(scores, Scen->ElapsedTimer(), current < count, difficulty, L"");
	}
	while (scores.Count() != 0) {
		if (scores[0] != NULL) {
			delete scores[0];
		}
		scores[0] = NULL;
		scores.Delete_Index(0);
	}
	bool result = CampaignScoreDone;
	CampaignScoreDone = false;
	return(result);
}

/// <summary>
/// Shows the score screen of the whole campaign when the last map has been played.
/// </summary>
/// <param name="show">Whether the screen is shown.</param>
void MPCooperative::vt_entry_8C(bool show) // RA2: 0x005A5570
{
	if (Campaign != NULL) {
		int count;
		Campaign->Get_Map_Count(&count);
		int current;
		Campaign->Get_Current_Map(&current);
		if (show && current >= count) {
			CampaignEndScoreClass * score = new CampaignEndScoreClass;
			if (score != NULL) {
				DynamicVectorClass<MPlayerScoreType *> scores;
				int time;
				Campaign->Get_Player_Scores(scores, &time);
				CoopCampEntry * record = NULL;
				int type;
				if (!Campaign->Get_Campaign_Type(&type)) {
					type = -1;
				} else {
					Campaigns->Get_Record(type, &record);
				}
				int difficulty = -1;
				if (type == 0 || type == 1) {
					difficulty = DIFF_EASY;
				} else if (type == 2 || type == 3) {
					difficulty = DIFF_NORMAL;
				} else if (type == 4) {
					difficulty = DIFF_HARD;
				}
				score->Presentation(scores, time, difficulty, type + 1, record->Name);
				delete score;
				while (scores.Count() != 0) {
					if (scores[0] != NULL) {
						delete scores[0];
					}
					scores[0] = NULL;
					scores.Delete_Index(0);
				}
			}
		}
	}
}

/// <summary>
/// Called when the lobby is made again after a map: puts the players and the settings of the lobby back that vt_entry_88 saved, sets up the next map of the campaign and its computer players, and sets the timing of the game; closes the connections.
/// </summary>
void MPCooperative::vt_entry_94(void) // RA2: 0x005A5720
{
	NetNumType net;
	NetNodeType node;
	unsigned short port;
	char * map;
	Campaign->Get_Map_Name(&map, -1);
	char name[260];
	sprintf(name, "%s.MAP", map);
	strcpy(Scen->NextScenarioName, name);
	Scen->Campaign = CAMPAIGN_NONE;
	Scen->IsSkipScore = true;
	int count = Session.Scenarios.Count();
	sprintf(name, "%s.MAP", map);
	for (int i = 0; i < count; i++) {
		if (strncmp(name, Session.Scenarios[i]->Get_Filename(), _MAX_PATH) == 0) {
			Session.Options.ScenarioIndex = i;
			break;
		}
	}
	GameID = 0;
	GameProduct = g_GameSKU;
	WestwoodOnline_Tournament = 0;
	strcpy(PlayerNickname, g_NickName.get());
	wcscpy(Session.GameName, HostName);
	Clear_Vector(&Session.Players);
	Session.NumPlayers = 0;
	for (int p = 0; p < SavedPlayers.Count(); p++) {
		NodeNameType * entry = new NodeNameType;
		wcscpy(entry->Name, SavedPlayers[p]->Name);
		entry->Slot.Clan = SavedPlayers[p]->Slot.Clan;
		entry->Slot.Country = SavedPlayers[p]->Slot.Country;
		entry->Slot.ColorIdx = SavedPlayers[p]->Slot.ColorIdx;
		entry->Slot.HouseIndex = SavedPlayers[p]->Slot.HouseIndex;
		strcpy(entry->Player.Serial, SavedPlayers[p]->Player.Serial);
		entry->Slot.InitialCountry = SavedPlayers[p]->Slot.InitialCountry;
		entry->Slot.InitialColor = SavedPlayers[p]->Slot.InitialColor;
		entry->Slot.SpectatorFlag = SavedPlayers[p]->Slot.SpectatorFlag;
		entry->Slot.Time = SavedPlayers[p]->Slot.Time;
		entry->Slot.Unknown77 = SavedPlayers[p]->Slot.Unknown77;
		entry->Slot.Clan = SavedPlayers[p]->Slot.Clan;
		entry->Address = SavedPlayers[p]->Address;
		Session.Players.Add(entry);
		Session.NumPlayers++;
	}
	int computers = Session.Scenarios[Session.Options.ScenarioIndex]->MaxPlayers - 2;
	if (computers > 0) {
		Session.Options.AIPlayers = computers;
		int used = 0;
		CoopCampEntry * record;
		Campaigns->Get_Record(CampaignIndex, &record);
		int current;
		Campaign->Get_Current_Map(&current);
		for (int slot = 2; slot < 8; slot++) {
			if (used < computers) {
				if (strcmp(record->AIName, "Normal") == 0) {
					Session.Options.AISlots.Difficulties[slot] = 1;
				} else if (strcmp(record->AIName, "Hard") == 0) {
					Session.Options.AISlots.Difficulties[slot] = 0;
				} else {
					Session.Options.AISlots.Difficulties[slot] = 2;
				}
				Session.Options.AISlots.Countries[slot] = HouseChoices[slot][current];
				Session.Options.AISlots.Colors[slot] = ColorChoices[slot][current];
				used++;
			} else {
				Session.Options.AISlots.Countries[slot] = -1;
				Session.Options.AISlots.Colors[slot] = -1;
			}
		}
	}
	Session.HostAddress = Session.Players[LocalPlayer]->Address;
	wcscpy(Session.MasterPlayerName, Session.Players[LocalPlayer]->Name);
	Session.MasterPlayerID = -1;
	Session.House = (HousesType)SavedField74;
	PacketTransport->Clear_Broadcast_Addresses();
	char address[32] = "xxx.xxx.xxx.xxx";
	for (int n = 1; n < Session.Players.Count(); n++) {
		Session.Players[n]->Address.Get_Address(net, node, &port);
		PacketTransport->Set_Broadcast_Address(address, port);
	}
	memcpy(Session.SlotAddresses, Addresses, sizeof(Addresses));
	memcpy(Session.WolPlayerState, PlayerStates, sizeof(PlayerStates));
	memcpy(Session.PlayerPortBase, PlayerPortBases, sizeof(PlayerPortBases));
	memcpy(Session.PlayerPorts, PlayerPorts, sizeof(PlayerPorts));
	wcscpy(Session.MasterPlayerName, MasterName);
	Session.MasterPlayerID = MasterID;
	::Seed = Seed;
	BuildLevel = TechLevel;
	Session.Options.UnitCount = UnitCount;
	Session.Options.Credits = Credits;
	Session.Options.FogOfWar = FogOfWar;
	Session.Options.BridgeDestruction = BridgeDestruction;
	Session.Options.Goodies = Goodies;
	Session.Options.MCVRedeploy = MCVRedeploy;
	Session.Options.AlliesAllowed = AlliesAllowed;
	Session.Options.HarvTruce = HarvTruce;
	Session.Options.Bases = Bases;
	Session.Options.CTF = CTF;
	Session.Options.AIDifficulty = (DiffType)AIDifficulty;
	Session.Options.ShortGame = ShortGame;
	Session.Options.SWAllowed = SWAllowed;
	Session.Options.BuildOffAlly = BuildOffAlly;
	Session.Options.CrapEngineers = CrapEngineers;
	Session.Options.GameSpeed = GameSpeed;
	Set_Scenario_Info_From_Index(Session.Options.ScenarioIndex);
	Options.GameSpeed = Session.Options.GameSpeed;
	Scen->Scenario = Session.Options.ScenarioIndex;
	switch (Session.Options.GameSpeed) {
		case 0:
			Session.MaxAhead = 40;
			Session.PrecalcDesiredFrameRate = 60;
			Session.FrameSendRate = 10;
			break;
		case 1:
			Session.MaxAhead = 40;
			Session.PrecalcDesiredFrameRate = 45;
			Session.FrameSendRate = 10;
			break;
		case 2:
			Session.MaxAhead = 30;
			Session.PrecalcDesiredFrameRate = 30;
			break;
		case 3:
			Session.MaxAhead = 20;
			Session.PrecalcDesiredFrameRate = 20;
			break;
		case 4:
			Session.MaxAhead = 20;
			Session.PrecalcDesiredFrameRate = 15;
			break;
		case 5:
			Session.MaxAhead = 20;
			Session.PrecalcDesiredFrameRate = 12;
			break;
		default:
			Session.MaxAhead = 10;
			Session.PrecalcDesiredFrameRate = 10;
			break;
	}
	Session.PrecalcMaxAhead = 0;
	if (!unknown_4) {
		Session.PrecalcDesiredFrameRate = 0;
	}
	int ping = 0;
	for (int k = 0; k < Session.Players.Count(); k++) {
		if (PlayerPings[k] > ping) {
			ping = PlayerPings[k];
		}
	}
	Session.LatencyFudge = 1;
	if (ping == 0 || ping == 1000) {
		Session.PrecalcDesiredFrameRate = 0;
		DebugString("Ping times not available\n");
	} else {
		DebugString("Max ping is %d ms\n", ping);
		if (ping > 1000) {
			Session.LatencyFudge = 2;
		} else if (ping < 600) {
			Session.LatencyFudge = 1;
		}
		int retry = ping * 60 / 1000;
		retry += retry * 120 / 100;
		Ipx.Set_Timing(retry, -1, retry * 8, true);
		if (unknown_4) {
			Ipx.Set_Lobby_Timing(5, -1, 120);
			if (unknown_4) {
				switch (Session.LatencyFudge) {
					case 0:
					case 1:
						Session.PrecalcMaxAhead = Session.MaxAhead;
						break;
					case 2:
						Session.PrecalcMaxAhead = Session.MaxAhead / 2 + Session.MaxAhead;
						break;
					case 3:
						Session.PrecalcMaxAhead = Session.MaxAhead * 2;
						break;
				}
				Session.PrecalcMaxAhead = (unsigned int)Session.PrecalcMaxAhead / 10 * 10; // RA2: an unsigned division
			}
		}
	}
	Session.MaxMaxAhead = Session.MaxAhead;
	memset(Session.ConnectionStats, 0, sizeof(Session.ConnectionStats));
	Squad1Names[0] = '\0';
	Options.GameSpeed = Session.Options.GameSpeed;
	Squad2Names[0] = '\0';
	PregameSetup();
	DoList.Init();
	OutList.Init();
	ToolTips->Activate(false);
	DebugString("@@@@ Before destroying connections players.count() = %d\n", Session.Players.Count());
	while (Ipx.Num_Connections()) {
		Ipx.Delete_Connection(Ipx.Connection_ID(0));
	}
	if (Session.ViaPacketRouter) {
		if (PacketRouterConnection) {
			OwnerTalk.Send_Shutdown();
		}
		Session.ViaPacketRouter = false;
	}
	DebugString("@@@@ After destroying connections players.count() = %d\n", Session.Players.Count());
}

/// <summary>
/// Makes the lobby's players again from the saved ones: every saved player whose name is a house of the game is given the house's index.
/// </summary>
void MPCooperative::vt_entry_98(void) // RA2: 0x005A6040
{
	Clear_Vector(&Session.Players);
	Session.NumPlayers = 0;
	for (int i = 0; i < SavedPlayers.Count(); i++) {
		for (int h = 0; h < Houses.Count(); h++) {
			if (wcsicmp(Houses[h]->UIName, SavedPlayers[i]->Name) == 0) {
				SavedPlayers[i]->Slot.HouseIndex = h;
				NodeNameType * entry = new NodeNameType;
				wcscpy(entry->Name, SavedPlayers[i]->Name);
				entry->Slot.Clan = SavedPlayers[i]->Slot.Clan;
				entry->Slot.Country = SavedPlayers[i]->Slot.Country;
				entry->Slot.ColorIdx = SavedPlayers[i]->Slot.ColorIdx;
				strcpy(entry->Player.Serial, SavedPlayers[i]->Player.Serial);
				entry->Slot.InitialCountry = SavedPlayers[i]->Slot.InitialCountry;
				entry->Slot.InitialColor = SavedPlayers[i]->Slot.InitialColor;
				entry->Slot.SpectatorFlag = SavedPlayers[i]->Slot.SpectatorFlag;
				entry->Slot.Time = SavedPlayers[i]->Slot.Time;
				entry->Slot.Unknown77 = SavedPlayers[i]->Slot.Unknown77;
				entry->Slot.Clan = SavedPlayers[i]->Slot.Clan;
				NetNumType net;
				NetNodeType node;
				unsigned short socket;
				SavedPlayers[i]->Address.Get_Address(net, node, &socket);
				entry->Address.Set_Address(net, node, socket);
				Session.Players.Add(entry);
				Session.NumPlayers++;
				break;
			}
		}
	}
}

/// <summary>
/// Sets the result to zero and tells whether there is a campaign.
/// </summary>
bool MPCooperative::vt_entry_9C(DynamicVectorClass<int> list, int * result) // RA2: 0x005A6230
{
	*result = 0;
	if (Campaign != NULL) {
		return(true);
	}
	return(false);
}

/// <summary>
/// Always true.
/// </summary>
bool MPCooperative::vt_entry_04(void) // RA2: 0x005A6280
{
	return(true);
}

/// <summary>
/// Always true.
/// </summary>
bool MPCooperative::vt_entry_08(void) // RA2: 0x005A6290
{
	return(true);
}

/// <summary>
/// Always false.
/// </summary>
bool MPCooperative::vt_entry_A0(void) // RA2: 0x005A62A0
{
	return(false);
}
