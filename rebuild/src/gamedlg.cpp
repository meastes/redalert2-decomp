/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/* $Header: /CounterStrike/GAMEDLG.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : GAMEDLG.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Maria del Mar McCready Legg, Joe L. Bostic                   *
 *                                                                                             *
 *                   Start Date : Jan 8, 1995                                                  *
 *                                                                                             *
 *                  Last Update : Jan 18, 1995   [MML]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   OptionsClass::Process -- Handles all the options graphic interface.                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "gamedlg.h"

#include "_map.h"
#include "_tooltip.h"
#include "audio.h" // RA2: AudioDeviceAvailable
#include "stringtable.h" // RA2: StringTable::LoadString
#include "cctooltip.h"
#include "data.h"
#include "dbgprint.h"
#include "dsaudio.h"
#include "globals.h"
#include "init.h"
#include "language\language.h"
#include "ownrdraw.h"
#include "queue.h"
#include "session.h"
#include "techno.h"

#include "special.hh"

namespace Game { extern int SpecialDialog; }


char const * const Game_Speed_Labels[OptionsClass::MAX_SPEED_SETTING] = { // RA2: string-table labels (the table holds the names, not ids)
	"TXT_SLOWEST",
	"TXT_SLOWER",
	"TXT_SLOW",
	"TXT_MEDIUM",
	"TXT_FAST",
	"TXT_FASTER",
	"TXT_FASTEST"
};

char const * const Game_Scroll_Labels[OptionsClass::MAX_SCROLL_SETTING] = { // RA2: string-table labels (the table holds the names, not ids)
	"TXT_SLOWEST",
	"TXT_SLOWER",
	"TXT_SLOW",
	"TXT_MEDIUM",
	"TXT_FAST",
	"TXT_FASTER",
	"TXT_FASTEST"
};

char const * const Game_Detail_Labels[OptionsClass::MAX_DETAIL_SETTING] = { // RA2: string-table labels (the table holds the names, not ids)
	"TXT_LOW",
	"TXT_MEDIUM",
	"TXT_HIGH"
};


BOOL CALLBACK Game_Controls_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
void __fastcall Game_Controls_Command(HWND window, int message, HWND control, unsigned code); // RA2: replaces Game_Controls_Dialog_On_COMMAND

/***********************************************************************************************
 * OptionsClass::Process -- Handles all the options graphic interface.                         *
 *                                                                                             *
 *    This routine is the main control for the visual representation of the options            *
 *    screen. It handles the visual overlay and the player input.                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 * OUTPUT:  none                                                                               *
 * WARNINGS:   none                                                                            *
 * HISTORY:                                                                                    *
 *   12/31/1994 MML : Created.                                                                 *
 *=============================================================================================*/
void GameControlsClass::Dialog(void)
{
	int res = -1;

	DebugString("GameControls: GameSpeed = %d, ScrollRate = %d, Detail = %d\n", Options.GameSpeed, Options.ScrollRate, Options.DetailLevel);

	if (GameActive == true) {
		_Dialog = OwnerDraw::Begin_Dialog(IDD_OPT_CTRL_GAME_MP, (DLGPROC)Game_Controls_Dialog_Proc); // RA2: Dialog at 0x4CF920 matches only so: no WOL dialog
	} else {
		_Dialog = OwnerDraw::Begin_Dialog(IDD_OPT_CTRL_GAME_SP, (DLGPROC)Game_Controls_Dialog_Proc);
	}

	if (_Dialog) {

		SetWindowLong(_Dialog, DWL_USER, (LONG)&res);

		OwnerDraw::Display_Dialog(_Dialog);

		while (res == -1) {
			if (OwnerDraw::Dialog_Message_Handler() == true) {
				res = 2;
			}
			if (!GameActive) {
				Title_Screen_Restore();
			}
		}
		if (res == 1) {
			Set();
			Options.Save_Settings();
		}

		OwnerDraw::End_Dialog(_Dialog);
	}

	DebugString("GameControls: GameSpeed = %d, ScrollRate = %d, Detail = %d\n", Options.GameSpeed, Options.ScrollRate, Options.DetailLevel);
}


