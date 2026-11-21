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
**	RA2: the score screens of the cooperative campaign: after each map the players see a dialog
**	(resource 0xBC6) with their kills, losses, buildings built and score, and tell each other
**	over the network when they are ready for the next map; after the last map of the campaign a
**	second dialog (resource 0xBC7) shows the totals. TibSun has no such file (its score.cpp draws
**	the single player score itself); the code is the executable's, from 0x00465A00 to 0x00467B40.
*/

#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: the includes this file needs
#include "campscor.h" // RA2: the score screens' classes
#include "dbgprint.h" // RA2: DebugString
#include "globals.h" // RA2: Houses
#include "houstype.h" // RA2: HouseTypeClass::IsMultiplayPassive
#include "ipxmgr.h" // RA2: Ipx
#include "mplayerscore.h" // RA2: MPlayerScoreType
#include "netdlg2.h" // RA2: Net2Callback, Process_Global_Packet
#include "ownrdraw.h" // RA2: OwnerDraw::Default_Dialog_Proc
#include "scenario.h" // RA2: Scen
#include "scheme.h" // RA2: ColorSchemes
#include "session.h" // RA2: Session and its score table
#include "stringtable.h" // RA2: StringTable::LoadString, with RA2's source file and line
#include "wonline.h" // RA2: WOL_Wait_Callback
#include <stdlib.h> // RA2: qsort
#include <stdio.h> // RA2: swprintf
#include <wchar.h> // RA2: wcscpy, wcscmp
#include "conquer.h" // RA2: Call_Back
#include "msgloop.h" // RA2: Windows_Message_Handler

char * __fastcall Wide_To_Narrow(wchar_t const * text); // RA2: 0x006FA120, static buffer (netdlg2.cpp declares it the same way)
void __fastcall Darken_Surface(Surface * surface, bool update); // RA2: 0x005159B0 (init.cpp)
void __fastcall Dialog_On_WM_PAINT(HWND dialog); // RA2: 0x005FEE70, before a dialog paints (netdlg2.cpp declares it the same way)
namespace OwnerDraw { BOOL __stdcall Set_Paint_Disabled(HWND window, LPARAM lparam); } // RA2: 0x005ECF40, defined in ownrdraw.cpp
int __fastcall Dialog_Box(int resource, int (__stdcall * proc)(struct HWND__ *, unsigned int, unsigned int, long), bool show); // RA2: 0x005EAB60 (ownrdraw.cpp)
void __fastcall Load_Score_Art(int side); // RA2: 0x006F2710 (txtlabel2.cpp)
void __cdecl Free_Score_Art(void); // RA2: 0x006F2750 (txtlabel2.cpp)
void __fastcall Load_MP_Score_Art(int side); // RA2: 0x006F2A30 (txtlabel2.cpp)
void __fastcall Free_MP_Score_Art(void); // RA2: 0x006F2AB0 (txtlabel2.cpp)

#define	CAMPSCORE_DIALOG			0xBC6 // RA2: the dialog resource of the score screen of a map
#define	CAMPSCORE_END_DIALOG		0xBC7 // RA2: the dialog resource of the screen of the whole campaign
#define	CAMPSCORE_CONTINUE			0x6D1 // RA2: the button that tells the others this player is ready
#define	CAMPSCORE_QUIT				0x5C0 // RA2: the button that leaves the screen
#define	CAMPSCORE_TITLE				0x71B // RA2: the label of the scenario's description
#define	CAMPSCORE_DIFFICULTY		0x732 // RA2: the label of the difficulty
#define	CAMPSCORE_NAME				0x733 // RA2: the label of the campaign's name
#define	CAMPSCORE_TIME				0x3EA // RA2: the label of the time the map took
#define	CAMPSCORE_SET_TEXT			(WM_USER + 178) // RA2: 0x4B2, sets the text of an owner drawn control
#define	CAMPSCORE_SET_COLOR			(WM_USER + 152) // RA2: 0x498, sets the text colour of an owner drawn control

#define	CAMPSCORE_END_CAMPAIGN		0x735 // RA2: the label of the campaign's number on the end screen
#define	CAMPSCORE_FIRST_NAME		0x73A // RA2: the winner's name on the podium of the end screen
#define	CAMPSCORE_FIRST_SCORE		0x73B // RA2: the winner's score
#define	CAMPSCORE_SECOND_NAME		0x739 // RA2: the second place's name
#define	CAMPSCORE_SECOND_SCORE		0x738 // RA2: the second place's score
#define	CAMPSCORE_END_ANIM			0x72B // RA2: the animated control of the end screen

/*
**	RA2: the messages of the score screens, sent as global packets (the command is the first int of the packet)
*/
enum CampScoreCommandType {
	NET_SCORE_READY = 0x23, // RA2: a player has pressed continue
	NET_SCORE_ABORTED = 0x24, // RA2: a player has left the screen
	NET_SCORE_WAITING = 0x25, // RA2: a player has taken the continue back
	NET_SCORE_CONTINUE = 0x26 // RA2: every player is ready, the screens close
};

#define	CAMPSCORE_COLUMNS			6 // RA2: name, kills, losses, built, score, status

static int CampaignDifficulty = -1; // RA2: 0x007D1480, the difficulty shown on the map's screen (0 easy, 1 medium, 2 brutal, -1 unknown); the constructor sets it again

/*
**	RA2: the controls of the table, a row per player slot (0x007D1484); the sixth column is the status
**	of the player (ready, waiting, aborted)
*/
static int CampScoreControls[MAX_MULTI_NAMES][CAMPSCORE_COLUMNS] = { // RA2: 0x007D1484
	{1041, 1049, 1699, 1701, 1702, 1802}, // RA2: the first row
	{1042, 1050, 1700, 1703, 1704, 1803}, // RA2: as above
	{1043, 1051, 1705, 1706, 1707, 1804}, // RA2: as above
	{1044, 1052, 1708, 1709, 1710, 1805}, // RA2: as above
	{1045, 1053, 1711, 1712, 1713, 1806}, // RA2: as above
	{1046, 1054, 1714, 1715, 1716, 1807}, // RA2: as above
	{1047, 1055, 1717, 1718, 1719, 1808}, // RA2: as above
	{1048, 1056, 1720, 1721, 1722, 1809} // RA2: as above
};

