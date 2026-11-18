/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the multiplayer game modes: the list of the modes MPModes.ini names, and what a mode
**	does for the lobby (the mode and team lists), for the teams and for the starting positions
**	and starting units of a skirmish or network game. TibSun has no such file; the code is the
**	executable's from 0x005B6C30.
*/

#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: the includes this file needs
#include "vector.h" // RA2: the includes this file needs
#include "multiplayer.h" // RA2: the includes this file needs
#include "mpsiege.h" // RA2: the includes this file needs
#include "mpmanbattle.h"
#include "mpfreeforall.h" // RA2: the includes this file needs
#include "mpcoop.h" // RA2: the real MPCooperative class (this file only constructs it)
#include "ccini.h" // RA2: the includes this file needs
#include "ccfile.h" // RA2: the includes this file needs
#include "stringtable.h" // RA2: the includes this file needs
#include "session.h" // RA2: the includes this file needs
#include "scenario.h" // RA2: the includes this file needs
#include "house.h" // RA2: the includes this file needs
#include "houstype.h" // RA2: the includes this file needs
#include "unittype.h" // RA2: the includes this file needs
#include "unit.h" // RA2: the includes this file needs
#include "techno.h" // RA2: the includes this file needs
#include "infatype.h" // RA2: the includes this file needs
#include "globals.h" // RA2: the includes this file needs
#include "win.h"
#include "_rules.h" // RA2: the includes this file needs
#include "rules.h" // RA2: the includes this file needs
#include "ccrand.h" // RA2: the includes this file needs
#include "dbgprint.h" // RA2: the includes this file needs
#include <windows.h> // RA2: the includes this file needs
#undef LoadString // RA2: as above
#include "rect.h" // RA2: the includes this file needs
#include "wonline.h"
#include "unholyalliance.h"



class MultiplayerBattle : public MultiplayerGameMode // RA2: constructed at 0x005A2320; only what this file needs of it (its own file declares the rest)
{
	public:
		MultiplayerBattle(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex); // RA2: as above
};




const UINT LB_ADDITEM = 0x4CD; // RA2: WM_USER + 205, the game's own list box message that adds an item (which the owner-drawn lists answer)

static char const * MultiplayerDialogSettings = "MultiplayerDialogSettings"; // RA2: 0x007A7170, a pointer to the name of the INI section the mode's INI file keeps its settings in

static inline bool Has_Settings(CCINIClass * ini) // RA2: the test of the constructor, whose result is kept as a flag
{
	if (ini == NULL) { // RA2: as above
		return(false); // RA2: as above
	}
	return(ini->Section_Present(MultiplayerDialogSettings)); // RA2: as above
}

/// <summary>
/// The list of the registered game modes. Its static local is built at the first call; every
/// caller that is expanded builds it too.
/// </summary>
DynamicVectorClass<MultiplayerGameMode *> & Game_Modes(void) // RA2: 0x005B6C30
{
	static DynamicVectorClass<MultiplayerGameMode *> modes; // RA2: the static at 0x00A71A10 with its guard bit at 0x00A719E0; its destructor runs at exit
	return(modes); // RA2: as above
}

/// <summary>
/// Makes a game mode from its list entry: the names, the INI file name, the map filter and
/// whether the computer may play; reads the INI file of the mode, whose
/// [MultiplayerDialogSettings] says whether Westwood Online tournaments may use the mode.
/// </summary>
MultiplayerGameMode::MultiplayerGameMode(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex) : // RA2: 0x005B6CD0
	MPTeams(), // RA2: as above
	UIName(title), // RA2: as above
	CSFTooltip(tooltip), // RA2: as above
	MPModeIndex(modeindex), // RA2: as above
	INIFilename(inifile), // RA2: as above
	MapFilter(mapfilter), // RA2: as above
	AIAllowed(aiallowed), // RA2: as above
	INI(NULL) // RA2: as above
{
	unknown_4 = false; // RA2: +0x04
	wolTourney = 1; // RA2: +0x3C, 1 unless the INI file says otherwise
	GetINI(); // RA2: expanded here; the INI is made and read
	if (Has_Settings(INI)) { // RA2: as above
		wolTourney = INI->Get_Int(MultiplayerDialogSettings, "WonlineTournamentAllowed", wolTourney); // RA2: as above
	}
}

/// <summary>
/// Deletes the teams and the INI file object.
/// </summary>
MultiplayerGameMode::~MultiplayerGameMode(void) // RA2: 0x005B6E20
{
	DeleteTeams(); // RA2: as above
	delete INI; // RA2: as above
}

/// <summary>
/// The country a player of this mode gets when the lobby does not choose one: none (-2).
/// </summary>
int MultiplayerGameMode::DefaultCountry(void) // RA2: 0x005B6EB0, slot 0x28 of the vtable
{
	return(-2); // RA2: as above
}

