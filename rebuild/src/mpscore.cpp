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
**	RA2: the multiplayer score screen: a dialog (resource 0x108) that lists up to eight players
**	with their kills, losses, buildings built and score. TibSun draws its score screen itself
**	(TibSun's mpscore.cpp, a MultiScore engine); the code here is the executable's, from
**	0x005AAA20 to 0x005AB3B0.
*/

#define GEOMETRY_H // RA2: this file holds no copy of the geometry constants (no initializer before its first function at 0x005AAA20), so globals.h must not compute LEVEL_LEPTON_H here
#include "always.h" // RA2: the includes this file needs
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"
#include "_surface.h" // RA2: HiddenSurface and AlternateSurface
#include "ccrand.h" // RA2: Random_Pick
#include "dbgprint.h" // RA2: DebugString
#include "globals.h" // RA2: Houses
#include "gscreen.h" // RA2: Update_Visible_Surface
#include "houstype.h" // RA2: HouseTypeClass::IsMultiplayPassive
#include "misc.h" // RA2: VideoModeWidth and VideoModeHeight
#include "mpscore.h"
#include "mplayerscore.h" // RA2: MPlayerScoreType
#include "ownrdraw.h" // RA2: OwnerDraw::Default_Dialog_Proc
#include "scenario.h" // RA2: Scen, ElapsedTimer
#include "scheme.h" // RA2: ColorSchemes
#include "session.h" // RA2: Session and its score table
#include "stringtable.h" // RA2: StringTable::LoadString, with RA2's source file and line
#include "theme.h" // RA2: Theme.Play_Song
#include "windlg.h" // RA2: Get_Display_Rect
#include <stdlib.h> // RA2: qsort
#include <stdio.h> // RA2: swprintf
#include <wchar.h> // RA2: wcscpy

int __fastcall Dialog_Box(int resource, int (__stdcall * proc)(struct HWND__ *, unsigned int, unsigned int, long), bool show); // RA2: 0x005EAB60 (ownrdraw.cpp)
void __fastcall Blend_Rect(Rect * area, Surface * surface, int color, int alpha); // RA2: 0x005FEB60 (ownrdraw.cpp), blends a colour over the rectangle
void __fastcall Inflate_Rect(Rect & rect, int dx, int dy); // RA2: 0x006F1B30 (txtlabel2.cpp)
void __fastcall Load_MP_Score_Art(int side); // RA2: 0x006F2A30 (txtlabel2.cpp)
void __fastcall Free_MP_Score_Art(void); // RA2: 0x006F2AB0 (txtlabel2.cpp)
void __fastcall Draw_MP_Score_Screen(Surface * surface, Rect const & window); // RA2: 0x006F3CD0 (txtlabel2.cpp)

int __stdcall MP_Score_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x005AAE40, defined below; the screen's Presentation passes it to Dialog_Box

#define	MPSCORE_DIALOG			0x108 // RA2: the dialog resource of the score screen
#define	MPSCORE_CONTINUE		0x6D1 // RA2: the button that closes the screen
#define	MPSCORE_GAME_LABEL		0x6D2 // RA2: the label of the game number; it also marks the top of the table
#define	MPSCORE_TITLE			0x694 // RA2: the label of the screen's title
#define	MPSCORE_TIME			0x3EA // RA2: the label of the game's duration
#define	MPSCORE_SET_TEXT		(WM_USER + 178) // RA2: 0x4B2, sets the text of an owner drawn control
#define	MPSCORE_SET_COLOR		(WM_USER + 152) // RA2: 0x498, sets the text colour of an owner drawn control
#define	MPSCORE_INIT			(WM_USER + 151) // RA2: 0x497, sent to the dialog once it is up

#define	MPSCORE_COLUMNS			5 // RA2: name, kills, losses, built, score

/*
**	RA2: the controls of the table, a row per player slot (0x007E5518)
*/
static int const MPScoreControls[MAX_MULTI_NAMES][MPSCORE_COLUMNS] = { // RA2: 0x007E5518
	{1041, 1049, 1699, 1701, 1702}, // RA2: the first row
	{1042, 1050, 1700, 1703, 1704}, // RA2: as above
	{1043, 1051, 1705, 1706, 1707}, // RA2: as above
	{1044, 1052, 1708, 1709, 1710}, // RA2: as above
	{1045, 1053, 1711, 1712, 1713}, // RA2: as above
	{1046, 1054, 1714, 1715, 1716}, // RA2: as above
	{1047, 1055, 1717, 1718, 1719}, // RA2: as above
	{1048, 1056, 1720, 1721, 1722} // RA2: as above
};

static MPlayerScoreType * SortedScores[MAX_MULTI_NAMES]; // RA2: 0x00A715F0, the score table ordered by score
static int ReservedA; // RA2: 0x00A71684, cleared by the screen's constructor and read by nothing else (YR: the same pair beside its sorted table)
static int ReservedB; // RA2: 0x00A71688, as above

void __fastcall MP_Score_Format_Time(wchar_t * buffer, int time);
void __fastcall MP_Score_Set_Number(HWND dialog, int id, int value);
void MP_Score_Tally(void);
void MP_Score_Sort(void);
int __cdecl MP_Score_Compare(void const * left, void const * right);


/// <summary>
/// Constructs the multiplayer score screen.
/// The screen holds no data of its own: it clears the file's static sort table (and the
/// two spare values beside it), and everything it shows comes from the session's score table.
/// </summary>
MPScoreScreen::MPScoreScreen(void) // RA2: 0x005AAA20
{
	ReservedB = ReservedA = 0; // RA2: as above
	for (int index = 0; index < MAX_MULTI_NAMES; index++) { // RA2: the executable clears the table with one rep stosd
		SortedScores[index] = NULL; // RA2: as above
	}
}


/// <summary>
/// Shows the multiplayer score screen and waits until the player closes it.
/// The art for the winning side's background is loaded first and the screen is drawn
/// before and after the dialog, so the dialog's background is right whichever way the
/// surfaces are flipped.
/// </summary>
void MPScoreScreen::Presentation(void) // RA2: 0x005AAA40
{
	Load_MP_Score_Art(Scen->IsGDI == false); // RA2: the side of the art is 1 unless the scenario is GDI's
	Rect rect(0, 0, VideoModeWidth, VideoModeHeight); // RA2: the whole screen
	Draw_MP_Score_Screen(HiddenSurface, rect); // RA2: as above
	Update_Visible_Surface(true, HiddenSurface, NULL); // RA2: as above
	Dialog_Box(MPSCORE_DIALOG, MP_Score_Dialog_Proc, false); // RA2: the dialog is shown without its own fade
	Draw_MP_Score_Screen(HiddenSurface, rect); // RA2: as above
	Update_Visible_Surface(true, HiddenSurface, NULL); // RA2: as above
	Free_MP_Score_Art(); // RA2: as above
}


/// <summary>
/// Formats a duration as hours, minutes and seconds.
/// The hours are capped at 99, so the longest time shown is 99:59:59.
/// </summary>
/// <param name="buffer">Receives the text; nothing is done when it is NULL.</param>
/// <param name="time">The duration in seconds.</param>
void __fastcall MP_Score_Format_Time(wchar_t * buffer, int time) // RA2: 0x005AAAE0
{
	if (buffer != NULL) { // RA2: the caller may pass no buffer
		if (time / 3600 > 99) { // RA2: more than 99 hours
			time = 359999; // RA2: 99:59:59
		}
		int hours = time / 3600; // RA2: as above
		time -= hours * 3600; // RA2: as above
		int minutes = time / 60; // RA2: as above
		time -= minutes * 60; // RA2: as above
		swprintf(buffer, StringTable::LoadString("TXT_TIME_FORMAT_HOURS", NULL, "D:\\Ra2\\MPScore.cpp", 129), hours, minutes, time); // RA2: as above
	}
}


/// <summary>
/// Sets the text of a control of the dialog to a number.
/// </summary>
/// <param name="dialog">The dialog that holds the control.</param>
/// <param name="id">The control's resource id.</param>
/// <param name="value">The number to show.</param>
void __fastcall MP_Score_Set_Number(HWND dialog, int id, int value) // RA2: 0x005AAB80
{
	wchar_t text[32]; // RA2: as above
	swprintf(text, L"%d", value); // RA2: as above
	HWND item = GetDlgItem(dialog, id); // RA2: as above
	SendMessage(item, MPSCORE_SET_TEXT, 0, (LPARAM)text); // RA2: as above
}


/// <summary>
/// Fills the session's score table from the houses of the game.
/// Passive houses (neutral ones) and the observer are left out. A house that is still
/// in the game when it ends has won, and its score is raised by a random bonus of up to
/// a half again.
/// </summary>
void MP_Score_Tally(void) // RA2: 0x005AABD0
{
	Session.NumScores = 0; // RA2: the table starts empty
	int count = Houses.Count(); // RA2: also bounds the buildings killed loop below
	for (int index = 0; index < count; index++) { // RA2: as above
		HouseClass * house = Houses[index]; // RA2: as above
		if (house != NULL && house->Class->IsMultiplayPassive != true && house != ObserverHouse) { // RA2: only real players
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
			Session.Score[slot].Lost[0] = house->BuildingsLost + house->UnitsLost; // RA2: as above

			int built = house->PBQuantity.Total(); // RA2: buildings built
			built += house->PUQuantity.Total(); // RA2: units built
			built += house->PIQuantity.Total(); // RA2: infantry built
			built += house->PAQuantity.Total(); // RA2: aircraft built
			Session.Score[slot].Built[0] = built; // RA2: as above

			if (house->PointTotal > 0) { // RA2: as above
				Session.Score[slot].Score[0] += house->PointTotal; // RA2: as above
			}
			if (!house->IsDefeated) { // RA2: the winner's bonus
				int score = Session.Score[slot].Score[0]; // RA2: as above
				Session.Score[slot].Score[0] = Random_Pick(score / 2, score) + score / 2 + score; // RA2: as above
			}

			DebugString("%hs: %s\n Scheme: %d\n Lost = %d\n Kills = %d\n Built = %d\n Score = %d\n", // RA2: the narrow name is printed by %hs
				Session.Score[slot].Name, // RA2: as above
				Session.Score[slot].Wins > 0 ? "Winner" : "Loser", // RA2: as above
				Session.Score[slot].Scheme, Session.Score[slot].Lost[0], // RA2: as above
				Session.Score[slot].Kills[0], Session.Score[slot].Built[0], // RA2: as above
				Session.Score[slot].Score[0]); // RA2: as above
		}
	}
}


/// <summary>
/// Orders the score table by score, the best first, into SortedScores.
/// </summary>
void MP_Score_Sort(void) // RA2: 0x005AADD0
{
	int count = Session.NumScores; // RA2: as above
	for (int index = 0; index < count; index++) { // RA2: as above
		SortedScores[index] = &Session.Score[index]; // RA2: as above
	}
	if (count > 1) { // RA2: as above
		qsort(SortedScores, count, sizeof(MPlayerScoreType *), MP_Score_Compare); // RA2: as above
	}
}


/// <summary>
/// Compares two entries of the sorted score table: the higher score comes first.
/// </summary>
/// <param name="left">Pointer to the table's pointer to the first entry.</param>
/// <param name="right">Pointer to the table's pointer to the second entry.</param>
/// <returns>int; negative when the first entry's score is higher, positive when it is lower, 0 when equal.</returns>
int __cdecl MP_Score_Compare(void const * left, void const * right) // RA2: 0x005AAE10
{
	MPlayerScoreType const * a = *(MPlayerScoreType const * const *)left; // RA2: as above
	MPlayerScoreType const * b = *(MPlayerScoreType const * const *)right; // RA2: as above
	if (a->Score[0] == b->Score[0]) { // RA2: the score is the first game's entry
		return(0); // RA2: as above
	}
	return a->Score[0] > b->Score[0] ? -1 : 1; // RA2: as above
}


/// <summary>
/// Message handler of the score screen dialog.
/// Fills the table once the dialog is up (the game number, the time and a row per
/// player, each in the player's colour), shades the table's background when the dialog
/// is painted and ends the dialog when the continue button is pressed.
/// </summary>
/// <param name="window">The dialog.</param>
/// <param name="message">The message.</param>
/// <param name="wparam">The message's first parameter.</param>
/// <param name="lparam">The message's second parameter.</param>
/// <returns>int; 0 unless the common dialog handler handled the message.</returns>
int __stdcall MP_Score_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005AAE40
{
	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam); // RA2: the common owner draw handling comes first
	if (rc != 0) { // RA2: as above
		return rc; // RA2: as above
	}

	int * done = (int *)GetWindowLong(window, DWL_USER); // RA2: set by Dialog_Box, the flag that ends the dialog
	switch (message) { // RA2: as above
		case WM_PAINT: // RA2: 0x0F
		{
			RECT first; // RA2: as above
			RECT last; // RA2: as above
			HWND item = GetDlgItem(window, MPSCORE_GAME_LABEL); // RA2: the top left of the table
			Get_Display_Rect(item, &first); // RA2: as above
			int id = MPScoreControls[Session.NumScores - 1][MPSCORE_COLUMNS - 1]; // RA2: the score of the last row is the bottom right
			item = GetDlgItem(window, id); // RA2: as above
			Get_Display_Rect(item, &last); // RA2: as above
			Rect area(first.left, first.top, last.right - first.left, last.bottom - first.top); // RA2: as above
			Inflate_Rect(area, -4, -8); // RA2: as above
			Blend_Rect(&area, AlternateSurface, 0, 0x9F); // RA2: a dark veil behind the table
			break;
		}

		case WM_COMMAND: // RA2: 0x111
			if ((unsigned short)wparam == MPSCORE_CONTINUE) { // RA2: as above
				*done = 1; // RA2: as above
				break;
			}
			break;

		case MPSCORE_INIT: // RA2: 0x497
		{
			wchar_t text[32]; // RA2: as above
			Theme.Play_Song((ThemeType)Theme.From_Name("SCORE")); // RA2: the score theme
			if (Session.Type == GAME_INTERNET) { // RA2: an Internet game shows its number
				swprintf(text, StringTable::LoadString("TXT_GAME", NULL, "D:\\Ra2\\MPScore.cpp", 284), GameID); // RA2: as above
			} else { // RA2: as above
				swprintf(text, StringTable::LoadString("TXT_GAME", NULL, "D:\\Ra2\\MPScore.cpp", 286), Session.GamesPlayed); // RA2: other games show the count of games played (0x00A3F58C)
			}
			wchar_t const * title; // RA2: as above
			if (Session.Type == GAME_SKIRMISH) { // RA2: as above
				title = StringTable::LoadString("GUI:SkirmishScore", NULL, "D:\\Ra2\\MPScore.cpp", 290); // RA2: as above
			} else { // RA2: as above
				title = StringTable::LoadString("GUI:MultiplayerScore", NULL, "D:\\Ra2\\MPScore.cpp", 292); // RA2: as above
			}
			HWND item = GetDlgItem(window, MPSCORE_TITLE); // RA2: as above
			SendMessage(item, MPSCORE_SET_TEXT, 0, (LPARAM)title); // RA2: as above
			item = GetDlgItem(window, MPSCORE_GAME_LABEL); // RA2: as above
			SendMessage(item, MPSCORE_SET_TEXT, 0, (LPARAM)text); // RA2: as above
			MP_Score_Format_Time(text, Scen->ElapsedTimer / 60); // RA2: the timer counts frames, 60 to the second
			item = GetDlgItem(window, MPSCORE_TIME); // RA2: as above
			SendMessage(item, MPSCORE_SET_TEXT, 0, (LPARAM)text); // RA2: as above
			MP_Score_Tally(); // RA2: as above
			MP_Score_Sort(); // RA2: as above
			for (int row = 0; row < Session.NumScores; row++) { // RA2: as above
				ColorScheme * scheme = ColorSchemes[SortedScores[row]->Scheme]; // RA2: as above
				RGBClass color = scheme->HSV; // RA2: HSVClass converts to RGBClass
				unsigned long rgb = color.Get_Red() | (color.Get_Green() << 8) | (color.Get_Blue() << 16); // RA2: a COLORREF
				int id = MPScoreControls[row][0]; // RA2: the name
				wchar_t const * name = SortedScores[row]->Name; // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, MPSCORE_SET_TEXT, 0, (LPARAM)name); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, MPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				id = MPScoreControls[row][1]; // RA2: the kills
				MP_Score_Set_Number(window, id, SortedScores[row]->Kills[0]); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, MPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				id = MPScoreControls[row][2]; // RA2: the losses
				MP_Score_Set_Number(window, id, SortedScores[row]->Lost[0]); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, MPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				id = MPScoreControls[row][3]; // RA2: the buildings built
				MP_Score_Set_Number(window, id, SortedScores[row]->Built[0]); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, MPSCORE_SET_COLOR, 0, rgb); // RA2: as above
				id = MPScoreControls[row][4]; // RA2: the score
				MP_Score_Set_Number(window, id, SortedScores[row]->Score[0]); // RA2: as above
				item = GetDlgItem(window, id); // RA2: as above
				SendMessage(item, MPSCORE_SET_COLOR, 0, rgb); // RA2: as above
			}
			break;
		}
	}
	return(0); // RA2: as above
}