static MPlayerScoreType * CampaignSortedScores[MAX_MULTI_NAMES]; // RA2: 0x00850448, the score table of the map ordered by score
static int EndScoreCampaign; // RA2: 0x00850488, the number of the campaign on the end screen
static int EndScoreTime; // RA2: 0x0085049C, the time the campaign took
static MPlayerScoreType * EndScores[MAX_MULTI_NAMES]; // RA2: 0x008504A8, the scores of the whole campaign ordered by score
static bool ScoreReady[MAX_MULTI_NAMES]; // RA2: 0x008504C8, whether each row is ready for the next map
static int EndScoreDifficulty; // RA2: 0x008504D0, the difficulty on the end screen
static int EndScoreCount; // RA2: 0x008504E8, how many players the end screen lists
bool CampaignScoreDone; // RA2: 0x00850504, set when the players are done with the screens
static int ScoreTime; // RA2: 0x00850508, the time the map took
static int ScoreCount; // RA2: 0x0085050C, how many players the screen of the map lists
static bool ScoreIsReady; // RA2: 0x00850510, true while this player has pressed the continue button
static wchar_t ScoreName[32]; // RA2: 0x00850514, the name of the campaign on the screen of the map
static wchar_t EndScoreName[32]; // RA2: 0x00850554, the name of the campaign on the end screen

void __fastcall Campaign_Score_Format_Time(wchar_t * buffer, int time); // RA2: 0x00465C20, defined below; the dialog procedure formats the time with it
void __fastcall Campaign_Score_Set_Number(HWND dialog, int id, int value); // RA2: 0x00465CC0, defined below
void __fastcall Campaign_Score_Reset_Ready(int count); // RA2: 0x00465F50, defined below
bool Campaign_Score_All_Ready(void); // RA2: 0x00465F80, defined below
int __cdecl Campaign_Score_Compare(void const * left, void const * right); // RA2: 0x00465F20, defined below; the sorts pass it to qsort
int __stdcall Campaign_Score_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x00465FC0, defined below; Presentation passes it to Begin_Dialog
void __fastcall Campaign_End_Score_Set_Number(HWND dialog, int id, int value); // RA2: 0x00467240, defined below
void __fastcall Campaign_End_Score_Format_Time(wchar_t * buffer, int time); // RA2: 0x00467290, defined below
int __stdcall Campaign_End_Score_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x00467330, defined below; the end screen passes it to Dialog_Box


/// <summary>
/// Constructs the score screen of a map.
/// </summary>
CampaignScoreClass::CampaignScoreClass(void) // RA2: 0x00465BF0
{
	Field4 = 0; // RA2: as above
	ScoreTime = 0; // RA2: as above
	CampaignDifficulty = -1; // RA2: the difficulty is not known yet
}


/// <summary>
/// Destroys the score screen of a map.
/// </summary>
CampaignScoreClass::~CampaignScoreClass(void) // RA2: 0x00465C10
{
}


/// <summary>
/// Formats a duration as hours, minutes and seconds.
/// The hours are capped at 99, so the longest time shown is 99:59:59.
/// </summary>
/// <param name="buffer">Receives the text; nothing is done when it is NULL.</param>
/// <param name="time">The duration in seconds.</param>
void __fastcall Campaign_Score_Format_Time(wchar_t * buffer, int time) // RA2: 0x00465C20
{
	if (buffer != NULL) { // RA2: the caller may pass no buffer
		if (time / 3600 > 99) { // RA2: more than 99 hours
			time = 359999; // RA2: 99:59:59
		}
		int hours = time / 3600; // RA2: as above
		time -= hours * 3600; // RA2: as above
		int minutes = time / 60; // RA2: as above
		time -= minutes * 60; // RA2: as above
		swprintf(buffer, StringTable::LoadString("TXT_TIME_FORMAT_HOURS", NULL, "D:\\Ra2\\CampScor.cpp", 102), hours, minutes, time); // RA2: as above
	}
}


/// <summary>
/// Sets the text of a control of the dialog to a number.
/// </summary>
/// <param name="dialog">The dialog that holds the control.</param>
/// <param name="id">The control's resource id.</param>
/// <param name="value">The number to show.</param>
void __fastcall Campaign_Score_Set_Number(HWND dialog, int id, int value) // RA2: 0x00465CC0
{
	wchar_t text[32]; // RA2: as above
	swprintf(text, L"%d", value); // RA2: as above
	HWND item = GetDlgItem(dialog, id); // RA2: as above
	SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)text); // RA2: as above
}