/// <summary>
/// The first registered game mode, or NULL when there is none.
/// </summary>
MultiplayerGameMode * First_Game_Mode(void) // RA2: 0x005B6EC0
{
	if (Game_Modes().Count() == 0) { // RA2: as above
		return(NULL); // RA2: as above
	}
	return(Game_Modes()[0]); // RA2: as above
}

/// <summary>
/// How many game modes are registered.
/// </summary>
int __cdecl MultiplayerGameMode::GetModeCount(void) // RA2: 0x005B6F80
{
	return(Game_Modes().Count()); // RA2: as above
}

/// <summary>
/// The registered game mode with a mode index, or NULL.
/// </summary>
MultiplayerGameMode * __fastcall MultiplayerGameMode::Get(int index) // RA2: 0x005B6FE0
{
	for (int i = 0; i < GetModeCount(); i++) { // RA2: as above
		MultiplayerGameMode * mode = Game_Modes()[i]; // RA2: as above
		if (mode->MPModeIndex == index) { // RA2: as above
			return(mode); // RA2: as above
		}
	}
	return(NULL); // RA2: as above
}

/// <summary>
/// Fills a combo box with the names of the game modes (each mode is the item's data) and
/// selects the one with the mode index; a skirmish lists only the modes that allow it.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void __fastcall Fill_Game_Mode_Combo(HWND combo, int index) // RA2: 0x005B70B0
{
	Clear_ComboBox(combo); // RA2: as above
	bool skirmish = Session.Type == GAME_SKIRMISH; // RA2: as above
	for (int i = 0; i < MultiplayerGameMode::GetModeCount(); i++) { // RA2: as above
		MultiplayerGameMode * mode = Game_Modes()[i]; // RA2: as above
		if (!skirmish || mode->IsSkirmishAllowed()) { // RA2: as above
			int item = SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)mode->UIName.text()); // RA2: as above
			if (mode->MPModeIndex == index) { // RA2: as above
				SendMessage(combo, CB_SETCURSEL, item, 0); // RA2: as above
			}
			SendMessage(combo, CB_SETITEMDATA, item, (LPARAM)mode); // RA2: as above
		}
	}
}
#endif

/// <summary>
/// Fills the game mode list box of the lobby with the names of the game modes (each mode is
/// the item's data) and selects the one with the mode index. Left out: a mode that Westwood
/// Online tournaments may not use while the channel is one, a mode that does not suit an
/// observer when the local player is one, and in a skirmish the modes that do not allow it.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void __fastcall Fill_Game_Mode_List(HWND list, int index) // RA2: 0x005B71E0
{
	SendMessage(list, LB_RESETCONTENT, 0, 0); // RA2: as above
	bool skirmish = Session.Type == GAME_SKIRMISH; // RA2: as above
	for (int i = 0; i < MultiplayerGameMode::GetModeCount(); i++) { // RA2: as above
		MultiplayerGameMode * mode = Game_Modes()[i]; // RA2: as above
		if (mode->wolTourney == 0 && Session.Type == GAME_INTERNET && Is_Current_Channel_Tournament()) { // RA2: as above
			continue; // RA2: as above
		}
		if (LobbyObserverSlot == 0 && Session.Slots[0]->Slot.SpectatorFlag == -1 && !mode->vt_entry_A0()) { // RA2: as above
			continue; // RA2: as above
		}
		if (!skirmish || mode->IsSkirmishAllowed()) { // RA2: as above
			int item = SendMessage(list, LB_ADDITEM, 0, (LPARAM)mode->UIName.text()); // RA2: as above
			if (mode->MPModeIndex == index) { // RA2: as above
				SendMessage(list, LB_SETCURSEL, item, 0); // RA2: as above
			}
			SendMessage(list, LB_SETITEMDATA, item, (LPARAM)mode); // RA2: as above
		}
	}
}
#endif

/// <summary>
/// Gives every player a team of their own (the team is the player's number), except those
/// that have no country, who are observers (team -1).
/// </summary>
bool MultiplayerGameMode::AssignTeams(void) // RA2: 0x005B7350
{
	for (int i = 0; i < Session.Players.Count(); i++) { // RA2: as above
		if (Session.Players[i]->Get_Country() == -3) { // RA2: as above
			Session.Players[i]->Slot.SpectatorFlag = -1; // RA2: as above
		} else { // RA2: as above
			Session.Players[i]->Slot.SpectatorFlag = i; // RA2: as above
		}
	}
	return(true); // RA2: as above
}

