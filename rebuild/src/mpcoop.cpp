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
#include "vector.h" // RA2: as above
#include "mpcoop.h" // RA2: as above
#include "ccini.h" // RA2: as above
#include "ccfile.h" // RA2: as above
#include "stringtable.h" // RA2: as above
#include "session.h" // RA2: as above
#include "globals.h" // RA2: as above
#include "houstype.h" // RA2: as above
#include "scenario.h" // RA2: as above
#include "gadget.h" // RA2: as above
#include "_rules.h" // RA2: as above
#include "rules.h" // RA2: as above
#include "mplayer.h" // RA2: as above
#include "mplayerscore.h" // RA2: as above
#include "mouse.h" // RA2: as above
#include "wonline.h" // RA2: as above
#include "ipxmgr.h" // RA2: as above
#include "_WSProto.h" // RA2: as above
#include "wsproto.h" // RA2: as above
#include "goptions.h" // RA2: as above
#include "_tooltip.h" // RA2: as above
#include "tooltip.h" // RA2: as above
#include "cctooltip.h" // RA2: as above
#include "ownertalk.h" // RA2: as above
#include "connect.h" // RA2: as above
#include "queue.h" // RA2: as above
#include "_map.h" // RA2: as above
#include "house.h" // RA2: as above
#include "netshare.h" // RA2: as above
#include "windlg.h" // RA2: as above
#include "dbgprint.h" // RA2: as above
#include <windows.h> // RA2: as above
#undef LoadString // RA2: as above
#include <stdio.h> // RA2: as above
#include <string.h> // RA2: as above
#include <stdlib.h> // RA2: as above
#include "rect.h" // RA2: as above


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
	if (filename == NULL) { // RA2: as above
		return(0); // RA2: as above
	}
	CCFileClass file(filename); // RA2: as above
	INIClass ini; // RA2: as above
	if (!ini.Load(file, false)) { // RA2: as above
		return(0); // RA2: as above
	}
	int count = 0; // RA2: as above
	for (int i = 0; i < 8; i++) { // RA2: as above
		char key[32]; // RA2: as above
		sprintf(key, "%d", i); // RA2: as above
		if (ini.Get_Int("Waypoints", key, -1) != -1) { // RA2: as above
			count++; // RA2: as above
		}
	}
	if (count == 0) { // RA2: as above
		count = ini.Get_Int("RandomMap", "NumPlayers", 0); // RA2: as above
	}
	return(count); // RA2: as above
}

/// <summary>
/// Makes the mode: it reads CoopCamp.ini and makes a campaign object for every campaign in it.
/// </summary>
MPCooperative::MPCooperative(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex) : // RA2: 0x005A2960
	MultiplayerGameMode(title, tooltip, inifile, mapfilter, false, modeindex) // RA2: the computer players are never allowed
{
	Campaigns = new CoopCampListClass; // RA2: 0x00490510
	Campaign = NULL; // RA2: as above
	Field48 = 0; // RA2: as above
	CampaignIndex = 0; // RA2: as above
	ScoreScreen = NULL; // RA2: as above
	unknown_4 = false; // RA2: as above
	SavedCampaignChosen = false; // RA2: as above
	SaveListed = false; // RA2: as above
	LoadedRecord = 0; // RA2: as above
	PlayersChanged = true; // RA2: as above
	for (int j = CampaignStates.Count() - 1; j >= 0; j--) { // RA2: as above
		CoopCampaignClass * state = CampaignStates[j]; // RA2: as above
		if (state) { // RA2: as above
			delete state; // RA2: as above
		}
	}
	DynamicVectorClass<CoopCampaignClass *> * states = &CampaignStates; // RA2: the vector is reached through a pointer
	int max = states->Length(); // RA2: as above
	states->Clear(); // RA2: as above
	states->Resize(max, NULL); // RA2: as above
	int count; // RA2: as above
	Campaigns->Get_Count(&count); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		CoopCampaignClass * state = new CoopCampaignClass; // RA2: as above
		state->Load_Campaign(i); // RA2: as above
		states->Add(state); // RA2: as above
	}
}

/// <summary>
/// Destroys the campaign list, the campaign and the campaign objects, the copies of the players and the score screen.
/// </summary>
MPCooperative::~MPCooperative(void) // RA2: 0x005A2B50
{
	if (Campaigns) { // RA2: as above
		delete Campaigns; // RA2: as above
		Campaigns = NULL; // RA2: as above
	}
	if (Campaign) { // RA2: as above
		delete Campaign; // RA2: as above
		Campaign = NULL; // RA2: as above
	}
	for (int j = CampaignStates.Count() - 1; j >= 0; j--) { // RA2: as above
		CoopCampaignClass * state = CampaignStates[j]; // RA2: as above
		if (state) { // RA2: as above
			delete state; // RA2: as above
		}
	}
	CampaignStates.Clear(); // RA2: as above
	for (int i = SavedPlayers.Count() - 1; i >= 0; i--) { // RA2: as above
		delete SavedPlayers[i]; // RA2: as above
	}
	int max = SavedPlayers.Length(); // RA2: as above
	SavedPlayers.Clear(); // RA2: as above
	SavedPlayers.Resize(max, NULL); // RA2: as above
	if (ScoreScreen) { // RA2: as above
		delete ScoreScreen; // RA2: as above
		ScoreScreen = NULL; // RA2: as above
	}
	unknown_4 = false; // RA2: as above
}

/// <summary>
/// Tells whether the campaign lets a player pick a house: the houses the campaign lists are the allowed ones, and the random and observer choices are when it lists more than one.
/// </summary>
bool MPCooperative::IsCountryAllowed(int country) // RA2: 0x005A2CD0
{
	if (Campaigns == NULL) { // RA2: as above
		return(true); // RA2: as above
	}
	CoopCampEntry * record; // RA2: as above
	Campaigns->Get_Record(CampaignIndex, &record); // RA2: as above
	if (record == NULL) { // RA2: as above
		return(true); // RA2: as above
	}
	bool found = false; // RA2: as above
	if (country == -2) { // RA2: as above
		if (record->Players.Count() > 1) { // RA2: as above
			return(true); // RA2: as above
		}
	} else if (country == -3) { // RA2: as above
		return(true); // RA2: as above
	}
	for (int i = 0; i < record->Players.Count(); i++) { // RA2: as above
		if (Find_Country_Index(record->Players[i]) == country) { // RA2: as above
			found = true; // RA2: as above
			break; // RA2: as above
		}
	}
	return(found); // RA2: as above
}

/// <summary>
/// Reads the campaign objects again: one for every campaign of CoopCamp.ini.
/// </summary>
bool MPCooperative::AssignTeams(void) // RA2: 0x005A2D70
{
	for (int j = CampaignStates.Count() - 1; j >= 0; j--) { // RA2: as above
		CoopCampaignClass * state = CampaignStates[j]; // RA2: as above
		if (state) { // RA2: as above
			delete state; // RA2: as above
		}
	}
	DynamicVectorClass<CoopCampaignClass *> * states = &CampaignStates; // RA2: as in the constructor
	int max = states->Length(); // RA2: as above
	states->Clear(); // RA2: as above
	states->Resize(max, NULL); // RA2: as above
	int count; // RA2: as above
	Campaigns->Get_Count(&count); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		CoopCampaignClass * state = new CoopCampaignClass; // RA2: as above
		state->Load_Campaign(i); // RA2: as above
		states->Add(state); // RA2: as above
	}
	return(true); // RA2: as above
}

/// <summary>
/// Finds the campaign whose first map is the selected scenario.
/// </summary>
int MPCooperative::Find_Campaign_Index(void) // RA2: 0x005A2E50
{
	if (Campaigns == NULL) { // RA2: as above
		return(-1); // RA2: as above
	}
	int found = -1; // RA2: as above
	for (int i = 0; i < Campaigns->Count; i++) { // RA2: as above
		CoopCampEntry * record; // RA2: as above
		Campaigns->Get_Record(i, &record); // RA2: as above
		for (int map = 0; map < 3; map++) { // RA2: as above
			if (Session.Options.ScenarioIndex != 0) { // RA2: as above
				if (strnicmp(Session.Scenarios[Session.Options.ScenarioIndex]->Get_Filename(), &record->Maps[map * _MAX_PATH], strlen(&record->Maps[map * _MAX_PATH])) == 0) { // RA2: as above
					found = i; // RA2: as above
				}
			}
		}
	}
	return(found); // RA2: as above
}