/// <summary>
/// Fills the session's score table from the houses of the game.
/// Passive houses (neutral ones) are left out. A house that is still in the game when the map
/// ends has won.
/// </summary>
void CampaignScoreClass::Tally_Scores(void) // RA2: 0x00465D10
{
	Session.NumScores = 0; // RA2: the table starts empty
	int count = Houses.Count(); // RA2: also bounds the buildings killed loop below
	for (int index = 0; index < count; index++) { // RA2: as above
		HouseClass * house = Houses[index]; // RA2: as above
		if (house != NULL && house->Class->IsMultiplayPassive != true) { // RA2: only real players
			int slot = Session.NumScores++; // RA2: as above
			Session.Score[slot].Wins = 0; // RA2: as above
			wcscpy(Session.Score[slot].Name, house->UIName); // RA2: the player's name
			Session.Score[slot].Lost[0] = 0; // RA2: the totals are in the first game's entries
			Session.Score[slot].Kills[0] = 0; // RA2: as above
			Session.Score[slot].Built[0] = 0; // RA2: as above
			Session.Score[slot].Score[0] = 0; // RA2: as above
			Session.Score[slot].Scheme = house->Scheme; // RA2: the colour of the player
			if (!house->IsDefeated) { // RA2: the player is still in the game
				Session.Score[slot].Wins++; // RA2: as above
				Session.Winner = slot; // RA2: as above
			}

			int kills = 0; // RA2: as above
			int unit;
			for (unit = 0; unit < 20; unit++) { // RA2: the units killed, per unit type slot
				kills += house->UnitsKilled[unit]; // RA2: as above
			}
			for (unit = 0; unit < count; unit++) { // RA2: the executable bounds the buildings by the number of houses, not by the 20 entries
				kills += house->BuildingsKilled[unit]; // RA2: as above
			}
			Session.Score[slot].Kills[0] = kills; // RA2: as above
			Session.Score[slot].Lost[0] = house->UnitsLost + house->BuildingsLost; // RA2: as above

			int built = house->ABQuantity.Total(); // RA2: buildings in service
			built += house->AUQuantity.Total(); // RA2: units in service
			built += house->AIQuantity.Total(); // RA2: infantry in service
			built += house->AAQuantity.Total(); // RA2: aircraft in service
			Session.Score[slot].Built[0] = house->UnitsLost + house->BuildingsLost + built; // RA2: as above

			if (house->PointTotal > 0) { // RA2: as above
				Session.Score[slot].Score[0] += house->PointTotal; // RA2: as above
			}

			DebugString("%s: %s\n Scheme: %d\n Lost = %d\n Kills = %d\n Built = %d\n Score = %d\n", // RA2: the executable prints the name with %s
				Session.Score[slot].Name, // RA2: as above
				Session.Score[slot].Wins > 0 ? "Winner" : "Loser", // RA2: as above
				Session.Score[slot].Scheme, Session.Score[slot].Lost[0], // RA2: as above
				Session.Score[slot].Kills[0], Session.Score[slot].Built[0], // RA2: as above
				Session.Score[slot].Score[0]); // RA2: as above
		}
	}
}


/// <summary>
/// Orders the session's score table by score, the best first, into CampaignSortedScores.
/// </summary>
void Campaign_Score_Sort(void) // RA2: 0x00465EE0
{
	int count = Session.NumScores; // RA2: as above
	for (int index = 0; index < count; index++) { // RA2: as above
		CampaignSortedScores[index] = &Session.Score[index]; // RA2: as above
	}
	if (count > 1) { // RA2: as above
		qsort(CampaignSortedScores, count, sizeof(MPlayerScoreType *), Campaign_Score_Compare); // RA2: as above
	}
}


/// <summary>
/// Compares two entries of a sorted score table: the higher score comes first.
/// </summary>
/// <param name="left">Pointer to the table's pointer to the first entry.</param>
/// <param name="right">Pointer to the table's pointer to the second entry.</param>
/// <returns>int; negative when the first entry's score is higher, positive when it is lower, 0 when equal.</returns>
int __cdecl Campaign_Score_Compare(void const * left, void const * right) // RA2: 0x00465F20
{
	MPlayerScoreType const * a = *(MPlayerScoreType const * const *)left; // RA2: as above
	MPlayerScoreType const * b = *(MPlayerScoreType const * const *)right; // RA2: as above
	if (a->Score[0] == b->Score[0]) { // RA2: the score is the first game's entry
		return(0); // RA2: as above
	}
	return a->Score[0] > b->Score[0] ? -1 : 1; // RA2: as above
}


/// <summary>
/// Marks every row ready, then the first rows not ready.
/// </summary>
/// <param name="count">How many rows start as not ready.</param>
void __fastcall Campaign_Score_Reset_Ready(int count) // RA2: 0x00465F50
{
	memset(ScoreReady, true, sizeof(ScoreReady)); // RA2: as above
	for (int index = 0; index < count; index++) { // RA2: as above
		ScoreReady[index] = false; // RA2: as above
	}
}


/// <summary>
/// Tells whether every row is ready.
/// </summary>
/// <returns>bool; true when all eight rows are ready.</returns>
bool Campaign_Score_All_Ready(void) // RA2: 0x00465F80
{
	int index;
	for (index = 0; index < 8; index++) { // RA2: as above
		if (!ScoreReady[index]) { // RA2: as above
			break; // RA2: as above
		}
	}
	return index == 8; // RA2: as above
}


/// <summary>
/// Sets whether a row is ready.
/// </summary>
/// <param name="index">The row.</param>
/// <param name="ready">Whether the row is ready for the next map.</param>
void __fastcall Campaign_Score_Set_Ready(int index, bool ready) // RA2: 0x00465FA0
{
	ScoreReady[index] = ready; // RA2: as above
}


/// <summary>
/// Tells whether a row is ready.
/// </summary>
/// <param name="index">The row.</param>
/// <returns>bool; true when the row is ready for the next map.</returns>
bool __fastcall Campaign_Score_Is_Ready(int index) // RA2: 0x00465FB0
{
	return ScoreReady[index]; // RA2: as above
}


/// <summary>
/// Tells the other players what this player did on the score screen.
/// </summary>
/// <param name="command">NET_SCORE_READY, NET_SCORE_ABORTED, NET_SCORE_WAITING or NET_SCORE_CONTINUE.</param>
static inline void Send_Score_Packet(CampScoreCommandType command) // RA2: the executable has this code in place at each use (the dialog procedure and Presentation call nothing here)
{
	GlobalPacketType packet; // RA2: as above
	memset(&packet, 0, sizeof(packet)); // RA2: as above
	packet.Command = (NetCommandType)command; // RA2: as above
	strcpy(packet.Name, Wide_To_Narrow(Session.Players[0]->Name)); // RA2: the first player is this one
	for (int index = 1; index < Session.Players.Count(); index++) { // RA2: as above
		Ipx.Send_Global_Message_To_Name(&packet, 0x2F, 1, Session.Players[index]->Name); // RA2: only the header of the packet is sent
		Ipx.Service(); // RA2: as above
	}
}