/// <summary>
/// Every player is allowed to start; a mode may refuse and say why.
/// </summary>
bool MultiplayerGameMode::CheckStartConditions(WideWstring * message) // RA2: 0x005B73A0
{
	return(true); // RA2: as above
}

/// <summary>
/// A slot of the vtable not named yet: this mode answers true (what it is asked is open).
/// </summary>
bool MultiplayerGameMode::vt_entry_50(void) // RA2: 0x005B73B0, slot 0x50 of the vtable; true
{
	return(true); // RA2: as above
}

/// <summary>
/// A slot of the vtable not named yet: this mode answers true (what it is asked is open).
/// </summary>
bool MultiplayerGameMode::vt_entry_54(void) // RA2: 0x005B73C0, slot 0x54 of the vtable; true
{
	return(true); // RA2: as above
}

/// <summary>
/// The teams need no change when the game starts.
/// </summary>
bool MultiplayerGameMode::AssignTeamsAtStart(void) // RA2: 0x005B73D0
{
	return(true); // RA2: as above
}

/// <summary>
/// Whether the computer may play in this mode (the last entry of the mode's line in MPModes.ini).
/// </summary>
bool MultiplayerGameMode::IsAIAllowed(void) // RA2: 0x005B73E0
{
	return(AIAllowed); // RA2: as above
}

/// <summary>
/// A skirmish does not offer the mode unless it says so.
/// </summary>
bool MultiplayerGameMode::IsSkirmishAllowed(void) // RA2: 0x005B73F0
{
	return(false); // RA2: as above
}

/// <summary>
/// The index of the first map in the session's list that this mode allows, or -1.
/// </summary>
int MultiplayerGameMode::FirstValidMapIndex(void) // RA2: 0x005B7400
{
	int count = Session.Scenarios.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		if (IsMapAllowed(Session.Scenarios[i])) { // RA2: as above
			return(i); // RA2: as above
		}
	}
	return(-1); // RA2: as above
}

/// <summary>
/// Whether a map suits this mode: a random map needs a mode that allows the computer; another
/// map must not be unofficial in a tournament channel and must list the mode's map filter.
/// </summary>
bool MultiplayerGameMode::IsMapAllowed(MultiMission const * map) // RA2: 0x005B7470
{
	if (map->Is_Random_Map()) { // RA2: as above
		return(IsAIAllowed()); // RA2: as above
	}
	if (!map->Get_Official() && Is_Current_Channel_Tournament()) { // RA2: as above
		return(false); // RA2: as above
	}
	return(map->Supports_Game_Mode(MapFilter)); // RA2: as above
}

/// <summary>
/// The country of a human player who has none chosen: the session picks a random side.
/// </summary>
int MultiplayerGameMode::RandomHumanCountryIndex(void) // RA2: 0x005B74C0
{
	return(Session.Random_Side()); // RA2: as above
}

/// <summary>
/// The computer's countries are chosen as the humans' are.
/// </summary>
int MultiplayerGameMode::RandomAICountryIndex(void) // RA2: 0x005B74D0
{
	return(RandomHumanCountryIndex()); // RA2: as above
}

/// <summary>
/// Fills a combo box with the entries of a team list (each entry is its own item data) and
/// selects the given one.
/// </summary>
void MultiplayerGameMode::PopulateTeamDropdown(HWND hwnd, DynamicVectorClass<MultiplayerTeam *> * teams, MultiplayerTeam * team) // RA2: 0x005B74E0
{
	Clear_ComboBox(hwnd); // RA2: as above
	for (int i = 0; i < teams->Count(); i++) { // RA2: as above
		MultiplayerTeam * entry = (*teams)[i]; // RA2: as above
		int item = SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)entry); // RA2: as above
		if (entry == team) { // RA2: as above
			SendMessage(hwnd, CB_SETCURSEL, item, 0); // RA2: as above
		}
		SendMessage(hwnd, CB_SETITEMDATA, item, (LPARAM)entry); // RA2: as above
	}
}

/// <summary>
/// Fills a list box with the maps of a list (each map is its own item data) and selects the given one.
/// </summary>
void MultiplayerGameMode::DrawTeamDropdown(HWND hwnd, DynamicVectorClass<MultiMission const *> * maps, MultiMission const * map) // RA2: 0x005B7550
{
	SendMessage(hwnd, LB_RESETCONTENT, 0, 0); // RA2: as above
	for (int i = 0; i < maps->Count(); i++) { // RA2: as above
		MultiMission const * entry = (*maps)[i]; // RA2: as above
		int item = SendMessage(hwnd, LB_ADDITEM, 0, (LPARAM)entry); // RA2: as above
		if (entry == map) { // RA2: as above
			SendMessage(hwnd, LB_SETCURSEL, item, 0); // RA2: as above
		}
		SendMessage(hwnd, LB_SETITEMDATA, item, (LPARAM)entry); // RA2: as above
	}
}