/// <summary>
/// Called when the lobby opens: tells the host there is no observer (O8), keeps or makes the campaign object and selects its campaign and first map.
/// </summary>
void MPCooperative::vt_entry_20(void) // RA2: 0x005A2EF0
{
	if (Campaigns == NULL) { // RA2: as above
		return; // RA2: as above
	}
	LobbyObserverSlot = -1; // RA2: as above
	DebugString("Sending observer slot of 8"); // RA2: as above
	Send_Host_Option("O8"); // RA2: as above
	if (WS_Find_Dialog(0xC2) != NULL || WS_Find_Dialog(0xBC) != NULL) { // RA2: as above
		if (Campaign != NULL) { // RA2: as above
			int type; // RA2: as above
			Campaign->Get_Campaign_Type(&type); // RA2: as above
			if ((Find_Campaign_Index() != type && WS_Top_Window_ID() == 0x6B) || WS_Top_Window_ID() != 0x6B) { // RA2: as above
				delete Campaign; // RA2: as above
				Campaign = NULL; // RA2: as above
			}
		}
		if (Campaign != NULL) { // RA2: as above
			if (Session.Players.Count() == 2) { // RA2: as above
				Campaign->Set_Names(Session.Players[0]->Name, Session.Players[1]->Name); // RA2: as above
			} else { // RA2: as above
				Campaign->Set_Names(L"", L""); // RA2: as above
			}
		}
	}
	if (Campaign == NULL) { // RA2: as above
		Campaign = new CoopCampaignClass; // RA2: as above
		if (Campaign == NULL) { // RA2: as above
			return; // RA2: as above
		}
		Campaign->Load_Campaign(0); // RA2: as above
		int index = Find_Campaign_Index(); // RA2: as above
		if (index < 0) { // RA2: as above
			CampaignIndex = 0; // RA2: as above
			char * name = NULL; // RA2: as above
			Campaign->Get_Map_Name(&name, 0); // RA2: as above
			int i; // RA2: as above
			for (i = 0; i < Session.Scenarios.Count(); i++) { // RA2: as above
				if (strnicmp(Session.Scenarios[i]->Get_Filename(), name, strlen(name)) == 0) { // RA2: as above
					Session.Options.ScenarioIndex = i; // RA2: as above
					Set_Scenario_Info_From_Index(i); // RA2: as above
					break; // RA2: as above
				}
			}
			if (i == Session.Scenarios.Count()) { // RA2: as above
				Session.Options.ScenarioIndex = 0; // RA2: as above
				Set_Scenario_Info_From_Index(0); // RA2: as above
			}
		} else { // RA2: as above
			CampaignIndex = index; // RA2: as above
			CoopCampEntry * record; // RA2: as above
			Campaigns->Get_Record(index, &record); // RA2: as above
			char * name = NULL; // RA2: as above
			Campaign->Get_Map_Name(&name, 0); // RA2: as above
			DebugString("@@@ Looking for %s", Session.Scenarios[Session.Options.ScenarioIndex]->Get_Filename()); // RA2: as above
			if (strnicmp(Session.Scenarios[Session.Options.ScenarioIndex]->Get_Filename(), name, strlen(name)) != 0) { // RA2: as above
				int other = (index == 0); // RA2: as above
				do { // RA2: as above
					Campaign->Load_Campaign(other); // RA2: as above
					Campaign->Load_Campaign(index); // RA2: as above
					Campaign->Get_Map_Name(&name, 0); // RA2: as above
				} while (strnicmp(Session.Scenarios[Session.Options.ScenarioIndex]->Get_Filename(), name, strlen(name)) != 0); // RA2: as above
			}
		}
	}
	PlayersChanged = true; // RA2: as above
	int type; // RA2: as above
	HWND dialog = GameoptWindow(); // RA2: as above
	if (dialog != NULL) { // RA2: as above
		Refresh_Player_Sides(dialog); // RA2: as above
		HWND item = GetDlgItem(dialog, IDC_CAMPAIGN_DESCRIPTION); // RA2: as above
		if (item != NULL && Campaign != NULL && Campaign->Get_Campaign_Type(&type)) { // RA2: as above
			char label[64]; // RA2: as above
			sprintf(label, "NAME:COOPDESC%d", type + 1); // RA2: as above
			SendMessageA(item, ST_SETTEXT, 0, (LPARAM)StringTable::LoadString(label, NULL, "D:\\Ra2\\MPCoop.cpp", 0x157)); // RA2: as above
			ShowWindow(item, SW_SHOW); // RA2: as above
		}
	}
}

/// <summary>
/// Marks the lobby's player section as changed.
/// </summary>
void MPCooperative::vt_entry_24(void) // RA2: 0x005A31F0
{
	PlayersChanged = true; // RA2: as above
}

/// <summary>
/// Makes the campaign object for the first campaign and returns the index of the scenario that is its first map.
/// </summary>
int MPCooperative::FirstValidMapIndex(void) // RA2: 0x005A3200
{
	int count = 0; // RA2: as above
	char * name = NULL; // RA2: as above
	if (Campaigns == NULL) { // RA2: as above
		Campaigns = new CoopCampListClass; // RA2: as above
	}
	Campaigns->Get_Count(&count); // RA2: as above
	if (count != 0) { // RA2: as above
		if (Campaign == NULL) { // RA2: as above
			Campaign = new CoopCampaignClass; // RA2: as above
		}
		Campaign->Load_Campaign(0); // RA2: as above
		Campaign->Get_Map_Name(&name, 0); // RA2: as above
		for (int i = 0; i < Session.Scenarios.Count(); i++) { // RA2: as above
			if (strnicmp(Session.Scenarios[i]->Get_Filename(), name, strlen(name)) == 0) { // RA2: as above
				return(i); // RA2: as above
			}
		}
		AssignTeams(); // RA2: as above
	}
	return(-1); // RA2: as above
}

/// <summary>
/// Gives the campaign the names of the two players, then lets the base class check.
/// </summary>
bool MPCooperative::CheckStartConditions(WideWstring * message) // RA2: 0x005A32E0
{
	if (Session.Players.Count() == 2 && Campaign != NULL) { // RA2: as above
		Campaign->Set_Names(Session.Players[0]->Name, Session.Players[1]->Name); // RA2: as above
	}
	return(MultiplayerGameMode::CheckStartConditions(message)); // RA2: as above
}

/// <summary>
/// Picks the campaign (or the saved one) the host selected and gives it the players; a player whose house the campaign does not allow gets a random or the only allowed one.
/// </summary>
bool MPCooperative::AssignTeamsAtStart(void) // RA2: 0x005A3320
{
	if (Campaigns == NULL || Campaign == NULL) { // RA2: as above
		return(false); // RA2: as above
	}
	int count; // RA2: as above
	Campaigns->Get_Count(&count); // RA2: as above
	int selected = Get_Selected_Scenario_Row(); // RA2: as above
	if (selected >= count) { // RA2: as above
		if (Session.Players.Count() == 2) { // RA2: as above
			Campaign->Set_Names(Session.Players[0]->Name, Session.Players[1]->Name); // RA2: as above
		}
		if (!Campaign->Load_Saved_Campaign()) { // RA2: as above
			return(false); // RA2: as above
		}
		SavedCampaignChosen = true; // RA2: as above
	} else if (WS_Top_Window_ID() == 0x6B) { // RA2: as above
		delete Campaign; // RA2: as above
		Campaign = CampaignStates[selected]; // RA2: as above
		CampaignStates[selected] = new CoopCampaignClass; // RA2: as above
		CampaignStates[selected]->Load_Campaign(selected); // RA2: as above
		LoadedRecord = selected; // RA2: as above
		if (Session.Players.Count() == 2) { // RA2: as above
			Campaign->Set_Names(Session.Players[0]->Name, Session.Players[1]->Name); // RA2: as above
		}
	}
	CoopCampEntry * record; // RA2: as above
	Campaigns->Get_Record(CampaignIndex, &record); // RA2: as above
	if (record == NULL) { // RA2: as above
		return(false); // RA2: as above
	}
	int players = record->Players.Count(); // RA2: as above
	if (players == 0) { // RA2: as above
		return(false); // RA2: as above
	}
	Refresh_Player_Sides(GameoptWindow()); // RA2: as above
	for (int i = 0; i < Session.Players.Count(); i++) { // RA2: as above
		if (!IsCountryAllowed(Session.Players[i]->Get_Country())) { // RA2: as above
			if (players > 1) { // RA2: as above
				Session.Players[i]->Set_Country(-2); // RA2: as above
			} else { // RA2: as above
				int house = Find_Country_Index(record->Players[0]); // RA2: as above
				Session.Players[i]->Set_Country(house); // RA2: as above
			}
		}
	}
	return(true); // RA2: as above
}

/// <summary>
/// The house a player starts with: the only one the campaign lists, else random.
/// </summary>
int MPCooperative::DefaultCountry(void) // RA2: 0x005A34B0
{
	CoopCampEntry * record; // RA2: as above
	Campaigns->Get_Record(CampaignIndex, &record); // RA2: as above
	if (record == NULL) { // RA2: as above
		return(-2); // RA2: as above
	}
	if (record->Players.Count() == 0) { // RA2: as above
		return(-2); // RA2: as above
	}
	if (record->Players.Count() > 1) { // RA2: as above
		return(-2); // RA2: as above
	}
	return(Find_Country_Index(record->Players[0])); // RA2: as above
}