/// <summary>
/// Message handler of the score screen dialog of a map.
/// Fills the screen once the dialog is up (the scenario, difficulty, campaign and time and a row per
/// player, each in the player's colour; computers start ready, players that left show as aborted),
/// and handles the continue button (which toggles this player's readiness and tells the others) and the
/// quit button.
/// </summary>
/// <param name="window">The dialog.</param>
/// <param name="message">The message.</param>
/// <param name="wparam">The message's first parameter.</param>
/// <param name="lparam">The message's second parameter.</param>
/// <returns>int; 0 unless the common dialog handler handled the message.</returns>
int __stdcall Campaign_Score_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x00465FC0
{
	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam); // RA2: the common owner draw handling comes first
	if (rc != 0) { // RA2: as above
		return rc; // RA2: as above
	}

	int * done = (int *)GetWindowLong(window, DWL_USER); // RA2: set by Presentation, the flag that ends the dialog
	switch (message) { // RA2: as above
		case WM_INITDIALOG: // RA2: 0x110
		{
			wchar_t text[32]; // RA2: as above
			HWND item = GetDlgItem(window, CAMPSCORE_TITLE); // RA2: as above
			SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)Session.Options.ScenarioDescription); // RA2: the description of the map
			swprintf(text, L"%s: ", StringTable::LoadString("GUI:Difficulty", NULL, "D:\\Ra2\\CampScor.cpp", 285)); // RA2: as above
			if (CampaignDifficulty == 0) { // RA2: as above
				swprintf(text, StringTable::LoadString("GUI:DifficultyString", NULL, "D:\\Ra2\\CampScor.cpp", 287), StringTable::LoadString("TXT_DIFFICULTY_EASY", NULL, "D:\\Ra2\\CampScor.cpp", 287)); // RA2: as above
			} else if (CampaignDifficulty == 1) { // RA2: as above
				swprintf(text, StringTable::LoadString("GUI:DifficultyString", NULL, "D:\\Ra2\\CampScor.cpp", 289), StringTable::LoadString("TXT_DIFFICULTY_MEDIUM", NULL, "D:\\Ra2\\CampScor.cpp", 289)); // RA2: as above
			} else if (CampaignDifficulty == 2) { // RA2: as above
				swprintf(text, StringTable::LoadString("GUI:DifficultyString", NULL, "D:\\Ra2\\CampScor.cpp", 291), StringTable::LoadString("TXT_DIFFICULTY_BRUTAL", NULL, "D:\\Ra2\\CampScor.cpp", 291)); // RA2: as above
			}
			item = GetDlgItem(window, CAMPSCORE_DIFFICULTY); // RA2: as above
			SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)text); // RA2: as above
			item = GetDlgItem(window, CAMPSCORE_NAME); // RA2: as above
			SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)ScoreName); // RA2: as above
			Campaign_Score_Format_Time(text, ScoreTime / 60); // RA2: the timer counts frames, 60 to the second
			item = GetDlgItem(window, CAMPSCORE_TIME); // RA2: as above
			SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)text); // RA2: as above
			memset(ScoreReady, true, sizeof(ScoreReady)); // RA2: as above
			for (int row = 0; row < Session.NumScores; row++) { // RA2: as above
				if (wcscmp(CampaignSortedScores[row]->Name, StringTable::LoadString("TXT_COMPUTER", NULL, "D:\\Ra2\\CampScor.cpp", 307)) != 0) { // RA2: computers are always ready
					ScoreReady[row] = false; // RA2: as above
				}
				ColorScheme * scheme = ColorSchemes[CampaignSortedScores[row]->Scheme]; // RA2: as above
				RGBClass color = scheme->HSV; // RA2: HSVClass converts to RGBClass
				unsigned long rgb = color.Get_Red() | (color.Get_Green() << 8) | (color.Get_Blue() << 16); // RA2: a COLORREF
				int id = CampScoreControls[row][0]; // RA2: the name
				wchar_t const * name = CampaignSortedScores[row]->Name; // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)name); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				id = CampScoreControls[row][1]; // RA2: the kills
				Campaign_Score_Set_Number(window, id, CampaignSortedScores[row]->Kills[0]); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				id = CampScoreControls[row][2]; // RA2: the losses
				Campaign_Score_Set_Number(window, id, CampaignSortedScores[row]->Lost[0]); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				id = CampScoreControls[row][3]; // RA2: the buildings built
				Campaign_Score_Set_Number(window, id, CampaignSortedScores[row]->Built[0]); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				id = CampScoreControls[row][4]; // RA2: the score
				Campaign_Score_Set_Number(window, id, CampaignSortedScores[row]->Score[0]); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above

				int count = Session.Players.Count(); // RA2: is the player still connected?
				bool found = false; // RA2: as above
				for (int player = 0; player < count; player++) { // RA2: as above
					if (wcscmp(Session.Players[player]->Name, CampaignSortedScores[row]->Name) == 0) { // RA2: as above
						found = true; // RA2: as above
						break; // RA2: as above
					}
				}
				id = CampScoreControls[row][5]; // RA2: the status
				if (found) { // RA2: as above
					wchar_t const * status = StringTable::LoadString("TXT_WAITING", NULL, "D:\\Ra2\\CampScor.cpp", 367); // RA2: as above
					item = GetDlgItem(window, id); // RA2: as above
					SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)status); // RA2: as above
				} else if (!ScoreReady[row]) { // RA2: a human player that is gone
					wchar_t const * status = StringTable::LoadString("TXT_ABORTED", NULL, "D:\\Ra2\\CampScor.cpp", 370); // RA2: as above
					item = GetDlgItem(window, id); // RA2: as above
					SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)status); // RA2: as above
					EnableWindow(GetDlgItem(window, CAMPSCORE_CONTINUE), FALSE); // RA2: nobody to wait for, so no continue
				}
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
			}
			ScoreIsReady = false; // RA2: as above
			return(0); // RA2: as above
		}

		case WM_PAINT: // RA2: 0x0F
			Dialog_On_WM_PAINT(window); // RA2: as above
			Slide_Pending_Dialog(window); // RA2: as above
			return(0); // RA2: as above

		case WM_COMMAND: // RA2: 0x111
			switch (wparam & 0xFFFF) { // RA2: as above
				case CAMPSCORE_CONTINUE: // RA2: toggles this player's readiness
					if (!ScoreIsReady) { // RA2: now ready
						if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) { // RA2: only network games have others to tell
							Send_Score_Packet(NET_SCORE_READY); // RA2: as above
						}
						for (int row = 0; row < Session.NumScores; row++) { // RA2: as above
							int id = CampScoreControls[row][5]; // RA2: as above
							if (wcscmp(Session.Players[0]->Name, CampaignSortedScores[row]->Name) == 0) { // RA2: this player's row
								wchar_t const * status = StringTable::LoadString("TXT_READY", NULL, "D:\\Ra2\\CampScor.cpp", 415); // RA2: as above
								HWND item = GetDlgItem(window, id); // RA2: as above
								SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)status); // RA2: as above
								break; // RA2: as above
							}
						}
						wchar_t const * label = StringTable::LoadString("GUI:Cancel", NULL, "D:\\Ra2\\CampScor.cpp", 419); // RA2: the button takes the readiness back
						HWND item = GetDlgItem(window, CAMPSCORE_CONTINUE); // RA2: as above
						SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)label); // RA2: as above
						EnableWindow(GetDlgItem(window, CAMPSCORE_QUIT), FALSE); // RA2: as above
						*done = 1; // RA2: as above
						ScoreIsReady = true; // RA2: as above
						InvalidateRect(window, NULL, FALSE); // RA2: as above
						return(0); // RA2: as above
					} else { // RA2: no longer ready
						if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) { // RA2: as above
							Send_Score_Packet(NET_SCORE_WAITING); // RA2: as above
						}
						for (int row = 0; row < Session.NumScores; row++) { // RA2: as above
							int id = CampScoreControls[row][5]; // RA2: as above
							if (wcscmp(Session.Players[0]->Name, CampaignSortedScores[row]->Name) == 0) { // RA2: as above
								wchar_t const * status = StringTable::LoadString("TXT_WAITING", NULL, "D:\\Ra2\\CampScor.cpp", 434); // RA2: as above
								HWND item = GetDlgItem(window, id); // RA2: as above
								SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)status); // RA2: as above
								break; // RA2: as above
							}
						}
						wchar_t const * label = StringTable::LoadString("GUI:Continue", NULL, "D:\\Ra2\\CampScor.cpp", 439); // RA2: as above
						HWND item = GetDlgItem(window, CAMPSCORE_CONTINUE); // RA2: as above
						SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)label); // RA2: as above
						EnableWindow(GetDlgItem(window, CAMPSCORE_QUIT), TRUE); // RA2: as above
						*done = 1; // RA2: as above
						ScoreIsReady = false; // RA2: as above
						InvalidateRect(window, NULL, FALSE); // RA2: as above
						return(0); // RA2: as above
					}

				case CAMPSCORE_QUIT: // RA2: leaves the campaign
					*done = 2; // RA2: as above
					CampaignScoreDone = false; // RA2: as above
					if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) { // RA2: as above
						Send_Score_Packet(NET_SCORE_ABORTED); // RA2: as above
					}
					return(1); // RA2: as above
			}
			break;

		case WM_USER + 0xED: // RA2: 0x4ED, the dialog is closing
			Darken_Surface(NULL, true); // RA2: as above
			OwnerDraw::Set_Paint_Disabled(window, 1); // RA2: as above
			ShowWindow(window, SW_HIDE); // RA2: as above
			break;
	}
	return(0); // RA2: as above
}