/// <summary>
/// Makes the teams again and fills a combo box with their titles (each team is the item's
/// data), selecting the team of the player with the list index.
/// </summary>
void MultiplayerGameMode::PopulateTeamDropdownForPlayer(HWND hwnd, int index) // RA2: 0x005B75D0
{
	Clear_ComboBox(hwnd); // RA2: as above
	for (int i = 0; i < MPTeams.Count(); i++) { // RA2: as above
		delete MPTeams[i]; // RA2: as above
	}
	MPTeams.Clear(); // RA2: as above
	CreateMPTeams(&MPTeams); // RA2: as above
	if (index >= 0 && index < Session.Players.Count()) { // RA2: as above
		int slot = Session.Players[index]->Slot.SpectatorFlag; // RA2: as above
		for (int i = 0; i < MPTeams.Count(); i++) { // RA2: as above
			MultiplayerTeam * entry = MPTeams[i]; // RA2: as above
			int item = SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)entry->Title.text()); // RA2: as above
			if (entry->Index == slot) { // RA2: as above
				SendMessage(hwnd, CB_SETCURSEL, item, 0); // RA2: as above
			}
			SendMessage(hwnd, CB_SETITEMDATA, item, (LPARAM)entry); // RA2: as above
		}
	}
}

/// <summary>
/// The team with a team number; the teams are made first when there are fewer than three.
/// </summary>
MultiplayerTeam * MultiplayerGameMode::GetTeam(int index) // RA2: 0x005B76B0
{
	if (MPTeams.Count() < 3) { // RA2: as above
		for (int i = 0; i < MPTeams.Count(); i++) { // RA2: as above
			delete MPTeams[i]; // RA2: as above
		}
		MPTeams.Clear(); // RA2: as above
		CreateMPTeams(&MPTeams); // RA2: as above
	}
	for (int i = 0; i < MPTeams.Count(); i++) { // RA2: as above
		MultiplayerTeam * team = MPTeams[i]; // RA2: as above
		if (team != NULL && team->Index == index) { // RA2: as above
			return(team); // RA2: as above
		}
	}
	return(NULL); // RA2: as above
}

/// <summary>
/// Makes the teams of the mode: the observers, then one for each team number up to the
/// number of players.
/// </summary>
void MultiplayerGameMode::CreateMPTeams(DynamicVectorClass<MultiplayerTeam *> * teams) // RA2: 0x005B7720
{
	teams->Add(new MultiplayerObserverTeam); // RA2: as above
	for (int i = 1; i <= Session.MaxPlayers; i++) { // RA2: as above
		teams->Add(Create_Battle_Team(i)); // RA2: as above
	}
}

/// <summary>
/// Deletes the teams.
/// </summary>
void MultiplayerGameMode::DeleteTeams(void) // RA2: 0x005B77E0
{
	for (int i = 0; i < MPTeams.Count(); i++) { // RA2: as above
		delete MPTeams[i]; // RA2: as above
	}
	MPTeams.Clear(); // RA2: as above
}

/// <summary>
/// The scenario's alliances may be changed.
/// </summary>
bool MultiplayerGameMode::UnfixAlliances(void) // RA2: 0x005B7820
{
	Scen->Special.IsAllianceFixed = false; // RA2: as above
	return(true); // RA2: as above
}

/// <summary>
/// Every country may be picked in this mode.
/// </summary>
bool MultiplayerGameMode::IsCountryAllowed(int country) // RA2: 0x005B7830, slot 0x1C of the vtable; every country is allowed
{
	return(true); // RA2: as above
}

/// <summary>
/// The mode's INI file: made at the first call, and read when the file is there.
/// </summary>
CCINIClass * MultiplayerGameMode::GetINI(void) // RA2: 0x005B7840
{
	if (INI == NULL) { // RA2: as above
		INI = new CCINIClass; // RA2: as above
		CCFileClass file(INIFilename); // RA2: as above
		if (file.Is_Available(false)) { // RA2: as above
			INI->Load(file, false, false); // RA2: as above
		}
	}
	return(INI); // RA2: as above
}

static bool ObserverCellTaken; // RA2: 0x00A71A3C, set when an observer was given a cell; AssignStartingPositionsToHouse lets the next house share it once
static bool ObserverCellShared; // RA2: 0x00A71A3D, set when the next house was given the observer's cell