/// <summary>
/// Takes the campaign settings the host sent: the campaign, the choices of houses and colors, and the names of the players.
/// </summary>
bool MPCooperative::vt_entry_48(int index, DynamicVectorClass<int> picks, char const * map, char const * name) // RA2: 0x005A3500
{
	if (Campaign == NULL) { // RA2: as above
		Recreate_Campaign(); // RA2: as above
		PlayersChanged = true; // RA2: as above
	}
	if (Campaign != NULL && Campaigns != NULL) { // RA2: as above
		int type; // RA2: as above
		Campaign->Get_Campaign_Type(&type); // RA2: as above
		if (type != index) { // RA2: as above
			PlayersChanged = true; // RA2: as above
		}
		Campaign->Set_Campaign_Info(*Campaigns, index, picks, map, name); // RA2: as above
		CampaignIndex = index; // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}

/// <summary>
/// Adds the scores of the two players to the campaign's.
/// </summary>
bool MPCooperative::vt_entry_4C(unsigned index, unsigned time, MPlayerScoreType * scores) // RA2: 0x005A35D0
{
	if (Campaign == NULL) { // RA2: as above
		return(false); // RA2: as above
	}
	HWND lobby = GameoptWindow(); // RA2: as above
	if (lobby != NULL && PlayersChanged) { // RA2: as above
		Refresh_Player_Sides(lobby); // RA2: as above
		PlayersChanged = false; // RA2: as above
	}
	return(Campaign->Add_Score(index, time, scores[0], scores[1])); // RA2: as above
}

/// <summary>
/// Fills a side combo box with the houses the campaign lets the players pick (and the random choice when there is more than one).
/// </summary>
void MPCooperative::Fill_House_Combo(HWND dialog, int id) // RA2: 0x005A3650
{
	int count = HouseTypes.Count(); // RA2: as above
	if (count != 0) { // RA2: as above
		HWND combo = GetDlgItem(dialog, id); // RA2: as above
		ShowWindow(combo, SW_HIDE); // RA2: as above
		SendMessageA(combo, CB_RESETCONTENT, 0, 0); // RA2: as above
		CoopCampEntry * record; // RA2: as above
		Campaigns->Get_Record(CampaignIndex, &record); // RA2: as above
		if (record != NULL) { // RA2: as above
			int players = record->Players.Count(); // RA2: as above
			if (players != 0) { // RA2: as above
				if (players > 1) { // RA2: as above
					wchar_t const * name = StringTable::LoadString("GUI:Random", NULL, "D:\\Ra2\\MPCoop.cpp", 0x225); // RA2: as above
					LRESULT index = SendMessageA(combo, CB_ADDITEM, 0, (LPARAM)name); // RA2: as above
					SendMessageA(combo, CB_SETITEMDATA, index, -2); // RA2: as above
				}
				for (int i = 0; i < players; i++) { // RA2: as above
					int house = Find_Country_Index(record->Players[i]); // RA2: as above
					if (house >= 0 && house < count) { // RA2: as above
						wchar_t const * name = HouseTypes[house]->UIName; // RA2: as above
						LRESULT index = SendMessageA(combo, CB_ADDITEM, 0, (LPARAM)name); // RA2: as above
						SendMessageA(combo, CB_SETITEMDATA, index, house); // RA2: as above
					}
				}
				SendMessageA(combo, CB_SETCURSEL, 0, 0); // RA2: as above
				ShowWindow(combo, SW_SHOW); // RA2: as above
			}
		}
	}
}

/// <summary>
/// Fills the side combo boxes of the player slots with the campaign's houses and selects the only house, or the random choice; hides the slots that are not used and shows the campaign's description.
/// </summary>
void MPCooperative::Refresh_Player_Sides(HWND dialog) // RA2: 0x005A3760
{
	if (dialog != NULL) { // RA2: as above
		int count = Session.ScenarioMaxPlayers; // RA2: as above
		CoopCampEntry * record; // RA2: as above
		Campaigns->Get_Record(CampaignIndex, &record); // RA2: as above
		if (record != NULL) { // RA2: as above
			int players = record->Players.Count(); // RA2: as above
			if (players != 0) { // RA2: as above
				for (int i = 0; i < count; i++) { // RA2: as above
					int id = Player_Side_Combo_ID(i); // RA2: as above
					Fill_House_Combo(dialog, id); // RA2: as above
					if (players > 1) { // RA2: as above
						Select_Side_Combo(dialog, id, -2); // RA2: as above
						Session.Slots[i]->Set_Country(-2); // RA2: as above
					} else { // RA2: as above
						int house = Find_Country_Index(record->Players[0]); // RA2: as above
						Select_Side_Combo(dialog, id, house); // RA2: as above
						Session.Slots[i]->Set_Country(house); // RA2: as above
					}
				}
				Hide_Slot_Controls(dialog, count); // RA2: as above
				HWND lobby = GameoptWindow(); // RA2: as above
				int type; // RA2: as above
				if (lobby != NULL && Campaign != NULL && Campaign->Get_Campaign_Type(&type)) { // RA2: as above
					char label[64]; // RA2: as above
					sprintf(label, "NAME:COOPDESC%d", type + 1); // RA2: as above
					HWND item = GetDlgItem(lobby, IDC_CAMPAIGN_DESCRIPTION); // RA2: as above
					if (item != NULL) { // RA2: as above
						SendMessageA(item, ST_SETTEXT, 0, (LPARAM)StringTable::LoadString(label, NULL, "D:\\Ra2\\MPCoop.cpp", 0x273)); // RA2: as above
						ShowWindow(item, SW_SHOW); // RA2: as above
						InvalidateRect(item, NULL, FALSE); // RA2: as above
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
	if (Campaign == NULL) { // RA2: as above
		return(false); // RA2: as above
	}
	Campaign->Get_Campaign_Type(&CampaignIndex); // RA2: as above
	HWND lobby = GameoptWindow(); // RA2: as above
	if (lobby != NULL && PlayersChanged) { // RA2: as above
		Refresh_Player_Sides(lobby); // RA2: as above
		PlayersChanged = false; // RA2: as above
	}
	return(Campaign->Send_Campaign_Options(*Campaigns)); // RA2: as above
}

/// <summary>
/// Sets the colors of the eight slots to random (-2) and then the colors the players of the session chose.
/// </summary>
void MPCooperative::Fill_Player_Colors(void) // RA2: 0x005A3920; the executable expands it in vt_entry_54 and keeps this copy
{
	int i; // RA2: as above
	for (i = 0; i < 8; i++) { // RA2: as above
		PlayerColors[i] = -2; // RA2: as above
	}
	int count = Session.Players.Count(); // RA2: as above
	for (i = 0; i < count; i++) { // RA2: as above
		PlayerColors[i] = Session.Players[i]->Get_Color_Or_Random(); // RA2: as above
	}
}

/// <summary>
/// Chooses the houses and colors of the players and of the computer players for every map of the campaign and sends them to the other player as the game option C1.
/// </summary>
bool MPCooperative::vt_entry_54(void) // RA2: 0x005A3960
{
	CoopCampEntry * record; // RA2: as above
	Campaigns->Get_Record(CampaignIndex, &record); // RA2: as above
	char message[400]; // RA2: as above
	sprintf(message, "C1,%d", record->MapCount); // RA2: as above
	for (int list = 0; list < 8; list++) { // RA2: as above
		int length = HouseChoices[list].Length(); // RA2: as above
		HouseChoices[list].Clear(); // RA2: as above
		HouseChoices[list].Resize(length); // RA2: as above
		length = ColorChoices[list].Length(); // RA2: as above
		ColorChoices[list].Clear(); // RA2: as above
		ColorChoices[list].Resize(length); // RA2: as above
	}
	int players = Session.Players.Count(); // RA2: as above
	for (int map = 0; map < record->MapCount; map++) { // RA2: as above
		Fill_Player_Colors(); // RA2: as above
		for (int slot = 0; slot < 8; slot++) { // RA2: as above
			if (slot < players) { // RA2: as above
				HouseChoices[slot].Add(RandomHumanCountryIndex()); // RA2: as above
				ColorChoices[slot].Add(PlayerColors[slot]); // RA2: as above
			} else { // RA2: as above
				HouseChoices[slot].Add(RandomAICountryIndex()); // RA2: as above
				int color; // RA2: as above
				bool taken; // RA2: as above
				do { // RA2: as above
					color = Scen->RandomNumber(0, 7); // RA2: as above
					taken = false; // RA2: as above
					if (color != -2) { // RA2: as above
						for (int i = 0; i < 8; i++) { // RA2: as above
							if (PlayerColors[i] == color) { // RA2: as above
								taken = true; // RA2: as above
								break; // RA2: as above
							}
						}
					}
				} while (taken); // RA2: as above
				PlayerColors[slot] = color; // RA2: as above
				ColorChoices[slot].Add(color); // RA2: as above
			}
			char text[16]; // RA2: as above
			sprintf(text, ",%d,%d", HouseChoices[slot][map], ColorChoices[slot][map]); // RA2: as above
			strcat(message, text); // RA2: as above
		}
	}
	DebugString("@@@@ Sending campaign game AI settings: %s\n", message); // RA2: as above
	Send_Host_Option(message); // RA2: as above
	DebugString("@@@@ Sent\n"); // RA2: as above
	return(true); // RA2: as above
}

/// <summary>
/// Takes the game option C1 the host sent: the number of maps and the house and color of every slot for each of them.
/// </summary>
/// <param name="kind">The digit after the C of the option.</param>
/// <param name="text">The rest of the option, split at the commas.</param>
void MPCooperative::vt_entry_64(unsigned kind, char * text) // RA2: 0x005A3CA0
{
	if (kind == 1) { // RA2: as above
		int rounds = atol(strtok(text, ",")); // RA2: as above
		int i; // RA2: as above
		for (i = 0; i < 8; i++) { // RA2: as above
			int capacity = HouseChoices[i].Length(); // RA2: as above
			HouseChoices[i].Clear(); // RA2: as above
			HouseChoices[i].Resize(capacity); // RA2: as above
			capacity = ColorChoices[i].Length(); // RA2: as above
			ColorChoices[i].Clear(); // RA2: as above
			ColorChoices[i].Resize(capacity); // RA2: as above
		}
		for (int round = 0; round < rounds; round++) { // RA2: as above
			for (i = 0; i < 8; i++) { // RA2: as above
				char * token = strtok(NULL, ","); // RA2: as above
				if (token != NULL) { // RA2: as above
					HouseChoices[i].Add(atol(token)); // RA2: as above
				} else { // RA2: as above
					HouseChoices[i].Add(-1); // RA2: as above
				}
				token = strtok(NULL, ","); // RA2: as above
				if (token != NULL) { // RA2: as above
					ColorChoices[i].Add(atol(token)); // RA2: as above
				} else { // RA2: as above
					ColorChoices[i].Add(-1); // RA2: as above
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
	HWND dialog = GameoptWindow(); // RA2: as above
	if (dialog != NULL && Campaign != NULL) { // RA2: as above
		int type; // RA2: as above
		if (Campaign->Get_Campaign_Type(&type)) { // RA2: as above
			char label[64]; // RA2: as above
			sprintf(label, "NAME:COOPDESC%d", type + 1); // RA2: as above
			HWND item = GetDlgItem(dialog, IDC_CAMPAIGN_DESCRIPTION); // RA2: as above
			if (item != NULL) { // RA2: as above
				SendMessageA(item, ST_SETTEXT, 0, (LPARAM)StringTable::LoadString(label, NULL, "D:\\Ra2\\MPCoop.cpp", 0x31E)); // RA2: as above
				ShowWindow(item, SW_SHOW); // RA2: as above
			}
		}
	}
	return(true); // RA2: as above
}

/// <summary>
/// The teams of the dropdown are the base class's.
/// </summary>
void MPCooperative::PopulateTeamDropdownForPlayer(HWND hwnd, int index) // RA2: 0x005A3F70
{
	MultiplayerGameMode::PopulateTeamDropdownForPlayer(hwnd, index); // RA2: as above
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
	int count = 0; // RA2: as above
	int i; // RA2: as above
	for (i = 0; i < coords->Count(); i++) { // RA2: as above
		if (satisfied[i]) { // RA2: as above
			count++; // RA2: as above
		}
	}
	int index; // RA2: as above
	if (count == 0) { // RA2: as above
		index = Scen->RandomNumber(0, coords->Count() - 1); // RA2: as above
		satisfied[index] = true; // RA2: as above
		*result = (*coords)[index];
		return(result); // RA2: as above
	} else { // RA2: as above
		int nearest = -1; // RA2: as above
		int nearest_index = -1; // RA2: as above
		if (count == 1) { // RA2: as above
			for (i = 0; i < coords->Count(); i++) { // RA2: as above
				if (!satisfied[i]) { // RA2: as above
					int sum = 0; // RA2: as above
					for (int j = 0; j < coords->Count(); j++) { // RA2: as above
						if (satisfied[j]) { // RA2: as above
							sum += (*coords)[i].Distance_To((*coords)[j]); // RA2: as above
						}
					}
					if (sum < nearest || nearest < 0) { // RA2: as above
						nearest = sum; // RA2: as above
						nearest_index = i; // RA2: as above
					}
				}
			}
			satisfied[nearest_index] = true; // RA2: as above
			*result = (*coords)[nearest_index];
			return(result); // RA2: as above
		} else { // RA2: as above
			int farthest = -1; // RA2: as above
			int farthest_index = -1; // RA2: as above
			for (i = 0; i < coords->Count(); i++) { // RA2: as above
				if (!satisfied[i]) { // RA2: as above
					int sum = 0; // RA2: as above
					for (int j = 0; j < coords->Count(); j++) { // RA2: as above
						if (satisfied[j]) { // RA2: as above
							sum += (*coords)[i].Distance_To((*coords)[j]); // RA2: as above
						}
					}
					if (sum > farthest) { // RA2: as above
						farthest = sum; // RA2: as above
						farthest_index = i; // RA2: as above
					}
				}
			}
			index = farthest_index; // RA2: as above
		}
	}
	satisfied[index] = true; // RA2: as above
	*result = (*coords)[index];
	return(result); // RA2: as above
}

/// <summary>
/// Gives every house that takes part a starting cell, which becomes the center of its base.
/// </summary>
bool MPCooperative::StartingPositionsToHouseBaseCells(char unused) // RA2: 0x005A4170
{
	DynamicVectorClass<Cell> cells = Build_Start_Waypoint_List(unused); // RA2: as above
	unsigned char taken[16]; // RA2: as above
	memset(taken, 0, sizeof(taken)); // RA2: as above
	for (int i = 0; i < Houses.Count(); i++) { // RA2: as above
		HouseClass * house = Houses[i]; // RA2: as above
		if (!house->Class->IsMultiplayPassive) { // RA2: as above
			DebugString("Assigning Starting Points for house %d (%s)\n", i, house->Class->Name()); // RA2: as above
			Cell cell; // RA2: as above
			Cell chosen = *AssignStartingPositionsToHouse(&cell, i, &cells, taken); // RA2: as above
			house->Center = Coord(chosen); // RA2: as above
		}
	}
	return(true); // RA2: as above
}

/// <summary>
/// Sets up the computer players of the selected map: all the slots from the third on that the map has players for, with the difficulty the campaign names.
/// </summary>
bool MPCooperative::vt_entry_58(void) // RA2: 0x005A4250
{
	int players = RandomMapWaypointCount(Session.Options.ScenarioIndex) - 2; // RA2: as above
	DebugString("@@@@ Adding %d AI players\n", players); // RA2: as above
	if (players <= 0) { // RA2: as above
		return(false); // RA2: as above
	}
	int added = 0; // RA2: as above
	Session.Options.AIPlayers = players; // RA2: as above
	CoopCampEntry * record; // RA2: as above
	Campaigns->Get_Record(CampaignIndex, &record); // RA2: as above
	if (record == NULL) { // RA2: as above
		return(false); // RA2: as above
	}
	if (record->Enemies.Count() == 0) { // RA2: as above
		return(false); // RA2: as above
	}
	for (int slot = 2; slot < 8; slot++) { // RA2: as above
		if (added < players) { // RA2: as above
			DebugString("@@@@ Setting slot %d for AI\n", slot); // RA2: as above
			char * name = record->AIName; // RA2: as above
			if (strcmp(name, "Normal") == 0) { // RA2: as above
				Session.Options.AISlots.Difficulties[slot] = 1; // RA2: as above
			} else if (strcmp(name, "Hard") == 0) { // RA2: as above
				Session.Options.AISlots.Difficulties[slot] = 0; // RA2: as above
			} else { // RA2: as above
				Session.Options.AISlots.Difficulties[slot] = 2; // RA2: as above
			}
			DebugString("@@@@     AI difficulty: %d\n", Session.Options.AISlots.Difficulties[slot]); // RA2: as above
			Session.Options.AISlots.Countries[slot] = -2; // RA2: as above
			Session.Options.AISlots.Colors[slot] = -2; // RA2: as above
			added++; // RA2: as above
		} else { // RA2: as above
			Session.Options.AISlots.Difficulties[slot] = -1; // RA2: as above
			Session.Options.AISlots.Countries[slot] = -1; // RA2: as above
			Session.Options.AISlots.Colors[slot] = -1; // RA2: as above
		}
	}
	return(true); // RA2: as above
}

/// <summary>
/// Makes allies of all the human houses.
/// </summary>
bool MPCooperative::AllyTeams(void) // RA2: 0x005A43C0
{
	DebugString("@@@@ Making Alliances\n"); // RA2: as above
	for (int i = 0; i < Houses.Count(); i++) { // RA2: as above
		HouseClass * first = Houses[i]; // RA2: as above
		DebugString("@@@@     House1 = %s\n", Wide_To_Narrow(first->UIName)); // RA2: as above
		if (!first->Class->IsMultiplayPassive && first->IsHuman) { // RA2: as above
			for (int j = i + 1; j < Houses.Count(); j++) { // RA2: as above
				HouseClass * second = Houses[j]; // RA2: as above
				DebugString("@@@@         House2 = %s\n", Wide_To_Narrow(second->UIName)); // RA2: as above
				if (!second->Class->IsMultiplayPassive && second->IsHuman) { // RA2: as above
					char * second_name = Wide_To_Narrow(second->UIName); // RA2: as above
					char * first_name = Wide_To_Narrow(first->UIName); // RA2: as above
					DebugString("@@@@ Making Allies of %s(%d/%d/%d) and %s(%d/%d/%d)\n", // RA2: as above
						first_name, first->Class->House, first->Class->Side, first->Class->Scheme, // RA2: as above
						second_name, second->Class->House, second->Class->Side, second->Class->Scheme); // RA2: as above
					first->Make_Ally(second, false); // RA2: as above
					second->Make_Ally(first, false); // RA2: as above
				}
			}
		}
	}
	return(true); // RA2: as above
}

/// <summary>
/// Picks a random house of the ones the campaign lists for the players.
/// </summary>
int MPCooperative::RandomHumanCountryIndex(void) // RA2: 0x005A4540
{
	int count = HouseTypes.Count(); // RA2: as above
	bool found = false; // RA2: as above
	int campaign = 0; // RA2: as above
	Campaign->Get_Campaign_Type(&campaign); // RA2: as above
	CoopCampEntry * record; // RA2: as above
	Campaigns->Get_Record(campaign, &record); // RA2: as above
	int house; // RA2: as above
	do { // RA2: as above
		house = Scen->RandomNumber(0, count - 1); // RA2: as above
		for (int i = 0; i < record->Players.Count(); i++) { // RA2: as above
			if (house == Find_Country_Index(record->Players[i])) { // RA2: as above
				found = true; // RA2: as above
			}
		}
	} while (!found); // RA2: as above
	DebugString("@@@@ Choosing a random player house: %d(%s)\n", house, Wide_To_Narrow(HouseTypes[house]->UIName)); // RA2: as above
	return(house); // RA2: as above
}

/// <summary>
/// Picks a random house of the ones the campaign lists for the computer players.
/// </summary>
int MPCooperative::RandomAICountryIndex(void) // RA2: 0x005A45F0
{
	int count = HouseTypes.Count(); // RA2: as above
	bool found = false; // RA2: as above
	int campaign = 0; // RA2: as above
	Campaign->Get_Campaign_Type(&campaign); // RA2: as above
	CoopCampEntry * record; // RA2: as above
	Campaigns->Get_Record(campaign, &record); // RA2: as above
	int house; // RA2: as above
	do { // RA2: as above
		house = Scen->RandomNumber(0, count - 1); // RA2: as above
		for (int i = 0; i < record->Enemies.Count(); i++) { // RA2: as above
			if (house == Find_Country_Index(record->Enemies[i])) { // RA2: as above
				found = true; // RA2: as above
			}
		}
	} while (!found); // RA2: as above
	DebugString("@@@@ Choosing a random AI house: %d(%s)\n", house, Wide_To_Narrow(HouseTypes[house]->UIName)); // RA2: as above
	return(house); // RA2: as above
}

/// <summary>
/// The start of the game is the base class's.
/// </summary>
bool MPCooperative::OnGameStart(void) // RA2: 0x005A46A0
{
	return(MultiplayerGameMode::OnGameStart()); // RA2: as above
}

/// <summary>
/// The mode never allows computer players (it sets up its own).
/// </summary>
bool MPCooperative::IsAIAllowed(void) // RA2: 0x005A46B0
{
	return(false); // RA2: as above
}

/// <summary>
/// Called when a map is over: when a human player is still alive the campaign goes on to its next map and is saved; the players and the settings of the lobby are kept so that the lobby can be made again.
/// </summary>
/// <param name="player_wins">Whether the local player won.</param>
void MPCooperative::vt_entry_88(bool player_wins) // RA2: 0x005A46C0
{
	bool win = false; // RA2: as above
	for (int i = 0; i < Houses.Count(); i++) { // RA2: as above
		if (Houses[i] != NULL && Houses[i]->Is_Human_Player() && !Houses[i]->IsDefeated) { // RA2: as above
			win = true; // RA2: as above
		}
	}
	DebugString("@@@@ End-of-Game: PlayerWins:%d, win:%d\n", player_wins, win); // RA2: as above
	if (win) { // RA2: as above
		int scheme = GadgetClass::Get_Color_Scheme(); // RA2: as above
		if (Campaign != NULL) { // RA2: as above
			Campaign->Next_Map(); // RA2: as above
			if (Campaign->Save_Campaign()) { // RA2: as above
				Session.Messages.Add_Message(NULL, 0, StringTable::LoadString("TXT_COOP_SAVE_SUCCESS", NULL, "D:\\Ra2\\MPCoop.cpp", 0x42E), scheme, TextPrintType(TPF_6PT_GRAD | TPF_NOSHADOW), (int)(Rule->MessageDelay * 900.0), false); // RA2: as above
			} else { // RA2: as above
				Session.Messages.Add_Message(NULL, 0, StringTable::LoadString("TXT_COOP_SAVE_FAILED", NULL, "D:\\Ra2\\MPCoop.cpp", 0x430), scheme, TextPrintType(TPF_6PT_GRAD | TPF_NOSHADOW), (int)(Rule->MessageDelay * 900.0), false); // RA2: as above
			}
		}
	}
	wcscpy(HostName, Session.GameName); // RA2: as above
	Scen->IsSkipScore = true; // RA2: as above
	Scen->IsEndOfGame = false; // RA2: as above
	Scen->IsNoMapSel = true; // RA2: as above
	Scen->IsOneTimeOnly = false; // RA2: as above
	Clear_Vector(&SavedPlayers); // RA2: as above
	LocalPlayer = 0; // RA2: as above
	for (int p = 0; p < Session.Players.Count(); p++) { // RA2: as above
		NodeNameType * entry = new NodeNameType; // RA2: as above
		wcscpy(entry->Name, Session.Players[p]->Name); // RA2: as above
		entry->Slot.Clan = Session.Players[p]->Slot.Clan; // RA2: as above
		entry->Slot.Country = Session.Players[p]->Slot.Country; // RA2: as above
		entry->Slot.ColorIdx = Session.Players[p]->Slot.ColorIdx; // RA2: as above
		entry->Slot.HouseIndex = Session.Players[p]->Slot.HouseIndex; // RA2: as above
		strcpy(entry->Player.Serial, Session.Players[p]->Player.Serial); // RA2: as above
		entry->Slot.InitialCountry = Session.Players[p]->Slot.InitialCountry; // RA2: as above
		entry->Slot.InitialColor = Session.Players[p]->Slot.InitialColor; // RA2: as above
		entry->Slot.SpectatorFlag = Session.Players[p]->Slot.SpectatorFlag; // RA2: as above
		entry->Slot.Time = Session.Players[p]->Slot.Time; // RA2: as above
		entry->Slot.Unknown77 = Session.Players[p]->Slot.Unknown77; // RA2: as above
		entry->Slot.Clan = Session.Players[p]->Slot.Clan; // RA2: as above
		entry->Address = Session.Players[p]->Address; // RA2: as above
		SavedPlayers.Add(entry); // RA2: as above
		if (Session.HostAddress == Session.Players[p]->Address) { // RA2: as above
			LocalPlayer = p; // RA2: as above
		}
	}
	memcpy(Addresses, Session.SlotAddresses, sizeof(Addresses)); // RA2: as above
	memcpy(PlayerStates, Session.WolPlayerState, sizeof(PlayerStates)); // RA2: as above
	memcpy(PlayerPortBases, Session.PlayerPortBase, sizeof(PlayerPortBases)); // RA2: as above
	memcpy(PlayerPorts, Session.PlayerPorts, sizeof(PlayerPorts)); // RA2: as above
	wcscpy(MasterName, Session.MasterPlayerName); // RA2: as above
	MasterID = Session.MasterPlayerID; // RA2: as above
	SavedField74 = Session.House; // RA2: as above
	Seed = ::Seed; // RA2: as above
	TechLevel = BuildLevel; // RA2: as above
	UnitCount = Session.Options.UnitCount; // RA2: as above
	Credits = Session.Options.Credits; // RA2: as above
	FogOfWar = Session.Options.FogOfWar; // RA2: as above
	BridgeDestruction = Session.Options.BridgeDestruction; // RA2: as above
	Goodies = Session.Options.Goodies; // RA2: as above
	MCVRedeploy = Session.Options.MCVRedeploy; // RA2: as above
	AlliesAllowed = Session.Options.AlliesAllowed; // RA2: as above
	HarvTruce = Session.Options.HarvTruce; // RA2: as above
	Bases = Session.Options.Bases; // RA2: as above
	CTF = Session.Options.CTF; // RA2: as above
	AIPlayers = Session.Options.AIPlayers; // RA2: as above
	AIDifficulty = Session.Options.AIDifficulty; // RA2: as above
	ShortGame = Session.Options.ShortGame; // RA2: as above
	SWAllowed = Session.Options.SWAllowed; // RA2: as above
	BuildOffAlly = Session.Options.BuildOffAlly; // RA2: as above
	CrapEngineers = Session.Options.CrapEngineers; // RA2: as above
	GameSpeed = Session.Options.GameSpeed; // RA2: as above
	Session.MultiplayerObserver = false; // RA2: as above
	Map.Set_Cursor_Shape(NULL); // RA2: as above
	Map.PendingObjectPtr = 0; // RA2: nothing is being placed any more
	Map.PendingObject = 0; // RA2: as above
	Map.PendingHouse = HOUSE_NONE; // RA2: as above
	for (int row = 0; row < 4; row++) { // RA2: as above
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
	unknown_4 = true; // RA2: as above
	if (ScoreScreen == NULL) { // RA2: as above
		ScoreScreen = new CampaignScoreClass; // RA2: as above
	}
	SendMessageA(list, LB_RESETCONTENT, 0, 0); // RA2: as above
	SaveListed = false; // RA2: as above
	if (Campaign == NULL) { // RA2: as above
		if (Session.Players.Count() == 2) { // RA2: as above
			Campaign = new CoopCampaignClass(Session.Players[0]->Name, Session.Players[1]->Name); // RA2: as above
		}
	} else if (Session.Players.Count() != 2) { // RA2: as above
		Campaign->Set_Names(L"", L""); // RA2: as above
	}
	if (CampaignStates.Count() == 0) { // RA2: as above
		int records; // RA2: as above
		Campaigns->Get_Count(&records); // RA2: as above
		for (int r = 0; r < records; r++) { // RA2: as above
			CoopCampaignClass * state = new CoopCampaignClass; // RA2: as above
			state->Load_Campaign(r); // RA2: as above
			CampaignStates.Add(state); // RA2: as above
		}
	}
	DynamicVectorClass<wchar_t *> names; // RA2: as above
	Campaigns->Copy_Names(names); // RA2: as above
	int count = names.Length(); // RA2: as above
	int i; // RA2: as above
	for (i = 0; i < count; i++) { // RA2: as above
		if (names[i] != NULL) { // RA2: as above
			char * mapname; // RA2: as above
			CampaignStates[i]->Get_Map_Name(&mapname, -1); // RA2: as above
			int map_count = maps->Count(); // RA2: as above
			char name[260]; // RA2: as above
			MultiMission const * found = NULL; // RA2: as above
			sprintf(name, "%s.MAP", mapname); // RA2: as above
			for (int k = 0; k < map_count; k++) { // RA2: as above
				if (found != NULL) { // RA2: as above
					break; // RA2: as above
				}
				char const * scenario = (*maps)[k]->Get_Filename(); // RA2: as above
				if (stricmp(scenario, name) == 0) { // RA2: as above
					found = (*maps)[k]; // RA2: as above
				}
			}
			if (found != NULL) { // RA2: as above
				wchar_t text[260]; // RA2: as above
				swprintf(text, L"%d. %s", i + 1, names[i]); // RA2: as above
				SendMessageA(list, LB_ADDITEM, 0, (LPARAM)text); // RA2: as above
				SendMessageA(list, LB_SETITEMDATA, i, (LPARAM)found); // RA2: as above
			} else { // RA2: as above
				DebugString("@@@@ Could not find %s for %s\n", name, names[i]); // RA2: as above
			}
			delete names[i]; // RA2: as above
		}
	}
	if (Campaign != NULL && Campaign->Has_Saved_Campaign() && !SaveListed) { // RA2: as above
		char name[260]; // RA2: as above
		Campaign->Saved_Map_Is_Undefined(name); // RA2: as above
		strcat(name, ".MAP"); // RA2: as above
		int map_count = maps->Count(); // RA2: as above
		MultiMission const * found = NULL; // RA2: as above
		for (int k = 0; k < map_count; k++) { // RA2: as above
			if (found != NULL) { // RA2: as above
				break; // RA2: as above
			}
			char const * scenario = (*maps)[k]->Get_Filename(); // RA2: as above
			if (stricmp(scenario, name) == 0) { // RA2: as above
				found = (*maps)[k]; // RA2: as above
			}
		}
		if (found != NULL) { // RA2: as above
			SendMessageA(list, LB_ADDITEM, 0, (LPARAM)StringTable::LoadString("TXT_COOP_USE_SAVE", NULL, "D:\\Ra2\\MPCoop.cpp", 0x528)); // RA2: as above
			SendMessageA(list, LB_SETITEMDATA, i, (LPARAM)found); // RA2: as above
			SaveListed = true; // RA2: as above
		}
	}
	SendMessageA(list, LB_SETCURSEL, 0, 0); // RA2: as above
}

/// <summary>
/// Makes the campaign object again: the first campaign, named after the two players when there are two.
/// </summary>
bool MPCooperative::Recreate_Campaign(void) // RA2: 0x005A4F20
{
	if (Campaign != NULL) { // RA2: as above
		delete Campaign; // RA2: as above
	}
	Campaign = NULL; // RA2: as above
	Campaign = new CoopCampaignClass; // RA2: as above
	Campaign->Load_Campaign(0); // RA2: as above
	if (Session.Players.Count() == 2) { // RA2: as above
		Campaign->Set_Names(Session.Players[0]->Name, Session.Players[1]->Name); // RA2: as above
	}
	return((Campaign != NULL) ? true : false); // RA2: as above
}

/// <summary>
/// Leaves the lobby: hides the description of the campaign and puts every player on his own team.
/// </summary>
void MPCooperative::vt_entry_84(void) // RA2: 0x005A4F90
{
	HWND dialog = GameoptWindow(); // RA2: as above
	if (dialog != NULL) { // RA2: as above
		HWND item = GetDlgItem(dialog, IDC_CAMPAIGN_DESCRIPTION); // RA2: as above
		if (item != NULL) { // RA2: as above
			SendMessageA(item, ST_SETTEXT, 0, (LPARAM)L""); // RA2: as above
			ShowWindow(item, SW_HIDE); // RA2: as above
		}
		Fill_Side_Combos(dialog); // RA2: as above
	}
	for (int i = 0; i < Session.Players.Count(); i++) { // RA2: as above
		Session.Players[i]->Slot.SpectatorFlag = i; // RA2: as above
	}
}

/// <summary>
/// Called when the lobby is left or entered again: in the map dialog it takes the row of the saved campaign off the list; in the lobby it loads the campaign again when the saved one was chosen, selects its map, and sends the options.
/// </summary>
void MPCooperative::vt_entry_78(void) // RA2: 0x005A5000
{
	if (WS_Top_Window_ID() == 0x6B) { // RA2: as above
		if (SaveListed) { // RA2: as above
			HWND list = GetDlgItem(WS_Top_Window(), IDC_SCENARIO_LIST); // RA2: as above
			int count = SendMessageA(list, LB_GETCOUNT, 0, 0); // RA2: as above
			if (count == -1) { // RA2: as above
				if (Campaign != NULL) { // RA2: as above
					Campaign->Set_Names(L"", L""); // RA2: as above
				}
				return; // RA2: as above
			}
			if (SendMessageA(list, LB_GETCURSEL, 0, 0) == count - 1) { // RA2: as above
				SendMessageA(list, LB_SETCURSEL, count - 2, 0); // RA2: as above
			}
			SendMessageA(list, LB_DELETESTRING, count - 1, 0); // RA2: as above
			SaveListed = false; // RA2: as above
		}
	} else { // RA2: as above
		if (Campaign != NULL && SavedCampaignChosen) { // RA2: as above
			if (LoadedRecord != 0) { // RA2: as above
				Campaign->Load_Campaign(0); // RA2: as above
				Campaign->Load_Campaign(LoadedRecord); // RA2: as above
			} else { // RA2: as above
				Campaign->Load_Campaign(1); // RA2: as above
				Campaign->Load_Campaign(LoadedRecord); // RA2: as above
			}
			char * map; // RA2: as above
			Campaign->Get_Map_Name(&map, -1); // RA2: as above
			int count = Session.Scenarios.Count(); // RA2: as above
			char name[260]; // RA2: as above
			sprintf(name, "%s.MAP", map); // RA2: as above
			int i; // RA2: as above
			for (i = 0; i < count; i++) { // RA2: as above
				char const * scenario = Session.Scenarios[i]->Get_Filename(); // RA2: as above
				if (stricmp(scenario, name) == 0) { // RA2: as above
					break; // RA2: as above
				}
			}
			Session.Options.ScenarioIndex = i; // RA2: as above
			Scen->Scenario = i; // RA2: as above
			Set_Scenario_Info_From_Index(i); // RA2: as above
		}
		HWND dialog = GameoptWindow(); // RA2: as above
		if (dialog != NULL) { // RA2: as above
			Set_Item_Text(dialog, this); // RA2: as above
			Set_Skirmish_Dialog_Text(dialog); // RA2: as above
			Update_Network_Dialog_Preview(dialog); // RA2: as above
		}
		Send_Lobby_Options(true, true); // RA2: as above
	}
	if (Campaign != NULL) { // RA2: as above
		Campaign->Set_Names(L"", L""); // RA2: as above
	}
}

/// <summary>
/// Makes the campaign object when there is none and, when the saved campaign is there, offers it: as a row in the list of maps, or as a lobby message.
/// </summary>
void MPCooperative::vt_entry_7C(void) // RA2: 0x005A5190
{
	if (Campaign == NULL) { // RA2: as above
		Campaign = new CoopCampaignClass; // RA2: as above
		Campaign->Load_Campaign(0); // RA2: as above
	}
	if (Campaign != NULL) { // RA2: as above
		if (Session.Players.Count() == 2) { // RA2: as above
			Campaign->Set_Names(Session.Players[0]->Name, Session.Players[1]->Name); // RA2: as above
		}
		vt_entry_50(); // RA2: as above
		if (Campaign->Has_Saved_Campaign()) { // RA2: as above
			if (WS_Top_Window_ID() == 0x6B) { // RA2: as above
				if (!SaveListed) { // RA2: as above
					char name[260]; // RA2: as above
					Campaign->Saved_Map_Is_Undefined(name); // RA2: as above
					strcat(name, ".MAP"); // RA2: as above
					MultiMission * map = NULL; // RA2: as above
					int count = Session.Scenarios.Count(); // RA2: as above
					for (int i = 0; i < count; i++) { // RA2: as above
						if (map != NULL) { // RA2: as above
							break; // RA2: as above
						}
						char const * scenario = Session.Scenarios[i]->Get_Filename(); // RA2: as above
						if (stricmp(scenario, name) == 0) { // RA2: as above
							map = Session.Scenarios[i]; // RA2: as above
						}
					}
					if (map != NULL) { // RA2: as above
						HWND list = GetDlgItem(WS_Top_Window(), IDC_SCENARIO_LIST); // RA2: as above
						int rows = SendMessageA(list, LB_GETCOUNT, 0, 0); // RA2: as above
						if (rows != -1) { // RA2: as above
							SendMessageA(list, LB_ADDITEM, 0, (LPARAM)StringTable::LoadString("TXT_COOP_USE_SAVE", NULL, "D:\\Ra2\\MPCoop.cpp", 0x5E1)); // RA2: as above
							SendMessageA(list, LB_SETITEMDATA, rows, (LPARAM)map); // RA2: as above
							SaveListed = true; // RA2: as above
						}
					}
				}
			} else { // RA2: as above
				Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_SAVE_CAMP_AVAILABLE", NULL, "D:\\Ra2\\MPCoop.cpp", 0x5E7)); // RA2: as above
			}
		}
	}
}

/// <summary>
/// Returns 2 (the base class returns -1).
/// </summary>
int MPCooperative::vt_entry_80(void) // RA2: 0x005A5340
{
	return(2); // RA2: as above
}

/// <summary>
/// Shows the score screen of the map that was played and returns whether the players are done with it.
/// </summary>
bool MPCooperative::vt_entry_90(void) // RA2: 0x005A5350
{
	if (ScoreScreen == NULL) { // RA2: as above
		ScoreScreen = new CampaignScoreClass; // RA2: as above
	}
	if (Campaign == NULL) { // RA2: as above
		return(false); // RA2: as above
	}
	if (Campaigns == NULL) { // RA2: as above
		return(false); // RA2: as above
	}
	DynamicVectorClass<MPlayerScoreType *> scores; // RA2: as above
	int time; // RA2: as above
	Campaign->Get_Player_Scores(scores, &time); // RA2: as above
	CoopCampEntry * record = NULL; // RA2: as above
	int type; // RA2: as above
	if (!Campaign->Get_Campaign_Type(&type)) { // RA2: as above
		type = -1; // RA2: as above
	} else { // RA2: as above
		Campaigns->Get_Record(type, &record); // RA2: as above
	}
	int difficulty = -1; // RA2: as above
	if (type == 0 || type == 1) { // RA2: as above
		difficulty = 0; // RA2: as above
	} else if (type == 2 || type == 3) { // RA2: as above
		difficulty = 1; // RA2: as above
	} else if (type == 4) { // RA2: as above
		difficulty = 2; // RA2: as above
	}
	int count; // RA2: as above
	Campaign->Get_Map_Count(&count); // RA2: as above
	int current; // RA2: as above
	Campaign->Get_Current_Map(&current); // RA2: as above
	if (record != NULL) { // RA2: as above
		ScoreScreen->Presentation(scores, Scen->ElapsedTimer(), current < count, difficulty, record->Name); // RA2: as above
	} else { // RA2: as above
		ScoreScreen->Presentation(scores, Scen->ElapsedTimer(), current < count, difficulty, L""); // RA2: as above
	}
	while (scores.Count() != 0) { // RA2: as above
		if (scores[0] != NULL) { // RA2: as above
			delete scores[0]; // RA2: as above
		}
		scores[0] = NULL; // RA2: as above
		scores.Delete_Index(0); // RA2: as above
	}
	bool result = CampaignScoreDone; // RA2: as above
	CampaignScoreDone = false; // RA2: as above
	return(result); // RA2: as above
}

/// <summary>
/// Shows the score screen of the whole campaign when the last map has been played.
/// </summary>
/// <param name="show">Whether the screen is shown.</param>
void MPCooperative::vt_entry_8C(bool show) // RA2: 0x005A5570
{
	if (Campaign != NULL) { // RA2: as above
		int count; // RA2: as above
		Campaign->Get_Map_Count(&count); // RA2: as above
		int current; // RA2: as above
		Campaign->Get_Current_Map(&current); // RA2: as above
		if (show && current >= count) { // RA2: as above
			CampaignEndScoreClass * score = new CampaignEndScoreClass; // RA2: as above
			if (score != NULL) { // RA2: as above
				DynamicVectorClass<MPlayerScoreType *> scores; // RA2: as above
				int time; // RA2: as above
				Campaign->Get_Player_Scores(scores, &time); // RA2: as above
				CoopCampEntry * record = NULL; // RA2: as above
				int type; // RA2: as above
				if (!Campaign->Get_Campaign_Type(&type)) { // RA2: as above
					type = -1; // RA2: as above
				} else { // RA2: as above
					Campaigns->Get_Record(type, &record); // RA2: as above
				}
				int difficulty = -1; // RA2: as above
				if (type == 0 || type == 1) { // RA2: as above
					difficulty = 0; // RA2: as above
				} else if (type == 2 || type == 3) { // RA2: as above
					difficulty = 1; // RA2: as above
				} else if (type == 4) { // RA2: as above
					difficulty = 2; // RA2: as above
				}
				score->Presentation(scores, time, difficulty, type + 1, record->Name); // RA2: as above
				delete score; // RA2: as above
				while (scores.Count() != 0) { // RA2: as above
					if (scores[0] != NULL) { // RA2: as above
						delete scores[0]; // RA2: as above
					}
					scores[0] = NULL; // RA2: as above
					scores.Delete_Index(0); // RA2: as above
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
	NetNumType net; // RA2: as above
	NetNodeType node; // RA2: as above
	unsigned short port; // RA2: as above
	char * map; // RA2: as above
	Campaign->Get_Map_Name(&map, -1); // RA2: as above
	char name[260]; // RA2: as above
	sprintf(name, "%s.MAP", map); // RA2: as above
	strcpy(Scen->NextScenarioName, name); // RA2: as above
	Scen->Campaign = CAMPAIGN_NONE; // RA2: as above
	Scen->IsSkipScore = true; // RA2: as above
	int count = Session.Scenarios.Count(); // RA2: as above
	sprintf(name, "%s.MAP", map); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		if (strncmp(name, Session.Scenarios[i]->Get_Filename(), _MAX_PATH) == 0) { // RA2: as above
			Session.Options.ScenarioIndex = i; // RA2: as above
			break; // RA2: as above
		}
	}
	GameID = 0; // RA2: as above
	GameProduct = g_GameSKU; // RA2: as above
	WestwoodOnline_Tournament = 0; // RA2: as above
	strcpy(PlayerNickname, g_NickName.get()); // RA2: as above
	wcscpy(Session.GameName, HostName); // RA2: as above
	Clear_Vector(&Session.Players); // RA2: as above
	Session.NumPlayers = 0; // RA2: as above
	for (int p = 0; p < SavedPlayers.Count(); p++) { // RA2: as above
		NodeNameType * entry = new NodeNameType; // RA2: as above
		wcscpy(entry->Name, SavedPlayers[p]->Name); // RA2: as above
		entry->Slot.Clan = SavedPlayers[p]->Slot.Clan; // RA2: as above
		entry->Slot.Country = SavedPlayers[p]->Slot.Country; // RA2: as above
		entry->Slot.ColorIdx = SavedPlayers[p]->Slot.ColorIdx; // RA2: as above
		entry->Slot.HouseIndex = SavedPlayers[p]->Slot.HouseIndex; // RA2: as above
		strcpy(entry->Player.Serial, SavedPlayers[p]->Player.Serial); // RA2: as above
		entry->Slot.InitialCountry = SavedPlayers[p]->Slot.InitialCountry; // RA2: as above
		entry->Slot.InitialColor = SavedPlayers[p]->Slot.InitialColor; // RA2: as above
		entry->Slot.SpectatorFlag = SavedPlayers[p]->Slot.SpectatorFlag; // RA2: as above
		entry->Slot.Time = SavedPlayers[p]->Slot.Time; // RA2: as above
		entry->Slot.Unknown77 = SavedPlayers[p]->Slot.Unknown77; // RA2: as above
		entry->Slot.Clan = SavedPlayers[p]->Slot.Clan; // RA2: as above
		entry->Address = SavedPlayers[p]->Address; // RA2: as above
		Session.Players.Add(entry); // RA2: as above
		Session.NumPlayers++; // RA2: as above
	}
	int computers = Session.Scenarios[Session.Options.ScenarioIndex]->MaxPlayers - 2; // RA2: as above
	if (computers > 0) { // RA2: as above
		Session.Options.AIPlayers = computers; // RA2: as above
		int used = 0; // RA2: as above
		CoopCampEntry * record; // RA2: as above
		Campaigns->Get_Record(CampaignIndex, &record); // RA2: as above
		int current; // RA2: as above
		Campaign->Get_Current_Map(&current); // RA2: as above
		for (int slot = 2; slot < 8; slot++) { // RA2: as above
			if (used < computers) { // RA2: as above
				if (strcmp(record->AIName, "Normal") == 0) { // RA2: as above
					Session.Options.AISlots.Difficulties[slot] = 1; // RA2: as above
				} else if (strcmp(record->AIName, "Hard") == 0) { // RA2: as above
					Session.Options.AISlots.Difficulties[slot] = 0; // RA2: as above
				} else { // RA2: as above
					Session.Options.AISlots.Difficulties[slot] = 2; // RA2: as above
				}
				Session.Options.AISlots.Countries[slot] = HouseChoices[slot][current]; // RA2: as above
				Session.Options.AISlots.Colors[slot] = ColorChoices[slot][current]; // RA2: as above
				used++; // RA2: as above
			} else { // RA2: as above
				Session.Options.AISlots.Countries[slot] = -1; // RA2: as above
				Session.Options.AISlots.Colors[slot] = -1; // RA2: as above
			}
		}
	}
	Session.HostAddress = Session.Players[LocalPlayer]->Address; // RA2: as above
	wcscpy(Session.MasterPlayerName, Session.Players[LocalPlayer]->Name); // RA2: as above
	Session.MasterPlayerID = -1; // RA2: as above
	Session.House = (HousesType)SavedField74; // RA2: as above
	PacketTransport->Clear_Broadcast_Addresses(); // RA2: as above
	char address[32] = "xxx.xxx.xxx.xxx"; // RA2: as above
	for (int n = 1; n < Session.Players.Count(); n++) { // RA2: as above
		Session.Players[n]->Address.Get_Address(net, node, &port); // RA2: as above
		PacketTransport->Set_Broadcast_Address(address, port); // RA2: as above
	}
	memcpy(Session.SlotAddresses, Addresses, sizeof(Addresses)); // RA2: as above
	memcpy(Session.WolPlayerState, PlayerStates, sizeof(PlayerStates)); // RA2: as above
	memcpy(Session.PlayerPortBase, PlayerPortBases, sizeof(PlayerPortBases)); // RA2: as above
	memcpy(Session.PlayerPorts, PlayerPorts, sizeof(PlayerPorts)); // RA2: as above
	wcscpy(Session.MasterPlayerName, MasterName); // RA2: as above
	Session.MasterPlayerID = MasterID; // RA2: as above
	::Seed = Seed; // RA2: as above
	BuildLevel = TechLevel; // RA2: as above
	Session.Options.UnitCount = UnitCount; // RA2: as above
	Session.Options.Credits = Credits; // RA2: as above
	Session.Options.FogOfWar = FogOfWar; // RA2: as above
	Session.Options.BridgeDestruction = BridgeDestruction; // RA2: as above
	Session.Options.Goodies = Goodies; // RA2: as above
	Session.Options.MCVRedeploy = MCVRedeploy; // RA2: as above
	Session.Options.AlliesAllowed = AlliesAllowed; // RA2: as above
	Session.Options.HarvTruce = HarvTruce; // RA2: as above
	Session.Options.Bases = Bases; // RA2: as above
	Session.Options.CTF = CTF; // RA2: as above
	Session.Options.AIDifficulty = (DiffType)AIDifficulty; // RA2: as above
	Session.Options.ShortGame = ShortGame; // RA2: as above
	Session.Options.SWAllowed = SWAllowed; // RA2: as above
	Session.Options.BuildOffAlly = BuildOffAlly; // RA2: as above
	Session.Options.CrapEngineers = CrapEngineers; // RA2: as above
	Session.Options.GameSpeed = GameSpeed; // RA2: as above
	Set_Scenario_Info_From_Index(Session.Options.ScenarioIndex); // RA2: as above
	Options.GameSpeed = Session.Options.GameSpeed; // RA2: as above
	Scen->Scenario = Session.Options.ScenarioIndex; // RA2: as above
	switch (Session.Options.GameSpeed) { // RA2: as above
		case 0: // RA2: as above
			Session.MaxAhead = 40; // RA2: as above
			Session.PrecalcDesiredFrameRate = 60; // RA2: as above
			Session.FrameSendRate = 10; // RA2: as above
			break; // RA2: as above
		case 1: // RA2: as above
			Session.MaxAhead = 40; // RA2: as above
			Session.PrecalcDesiredFrameRate = 45; // RA2: as above
			Session.FrameSendRate = 10; // RA2: as above
			break; // RA2: as above
		case 2: // RA2: as above
			Session.MaxAhead = 30; // RA2: as above
			Session.PrecalcDesiredFrameRate = 30; // RA2: as above
			break; // RA2: as above
		case 3: // RA2: as above
			Session.MaxAhead = 20; // RA2: as above
			Session.PrecalcDesiredFrameRate = 20; // RA2: as above
			break; // RA2: as above
		case 4: // RA2: as above
			Session.MaxAhead = 20; // RA2: as above
			Session.PrecalcDesiredFrameRate = 15; // RA2: as above
			break; // RA2: as above
		case 5: // RA2: as above
			Session.MaxAhead = 20; // RA2: as above
			Session.PrecalcDesiredFrameRate = 12; // RA2: as above
			break; // RA2: as above
		default: // RA2: as above
			Session.MaxAhead = 10; // RA2: as above
			Session.PrecalcDesiredFrameRate = 10; // RA2: as above
			break; // RA2: as above
	}
	Session.PrecalcMaxAhead = 0; // RA2: as above
	if (!unknown_4) { // RA2: as above
		Session.PrecalcDesiredFrameRate = 0; // RA2: as above
	}
	int ping = 0; // RA2: as above
	for (int k = 0; k < Session.Players.Count(); k++) { // RA2: as above
		if (PlayerPings[k] > ping) { // RA2: as above
			ping = PlayerPings[k]; // RA2: as above
		}
	}
	Session.LatencyFudge = 1; // RA2: as above
	if (ping == 0 || ping == 1000) { // RA2: as above
		Session.PrecalcDesiredFrameRate = 0; // RA2: as above
		DebugString("Ping times not available\n"); // RA2: as above
	} else { // RA2: as above
		DebugString("Max ping is %d ms\n", ping); // RA2: as above
		if (ping > 1000) { // RA2: as above
			Session.LatencyFudge = 2; // RA2: as above
		} else if (ping < 600) { // RA2: as above
			Session.LatencyFudge = 1; // RA2: as above
		}
		int retry = ping * 60 / 1000; // RA2: as above
		retry += retry * 120 / 100; // RA2: as above
		Ipx.Set_Timing(retry, -1, retry * 8, true); // RA2: as above
		if (unknown_4) { // RA2: as above
			Ipx.Set_Lobby_Timing(5, -1, 120); // RA2: as above
			if (unknown_4) { // RA2: as above
				switch (Session.LatencyFudge) { // RA2: as above
					case 0: // RA2: as above
					case 1: // RA2: as above
						Session.PrecalcMaxAhead = Session.MaxAhead; // RA2: as above
						break; // RA2: as above
					case 2: // RA2: as above
						Session.PrecalcMaxAhead = Session.MaxAhead / 2 + Session.MaxAhead; // RA2: as above
						break; // RA2: as above
					case 3: // RA2: as above
						Session.PrecalcMaxAhead = Session.MaxAhead * 2; // RA2: as above
						break; // RA2: as above
				}
				Session.PrecalcMaxAhead = (unsigned int)Session.PrecalcMaxAhead / 10 * 10; // RA2: an unsigned division
			}
		}
	}
	Session.MaxMaxAhead = Session.MaxAhead; // RA2: as above
	memset(Session.ConnectionStats, 0, sizeof(Session.ConnectionStats)); // RA2: as above
	Squad1Names[0] = '\0'; // RA2: as above
	Options.GameSpeed = Session.Options.GameSpeed; // RA2: as above
	Squad2Names[0] = '\0'; // RA2: as above
	PregameSetup(); // RA2: as above
	DoList.Init(); // RA2: as above
	OutList.Init(); // RA2: as above
	ToolTips->Activate(false); // RA2: as above
	DebugString("@@@@ Before destroying connections players.count() = %d\n", Session.Players.Count()); // RA2: as above
	while (Ipx.Num_Connections()) { // RA2: as above
		Ipx.Delete_Connection(Ipx.Connection_ID(0)); // RA2: as above
	}
	if (Session.ViaPacketRouter) { // RA2: as above
		if (PacketRouterConnection) { // RA2: as above
			OwnerTalk.Send_Shutdown(); // RA2: as above
		}
		Session.ViaPacketRouter = false; // RA2: as above
	}
	DebugString("@@@@ After destroying connections players.count() = %d\n", Session.Players.Count()); // RA2: as above
}

/// <summary>
/// Makes the lobby's players again from the saved ones: every saved player whose name is a house of the game is given the house's index.
/// </summary>
void MPCooperative::vt_entry_98(void) // RA2: 0x005A6040
{
	Clear_Vector(&Session.Players); // RA2: as above
	Session.NumPlayers = 0; // RA2: as above
	for (int i = 0; i < SavedPlayers.Count(); i++) { // RA2: as above
		for (int h = 0; h < Houses.Count(); h++) { // RA2: as above
			if (wcsicmp(Houses[h]->UIName, SavedPlayers[i]->Name) == 0) { // RA2: as above
				SavedPlayers[i]->Slot.HouseIndex = h; // RA2: as above
				NodeNameType * entry = new NodeNameType; // RA2: as above
				wcscpy(entry->Name, SavedPlayers[i]->Name); // RA2: as above
				entry->Slot.Clan = SavedPlayers[i]->Slot.Clan; // RA2: as above
				entry->Slot.Country = SavedPlayers[i]->Slot.Country; // RA2: as above
				entry->Slot.ColorIdx = SavedPlayers[i]->Slot.ColorIdx; // RA2: as above
				strcpy(entry->Player.Serial, SavedPlayers[i]->Player.Serial); // RA2: as above
				entry->Slot.InitialCountry = SavedPlayers[i]->Slot.InitialCountry; // RA2: as above
				entry->Slot.InitialColor = SavedPlayers[i]->Slot.InitialColor; // RA2: as above
				entry->Slot.SpectatorFlag = SavedPlayers[i]->Slot.SpectatorFlag; // RA2: as above
				entry->Slot.Time = SavedPlayers[i]->Slot.Time; // RA2: as above
				entry->Slot.Unknown77 = SavedPlayers[i]->Slot.Unknown77; // RA2: as above
				entry->Slot.Clan = SavedPlayers[i]->Slot.Clan; // RA2: as above
				NetNumType net; // RA2: as above
				NetNodeType node; // RA2: as above
				unsigned short socket; // RA2: as above
				SavedPlayers[i]->Address.Get_Address(net, node, &socket); // RA2: as above
				entry->Address.Set_Address(net, node, socket); // RA2: as above
				Session.Players.Add(entry); // RA2: as above
				Session.NumPlayers++; // RA2: as above
				break; // RA2: as above
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
	if (Campaign != NULL) { // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}

/// <summary>
/// Always true.
/// </summary>
bool MPCooperative::vt_entry_04(void) // RA2: 0x005A6280
{
	return(true); // RA2: as above
}

/// <summary>
/// Always true.
/// </summary>
bool MPCooperative::vt_entry_08(void) // RA2: 0x005A6290
{
	return(true); // RA2: as above
}

/// <summary>
/// Always false.
/// </summary>
bool MPCooperative::vt_entry_A0(void) // RA2: 0x005A62A0
{
	return(false); // RA2: as above
}