/// <summary>
/// The idle service of the score screens: the network service of the game type, or the plain
/// game service.
/// </summary>
/// <returns>bool; whatever the network service of the game type returns, false for other games.</returns>
bool Campaign_Score_Service(void) // RA2: 0x00466A20
{
	if (Session.Type == GAME_IPX) { // RA2: as above
		return Net2Callback(); // RA2: the lobby's service of a LAN game
	}
	if (Session.Type == GAME_INTERNET) { // RA2: as above
		return WOL_Wait_Callback(); // RA2: the service of an Internet game
	}
	Call_Back(); // RA2: as above
	return false; // RA2: as above
}


/// <summary>
/// Sets the text of a dialog control (ra2-decomp's twin helper in campscor.cpp).
/// </summary>
inline void _Set_Text(HWND dialog, int id, wchar_t const * text)
{
	HWND item = GetDlgItem(dialog, id);
	SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)text);
}

/// <summary>
/// Shows the score screen of a map and waits until the players are done with it.
/// The scores are copied from the caller's list, the session's table is filled from the houses and
/// sorted, and the dialog is run until this player has quit or pressed continue and every other
/// player has too (a player's presses and quits arrive as global messages). The caller learns the
/// outcome from CampaignScoreDone.
/// </summary>
/// <param name="scores">The scores of the players.</param>
/// <param name="time">The time the map took, in frames.</param>
/// <param name="more">Whether more maps follow; the screen is left when this player is ready if not.</param>
/// <param name="difficulty">The difficulty, 0 to 2, or -1 when it is not known.</param>
/// <param name="name">The name of the campaign.</param>
void CampaignScoreClass::Presentation(DynamicVectorClass<MPlayerScoreType *> scores, int time, bool more, int difficulty, wchar_t const * name) // RA2: 0x00466A50
{
	ScoreCount = scores.Count(); // RA2: as above
	ScoreTime = time; // RA2: as above
	CampaignDifficulty = difficulty; // RA2: as above
	wcscpy(ScoreName, name); // RA2: as above
	Campaign_Score_Reset_Ready(8); // RA2: no row is ready, the rows of the missing players are made so below
	int row;
	for (row = 0; row < ScoreCount; row++) { // RA2: as above
		CampaignSortedScores[row] = scores[row]; // RA2: as above
	}
	Tally_Scores(); // RA2: the table of the session is filled from the houses
	Campaign_Score_Sort(); // RA2: and ordered, replacing the copy of the caller's list
	Load_Score_Art(Scen->IsGDI == false); // RA2: the side of the art is 1 unless the scenario is GDI's
	Darken_Surface(NULL, true); // RA2: as above
	if (Session.Type == GAME_INTERNET) { // RA2: as above
		Ipx.Clear_Global_Routed(); // RA2: as above
	}
	HWND window = OwnerDraw::Begin_Dialog(CAMPSCORE_DIALOG, Campaign_Score_Dialog_Proc, 0); // RA2: as above
	int result = 0; // RA2: the dialog's flag: 1 when the continue button was pressed, 2 when quit
	if (window != NULL) { // RA2: as above
		SetWindowLong(window, DWL_USER, (LONG)&result); // RA2: as above
		OwnerDraw::Display_Dialog(window); // RA2: as above
		while (result == 0 || result == 1) { // RA2: as above
			Windows_Message_Handler(); // RA2: as above
			if (result == 1) { // RA2: this player has pressed continue or taken it back
				for (row = 0; row < Session.NumScores; row++) { // RA2: as above
					if (wcscmp(Session.Players[0]->Name, CampaignSortedScores[row]->Name) == 0) { // RA2: as above
						break; // RA2: as above
					}
				}
				if (ScoreIsReady) { // RA2: as above
					ScoreReady[row] = true; // RA2: as above
				} else { // RA2: as above
					ScoreReady[row] = false; // RA2: as above
				}
				if (Campaign_Score_All_Ready() && more) { // RA2: as above
					CampaignScoreDone = true; // RA2: as above
					Send_Score_Packet(NET_SCORE_CONTINUE); // RA2: as above
					break; // RA2: as above
				}
				if (!more) { // RA2: as above
					CampaignScoreDone = false; // RA2: as above
					break; // RA2: as above
				}
				result = 0; // RA2: as above
			}

			Ipx.Service(); // RA2: as above
			if (!Ipx.Get_Global_Message(&Session.GPacket, &Session.GPacketlen, &Session.GAddress, &Session.GProductID) // RA2: as above
				|| Session.GProductID != 0xAA03 // RA2: as above
				|| Process_Global_Packet(&Session.GPacket, &Session.GAddress)) { // RA2: as above
				continue; // RA2: as above
			}

			if ((int)Session.GPacket.Command == NET_SCORE_READY) { // RA2: as above
				for (row = 0; row < Session.NumScores; row++) { // RA2: as above
					if (strcmp(Session.GPacket.Name, Wide_To_Narrow(CampaignSortedScores[row]->Name)) == 0) { // RA2: as above
						int id = CampScoreControls[row][5]; // RA2: as above
						_Set_Text(window, id, StringTable::LoadString("TXT_READY", NULL, "D:\\Ra2\\CampScor.cpp", 582)); // RA2: as above
						ScoreReady[row] = true; // RA2: as above
						break; // RA2: as above
					}
				}
				if (Campaign_Score_All_Ready() && more) { // RA2: as above
					CampaignScoreDone = true; // RA2: as above
					Send_Score_Packet(NET_SCORE_CONTINUE); // RA2: as above
					break; // RA2: as above
				}
			} else if ((int)Session.GPacket.Command == NET_SCORE_ABORTED) { // RA2: as above
				for (row = 0; row < Session.NumScores; row++) { // RA2: as above
					if (strcmp(Session.GPacket.Name, Wide_To_Narrow(CampaignSortedScores[row]->Name)) == 0) { // RA2: as above
						int id = CampScoreControls[row][5]; // RA2: as above
						_Set_Text(window, id, StringTable::LoadString("TXT_ABORTED", NULL, "D:\\Ra2\\CampScor.cpp", 602)); // RA2: as above
						ScoreReady[row] = false; // RA2: as above
						break; // RA2: as above
					}
				}
				for (row = 0; row < Session.Players.Count(); row++) { // RA2: the player is dropped from the session's list
					if (strcmp(Session.GPacket.Name, Wide_To_Narrow(Session.Players[row]->Name)) == 0) { // RA2: as above
						delete Session.Players[row]; // RA2: as above
						Session.Players.Delete(Session.Players[row]); // RA2: as above
						break; // RA2: as above
					}
				}
				EnableWindow(GetDlgItem(window, CAMPSCORE_CONTINUE), FALSE); // RA2: as above
				EnableWindow(GetDlgItem(window, CAMPSCORE_QUIT), TRUE); // RA2: as above
			} else if ((int)Session.GPacket.Command == NET_SCORE_WAITING) { // RA2: as above
				for (row = 0; row < Session.NumScores; row++) { // RA2: as above
					if (strcmp(Session.GPacket.Name, Wide_To_Narrow(CampaignSortedScores[row]->Name)) == 0) { // RA2: as above
						int id = CampScoreControls[row][5]; // RA2: as above
						_Set_Text(window, id, StringTable::LoadString("TXT_WAITING", NULL, "D:\\Ra2\\CampScor.cpp", 625)); // RA2: as above
						ScoreReady[row] = false; // RA2: as above
						break; // RA2: as above
					}
				}
			} else if ((int)Session.GPacket.Command == NET_SCORE_CONTINUE) { // RA2: as above
				CampaignScoreDone = true; // RA2: as above
				break; // RA2: as above
			} else if (Campaign_Score_All_Ready() && more) { // RA2: as above
				CampaignScoreDone = true; // RA2: as above
				Send_Score_Packet(NET_SCORE_CONTINUE); // RA2: as above
				break; // RA2: as above
			}
		}
		Darken_Surface(NULL, true); // RA2: as above
		OwnerDraw::End_Dialog(window); // RA2: as above
		Darken_Surface(NULL, true); // RA2: as above
	}
	if (Session.Type == GAME_INTERNET) { // RA2: as above
		Ipx.Set_Global_Routed(); // RA2: as above
	}
	Free_Score_Art(); // RA2: as above
}