/// <summary>
/// Whether the house belongs to a human player who observes the game: the one whose name is a
/// player's and whose team is -1.
/// </summary>
static inline bool Is_Observer(HouseClass * house) // RA2: expanded where AssignStartingPositionsToHouse and GenerateUnits ask
{
	if (house != NULL) { // RA2: as above
		for (int i = 0; i < Session.Players.Count(); i++) { // RA2: as above
			if (house->Is_Human_Player() && wcscmp(house->UIName, Session.Players[i]->Name) == 0) { // RA2: as above
				return(Session.Players[i]->Slot.SpectatorFlag == -1); // RA2: as above
			}
		}
	}
	return(false); // RA2: as above
}

/// <summary>
/// Picks the starting cell of a house and marks it taken. The first house gets a random cell;
/// an observer shares the cell the next house takes; any other house gets the free cell
/// whose distance to the taken ones adds up to the most.
/// </summary>
/// <param name="result">Where the cell is returned.</param>
/// <param name="house">The index of the house.</param>
/// <param name="coords">The starting cells.</param>
/// <param name="satisfied">One flag per starting cell: taken or not.</param>
Cell * MultiplayerGameMode::AssignStartingPositionsToHouse(Cell * result, int house, DynamicVectorClass<Cell> * coords, unsigned char * satisfied) // RA2: 0x005B7920
{
	int i = 0; // RA2: as above
	while (i < coords->Count() && !satisfied[i]) { // RA2: as above
		i++; // RA2: as above
	}
	if (i == coords->Count()) { // RA2: as above
		ObserverCellTaken = false; // RA2: as above
		ObserverCellShared = false; // RA2: as above
		if (Is_Observer(Houses[house])) { // RA2: as above
			i = Scen->RandomNumber(0, coords->Count() - 1); // RA2: as above
			satisfied[i] = true; // RA2: as above
			ObserverCellTaken = true; // RA2: as above
			DebugString("Observer observing random cell: %d (%d,%d)\n", i, (*coords)[i].X, (*coords)[i].Y); // RA2: as above
			*result = (*coords)[i]; // RA2: as above
			return(result); // RA2: as above
		}
		i = Scen->RandomNumber(0, coords->Count() - 1); // RA2: as above
		DebugString("First cell taken: %d (%d,%d)\n", i, (*coords)[i].X, (*coords)[i].Y); // RA2: as above
		satisfied[i] = true; // RA2: as above
		*result = (*coords)[i]; // RA2: as above
		return(result); // RA2: as above
	}
	if (ObserverCellTaken && !ObserverCellShared) { // RA2: as above
		ObserverCellShared = true; // RA2: as above
		*result = (*coords)[i]; // RA2: as above
		return(result); // RA2: as above
	}
	if (Is_Observer(Houses[house])) { // RA2: as above
		ObserverCellShared = true; // RA2: as above
		ObserverCellTaken = true; // RA2: as above
		DebugString("Observer Observing cell: %d (%d,%d)\n", i, (*coords)[i].X, (*coords)[i].Y); // RA2: as above
		*result = (*coords)[i]; // RA2: as above
		return(result); // RA2: as above
	}
	int best_sum = -1; // RA2: as above
	int best = -1; // RA2: as above
	for (int b = 0; b < coords->Count(); b++) { // RA2: as above
		if (!satisfied[b]) { // RA2: as above
			int sum = 0; // RA2: as above
			for (int c = 0; c < coords->Count(); c++) { // RA2: as above
				if (satisfied[c]) { // RA2: as above
					sum += (*coords)[b].Distance_To((*coords)[c]); // RA2: as above
				}
			}
			if (sum > best_sum) { // RA2: as above
				best_sum = sum; // RA2: as above
				best = b; // RA2: as above
			}
		}
	}
	satisfied[best] = true; // RA2: as above
	DebugString("Starting cell taken: %d (%d,%d)\n", best, (*coords)[best].X, (*coords)[best].Y); // RA2: as above
	*result = (*coords)[best]; // RA2: as above
	return(result); // RA2: as above
}


/// <summary>
/// Gives every house that takes part a starting cell, which becomes the center of its base.
/// </summary>
/// <param name="unused">The argument the starting cell list is asked for with; RA2's list ignores it.</param>
bool MultiplayerGameMode::StartingPositionsToHouseBaseCells(char unused) // RA2: 0x005B7C00
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
/// Adds the bit of each taking-part house's type (its heap ID) to a mask.
/// </summary>
static inline void Player_Owner_Mask(int & mask) // RA2: expanded in GenerateUnits; the mask is kept in memory there
{
	for (int h = 0; h < Houses.Count(); h++) { // RA2: as above
		HouseTypeClass * type = Houses[h]->Class; // RA2: as above
		if (!type->IsMultiplayPassive) { // RA2: as above
			mask |= 1 << Houses[h]->Class->HeapID; // RA2: as above
		}
	}
}