/// <summary>
/// Sets the game options from the game controls dialog.
/// This routine is called when the player accepts the dialog. Each control is asked for
/// its current value and the answer is handed to the option it governs, along with any
/// notification the rest of the game needs -- the map is told to rebuild its cell drawers
/// when the detail level changes, and a game speed change during a network game is issued
/// as an event so that every player stays in step.
/// </summary>
void GameControlsClass::Set(void)
{
	HWND handle;

	handle = GetDlgItem(_Dialog, IDC_GAME_SPEED_SLIDER);
	if (handle) {
		int gamespeed = (OptionsClass::MAX_SPEED_SETTING-1) - Slider_GetPos(handle);
		if (Options.GameSpeed != gamespeed) {
			if (GameActive == true && Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
				OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::GAMESPEED, gamespeed));
			} else {
				Options.GameSpeed = gamespeed;
			}
		}
	}

	handle = GetDlgItem(_Dialog, IDC_SCROLL_SPEED_SLIDER);
	if (handle) {
		Options.ScrollRate = (OptionsClass::MAX_SCROLL_SETTING-1) - Slider_GetPos(handle);
	}

	handle = GetDlgItem(_Dialog, IDC_DETAIL_LEVEL_SLIDER);
	if (handle) {
		int detailevel = Slider_GetPos(handle);
		if (Options.DetailLevel != detailevel) {
			Options.DetailLevel = detailevel;
			Map.Reinit_Cell_Drawers();
		}
	}

	handle = GetDlgItem(_Dialog, IDC_TARGET_LINES); // RA2: Set at 0x4CFA00 matches only so: no sidebar cameo text check box before this
	if (handle) {
		Options.ActionLines = Button_GetCheck(handle) == TRUE;
		TechnoClass::Set_Action_Lines(Options.ActionLines);
	}

	handle = GetDlgItem(_Dialog, 1540); // RA2: the show-hidden check box
	if (handle) {
		Options.IsShowHidden = Button_GetCheck(handle) == TRUE;
	}

	handle = GetDlgItem(_Dialog, IDC_TOOLTIPS);
	if (handle) {
		Options.ToolTips = Button_GetCheck(handle) == TRUE;
		if (ToolTips != NULL && GameActive == true) {
			ToolTips->Activate(Options.ToolTips);
		}
	}

	if (GameActive == false) { // RA2: Set at 0x4CFA00 matches only so: no scroll coasting check box before this
		handle = GetDlgItem(_Dialog, IDC_DIFFICULTY_SLIDER);
		if (handle) {
			Options.Difficulty = Slider_GetPos(handle);
		}
	}
}