/// <summary>
/// Constructs the end screen of a campaign.
/// </summary>
CampaignEndScoreClass::CampaignEndScoreClass(void) // RA2: 0x00467200
{
	EndScoreDifficulty = -1; // RA2: no difficulty yet
	EndScoreCount = 0; // RA2: as above
	EndScoreTime = 0; // RA2: as above
}


/// <summary>
/// Destroys the end screen of a campaign.
/// </summary>
CampaignEndScoreClass::~CampaignEndScoreClass(void) // RA2: 0x00467230
{
}


/// <summary>
/// Sets the text of a control of the end screen to a number.
/// This is the end screen's own copy of Campaign_Score_Set_Number.
/// </summary>
/// <param name="dialog">The dialog that holds the control.</param>
/// <param name="id">The control's resource id.</param>
/// <param name="value">The number to show.</param>
void __fastcall Campaign_End_Score_Set_Number(HWND dialog, int id, int value) // RA2: 0x00467240
{
	wchar_t text[32]; // RA2: as above
	swprintf(text, L"%d", value); // RA2: as above
	HWND item = GetDlgItem(dialog, id); // RA2: as above
	SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)text); // RA2: as above
}


/// <summary>
/// Formats a duration as hours, minutes and seconds.
/// This is the end screen's own copy of Campaign_Score_Format_Time.
/// </summary>
/// <param name="buffer">Receives the text; nothing is done when it is NULL.</param>
/// <param name="time">The duration in seconds.</param>
void __fastcall Campaign_End_Score_Format_Time(wchar_t * buffer, int time) // RA2: 0x00467290
{
	if (buffer != NULL) { // RA2: as above
		if (time / 3600 > 99) { // RA2: as above
			time = 359999; // RA2: as above
		}
		int hours = time / 3600; // RA2: as above
		time -= hours * 3600; // RA2: as above
		int minutes = time / 60; // RA2: as above
		time -= minutes * 60; // RA2: as above
		swprintf(buffer, StringTable::LoadString("TXT_TIME_FORMAT_HOURS", NULL, "D:\\Ra2\\CampScor.cpp", 700), hours, minutes, time); // RA2: as above
	}
}