/// <summary>
/// Gives each house that takes part its base unit and starting units, from the money the
/// session grants for units: the average cost of what may start, times the number of units
/// the lobby asked for. Observers get none.
/// </summary>
bool MultiplayerGameMode::GenerateUnits(void) // RA2: 0x005B7CE0
{
	int credits = 0; // RA2: as above
	int amount; // RA2: as above
	if (Session.Options.UnitCount > 0) { // RA2: as above
		amount = 0; // RA2: as above
		Player_Owner_Mask(amount); // RA2: as above
		int total = 0; // RA2: as above
		int count = 0; // RA2: as above
		for (int u = 0; u < UnitTypes.Count(); u++) { // RA2: as above
			UnitTypeClass * type = UnitTypes[u]; // RA2: as above
			if (type->IsAllowedToStartInMultiplayer && (amount & type->Ownable) && type->Level <= BuildLevel) { // RA2: as above
				if (!Rule->BaseUnit.Is_In_List(type)) { // RA2: as above
					count++; // RA2: as above
					total += type->Raw_Cost(); // RA2: as above
				}
			}
		}
		for (int n = 0; n < InfantryTypes.Count(); n++) { // RA2: as above
			InfantryTypeClass * type = InfantryTypes[n]; // RA2: as above
			if (type->IsAllowedToStartInMultiplayer && (amount & type->Ownable) && type->Level <= BuildLevel) { // RA2: as above
				count++; // RA2: as above
				total += type->Raw_Cost(); // RA2: as above
			}
		}
		if (count == 0) { // RA2: as above
			count = 1; // RA2: as above
		}
		credits = (count / 2 + total) / count * Session.Options.UnitCount; // RA2: as above
	}
	for (int i = 0; i < Houses.Count(); i++) { // RA2: as above
		HouseClass * house = Houses[i]; // RA2: as above
		if (!house->Class->IsMultiplayPassive) { // RA2: as above
			if (!Is_Observer(house)) { // RA2: as above
				amount = credits; // RA2: as above
				if (!SpawnBaseUnits(house, (unsigned)&amount) || !GenerateStartingUnits(house, amount)) { // RA2: as above
					return(false); // RA2: as above
				}
				if (amount > 0) { // RA2: as above
					house->Refund_Money(amount); // RA2: as above
				}
				house->PBQuantity.Clear(); // RA2: as above
				house->PAQuantity.Clear(); // RA2: as above
				house->PIQuantity.Clear(); // RA2: as above
				house->PUQuantity.Clear(); // RA2: as above
			}
		}
	}
	DebugString("Finished unit generation. Random number is %d\n", Random_Pick(0, 0xFFFF)); // RA2: as above
	return(true); // RA2: as above
}

/// <summary>
/// With bases switched on, puts the house's base unit (the first of the base units its country
/// may own) on the center of its base, or as near as it fits.
/// </summary>
/// <returns>Returns false when there is no base unit or it cannot be placed.</returns>
bool MultiplayerGameMode::SpawnBaseUnits(HouseClass * house, unsigned unused) // RA2: 0x005B7F90
{
	if (!Session.Options.Bases) { // RA2: as above
		return(true); // RA2: as above
	}
	UnitTypeClass const * type = house->Get_First_Ownable(Rule->BaseUnit); // RA2: as above
	if (type != NULL) { // RA2: as above
		UnitClass * unit = new UnitClass(type, house); // RA2: as above
		if (unit != NULL) { // RA2: as above
			if (unit->Unlimbo(house->Center)) { // RA2: as above
				return(true); // RA2: as above
			}
			if (Scan_Place_Object(unit, Cell(house->Center), 1)) { // RA2: as above
				return(true); // RA2: as above
			}
			delete unit; // RA2: as above
		}
	}
	return(false); // RA2: as above
}