/// <summary>
/// Handles the messages sent to the game controls dialog.
/// This routine gives the ownerdraw layer first refusal on every message. Anything it
/// leaves alone is used to prime the sliders and check boxes from the current options, to
/// track the label alongside a slider the player is dragging, and to route commands on to
/// Game_Controls_Command.
/// </summary>
/// <returns>Returns with a non-zero value if the message was consumed by the ownerdraw
/// layer.</returns>
BOOL CALLBACK Game_Controls_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	HWND handle;
	int index;

	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (rc != 0) { // RA2: Game_Controls_Dialog_Proc at 0x4CFC00 returns the ownerdraw answer at once
		return(rc);
	}

	switch (message) {
		case OD_SUBCLASSED: // RA2: the controls are primed once OwnerDraw has taken them over, not on WM_INITDIALOG
			handle = GetDlgItem(window, IDC_GAME_SPEED_SLIDER);
			if (Session.Type == GAME_NORMAL && !GameControlsShown) { // RA2: the game speed slider stays hidden in single player unless -SPEEDCONTROL was given
				ShowWindow(handle, SW_HIDE);
				handle = GetDlgItem(window, 1812 /* RA2: the game speed caption, no name in language.h */);
				ShowWindow(handle, SW_HIDE);
				handle = GetDlgItem(window, IDC_GAME_SPEED_LABEL);
				ShowWindow(handle, SW_HIDE);
			} else if (handle) {
				SendMessage(handle, OD_TRACKNUMBERS, 0, 0);
				Slider_SetRange(handle, 0, (OptionsClass::MAX_SPEED_SETTING - 1));
				Slider_SetPos(handle, (OptionsClass::MAX_SPEED_SETTING - 1) - Options.GameSpeed);
			}

			if (MuteSWLaunches) { // RA2: muted launches hide the game speed slider as well
				handle = GetDlgItem(window, IDC_GAME_SPEED_SLIDER);
				ShowWindow(handle, SW_HIDE);
				handle = GetDlgItem(window, 1812 /* RA2: the game speed caption, no name in language.h */);
				ShowWindow(handle, SW_HIDE);
				handle = GetDlgItem(window, IDC_GAME_SPEED_LABEL);
				ShowWindow(handle, SW_HIDE);
			}

			handle = GetDlgItem(window, IDC_SCROLL_SPEED_SLIDER);
			if (handle) {
				SendMessage(handle, OD_TRACKNUMBERS, 0, 0);
				Slider_SetRange(handle, 0, (OptionsClass::MAX_SCROLL_SETTING - 1));
				Slider_SetPos(handle, (OptionsClass::MAX_SCROLL_SETTING - 1) - Options.ScrollRate);
			}

			handle = GetDlgItem(window, IDC_DETAIL_LEVEL_SLIDER);
			if (handle) {
				SendMessage(handle, OD_TRACKNUMBERS, 0, 0);
				Slider_SetRange(handle, 0, (OptionsClass::MAX_DETAIL_SETTING - 1));
				Slider_SetPos(handle, Options.DetailLevel);
			}

			handle = GetDlgItem(window, IDC_TARGET_LINES); // RA2: no sidebar cameo text check box
			if (handle) {
				Button_SetCheck(handle, Options.ActionLines != false);
			}

			handle = GetDlgItem(window, 1540); // RA2: the show-hidden check box (as in Set)
			if (handle) {
				Button_SetCheck(handle, Options.IsShowHidden != false);
			}

			handle = GetDlgItem(window, IDC_TOOLTIPS);
			if (handle) {
				Button_SetCheck(handle, Options.ToolTips != false);
			}

			if (GameActive == true) { // RA2: no scroll coasting check box
				handle = GetDlgItem(window, IDC_OPT_SOUND_BTN);
				if (handle) {
					EnableWindow(handle, AudioDeviceAvailable()); // RA2: the audio library's own test, not Audio_Available
					return(0); // RA2: the dialog proc answers 0 directly
				}
			} else {
				handle = GetDlgItem(window, IDC_DIFFICULTY_SLIDER);
				if (handle) {
					SendMessage(handle, OD_TRACKNUMBERS, 0, 0);
					Slider_SetRange(handle, 0, (OptionsClass::MAX_DIFFICULTY_SETTING - 1));
					Slider_SetPos(handle, Options.Difficulty);
					return(0);
				}
			}
			break;

		case WM_COMMAND: // RA2: the command handler is expanded here (and also kept out of line)
			Game_Controls_Command(window, LOWORD(wparam), (HWND)lparam, HIWORD(wparam));
			break;

		case WM_HSCROLL:
			if (LOWORD(wparam) == SB_THUMBTRACK) {
				index = HIWORD(wparam);
				char const * name; // RA2: the captions are string-table labels, not ids

				if ((HWND)lparam == GetDlgItem(window, IDC_GAME_SPEED_SLIDER)) {
					name = Game_Speed_Labels[index];
					handle = GetDlgItem(window, IDC_GAME_SPEED_LABEL);
				} else if ((HWND)lparam == GetDlgItem(window, IDC_SCROLL_SPEED_SLIDER)) {
					name = Game_Scroll_Labels[index];
					handle = GetDlgItem(window, IDC_SCROLL_SPEED_LABEL);
				} else if ((HWND)lparam == GetDlgItem(window, IDC_DETAIL_LEVEL_SLIDER)) {
					name = Game_Detail_Labels[index];
					handle = GetDlgItem(window, IDC_DETAIL_LEVEL_LABEL);
				} else {
					if (GameActive != false) {
						return(0);
					}
					if ((HWND)lparam != GetDlgItem(window, IDC_DIFFICULTY_SLIDER)) {
						return(0);
					}
					name = Difficulty_Labels[index];
					handle = GetDlgItem(window, IDC_DIFFICULTY_LABEL);
				}
				if (handle) {
					SendMessage(handle, OD_SETTEXTW, 0, (LPARAM)StringTable::LoadString(name, NULL, "D:\\Ra2\\GameDlg.CPP", 420)); // RA2: the label's text, set as wide caption
					return(0);
				}
			}
			break;
	}
	return(0);
}


/// <summary>
/// Handles the button presses of the game controls dialog.
/// This routine is called by the dialog procedure whenever a control notifies it. The
/// answer is stored back through the result pointer the dialog was created with, which is
/// what releases GameControlsClass::Dialog from its message loop.
/// </summary>
/// <param name="window">The game controls dialog window.</param>
/// <param name="message">The identifier of the control that was activated.</param>
/// <param name="control">The window handle of the control.</param>
/// <param name="code">The notification code the control sent.</param>
void __fastcall Game_Controls_Command(HWND window, int message, HWND control, unsigned code) // RA2: Game_Controls_Command at 0x4CFFB0, __fastcall with the control handle unused
{
	int* retval = (int *)GetWindowLong(window, DWL_USER);

	switch (message) {
		case IDC_OPT_KEYBOARD_BTN:
			if (code == 0 && GameActive == true) {
				SpecialDialog = SDLG_KEYBOARD;
				*retval = IDOK;
			}
			break;

		case IDC_OPT_SOUND_BTN:
			if (code == 0 && GameActive == true) {
				SpecialDialog = SDLG_SOUND;
				*retval = IDOK;
			}
			break;

		case 1670: // RA2: 1670, the OK button, replaces IDOK/IDCANCEL; no cancel case
			*retval = IDOK;
			break;
	}
}