/// <summary>
/// Message handler of the end screen dialog of a campaign.
/// Fills the screen once the dialog is up (the campaign, difficulty, time, a row per player and the
/// two best players on the podium) and ends the dialog when the continue button is pressed.
/// </summary>
/// <param name="window">The dialog.</param>
/// <param name="message">The message.</param>
/// <param name="wparam">The message's first parameter.</param>
/// <param name="lparam">The message's second parameter.</param>
/// <returns>int; 0 unless the common dialog handler handled the message.</returns>
int __stdcall Campaign_End_Score_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x00467330
{
	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam); // RA2: the common owner draw handling comes first
	if (rc != 0) { // RA2: as above
		return rc; // RA2: as above
	}

	int * done = (int *)GetWindowLong(window, DWL_USER); // RA2: the flag that ends the dialog
	switch (message) { // RA2: as above
		case WM_PAINT: // RA2: 0x0F
			Dialog_On_WM_PAINT(window); // RA2: as above
			Slide_Pending_Dialog(window); // RA2: as above
			break;

		case WM_INITDIALOG: // RA2: 0x110
		{
			HWND item;
			wchar_t text[32]; // RA2: as above
			text[0] = 0; // RA2: as above
			item = GetDlgItem(window, CAMPSCORE_NAME); // RA2: as above
			SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)EndScoreName); // RA2: as above
			swprintf(text, StringTable::LoadString("GUI:CampaignLabel", NULL, "D:\\Ra2\\CampScor.cpp", 722), EndScoreCampaign); // RA2: as above
			item = GetDlgItem(window, CAMPSCORE_END_CAMPAIGN); // RA2: as above
			SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)text); // RA2: as above
			if (EndScoreDifficulty == 0) { // RA2: as above
				swprintf(text, StringTable::LoadString("GUI:DifficultyString", NULL, "D:\\Ra2\\CampScor.cpp", 726), StringTable::LoadString("TXT_DIFFICULTY_EASY", NULL, "D:\\Ra2\\CampScor.cpp", 726)); // RA2: as above
			} else if (EndScoreDifficulty == 1) { // RA2: as above
				swprintf(text, StringTable::LoadString("GUI:DifficultyString", NULL, "D:\\Ra2\\CampScor.cpp", 728), StringTable::LoadString("TXT_DIFFICULTY_MEDIUM", NULL, "D:\\Ra2\\CampScor.cpp", 728)); // RA2: as above
			} else if (EndScoreDifficulty == 2) { // RA2: as above
				swprintf(text, StringTable::LoadString("GUI:DifficultyString", NULL, "D:\\Ra2\\CampScor.cpp", 730), StringTable::LoadString("TXT_DIFFICULTY_BRUTAL", NULL, "D:\\Ra2\\CampScor.cpp", 730)); // RA2: as above
			} else { // RA2: as above
				swprintf(text, StringTable::LoadString("GUI:Blank", NULL, "D:\\Ra2\\CampScor.cpp", 732)); // RA2: as above
			}
			item = GetDlgItem(window, CAMPSCORE_DIFFICULTY); // RA2: as above
			SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)text); // RA2: as above
			Campaign_End_Score_Format_Time(text, EndScoreTime / 60); // RA2: as above
			item = GetDlgItem(window, CAMPSCORE_TIME); // RA2: as above
			SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)text); // RA2: as above
			for (int row = 0; row < EndScoreCount; row++) { // RA2: as above
				ColorScheme * scheme = ColorSchemes[EndScores[row]->Scheme]; // RA2: as above
				RGBClass color = scheme->HSV; // RA2: HSVClass converts to RGBClass
				unsigned long rgb = color.Get_Red() | (color.Get_Green() << 8) | (color.Get_Blue() << 16); // RA2: a COLORREF
				int id = CampScoreControls[row][0]; // RA2: the name
				wchar_t const * name = EndScores[row]->Name; // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)name); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				id = CampScoreControls[row][1]; // RA2: the kills
				Campaign_End_Score_Set_Number(window, id, EndScores[row]->Kills[0]); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				id = CampScoreControls[row][2]; // RA2: the losses
				Campaign_End_Score_Set_Number(window, id, EndScores[row]->Lost[0]); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				id = CampScoreControls[row][3]; // RA2: the buildings built
				Campaign_End_Score_Set_Number(window, id, EndScores[row]->Built[0]); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				id = CampScoreControls[row][4]; // RA2: the score
				Campaign_End_Score_Set_Number(window, id, EndScores[row]->Score[0]); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				if (row == 0) { // RA2: the winner on the podium
					name = EndScores[0]->Name; // RA2: as above
					item = GetDlgItem(window, CAMPSCORE_FIRST_NAME); // RA2: as above
					SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)name); // RA2: as above
					item = GetDlgItem(window, CAMPSCORE_FIRST_NAME); // RA2: as above
					SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
					swprintf(text, L"%d", EndScores[0]->Score[0]); // RA2: as above
					item = GetDlgItem(window, CAMPSCORE_FIRST_SCORE); // RA2: as above
					SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)text); // RA2: as above
					item = GetDlgItem(window, CAMPSCORE_FIRST_SCORE); // RA2: as above
					SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				} else if (row == 1) { // RA2: the second place
					name = EndScores[1]->Name; // RA2: as above
					item = GetDlgItem(window, CAMPSCORE_SECOND_NAME); // RA2: as above
					SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)name); // RA2: as above
					item = GetDlgItem(window, CAMPSCORE_SECOND_NAME); // RA2: as above
					SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
					swprintf(text, L"%d", EndScores[1]->Score[0]); // RA2: as above
					item = GetDlgItem(window, CAMPSCORE_SECOND_SCORE); // RA2: as above
					SendMessage(item, CAMPSCORE_SET_TEXT, 0, (LPARAM)text); // RA2: as above
					item = GetDlgItem(window, CAMPSCORE_SECOND_SCORE); // RA2: as above
					SendMessage(item, CAMPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				}
			}
			break;
		}

		case WM_COMMAND: // RA2: 0x111
			if ((unsigned short)wparam == CAMPSCORE_CONTINUE) { // RA2: as above
				*done = 1; // RA2: as above
				break;
			}
			break;

		case WM_USER + 0x97: // RA2: 0x497, sent to the dialog once it is up
		{
			HWND item = GetDlgItem(window, CAMPSCORE_END_ANIM); // RA2: as above
			PostMessage(item, WM_USER + 0xD7, 0, (LPARAM)window); // RA2: as above
		}
			break;

		case WM_USER + 0xD8: // RA2: 0x4D8
			if (wparam == 0) { // RA2: as above
				SendDlgItemMessage(window, CAMPSCORE_END_ANIM, WM_USER + 0xD5, wparam, 0x1D); // RA2: as above
			}
			break;

		case WM_USER + 0xEC: // RA2: 0x4EC
			SendDlgItemMessage(window, CAMPSCORE_END_ANIM, WM_USER + 0xD3, 0, 0); // RA2: as above
			break;

		case WM_USER + 0xED: // RA2: 0x4ED, the dialog is closing
			Darken_Surface(NULL, true); // RA2: as above
			ShowWindow(window, SW_HIDE); // RA2: as above
			OwnerDraw::Set_Paint_Disabled(window, 1); // RA2: as above
			break;
	}
	return(0); // RA2: as above
}