/// <summary>
/// Spends the money of a house on random starting units, as long as that is possible: units
/// (not the base unit) while more than a third of the money is left, infantry after that, of the
/// types the house may own at its tech level. Each is placed near the center of the base and
/// made elite when the scenario says so; the house's own player holds still, the computer's
/// units guard the area. Gives up after 20 failed placements.
/// </summary>
/// <returns>Returns false when no type fits the house.</returns>
#ifdef RA2_NON_MATCHING // not exact: 512 bytes differ (adopted text)
bool MultiplayerGameMode::GenerateStartingUnits(HouseClass * house, int & amount) // RA2: 0x005B8060
{
	if (amount > 0) { // RA2: as above
		int total = amount; // RA2: as above
		DynamicVectorClass<UnitTypeClass *> units; // RA2: as above
		DynamicVectorClass<InfantryTypeClass *> infantry; // RA2: as above
		int owner = 1 << house->Class->HeapID; // RA2: as above
		for (int u = 0; u < UnitTypes.Count(); u++) { // RA2: as above
			UnitTypeClass * type = UnitTypes[u]; // RA2: as above
			if (type->IsAllowedToStartInMultiplayer && (type->Ownable & owner) && type->Level <= house->Control.TechLevel) { // RA2: as above
				if (!Rule->BaseUnit.Is_In_List(type)) { // RA2: as above
					units.Add(type); // RA2: as above
				}
			}
		}
		for (int n = 0; n < InfantryTypes.Count(); n++) { // RA2: as above
			InfantryTypeClass * type = InfantryTypes[n]; // RA2: as above
			if (type->IsAllowedToStartInMultiplayer && (type->Ownable & owner) && type->Level <= house->Control.TechLevel) { // RA2: as above
				infantry.Add(type); // RA2: as above
			}
		}
		if (units.Count() == 0 && infantry.Count() == 0) { // RA2: as above
			return(false); // RA2: as above
		}
		int tries = 20; // RA2: as above
		while (amount > 0 && tries > 0) { // RA2: as above
			TechnoTypeClass * type; // RA2: as above
			if (amount > total / 3 && units.Count() != 0) { // RA2: as above
				type = units[Scen->RandomNumber(0, units.Count() - 1)]; // RA2: as above
			} else { // RA2: as above
				type = infantry[Scen->RandomNumber(0, infantry.Count() - 1)]; // RA2: as above
			}
			ObjectClass * object = type->Create_One_Of(house); // RA2: as above
			TechnoClass * techno = dynamic_cast<TechnoClass *>(object); // RA2: as above
			if (!Scan_Place_Object(techno, Cell(house->Center), 4)) { // RA2: as above
				delete techno; // RA2: as above
				tries--; // RA2: as above
			} else { // RA2: as above
				amount -= type->Raw_Cost(); // RA2: as above
				if (Scen->Special.IsInitialVeteran) { // RA2: as above
					techno->Veterancy.Set_Elite(true); // RA2: as above
				}
				if (house->Is_Human_Player()) { // RA2: as above
					techno->Set_Mission(MISSION_GUARD); // RA2: as above
				} else { // RA2: as above
					techno->Set_Mission(MISSION_GUARD_AREA); // RA2: as above
				}
			}
		}
	}
	return(true); // RA2: as above
}
#endif

/// <summary>
/// A slot of the vtable not named yet: this mode answers true (what it is asked is open).
/// </summary>
bool MultiplayerGameMode::vt_entry_70(void) // RA2: 0x005B8420, slot 0x70 of the vtable; true
{
	return(true); // RA2: as above
}

/// <summary>
/// The team of the player that has a house (the player's team is -1 for an observer), or -1
/// when no player has the house.
/// </summary>
int MultiplayerGameMode::TeamOfHouse(int house) // RA2: 0x005B8430
{
	for (int i = 0; i < Session.Players.Count(); i++) { // RA2: as above
		if (Session.Players[i]->Slot.HouseIndex == house) { // RA2: as above
			return(Session.Players[i]->Slot.SpectatorFlag); // RA2: as above
		}
	}
	return(-1); // RA2: as above
}

/// <summary>
/// Nothing to do when the game starts.
/// </summary>
bool MultiplayerGameMode::OnGameStart(void) // RA2: 0x005B8470, slot 0x74 of the vtable; true
{
	return(true); // RA2: as above
}

/// <summary>
/// The base of the objects that make game modes: its vtable is all it holds.
/// </summary>
MultiplayerGameMode::InitializerBase::InitializerBase(void) // RA2: 0x005B8480
{
}

static inline void Trim_Trailing(Wstring & string) // RA2: the line of the list entry is trimmed at its end the same way for every part
{
	string.trimTrailing(" "); // RA2: as above
}