/// <summary>
/// Orders the scores of the end screen by score, the best first.
/// </summary>
void Campaign_End_Score_Sort(void) // RA2: 0x00467A00
{
	if (EndScoreCount > 1) { // RA2: as above
		qsort(EndScores, EndScoreCount, sizeof(MPlayerScoreType *), Campaign_Score_Compare); // RA2: as above
	}
}


/// <summary>
/// Shows the end screen of a campaign and waits until the player closes it.
/// </summary>
/// <param name="scores">The totals of the players over the campaign.</param>
/// <param name="time">The time the campaign took, in frames.</param>
/// <param name="difficulty">The difficulty, 0 to 2, or -1 when it is not known.</param>
/// <param name="campaign">The number of the campaign.</param>
/// <param name="name">The name of the campaign.</param>
void CampaignEndScoreClass::Presentation(DynamicVectorClass<MPlayerScoreType *> scores, int time, int difficulty, int campaign, wchar_t const * name) // RA2: 0x00467A20
{
	EndScoreCount = scores.Count(); // RA2: as above
	wcscpy(EndScoreName, name); // RA2: as above
	for (int row = 0; row < EndScoreCount; row++) { // RA2: as above
		EndScores[row] = scores[row]; // RA2: as above
	}
	if (EndScoreCount > 1) { // RA2: as above
		qsort(EndScores, EndScoreCount, sizeof(MPlayerScoreType *), Campaign_Score_Compare); // RA2: as above
	}
	EndScoreCampaign = campaign; // RA2: as above
	EndScoreDifficulty = difficulty; // RA2: as above
	EndScoreTime = time; // RA2: as above
	int side = Scen->IsGDI == false; // RA2: the side of the art is 1 unless the scenario is GDI's
	Load_Score_Art(side); // RA2: as above
	Load_MP_Score_Art(side); // RA2: as above
	Darken_Surface(NULL, true); // RA2: as above
	Dialog_Box(CAMPSCORE_END_DIALOG, Campaign_End_Score_Dialog_Proc, false); // RA2: as above
	Free_MP_Score_Art(); // RA2: as above
	Free_Score_Art(); // RA2: as above
}