/// <summary>
/// Reads one section of MPModes.ini; each entry is "index=title,tooltip,inifile,mapfilter[,ai]"
/// where the title and the tooltip name strings of the string table and the last part says
/// whether the computer may play ("false" or anything else). Every mode is made by the
/// initializer and put into the list of the game modes in the order of the mode indexes.
/// </summary>
void __fastcall Read_Game_Modes(Wstring const & section, MultiplayerGameMode::InitializerBase & init) // RA2: 0x005B8490
{
	CCFileClass file("MPModes.ini"); // RA2: as above
	if (file.Is_Available(false)) { // RA2: as above
		INIClass ini; // RA2: as above
		if (ini.Load(file, false) > 0) { // RA2: as above
			int count = ini.Entry_Count(section); // RA2: as above
			for (int i = 0; i < count; i++) { // RA2: as above
				Wstring entry = ini.Get_Entry(section, i); // RA2: as above
				int index = entry.toInt(); // RA2: as above
				Wstring value; // RA2: as above
				int length = ini.Get_String(section, entry, "", value); // RA2: as above
				if (length <= 0) { // RA2: as above
					continue; // RA2: as above
				}

				Wstring name; // RA2: as above
				int pos = value.getToken(0, ",", name); // RA2: as above
				if (!name.length()) { // RA2: as above
					continue; // RA2: as above
				}
				name.trimLeading(" "); // RA2: as above
				Trim_Trailing(name); // RA2: as above
				WideWstring title(StringTable::LoadString(name.get(), NULL, "D:\\Ra2\\Multiplayer.cpp", 773)); // RA2: as above

				Wstring tip; // RA2: as above
				pos = value.getToken(pos, ",", tip); // RA2: as above
				if (!tip.length()) { // RA2: as above
					continue; // RA2: as above
				}
				tip.trimLeading(" "); // RA2: as above
				Trim_Trailing(tip); // RA2: as above
				WideWstring tooltip(StringTable::LoadString(tip.get(), NULL, "D:\\Ra2\\Multiplayer.cpp", 784)); // RA2: as above

				Wstring inifile; // RA2: as above
				pos = value.getToken(pos, ",", inifile); // RA2: as above
				if (!inifile.length()) { // RA2: as above
					continue; // RA2: as above
				}
				inifile.trimLeading(" "); // RA2: as above
				Trim_Trailing(inifile); // RA2: as above

				Wstring mapfilter; // RA2: as above
				pos = value.getToken(pos, ",", mapfilter); // RA2: as above
				mapfilter.trimLeading(" "); // RA2: as above
				Trim_Trailing(mapfilter); // RA2: as above

				Wstring ai; // RA2: as above
				value.getToken(pos, ",", ai); // RA2: as above
				ai.trimLeading(" "); // RA2: as above
				Trim_Trailing(ai); // RA2: as above
				bool aiallowed = true; // RA2: as above
				if (ai.length() > 0) { // RA2: as above
					aiallowed = stricmp(ai.get(), "false") != 0; // RA2: as above
				}

				if (title.length() <= 0 || inifile.length() <= 0 || mapfilter.length() <= 0) { // RA2: as above
					continue; // RA2: as above
				}

				MultiplayerGameMode * mode = init.Create(title, tooltip, inifile, mapfilter, aiallowed, index); // RA2: as above
				int at; // RA2: as above
				for (at = 0; at < Game_Modes().Count(); at++) { // RA2: as above
					if (Game_Modes()[at]->MPModeIndex > mode->MPModeIndex) { // RA2: as above
						break; // RA2: as above
					}
				}
				if (at == 0) { // RA2: as above
					Game_Modes().Add_Head(mode); // RA2: as above
				} else { // RA2: as above
					Game_Modes().Insert_After(at - 1, mode); // RA2: as above
				}
			}
		}
	}
}

/// <summary>
/// Nothing to free: the base only holds its vtable.
/// </summary>
MultiplayerGameMode::InitializerBase::~InitializerBase(void) // RA2: 0x005B8BC0
{
}

void Free_Game_Modes(void); // RA2: 0x005B8D40, defined below; Init_Game_Modes expands it

/// <summary>
/// Makes the game modes of the five kinds MPModes.ini has sections for, in place of those there are.
/// </summary>
void Init_Game_Modes(void) // RA2: 0x005B8BD0
{
	Free_Game_Modes(); // RA2: as above
	MultiplayerGameMode::Initializer<MultiplayerBattle> battle; // RA2: as above
	Read_Game_Modes("Battle", battle); // RA2: as above
	MultiplayerGameMode::Initializer<MultiplayerManBattle> manbattle; // RA2: as above
	Read_Game_Modes("ManBattle", manbattle); // RA2: as above
	MultiplayerGameMode::Initializer<MultiplayerSiege> siege; // RA2: as above
	Read_Game_Modes("Siege", siege); // RA2: as above
	MultiplayerGameMode::Initializer<UnholyAlliance> unholy; // RA2: as above
	Read_Game_Modes("Unholy", unholy); // RA2: as above
	MultiplayerGameMode::Initializer<FreeForAll> freeforall; // RA2: as above
	Read_Game_Modes("FreeForAll", freeforall); // RA2: as above
	MultiplayerGameMode::Initializer<MPCooperative> cooperative; // RA2: as above
	Read_Game_Modes("Cooperative", cooperative); // RA2: as above
}

/// <summary>
/// Deletes the registered game modes.
/// </summary>
void Free_Game_Modes(void) // RA2: 0x005B8D40
{
	for (int i = 0; i < Game_Modes().Count(); i++) { // RA2: as above
		delete Game_Modes()[i]; // RA2: as above
	}
}
